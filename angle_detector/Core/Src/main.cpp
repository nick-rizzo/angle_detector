/* USER CODE BEGIN Header */
/**
  * ===============================================================================================
  * @file           : main.c
  * @brief          : Main program body
  * @authors        : Nick Rizzo
  * ===============================================================================================
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  * ===============================================================================================
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <algorithm>
#include "string.h"
#include "queue.h"
#include "mpu6050.h"
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GYRO_QUEUE_LEN 1
// #define MPU6050_ADDRESS (0x68<<1)
#define MPU6050_ADDRESS 0xD0
#define PITCH_OLED_ADDRESS 0x78
#define ROLL_OLED_ADDRESS 0x78
#define ALERT_OLED_ADDRESS 0x7A
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
using namespace std;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;
DMA_HandleTypeDef hdma_i2c1_tx;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* Definitions for gyro_rx */
osThreadId_t gyro_rxHandle;
const osThreadAttr_t gyro_rx_attributes = {
  .name = "gyro_rx",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for pitch_oled_tx */
osThreadId_t pitch_oled_txHandle;
const osThreadAttr_t pitch_oled_tx_attributes = {
  .name = "pitch_oled_tx",
  .stack_size = 892 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for roll_oled_tx */
osThreadId_t roll_oled_txHandle;
const osThreadAttr_t roll_oled_tx_attributes = {
  .name = "roll_oled_tx",
  .stack_size = 892 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for coll_detect */
osThreadId_t coll_detectHandle;
const osThreadAttr_t coll_detect_attributes = {
  .name = "coll_detect",
  .stack_size = 768 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* USER CODE BEGIN PV */
static xQueueHandle pitch_queue;
static xQueueHandle roll_queue;
uint16_t timer_val;
volatile bool send_alert = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM3_Init(void);
void start_gyro_rx(void *argument);
void start_pitch_oled_tx(void *argument);
void start_roll_oled_tx(void *argument);
void start_coll_det(void *argument);

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_USART2_UART_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  TIM5->CCR1 = 10;
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  pitch_queue = xQueueCreate(GYRO_QUEUE_LEN, sizeof(float));
  roll_queue = xQueueCreate(GYRO_QUEUE_LEN, sizeof(float));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of gyro_rx */
  gyro_rxHandle = osThreadNew(start_gyro_rx, NULL, &gyro_rx_attributes);

  /* creation of pitch_oled_tx */
  pitch_oled_txHandle = osThreadNew(start_pitch_oled_tx, NULL, &pitch_oled_tx_attributes);

  /* creation of roll_oled_tx */
  roll_oled_txHandle = osThreadNew(start_roll_oled_tx, NULL, &roll_oled_tx_attributes);

  /* creation of coll_detect */
  coll_detectHandle = osThreadNew(start_coll_det, NULL, &coll_detect_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 400000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 90-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 444-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 90-1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 500000-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ECHO_IN_Pin */
  GPIO_InitStruct.Pin = ECHO_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ECHO_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TEST_PIN_Pin */
  GPIO_InitStruct.Pin = TEST_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TEST_PIN_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void angle_display(ssd1306_oled display, int8_t angle){
  #define NUM_CHARS 3
  #define CHAR_OFFSET 72
  #define DISPLAY_BOUNDARY (DISPLAY_HEIGHT-1 - LINE_Y_COORD)
  static int8_t old_angle = 0;
  int8_t angle_corr = 0;
  if (abs(angle) > DISPLAY_BOUNDARY){
    angle_corr = (angle < 0) ? (-DISPLAY_BOUNDARY) : DISPLAY_BOUNDARY;
  }
  else {
    angle_corr = angle;
  }
  //clear old artifacts
  if (abs(angle_corr) < abs(old_angle)){
    display.clear_box(63, 31, LINE_X_COORD, 8);
    display.clear_box(63, 31, 0, (uint8_t)(LINE_Y_COORD+1));
  }
  //draw new pixel
  display.draw_triangle({LINE_X_COORD,(LINE_Y_COORD)},
                            {(DISPLAY_WIDTH), (uint8_t)(LINE_Y_COORD)},
                            {(DISPLAY_WIDTH), (uint8_t)(LINE_Y_COORD-angle_corr)});

  display.draw_triangle({DISPLAY_WIDTH-LINE_X_COORD,(LINE_Y_COORD+1)},
                            {0, (uint8_t)(LINE_Y_COORD+1)},
                            {0, (uint8_t)(LINE_Y_COORD+1+angle_corr)});

  display.draw_box(128, 3, 0, LINE_Y_COORD);
  char x_str[NUM_CHARS+1]; // account for null termination
  sprintf(x_str, "%3d", (int)abs(angle));
  display.clear_box((NUM_CHARS*10), 8, CHAR_OFFSET, 0);
  for (uint8_t i=0;i<NUM_CHARS;i++){
    display.place_char(x_str[i], (CHAR_OFFSET+(10*i)), 0);
  }
  display.insert_shape(CHAR_OFFSET+(NUM_CHARS*10),0, DEGREES);
  display.ssd1306_update_display();

  old_angle = angle_corr;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if (GPIO_Pin == GPIO_PIN_8){
    HAL_TIM_Base_Start_IT(&htim3);
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_start_gyro_rx */
/**
  * @brief  Function implementing the gyro_rx thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_start_gyro_rx */
void start_gyro_rx(void *argument)
{
  /* USER CODE BEGIN 5 */
  mpu6050 accel_gyro(hi2c2, MPU6050_ADDRESS);
  gyro_accel_st raw_data;
  accel_reading_st angle_data;

  /* Infinite loop */
  while(1){
    osDelayUntil(16);
    accel_gyro.read_gyro_data();
    raw_data = accel_gyro.get_gyro_data();
    angle_data = accel_gyro.return_angle(raw_data);
    if (xQueueOverwrite(pitch_queue, (void *)&angle_data.y_accel) != pdTRUE){
      //Error
    }
    if (xQueueOverwrite(roll_queue, (void *)&angle_data.x_accel) != pdTRUE){
      //Error
    }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_start_pitch_oled_tx */
/**
  * @brief Function implementing the pitch_oled_tx thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_start_pitch_oled_tx */
void start_pitch_oled_tx(void *argument)
{
  /* USER CODE BEGIN start_pitch_oled_tx */
  float pitch_val;
  pitch_display pitch_oled(hi2c1, PITCH_OLED_ADDRESS);
  pitch_oled.display_init();

  while(1){
    // osDelayUntil(16);
    if (xQueueReceive(pitch_queue, (void *)&pitch_val, 10) == pdTRUE){
      angle_display(pitch_oled, pitch_val);
    }
  }
  /* USER CODE END start_pitch_oled_tx */
}

/* USER CODE BEGIN Header_start_roll_oled_tx */
/**
* @brief Function implementing the roll_oled_tx thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_roll_oled_tx */
void start_roll_oled_tx(void *argument)
{
  /* USER CODE BEGIN start_roll_oled_tx */
  float roll_val;
  roll_display roll_oled(hi2c3, ROLL_OLED_ADDRESS);
  roll_oled.display_init();

  while(1){
    // osDelayUntil(16);
    if (xQueueReceive(roll_queue, (void *)&roll_val, 10) == pdTRUE){
      angle_display(roll_oled, roll_val);
    }
  }
  /* USER CODE END start_roll_oled_tx */
}

/* USER CODE BEGIN Header_start_coll_det */
/**
* @brief Function implementing the coll_detect thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_coll_det */
void start_coll_det(void *argument)
{
  /* USER CODE BEGIN start_coll_det */
  center_display main_oled(hi2c2, ALERT_OLED_ADDRESS);
  main_oled.display_init();

  /* Infinite loop */
  while(1){
    if (send_alert){
      //display alert
      send_alert = 0;
        main_oled.display_warning();
        osDelay(200);
        main_oled.clear_display();
        osDelay(200);
    }
    else{
      osDelay(1);
    }
  }
  /* USER CODE END start_coll_det */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if (htim->Instance == TIM3){
    HAL_TIM_Base_Stop_IT(&htim3);
    GPIO_PinState echo_state = HAL_GPIO_ReadPin(ECHO_IN_GPIO_Port, ECHO_IN_Pin);
    if (echo_state == GPIO_PIN_RESET){
      send_alert = 1;
    }
  }
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
