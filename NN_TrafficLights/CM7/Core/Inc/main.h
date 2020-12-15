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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "math.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
extern int verbosity;
extern int verbo_changed;
void change_verbosity(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TL0_R_Pin GPIO_PIN_4
#define TL0_R_GPIO_Port GPIOA
#define TL0_Y_Pin GPIO_PIN_5
#define TL0_Y_GPIO_Port GPIOA
#define TL0_G_Pin GPIO_PIN_6
#define TL0_G_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define ZL0_R_Pin GPIO_PIN_1
#define ZL0_R_GPIO_Port GPIOB
#define ZL0_G_Pin GPIO_PIN_2
#define ZL0_G_GPIO_Port GPIOB
#define ZL1_R_Pin GPIO_PIN_10
#define ZL1_R_GPIO_Port GPIOB
#define ZL1_G_Pin GPIO_PIN_11
#define ZL1_G_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define CHK_Pin GPIO_PIN_13
#define CHK_GPIO_Port GPIOD
#define TL1_R_Pin GPIO_PIN_7
#define TL1_R_GPIO_Port GPIOC
#define TL1_Y_Pin GPIO_PIN_8
#define TL1_Y_GPIO_Port GPIOC
#define TL1_G_Pin GPIO_PIN_9
#define TL1_G_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
