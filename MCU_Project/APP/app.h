/**
* @file  app.h
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#ifndef APP_H
#define APP_H

#include "stm32l0xx_hal.h"

//ultra-low power consumption
// #define ULTRA_LOW

#define ZIGBEE_DATE_UPDATE 25 //min
#define EPD_FULL_UPDATE 240   //min
#define RTC_ADJ 1440          //min
#define SLEEP_DELAY 5000
#define LOW_BATTERY_THR 3.60

typedef enum
{
    LOOSE,
    PRESS
} BTN_STATE_T;

typedef enum
{
    NETUNJOIN,      //未入网
    WAITJOIN,       //等待配网
    NETJOIN,        //已入网
    WAITRTC,        //等待授时
    TIME_REFRESH,   //时间刷新
    MCU_SLEEP,      //睡眠
    MCU_WAIT_SLEEP, //等待休眠
    LOW_BATTERY,    //低电量
    MCU_OTA         //OTA更新
} STATE_T;

typedef enum
{
    UNWAKEUP,
    MCU_WAKEUP_RTC,
    MCU_WAKEUP_USB,
    MCU_WAKEUP_BTN,
    MCU_WAKEUP_SOC
} EXIT_T;

typedef enum
{
    FLASH_ERASE,
    FLASH_WRITE,
    OTA_TIMEOUT,
    OTA_ERROR,
    OTA_SUCCESS
} OTA_STATE_T;

typedef struct
{
    BTN_STATE_T btn_State;
    uint8_t btn_pdcount; //pulldown count
} BUTTON_T;

typedef struct
{
    uint8_t sensor_update;
    uint8_t epd_update_time;
    uint32_t rtc_update;
    uint32_t wakeup_delay;
} COUNT_T;

typedef struct
{
    uint32_t ota_timeout;
    uint8_t timeout_count;
    OTA_STATE_T state;
} OTA_T;

typedef struct
{
    COUNT_T app_count;
    STATE_T state;
    BUTTON_T button;
    EXIT_T app_exit;
    OTA_T ota;
} APP_T;

void APP_Init(APP_T *data);
void APP_Check_Hardware();
void APP_Serve(APP_T *data);
void Network_Handler(uint8_t network, APP_T *data);
void APP_RTC_Set(uint8_t hour, uint8_t minute, uint8_t second, APP_T *data);
void Button_Handler(APP_T *app_data);

#endif