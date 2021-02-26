/**
* @file  app.c
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#include "app.h"

#include "rtc.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "sht30.h"
#include "gui.h"
#include "battery.h"
#include "sleep.h"
#include "zigbee.h"
#include "ota_flash.h"

BATTERY_T bat_Data;
SHT30_T sht30_Data;
CHARGE_T charge_Data;
RTC_TimeTypeDef stimestructureget;
RTC_DateTypeDef sdatestructureget;

static void APP_WIFI_update(SHT30_T sht30, BATTERY_T battery, CHARGE_T charge);
static void APP_Reset_Zigbee();
static void APP_Get_Time();
static void EXIT_Serve(APP_T *data);
static void OTA_Serve(APP_T *data);

static void APP_WIFI_update(SHT30_T sht30, BATTERY_T battery, CHARGE_T charge)
{
    HAL_GPIO_WritePin(ZIGBEE_WAKEUP_GPIO_Port, ZIGBEE_WAKEUP_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    uint8_t Battery_state;
    uint8_t Battery_per = battery.battery_per;
    if (Battery_per >= 80)
        Battery_state = 2;
    else if (Battery_per >= 30)
        Battery_state = 1;
    else
        Battery_state = 0;
    mcu_dp_value_update(DPID_TEMP_CURRENT, sht30.temp * 10);
    mcu_dp_value_update(DPID_HUMIDITY_VALUE, sht30.humidity * 10);
    mcu_dp_enum_update(DPID_BATTERY_STATE, Battery_state);
    mcu_dp_value_update(DPID_BATTERY_PERCENTAGE, Battery_per);
    mcu_dp_bool_update(DPID_CHARGE_STATE, charge.usb_flag);
    // HAL_GPIO_WritePin(ZIGBEE_WAKEUP_GPIO_Port, ZIGBEE_WAKEUP_Pin, GPIO_PIN_SET);
}

static void APP_Reset_Zigbee()
{
    HAL_GPIO_WritePin(ZIGBEE_WAKEUP_GPIO_Port, ZIGBEE_WAKEUP_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    mcu_network_start();
    // HAL_GPIO_WritePin(ZIGBEE_WAKEUP_GPIO_Port, ZIGBEE_WAKEUP_Pin, GPIO_PIN_SET);
}

static void APP_Get_Time()
{
    HAL_GPIO_WritePin(ZIGBEE_WAKEUP_GPIO_Port, ZIGBEE_WAKEUP_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    mcu_get_system_time();
    // HAL_GPIO_WritePin(ZIGBEE_WAKEUP_GPIO_Port, ZIGBEE_WAKEUP_Pin, GPIO_PIN_SET);
}

void APP_RTC_Set(uint8_t hour, uint8_t minute, uint8_t second, APP_T *data)
{

    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef DateToUpdate;

    sTime.Hours = hour;
    sTime.Minutes = minute;
    sTime.Seconds = second;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
    DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
    DateToUpdate.Month = RTC_MONTH_JANUARY;
    DateToUpdate.Date = 1;
    DateToUpdate.Year = 0;

    if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    if (data->state > NETJOIN && data->state < MCU_OTA)
    {
        data->state = TIME_REFRESH;
    }
}

void APP_Init(APP_T *data)
{
    data->app_count.epd_update_time = EPD_FULL_UPDATE;
    data->app_count.sensor_update = ZIGBEE_DATE_UPDATE;
}
void APP_Serve(APP_T *data)
{
    EXIT_Serve(data);

    switch (data->state)
    {
    // case NETUNJOIN:
    // case WAITJOIN:
    case NETJOIN:
    case WAITRTC:
        APP_Init(data);
        break;

    case TIME_REFRESH:
        HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
        charge_Data = Charg_Read();

        if (data->app_count.rtc_update >= RTC_ADJ)
        {
            APP_Get_Time();
            data->app_count.rtc_update = 0;
        }
        if (data->app_count.sensor_update >= ZIGBEE_DATE_UPDATE)
        {
            sht30_Data = SHT30_ReadData();
            bat_Data = Battery_Read();
            APP_WIFI_update(sht30_Data, bat_Data, charge_Data);
            if (bat_Data.battery_value < LOW_BATTERY_THR)
            {
                GUI_PowerLow_UI(bat_Data.battery_value);
                data->state = LOW_BATTERY;
                return;
            }
            data->app_count.sensor_update = 0;
        }

        if (data->app_count.epd_update_time >= EPD_FULL_UPDATE)
        {
            GUI_Main_UI_ALL(stimestructureget.Hours, stimestructureget.Minutes, sht30_Data.temp, sht30_Data.humidity, bat_Data.battery_per, charge_Data.charge_flag);
            data->app_count.epd_update_time = 0;
        }
        else
            GUI_UpDateTime(stimestructureget.Hours, stimestructureget.Minutes, sht30_Data.temp, sht30_Data.humidity, bat_Data.battery_per, charge_Data.charge_flag);

        Sleep_setClock(stimestructureget, data);
        data->state++;
        break;

    case MCU_SLEEP:
        if (with_data_rxbuff() == 0)
            Sleep_mcu();
        break;
    case MCU_WAIT_SLEEP:
        if (HAL_GetTick() - data->app_count.wakeup_delay > SLEEP_DELAY)
        {
            data->state = TIME_REFRESH;
            data->app_count.wakeup_delay = 0;
        }
        break;
    case LOW_BATTERY:
        APP_Init(data);
        Sleep_mcu();
        break;
    case MCU_OTA:
        HAL_GPIO_WritePin(ZIGBEE_WAKEUP_GPIO_Port, ZIGBEE_WAKEUP_Pin, GPIO_PIN_RESET);
        OTA_Serve(data);
        break;
    }
}

static void EXIT_Serve(APP_T *data)
{
    switch (data->app_exit)
    {
    case MCU_WAKEUP_RTC:
        if (data->state < LOW_BATTERY && data->state != WAITJOIN)
        {
            data->app_count.epd_update_time++;
            data->app_count.sensor_update++;
            data->app_count.rtc_update++;
            data->state = TIME_REFRESH;
        }
        break;
    case MCU_WAKEUP_USB:
        if (data->state > WAITRTC && data->state < MCU_OTA)
        {
            charge_Data = Charg_Read();
            if (1 == charge_Data.charge_flag)
                bat_Data = Battery_Read();
            APP_WIFI_update(sht30_Data, bat_Data, charge_Data);
            data->state = TIME_REFRESH;
        }
        break;

    case MCU_WAKEUP_BTN:
        if (data->state > NETJOIN && data->state < MCU_OTA)
        {
            data->app_count.wakeup_delay = HAL_GetTick();
            data->state = MCU_WAIT_SLEEP;
        }
        break;
    case MCU_WAKEUP_SOC:
        if (data->state > NETJOIN && data->state < MCU_OTA)
        {
            data->app_count.wakeup_delay = HAL_GetTick();
            data->state = MCU_WAIT_SLEEP;
        }
        break;
    default:
        break;
    }
    data->app_exit = UNWAKEUP;
}

static void OTA_Serve(APP_T *data)
{
    switch (data->ota.state)
    {
    case FLASH_ERASE:
        if (1 == OTA_Flash_Erase())
        {
            GUI_OtaUI(GUI_OTA_MESSAGE, ota_fw_info);
            data->ota.ota_timeout = HAL_GetTick();
            mcu_ota_fw_request(); //first request
            data->ota.state++;
        }
        else
        {
            GUI_OtaUI(GUI_FLASH_ERROR, ota_fw_info);
        }
        break;
    case FLASH_WRITE:
        if (HAL_GetTick() - data->ota.ota_timeout > 3000)
        {
            data->ota.ota_timeout = HAL_GetTick();
            data->ota.state++;
        }

        break;
    case OTA_TIMEOUT:
        if (data->ota.timeout_count >= 3)
        {
            data->ota.state++;
            return;
        }
        mcu_ota_fw_request();
        data->ota.timeout_count++;
        data->ota.state--;
        break;
    case OTA_ERROR:
        report_mcu_ota_result(1);
        HAL_Delay(1);
        mcu_ota_result_report(1);
        GUI_OtaUI(GUI_OTA_ERROR, ota_fw_info);
        HAL_Delay(2000);
        APP_Init(data);
        data->state = TIME_REFRESH;
        data->ota.state = FLASH_ERASE;
        break;

    case OTA_SUCCESS:
        report_mcu_ota_result(0);
        HAL_Delay(1);
        mcu_ota_result_report(0);
        GUI_OtaUI(GUI_OTA_SUCCESS, ota_fw_info);
        //reboot mcu to BootLoader update fw
        HAL_NVIC_SystemReset();
        break;
    }
}

void APP_Check_Hardware()
{
    uint8_t flashid[3];

    sht30_Data = SHT30_ReadData();
    if (sht30_Data.state != HAL_OK)
    {
        //error SHT30
        GUI_Init_UI(SENSOR_ERROR);
        while (1)
            ;
    }
    //check flash
    Get_FlashID(flashid);
    if (flashid[0] != MXICID && flashid[1] != MEMORY_TYPE)
    {
        //spi flash id error
        GUI_Init_UI(FLASH_ERROR);
        while (1)
            ;
    }
    GUI_Init_UI(NOERROR);
}

void Network_Handler(uint8_t network, APP_T *data)
{
    if (data->state == MCU_OTA)
        return;
    switch (network)
    {
    case ZIGBEE_NOT_JION:
        if (data->state == WAITJOIN)
        {
            GUI_Sleep_UI(JOINTIMEOUT);
            Sleep_mcu();
        }
        if (data->state == NETUNJOIN)
        {
            GUI_Sleep_UI(UNJOINTIMEOUT);
            Sleep_mcu();
        }
        data->state = NETUNJOIN;
        break;

    case ZIGBEE_JOIN_GATEWAY:
        APP_Get_Time();
        data->state = WAITRTC;
        break;

    case ZIGBEE_JOIN_ERROR:
        break;

    case ZIGBEE_JOINING:
        GUI_SetNet_UI();
        data->state = WAITJOIN;
        break;

    default:
        break;
    }
}
void Button_Handler(APP_T *data)
{
    //USE BUTTON
    if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == 0)
    {
        data->button.btn_State = PRESS;
        data->button.btn_pdcount++;
        if (data->button.btn_pdcount == 100)
            APP_Reset_Zigbee();
    }
    else
    {
        data->button.btn_State = LOOSE;
        data->button.btn_pdcount = 0;
    }
}