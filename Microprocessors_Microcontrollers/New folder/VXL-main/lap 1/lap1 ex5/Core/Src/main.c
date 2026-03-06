/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  // pin PA0->6 use for 7SEG led A, PB0->6 for 7SEG B
  // pin PA7->9 is red (10s), green (6s), yellow (4s) traffic light for led A
  // pin PB7->9 is red (10s), green (6s), yellow (4s) traffic light for led B
  ////////////////////////////////////////////////////////////////////////////
  int counter = 0;
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if(counter >= 20) counter = 0;

	  //traffic light A
	  if(counter <= 9){ //red light
		  display7SEG(9-counter, GPIOA, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
	  }
	  else if(counter <= 15){ //green light
		  display7SEG(15-counter, GPIOA, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
	  }
	  else{ //yellow light
		  display7SEG(19-counter, GPIOA, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
	  }

	  //traffic light B
	  if(counter <= 5){ //green light
		  display7SEG(5-counter, GPIOB, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
	  }
	  else if(counter <= 9){ //yellow light
		  display7SEG(9-counter, GPIOB, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, RESET);
	  }
	  else{ //red light
		  display7SEG(19-counter, GPIOB, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, SET);
	  }

	  counter ++;
	  HAL_Delay(1000);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
void display7SEG (int num, GPIO_TypeDef* type, uint16_t A, uint16_t B, uint16_t C, uint16_t D, uint16_t E, uint16_t F, uint16_t G){
	if(num == 0){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, RESET);
		HAL_GPIO_WritePin(type, E, RESET);
		HAL_GPIO_WritePin(type, F, RESET);
		HAL_GPIO_WritePin(type, G, SET);
	}
	else if(num == 1){
		HAL_GPIO_WritePin(type, A, SET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, SET);
		HAL_GPIO_WritePin(type, E, SET);
		HAL_GPIO_WritePin(type, F, SET);
		HAL_GPIO_WritePin(type, G, SET);
	}
	else if(num == 2){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, SET);
		HAL_GPIO_WritePin(type, D, RESET);
		HAL_GPIO_WritePin(type, E, RESET);
		HAL_GPIO_WritePin(type, F, SET);
		HAL_GPIO_WritePin(type, G, RESET);
	}
	else if(num == 3){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, RESET);
		HAL_GPIO_WritePin(type, E, SET);
		HAL_GPIO_WritePin(type, F, SET);
		HAL_GPIO_WritePin(type, G, RESET);
	}
	else if(num == 4){
		HAL_GPIO_WritePin(type, A, SET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, SET);
		HAL_GPIO_WritePin(type, E, SET);
		HAL_GPIO_WritePin(type, F, RESET);
		HAL_GPIO_WritePin(type, G, RESET);
	}
	else if(num == 5){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, SET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, RESET);
		HAL_GPIO_WritePin(type, E, SET);
		HAL_GPIO_WritePin(type, F, RESET);
		HAL_GPIO_WritePin(type, G, RESET);
	}
	else if(num == 6){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, SET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, RESET);
		HAL_GPIO_WritePin(type, E, RESET);
		HAL_GPIO_WritePin(type, F, RESET);
		HAL_GPIO_WritePin(type, G, RESET);
	}
	else if(num == 7){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, SET);
		HAL_GPIO_WritePin(type, E, SET);
		HAL_GPIO_WritePin(type, F, SET);
		HAL_GPIO_WritePin(type, G, SET);
	}
	else if(num == 8){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, RESET);
		HAL_GPIO_WritePin(type, E, RESET);
		HAL_GPIO_WritePin(type, F, RESET);
		HAL_GPIO_WritePin(type, G, RESET);
	}
	else if(num == 9){
		HAL_GPIO_WritePin(type, A, RESET);
		HAL_GPIO_WritePin(type, B, RESET);
		HAL_GPIO_WritePin(type, C, RESET);
		HAL_GPIO_WritePin(type, D, RESET);
		HAL_GPIO_WritePin(type, E, SET);
		HAL_GPIO_WritePin(type, F, RESET);
		HAL_GPIO_WritePin(type, G, RESET);
	}
	else{
		HAL_GPIO_WritePin(type, A, SET);
		HAL_GPIO_WritePin(type, B, SET);
		HAL_GPIO_WritePin(type, C, SET);
		HAL_GPIO_WritePin(type, D, SET);
		HAL_GPIO_WritePin(type, E, SET);
		HAL_GPIO_WritePin(type, F, SET);
		HAL_GPIO_WritePin(type, G, SET);
	}
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 PB5 PB6 PB7
                           PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
