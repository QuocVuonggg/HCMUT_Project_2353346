/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "timer.h"
#include "input_reading.h"

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
// Biến cho FSM chính
int mode = 1;
int startup_flag = 1; // Cờ để xử lý khởi động

// Biến cho việc quét LED 7 đoạn
int led_buffer[4] = {0, 0, 0, 0};
int led_index = 0;

// Các biến lưu trữ thời gian cho mỗi màu đèn (đơn vị: giây)
int time_duration_red = 5;
int time_duration_yellow = 2;
int time_duration_green = 3;

// Biến tạm để lưu giá trị khi đang chỉnh sửa ở Mode 2, 3, 4
int temp_value = 0;

// Biến cho FSM đèn giao thông (chỉ hoạt động ở Mode 1)
int traffic_state = 0; // 0: INIT, 1: RED1-GREEN2, 2: RED1-YELLOW2, 3: GREEN1-RED2, 4: YELLOW1-RED2
int timer_road1_counter = 0;
int timer_road2_counter = 0;

// Biến cho FSM xử lý nhấn giữ BUT1
static int button1_fsm_state = 0; // 0: IDLE, 1: PRESSED_WAIT_HOLD, 2: HOLD_AUTO_INCREMENT
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void setLedsRoad1(int color); // 0: RED, 1: YELLOW, 2: GREEN
void setLedsRoad2(int color);
void fsm_traffic_light_run();
void clearAllLeds();
void handle_button_logic();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Bảng mã cho LED 7 đoạn ANODE CHUNG
const unsigned char segment_code[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void display7SEG(int num) {
    if (num < 0 || num > 9) return;
    // Để bật 1 segment của ANODE chung, ta kéo chân đó xuống LOW
    uint16_t all_segments = SEG_A_Pin|SEG_B_Pin|SEG_C_Pin|SEG_D_Pin|SEG_E_Pin|SEG_F_Pin|SEG_G_Pin;
    uint16_t pins_to_turn_on = 0x0000;
    char segment_value = segment_code[num];

    // logic: 0 in segment_code means segment ON (pull LOW)
    if(((segment_value >> 0) & 1) == 0) pins_to_turn_on |= SEG_A_Pin;
    if(((segment_value >> 1) & 1) == 0) pins_to_turn_on |= SEG_B_Pin;
    if(((segment_value >> 2) & 1) == 0) pins_to_turn_on |= SEG_C_Pin;
    if(((segment_value >> 3) & 1) == 0) pins_to_turn_on |= SEG_D_Pin;
    if(((segment_value >> 4) & 1) == 0) pins_to_turn_on |= SEG_E_Pin;
    if(((segment_value >> 5) & 1) == 0) pins_to_turn_on |= SEG_F_Pin;
    if(((segment_value >> 6) & 1) == 0) pins_to_turn_on |= SEG_G_Pin;

    HAL_GPIO_WritePin(GPIOB, all_segments, GPIO_PIN_SET); // Tắt hết
    HAL_GPIO_WritePin(GPIOB, pins_to_turn_on, GPIO_PIN_RESET); // Bật những cái cần
}

void update7SEG(int index) {
	// Tắt hết 4 LED 7 đoạn để chống lem
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

	switch(index) {
		case 0:
			display7SEG(led_buffer[0]);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			display7SEG(led_buffer[1]);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			display7SEG(led_buffer[2]);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
			break;
		case 3:
			display7SEG(led_buffer[3]);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
			break;
		default:
			break;
	}
}

void clearAllLeds(){
    // Tắt hết 12 LED đơn bằng cách xuất mức HIGH
    HAL_GPIO_WritePin(GPIOA, RED0_Pin|RED1_Pin|RED2_Pin|RED3_Pin|YELLOW0_Pin|YELLOW1_Pin|YELLOW2_Pin|YELLOW3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GREEN0_Pin|GREEN1_Pin|GREEN2_Pin|GREEN3_Pin, GPIO_PIN_SET);
    // Tắt hết 4 LED 7 đoạn
    HAL_GPIO_WritePin(GPIOA, EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin, GPIO_PIN_SET);
}

// Logic cho Active Low (đường 1 = đèn 0 & 2)
void setLedsRoad1(int color) {
	if(color == 0) { // Bật đèn Đỏ
		HAL_GPIO_WritePin(GPIOA, RED0_Pin|RED2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, YELLOW0_Pin|YELLOW2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GREEN0_Pin|GREEN2_Pin, GPIO_PIN_SET);
	} else if (color == 1) { // Bật đèn Vàng
		HAL_GPIO_WritePin(GPIOA, RED0_Pin|RED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, YELLOW0_Pin|YELLOW2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GREEN0_Pin|GREEN2_Pin, GPIO_PIN_SET);
	} else if (color == 2) { // Bật đèn Xanh
		HAL_GPIO_WritePin(GPIOA, RED0_Pin|RED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, YELLOW0_Pin|YELLOW2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GREEN0_Pin|GREEN2_Pin, GPIO_PIN_RESET);
	}
}

// Logic cho Active Low (đường 2 = đèn 1 & 3)
void setLedsRoad2(int color) {
	if(color == 0) { // Bật đèn Đỏ
		HAL_GPIO_WritePin(GPIOA, RED1_Pin|RED3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin|YELLOW3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GREEN1_Pin|GREEN3_Pin, GPIO_PIN_SET);
	} else if (color == 1) { // Bật đèn Vàng
		HAL_GPIO_WritePin(GPIOA, RED1_Pin|RED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin|YELLOW3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GREEN1_Pin|GREEN3_Pin, GPIO_PIN_SET);
	} else if (color == 2) { // Bật đèn Xanh
		HAL_GPIO_WritePin(GPIOA, RED1_Pin|RED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin|YELLOW3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GREEN1_Pin|GREEN3_Pin, GPIO_PIN_RESET);
	}
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
  HAL_TIM_Base_Start_IT(&htim2);
  setTimer1(1000);
  setTimer2(250);
  setTimer3(50);
  setTimer4(1000); // Timer for button hold logic
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	if(startup_flag == 1 && timer1_flag == 1) {
		startup_flag = 0;
		setTimer1(1000);
	}

	if(startup_flag == 0) {
		handle_button_logic();
	}


	// === Chạy FSM và các hoạt động theo MODE ===
	if(mode == 1) {
		if(startup_flag == 0) fsm_traffic_light_run();
	}

	// === Xử lý nháy LED (cho mọi mode) ===
	if(timer2_flag == 1) {
		setTimer2(250); // Reset timer for 2Hz blinking
		HAL_GPIO_TogglePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);

		if(mode > 1) {
			switch(mode) {
				case 2: HAL_GPIO_TogglePin(GPIOA, RED0_Pin|RED1_Pin|RED2_Pin|RED3_Pin); break;
				case 3: HAL_GPIO_TogglePin(GPIOA, YELLOW0_Pin|YELLOW1_Pin|YELLOW2_Pin|YELLOW3_Pin); break;
				case 4: HAL_GPIO_TogglePin(GPIOB, GREEN0_Pin|GREEN1_Pin|GREEN2_Pin|GREEN3_Pin); break;
			}
		}
	}


	// === Cập nhật buffer cho LED 7 đoạn theo MODE ===
	if (mode == 1) {
		led_buffer[0] = timer_road1_counter / 10;
		led_buffer[1] = timer_road1_counter % 10;
		led_buffer[2] = timer_road2_counter / 10;
		led_buffer[3] = timer_road2_counter % 10;
	} else {
		led_buffer[0] = 0;
		led_buffer[1] = mode;
		led_buffer[2] = temp_value / 10;
		led_buffer[3] = temp_value % 10;
	}


	// === Quét LED 7 đoạn ===
	if(timer3_flag == 1) {
		setTimer3(50); // Reset timer để quét LED tiếp theo sau 50ms
		update7SEG(led_index++);
		if(led_index > 3) led_index = 0;
	}

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void handle_button_logic() {
	// Nút MODE (BUT0)
	if(is_button_pressed(0) == 1){
		while(is_button_pressed(0) == 1); // Chờ nhả phím
		mode++;
		if(mode > 4) {
			mode = 1;
			traffic_state = 0; // Reset FSM khi trở về mode 1
		} else {
			// Khi vào mode 2, 3, 4, hiển thị giá trị thời gian hiện tại
			if(mode == 2) temp_value = time_duration_red;
			if(mode == 3) temp_value = time_duration_yellow;
			if(mode == 4) temp_value = time_duration_green;
		}
		button1_fsm_state = 0; // Reset FSM của nút 1 khi chuyển mode
		clearAllLeds();
	}

	// Nút MODIFY (BUT1) - Sử dụng FSM
	if(mode > 1) {
		switch(button1_fsm_state) {
			case 0: // Trạng thái chờ (IDLE)
				if(is_button_pressed(1) == 1) {
					button1_fsm_state = 1; // Chuyển sang trạng thái chờ nhấn giữ
					setTimer4(1000); // Bắt đầu đếm 1 giây
				}
				break;

			case 1: // Trạng thái chờ nhấn giữ (PRESSED_WAIT_HOLD)
				if(is_button_pressed(1) == 0) {
					// Nhả ra trước 1 giây -> là nhấn đơn
					temp_value++;
					if(temp_value > 99) temp_value = 1;
					button1_fsm_state = 0; // Quay về trạng thái chờ
				} else {
					// Vẫn đang nhấn, kiểm tra xem đã đủ 1 giây chưa
					if(timer4_flag == 1) {
						// Đã giữ đủ 1 giây
						temp_value++; // Tăng giá trị lần đầu
						if(temp_value > 99) temp_value = 1;
						button1_fsm_state = 2; // Chuyển sang trạng thái tự động tăng
						setTimer4(1000); // Đặt lại timer cho chu kỳ 1 giây tiếp theo
					}
				}
				break;

			case 2: // Trạng thái tự động tăng (HOLD_AUTO_INCREMENT)
				if(is_button_pressed(1) == 0) {
					// Đã nhả nút, dừng tự động tăng
					button1_fsm_state = 0;
				} else {
					// Vẫn đang giữ, kiểm tra timer để tăng giá trị mỗi giây
					if(timer4_flag == 1) {
						temp_value++;
						if(temp_value > 99) temp_value = 1;
						setTimer4(1000); // Đặt lại timer cho chu kỳ 1 giây tiếp theo
					}
				}
				break;
		}
	}


	// Nút SET (BUT2)
	if(is_button_pressed(2) == 1 && mode > 1){
		while(is_button_pressed(2) == 1); // Chờ nhả phím
		if(mode == 2) time_duration_red = temp_value;
		if(mode == 3) time_duration_yellow = temp_value;
		if(mode == 4) time_duration_green = temp_value;
	}
}

// FSM đèn giao thông
void fsm_traffic_light_run() {
	switch(traffic_state) {
		case 0: // INIT State
			setLedsRoad1(0); // RED1
			setLedsRoad2(2); // GREEN2
			timer_road1_counter = time_duration_red;
			timer_road2_counter = time_duration_green;
			setTimer1(1000);
			traffic_state = 1;
			break;

		case 1: // RED1 - GREEN2
			if(timer1_flag == 1) {
				setTimer1(1000);
				timer_road1_counter--;
				timer_road2_counter--;
				if(timer_road2_counter <= 0) {
					traffic_state = 2;
					timer_road2_counter = time_duration_yellow;
					setLedsRoad2(1); // Switch to YELLOW2
				}
			}
			break;

		case 2: // RED1 - YELLOW2
			if(timer1_flag == 1) {
				setTimer1(1000);
				timer_road1_counter--;
				timer_road2_counter--;
				if(timer_road2_counter <= 0) {
					traffic_state = 3;
					timer_road1_counter = time_duration_green;
					timer_road2_counter = time_duration_red;
					setLedsRoad1(2); // Switch to GREEN1
					setLedsRoad2(0); // Switch to RED2
				}
			}
			break;

		case 3: // GREEN1 - RED2
			if(timer1_flag == 1) {
				setTimer1(1000);
				timer_road1_counter--;
				timer_road2_counter--;
				if(timer_road1_counter <= 0) {
					traffic_state = 4;
					timer_road1_counter = time_duration_yellow;
					setLedsRoad1(1); // Switch to YELLOW1
				}
			}
			break;

		case 4: // YELLOW1 - RED2
			if(timer1_flag == 1) {
				setTimer1(1000);
				timer_road1_counter--;
				timer_road2_counter--;
				if(timer_road1_counter <= 0) {
					traffic_state = 1;
					timer_road1_counter = time_duration_red;
					timer_road2_counter = time_duration_green;
					setLedsRoad1(0); // Switch to RED1
					setLedsRoad2(2); // Switch to GREEN2
				}
			}
			break;

		default:
			traffic_state = 0;
			break;
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
  // THAY ĐỔI PRESCALER VÀ PERIOD ĐỂ TẠO RA NGẮT 50MS
  // Giả sử System Clock là 8MHz.
  // Clock timer = 8,000,000 / (7999 + 1) = 1,000Hz
  // Chu kỳ ngắt = (Period + 1) / Clock timer = (49 + 1) / 1,000 = 50ms
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 49;
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

  /* USER CODE END 2 */

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
  HAL_GPIO_WritePin(GPIOA, RED0_Pin|RED1_Pin|RED2_Pin|RED3_Pin
                          |YELLOW0_Pin|YELLOW1_Pin|YELLOW2_Pin|YELLOW3_Pin
                          |LED_DEBUG_Pin|EN0_Pin|EN1_Pin|EN2_Pin
                          |EN3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GREEN0_Pin|GREEN1_Pin|GREEN2_Pin|GREEN3_Pin
                          |SEG_A_Pin|SEG_B_Pin|SEG_C_Pin|SEG_D_Pin
                          |SEG_E_Pin|SEG_F_Pin|SEG_G_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 PA4 PA5 PA6 PA7 PA8 PA9 PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = RED0_Pin|RED1_Pin|RED2_Pin|RED3_Pin
                          |YELLOW0_Pin|YELLOW1_Pin|YELLOW2_Pin|YELLOW3_Pin
                          |EN2_Pin|EN3_Pin|LED_DEBUG_Pin|EN0_Pin
                          |EN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB12 PB13 PB3 PB4 PB5 PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GREEN0_Pin|GREEN1_Pin|GREEN2_Pin|GREEN3_Pin
                          |SEG_A_Pin|SEG_B_Pin|SEG_C_Pin|SEG_D_Pin
                          |SEG_E_Pin|SEG_F_Pin|SEG_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = BUT0_Pin|BUT1_Pin|BUT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
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
  * where the assert_param error has occurred.
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
