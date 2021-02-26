#include "battery.h"
#include "adc.h"
#include "app.h"
#include "gui.h"

static void GPIO_ADC_en_Init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, ADC_EN_Pin, GPIO_PIN_RESET);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pins : PBPin PBPin */
    GPIO_InitStruct.Pin = ADC_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

BATTERY_T Battery_Read()
{
    uint16_t adcraw;
    BATTERY_T bat_data;

    GPIO_ADC_en_Init();
    HAL_GPIO_WritePin(ADC_EN_GPIO_Port, ADC_EN_Pin, GPIO_PIN_SET);
    MX_ADC_Init();
    HAL_ADC_Start(&hadc);

    if (HAL_ADC_PollForConversion(&hadc, 100) != HAL_OK)
    {
        Error_Handler(); //debug
    }
    adcraw = HAL_ADC_GetValue(&hadc);
    bat_data.battery_value = ((float)adcraw * 3.3 / 4096) * ADCtBatteryValue;
    HAL_ADC_Stop(&hadc);
    HAL_ADC_DeInit(&hadc);
    HAL_GPIO_WritePin(ADC_EN_GPIO_Port, ADC_EN_Pin, GPIO_PIN_RESET);

    if (bat_data.battery_value < BATTERY_LOW)
        bat_data.battery_per = 0;
    if (bat_data.battery_per > 100)
        bat_data.battery_per = 100;
    bat_data.battery_per = (float)((bat_data.battery_value - BATTERY_LOW) / ((BATTERY_FULL - BATTERY_LOW) / 100));
    return bat_data;
}

CHARGE_T Charg_Read()
{
    CHARGE_T data;
    data.usb_flag = HAL_GPIO_ReadPin(USB_IN_GPIO_Port, USB_IN_Pin);
    data.charge_flag = HAL_GPIO_ReadPin(CHRG_IN_GPIO_Port, CHRG_IN_Pin);
    return data;
}
