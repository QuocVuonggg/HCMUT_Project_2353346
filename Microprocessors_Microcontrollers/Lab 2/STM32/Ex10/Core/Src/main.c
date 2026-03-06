/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display7SEG(int num) {

    // 0=ON, 1=OFF
    const uint8_t seg_code[10] = {
        0b1000000, // 0
        0b1111001, // 1
        0b0100100, // 2
        0b0110000, // 3
        0b0011001, // 4
        0b0010010, // 5
        0b0000010, // 6
        0b1111000, // 7
        0b0000000, // 8
        0b0010000  // 9
    };

    uint8_t code = seg_code[num];


    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (code & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // a
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (code & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); // b
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (code & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET); // c
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (code & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET); // d
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (code & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET); // e
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (code & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET); // f
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (code & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET); // g
}
int counterMatrixLED = 10; // 100 ms = 10*10 ms
int rowMatrixLed = 0;
const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4];  // test values
/* --- Software Timer --- */
int timer0_counter = 0;
int timer0_flag = 0;
int timer1_counter = 0;
int timer1_flag = 0;
int timer2_counter = 0;
int timer2_flag = 0;

const int TIMER_CYCLE = 10;   // matches TIM2 period = 10ms

void setTimer0(int duration){
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}
void setTimer1(int duration){
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}
void setTimer2(int duration){
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}
void timer_run(){
    if(timer0_counter > 0){
        timer0_counter--;
        if(timer0_counter == 0) timer0_flag = 1;
    }
    if(timer1_counter > 0){
            timer1_counter--;
            if(timer1_counter == 0) timer1_flag = 1;
        }
    if(timer2_counter > 0){
                timer2_counter--;
                if(timer2_counter == 0) timer2_flag = 1;
            }
}
/* This function enables one digit and displays its number */

void update7SEG(int index){
    // Turn OFF all digits first (active LOW for PNP)
    HAL_GPIO_WritePin(GPIOA, EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin, GPIO_PIN_SET);

    switch (index){
    case 0:
        HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);  // Enable digit 1
        display7SEG(led_buffer[0]);                         // Show value
        break;
    case 1:
        HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);  // Enable digit 2
        display7SEG(led_buffer[1]);
        break;
    case 2:
        HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET);  // Enable digit 3
        display7SEG(led_buffer[2]);
        break;
    case 3:
        HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET);  // Enable digit 4
        display7SEG(led_buffer[3]);
        break;
    default:
        break;
    }
}
void updateClockBuffer(int hour, int minute){
    led_buffer[0] = hour / 10;   // tens of hour
    led_buffer[1] = hour % 10;   // ones of hour
    led_buffer[2] = minute / 10; // tens of minute
    led_buffer[3] = minute % 10; // ones of minute
}
int scroll_index = 0;
uint8_t scroll_data[16] = {
		0x00,0x00,0xFC,0x32,0x32,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//uint8_t matrix_buffer[8] = {
//		0x00,0x00,0xFC,0x32,0x32,0xFC,0x00,0x00};
uint8_t matrix_buffer[8];
void updateLEDMatrix(int index) {
    HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|ENM2_Pin|ENM3_Pin|
                              ENM4_Pin|ENM5_Pin|ENM6_Pin|ENM7_Pin,
                              GPIO_PIN_SET);
    uint8_t rowData = matrix_buffer[index];
    HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, (rowData & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, (rowData & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, (rowData & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, (rowData & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, (rowData & 0x10) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW5_GPIO_Port, ROW5_Pin, (rowData & 0x20) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW6_GPIO_Port, ROW6_Pin, (rowData & 0x40) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW7_GPIO_Port, ROW7_Pin, (rowData & 0x80) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    switch(index) {
        case 0: HAL_GPIO_WritePin(GPIOA, ENM0_Pin, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(GPIOA, ENM1_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(GPIOA, ENM2_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(GPIOA, ENM3_Pin, GPIO_PIN_RESET); break;
        case 4: HAL_GPIO_WritePin(GPIOA, ENM4_Pin, GPIO_PIN_RESET); break;
        case 5: HAL_GPIO_WritePin(GPIOA, ENM5_Pin, GPIO_PIN_RESET); break;
        case 6: HAL_GPIO_WritePin(GPIOA, ENM6_Pin, GPIO_PIN_RESET); break;
        case 7: HAL_GPIO_WritePin(GPIOA, ENM7_Pin, GPIO_PIN_RESET); break;
    }
}
void shiftMatrixLeft(void) {
    for (int i = 0; i < 8; i++) {
        // copy 8 columns from scroll_data
        int pos = (scroll_index + i) % 16; // loop over 16
        matrix_buffer[i] = scroll_data[pos];
    }

    scroll_index++;
    if (scroll_index >= 16) scroll_index = 0; // restart
}

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT (& htim2);
    setTimer0(1000);
    setTimer1(250);
    setTimer2(50);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    int hour = 15, minute = 59, second = 58;
    updateClockBuffer(hour,minute);
    setTimer0(1000);
  while (1)
  {
	  if(timer1_flag == 1){
	  	  	 	       	  	  	 	  	 	          timer1_flag = 0;
	  	  	 	       	  	  	 	  	 	          setTimer1(250);

	  	  	 	       	  	  	 	  	 	          update7SEG(index_led);
	  	  	 	       	  	  	 	  	 	          index_led++;
	  	  	 	       	  	  	 	  	 	          if(index_led >= 4) index_led = 0;
	  	  	 	       	  	  	 	  	 	      }

	  if(timer0_flag == 1){    // 1 second passed
	  	 	          timer0_flag = 0;
	  	 	          setTimer0(1000);     // reload 1s

	  	 	          second++;
	  	 	          if (second >= 60) {
	  	 	              second = 0;
	  	 	              minute++;
	  	 	          }
	  	 	          if (minute >= 60) {
	  	 	              minute = 0;
	  	 	              hour++;
	  	 	          }
	  	 	          if (hour >= 24) {
	  	 	              hour = 0;
	  	 	          }

	  	 	          updateClockBuffer(hour,minute);   // refresh led_buffer
	  	 	          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);

	  	 	      }


	  	 	 if(timer2_flag == 1) {
	  	 	     updateLEDMatrix(rowMatrixLed);
	  	 	     rowMatrixLed++;
	  	 	     if(rowMatrixLed >= 8) rowMatrixLed = 0;
	  	 	    timer2_flag = 0;
	  	 	 }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|GPIO_PIN_4|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin PA4 LED_RED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|GPIO_PIN_4|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin PB3 PB4 PB5
                           PB6 ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
int counter = 0;
int digitCounter = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){
		timer_run();
		counter++;
		if (counter >= 100){
			counter = 0;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		}
//		digitCounter++;
//		if (digitCounter >= 25){
//			digitCounter = 0;
//			update7SEG(index_led);
//			index_led++;
//			if(index_led>=4) index_led = 0;
//		}
		if (timer2_flag) {
		        timer2_flag = 0;
		        setTimer2(50);
		        updateLEDMatrix(rowMatrixLed);
		        rowMatrixLed = (rowMatrixLed + 1) % 8;
		    }
		static int scroll_timer = 0;
		    scroll_timer++;
		    if (scroll_timer >= 20) {  // 20 × 10ms = 200ms
		        scroll_timer = 0;
		        shiftMatrixLeft();
		    }
	}
}
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
