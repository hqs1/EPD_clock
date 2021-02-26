/**
* @file  sleep.c
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#include "sleep.h"
#include "main.h"
#include "rtc.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

static void SystemPower_Config(void);
static void Exit_Sleep_config();
static void WakeupGpio_Config();

static void Exit_Sleep_config()
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

#ifndef ULTRA_LOW
    HAL_GPIO_WritePin(GPIOB, ZIGBEE_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
    GPIO_InitStruct.Pin = ZIGBEE_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = MCU_WAKEUP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(MCU_WAKEUP_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = USB_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

static void SystemPower_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Enable Ultra low power mode */
    HAL_PWREx_EnableUltraLowPower();

    /* Enable the fast wake up from Ultra low power mode */
    HAL_PWREx_EnableFastWakeUp();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    // __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    // HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Disable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    // __HAL_RCC_GPIOC_CLK_DISABLE();
}

void Sleep_setClock(RTC_TimeTypeDef time, APP_T *data)
{
    RTC_TimeTypeDef nowtime;
    RTC_DateTypeDef nowdata;
    HAL_RTC_GetTime(&hrtc, &nowtime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &nowdata, RTC_FORMAT_BIN);

    if (nowtime.Minutes != time.Minutes)
    {
        return;
    }
    RTC_AlarmTypeDef sAlarm = {0};
    /** Enable the Alarm A */
    if (++time.Minutes > 59)
        time.Minutes = 0;
    sAlarm.AlarmTime.Minutes = time.Minutes;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 1;
    sAlarm.Alarm = RTC_ALARM_A;
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

static void WakeupGpio_Config()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, EPD_RST_Pin | EPD_DC_Pin | ZIGBEE_WAKEUP_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, EPD_CS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, EPD_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : PAPin PAPin PAPin PAPin 
                           PAPin */
    GPIO_InitStruct.Pin = EPD_CS_Pin | EPD_RST_Pin | EPD_DC_Pin |ZIGBEE_WAKEUP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = EDP_BUSY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(EDP_BUSY_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
    GPIO_InitStruct.Pin = EPD_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = CHRG_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CHRG_IN_GPIO_Port, &GPIO_InitStruct);
}

void Sleep_mcu()
{ //DeInit
    HAL_SPI_DeInit(&hspi1);
    HAL_UART_DeInit(&huart1);
    HAL_TIM_Base_Stop_IT(&htim6);
    HAL_TIM_Base_DeInit(&htim6);
#ifndef SWD_DEBUG
    SystemPower_Config();
    //set wakeup gpio exit
    Exit_Sleep_config();
#endif
    //go to sleep
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    //wakeup init
    SystemClock_Config();
    WakeupGpio_Config();
    MX_SPI1_Init();
    MX_USART1_UART_Init();
    MX_TIM6_Init();
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    HAL_TIM_Base_Start_IT(&htim6);
}