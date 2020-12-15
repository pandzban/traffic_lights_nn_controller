/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define HSEM_ID_0 (0U) /* HW semaphore 0*/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  /*
  Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
  perform system initialization (system clock config, external memory configuration.. )
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}
  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  //HAL_TIM_Base_Start_IT(&htim2); // timer do generacji sinusa( funkcja w pliku z IRQ)
  //SCB_CleanDCache_by_Addr((uint32_t*)(((uint32_t)txBuf) & ~(uint32_t)0x1F), (TX_LENGTH*2)+32);

  //HAL_I2S_Transmit_DMA(&hi2s3, txBuf, 4); //////////
  //HAL_I2S_DMAPause(&hi2s3);
  //SCB_DisableDCache();
  //HAL_I2S_DMAResume(&hi2s3);
  //SCB_DisableDCache(); /////////////////////////////
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //	lSample =  sin_val;
	  //	rSample = sin_val;
	  //	lSample = lSample>>1;
	  //	rSample = rSample>>1;
	  //	buff[0] = (lSample>>16)&0xFFFF; // polling not working
	  //	buff[1] = lSample&0xFFFF;
	  //	buff[2] = (rSample>>16)&0xFFFF;
	  //	buff[3] = rSample&0xFFFF;
	  //	HAL_I2S_Transmit(&hi2s3, buff, 4, 500);


	  //	  HAL_Delay(1);
	  //	  char buffer[16];
	  //	  char text[] = "\n";
	  //
	  //	  HAL_UART_Transmit(&huart3, (uint8_t*)buffer, sprintf(buffer, "%d", sin_val), 500);
	  //	  HAL_UART_Transmit(&huart3, (uint8_t*)text, sizeof(text)-1, 500);
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
//void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s){
//	char text[] = "error\n";
//	HAL_UART_Transmit(&huart3, (uint8_t*)text, sizeof(text)-1, 500);
//	char result[16];
//	int val = hi2s->ErrorCode; // zwraca 8 czyli DMA transmission error
//	HAL_UART_Transmit(&huart3, (uint8_t*)result, sprintf(result, "%d", val), 500);
//	char text2[] = "\n";
//	HAL_UART_Transmit(&huart3, (uint8_t*)text2, sizeof(text2)-1, 500);
//}
//
//void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s){
//	//restore signed 24 bit sample from 16-bit buffers
//
//
//
//	//sum to mono
//	int lSample =  sin_val;
//	int rSample = sin_val;
//
//
//	//run HP on left channel and LP on right channel
//	//lSample = Calc_IIR_Left(lSample);
//	//rSample = Calc_IIR_Right(rSample);
//		  char text[] = "halo";
//		  HAL_UART_Transmit(&huart3, (uint8_t*)text, sizeof(text)-1, 500);
//	//restore to buffer
//	txBuf[0] = (lSample>>16)&0xFFFF;
//	txBuf[1] = lSample&0xFFFF;
//	txBuf[2] = (rSample>>16)&0xFFFF;
//	txBuf[3] = rSample&0xFFFF;
//	//SCB_CleanDCache_by_Addr((uint32_t*)(((uint32_t)txBuf) & ~(uint32_t)0x1F), (TX_LENGTH*2)+32);
//	 //__DSB(); // wait for transfer to cache
//}
//void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s){
//
//
//	//sum to mono
//	int lSample = sin_val;
//	int rSample = sin_val;
//
//
//	//run HP on left channel and LP on right channel
//	//lSample = Calc_IIR_Left(lSample);
//	//rSample = Calc_IIR_Right(rSample);
//	char text[] = "halo2";
//			  HAL_UART_Transmit(&huart3, (uint8_t*)text, sizeof(text)-1, 500);
//	//restore to buffer
//	txBuf[4] = (lSample>>16)&0xFFFF;
//	txBuf[5] = lSample&0xFFFF;
//	txBuf[6] = (rSample>>16)&0xFFFF;
//	txBuf[7] = rSample&0xFFFF;
//	//SCB_CleanDCache_by_Addr((uint32_t*)(((uint32_t)txBuf) & ~(uint32_t)0x1F), (TX_LENGTH*2)+32);
//	 //__DSB();
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
