/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
//#define SWD_DEBUG 
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FLASH_CS_Pin GPIO_PIN_1
#define FLASH_CS_GPIO_Port GPIOA
#define EPD_CS_Pin GPIO_PIN_2
#define EPD_CS_GPIO_Port GPIOA
#define EPD_RST_Pin GPIO_PIN_3
#define EPD_RST_GPIO_Port GPIOA
#define EPD_DC_Pin GPIO_PIN_4
#define EPD_DC_GPIO_Port GPIOA
#define EDP_BUSY_Pin GPIO_PIN_0
#define EDP_BUSY_GPIO_Port GPIOB
#define EPD_EN_Pin GPIO_PIN_1
#define EPD_EN_GPIO_Port GPIOB
#define ZIGBEE_EN_Pin GPIO_PIN_2
#define ZIGBEE_EN_GPIO_Port GPIOB
#define ZIGBEE_WAKEUP_Pin GPIO_PIN_8
#define ZIGBEE_WAKEUP_GPIO_Port GPIOA
#define USB_IN_Pin GPIO_PIN_11
#define USB_IN_GPIO_Port GPIOA
#define USB_IN_EXTI_IRQn EXTI4_15_IRQn
#define CHRG_IN_Pin GPIO_PIN_12
#define CHRG_IN_GPIO_Port GPIOA
#define MCU_WAKEUP_Pin GPIO_PIN_15
#define MCU_WAKEUP_GPIO_Port GPIOA
#define MCU_WAKEUP_EXTI_IRQn EXTI4_15_IRQn
#define ADC_EN_Pin GPIO_PIN_3
#define ADC_EN_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_4
#define BUTTON_GPIO_Port GPIOB
#define BUTTON_EXTI_IRQn EXTI4_15_IRQn
#define FLASH_EN_Pin GPIO_PIN_5
#define FLASH_EN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
extern APP_T app_data;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
