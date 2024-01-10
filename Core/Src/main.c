/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "disp1color.h"
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

uint8_t data[16];
uint8_t led_matrix[16][32];
volatile uint16_t counter_bullet = 0;
uint16_t x_bullets[27];
uint16_t y_bullets[27];
uint8_t stage = 1;
uint8_t number_enemy;

uint8_t game_over = 0;
uint8_t replay;
uint16_t flag_draw[3];

uint16_t array_enemy[3][2];

volatile uint16_t x_start = 1;
volatile uint16_t y_start = 2;
volatile uint16_t x_aircraft, y_aircraft;
volatile uint16_t x_head_aircratf, y_head_aircratf;
volatile uint16_t x_beam, y_beam;
volatile uint8_t state_beam, state_left, state_right, f_beam;

volatile uint8_t state_draw = 0 ;
volatile uint16_t x_enemy, y_enemy;

extern uint8_t data1[16], data2[16], data3[16], data4[16];

//object
uint8_t aircrafts[5][4];
uint8_t enemy_aircrafts[3][3];
uint8_t enemy_boss[5][4];
uint8_t beam[1][1];

/* enemy_boss =
 * 1 1 1 1 1
 * 1 1 1 1 1
 * 1 1 0 1 1
 * 0 1 0 1 0
 */
//enemy_boss[0][0] = 1;
//enemy_boss[0][1] = 1;
//enemy_boss[0][2] = 1;
//enemy_boss[1][0] = 1;
//enemy_boss[1][1] = 1;
//enemy_boss[1][2] = 1;
//enemy_boss[1][3] = 1;
//enemy_boss[2][0] = 1;
//enemy_boss[2][1] = 1;
//enemy_boss[3][0] = 1;
//enemy_boss[3][1] = 1;
//enemy_boss[3][2] = 1;
//enemy_boss[3][3] = 1;
//enemy_boss[4][0] = 1;
//enemy_boss[4][1] = 1;
//enemy_boss[4][2] = 1;
//enemy_boss[4][3] = 1;

/* beam =
 * 1
 * 1
 */
//beam[0][0] = 1;
//beam[0][1] = 1;

//set ping led_matrix
void generate_ledmatrix()
{
	for(int i = 0; i < 16; i++)
	{
		for( int j = 0; j < 32; j++)
		{
			led_matrix[i][j] = 0;
		}
	}
}
//plush aircraft_mmatrix with led_matrix
void draw_aircraft(uint16_t x_a, uint16_t y_a){
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 4; j++){
//			led_matrix[y_a+i][x_a+j] |= aircrafts[i][j];
			if (aircrafts[i][j] == 1){
				led_matrix[y_a+i][x_a+j] = 1;
			}
		}
	}

}
//plush enemy_aircraft_mmatrix with led_matrix
void draw_enemy_aircraft(uint16_t x_ea, uint16_t y_ea){

//	for (int i=0; i < 3; i++){
//	  	for (int j=0; j<3; j++){
////	  		if (led_matrix[y_ea+i][x_ea+j] == 1){
////	  			state_draw = 1;
////	  		}
//	  		led_matrix[y_ea+i][x_ea+j] |= enemy_aircrafts[i][j];
////	  		if (enemy_aircrafts[i][j] == 1){
////	  			led_matrix[y_ea+i][x_ea+j] = 1;
////	  		}
//	  	}
//	}
//	if (state_draw == 0){
//		for (int i=0; i < 3; i++){
//			for (int j=0; j<3; j++){
//				led_matrix[y_ea+i][x_ea+j] |= enemy_aircrafts[i][j];
//			}
//		}
//	}

		for (int i=0; i < 3; i++){
			for (int j=0; j<3; j++){
				led_matrix[y_ea+i][x_ea+j] |= enemy_aircrafts[i][j];
			}
		}
}
//plush beam_mmatrix with led_matrix
void draw_beam(uint16_t x_b, uint16_t y_b){
	for(int i = 0; i < 1; i++){
		for (int j = 0; j < 1; j++){
	  		if (beam[i][j] == 1){
	  			led_matrix[y_b+i][x_b+j] = 1;
	  		}
		}
	}

//	uint16_t temp = x_b;
//	if (f_beam == 1){
//		led_matrix[y_b][temp] = 1;
//	}else{
//		led_matrix[y_b][temp] = 0;
//		x_b++;
//	}
}
//update_led_panel
void disp_row(int row){


	  if (row == 0){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data1, 16, 10);
		  }

		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_RESET);
	  }
	  if (row == 1){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data2, 16, 10);
		  }

		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_RESET);
	  }

	  if (row == 2){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data3, 16, 10);
		  }

		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
	  }

	  if (row == 3){

		  for(uint8_t i=0; i<6; i++){
		  		HAL_SPI_Transmit(&hspi1, &data4, 16, 10);
		  }

		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, SCLK_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
	  }


	  HAL_GPIO_WritePin(GPIOA, nOE_Pin, GPIO_PIN_SET);
		  for(uint32_t x=0; x<=500; x++) {};
	 HAL_GPIO_WritePin(GPIOA, nOE_Pin, GPIO_PIN_RESET);
  }
  // turn left = go up
void turn_left_aka_go_up(){

}
  //turn right = go down
void turn_right_aka_go_down(){

}
//void beam_start(){
//	if(HAL_GPIO_ReadPin(GPIOB, button_beam_Pin) == GPIO_PIN_SET){
//		state_beam = 1;
//	}
//}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_GPIO_WritePin(GPIOA, nOE_Pin, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //draw matrix_aircraft
  /* aircarfts =
    *  1 0 0 0
    *  1 1 1 0
    *  1 1 1 1
    *  1 1 1 0
    *  1 0 0 0
 */
  for (int i = 0; i < 5; i++){
 	  	for (int j = 0; j < 4; j++){
 	  		aircrafts[i][j] = 0;
 	  	}
 	  }
  aircrafts[0][0] = 1;
  aircrafts[1][0] = 1;
  aircrafts[1][1] = 1;
  aircrafts[1][2] = 1;
  aircrafts[2][0] = 1;
  aircrafts[2][1] = 1;
  aircrafts[2][2] = 1;
  aircrafts[2][3] = 1;
  aircrafts[3][0] = 1;
  aircrafts[3][1] = 1;
  aircrafts[3][2] = 1;
  aircrafts[4][0] = 1;

  //draw matrix_enemy_aircrafts
  /* enemy_aircrafts =
   * 0 0 1
   * 0 1 1
   * 0 0 1
   */
  for (int i=0; i < 3; i++){
	  for (int j=0; j<3; j++){
 		enemy_aircrafts[i][j] = 0;
	  }
  }
  enemy_aircrafts[0][2] = 1;
  enemy_aircrafts[1][1] = 1;
  enemy_aircrafts[1][2] = 1;
  enemy_aircrafts[2][2] = 1;
  //draw matrix_beam
  beam[0][0] = 1;
//  beam[0][1] = 1;

  x_beam = x_start + 5;
  y_beam = y_start + 2;

  x_enemy = 15;
  y_enemy = 7;

  for (int i = 0; i<15; i++){
	  flag_draw[i] = 0;
  }

  array_enemy[0][0] = 28;
  array_enemy[0][1] =  1;
  array_enemy[1][0] = 28;
  array_enemy[1][1] =  5;
  array_enemy[2][0] = 28;
  array_enemy[2][1] =  9;


  while (1)
  {
	  //generate led matrix 32*16
	  generate_ledmatrix();

	  //draw aircraft in led matrix
	  draw_aircraft(x_start,y_start);

	  //update beam in led matrix

	  for (int i = 0; i < counter_bullet; i++){
		  if (x_bullets[i] != 0){
			  if ((x_bullets[i] <= 31) && (led_matrix[x_bullets[i]+2][2]!= 1)) {
				  draw_beam(x_bullets[i], y_bullets[i]);
			  }
			  else
			  {
				  led_matrix[x_bullets[i]][2] = 0;
			  }
		  }
	  }

	  //draw enemy in led matrix
	  for (int k = 0; k < 3; k++ ){
		  for (int i=0; i < 3; i++){
			  for (int j=0; j<3; j++){
				  if (led_matrix[array_enemy[k][1]+i][array_enemy[k][0]+j] == 1){
					  flag_draw[k] = 1;
					  for ( int t = 0; t < 27; t++){
						  if (x_bullets[t] == array_enemy[k][1]+i){
							  x_bullets[t] = 0;
						  }
					  }
				  }
			  }
		  }
	  }
	  for (int k = 0; k<3; k++){
		  if (flag_draw[k] == 0){
			  draw_enemy_aircraft(array_enemy[k][0], array_enemy[k][1]);
		  }
	  }


//	  for (int i=0; i < 3; i++){
//		  for (int j=0; j<3; j++){
//			  if (led_matrix[y_enemy+i][x_enemy+j] == 1){
//				  state_draw = 1;
//			  }
//			  else{
//				  state_draw = 0;
//			  }
//		  }
//	  }



//	  draw_enemy_aircraft(15, 2);
//	  draw_enemy_aircraft(15, 8);
//	  draw_enemy_aircraft(15, 13);

//	  for (int i = 0; i < 27; i++){
//		  if (x_bullets[i] != 0){
//			  counter_bullet++;
//		  }
//	  }

//	  if(HAL_GPIO_ReadPin(GPIOB, button_beam_Pin) == GPIO_PIN_SET){
//		  x_bullets[counter_bullet] = x_beam;
//		  counter_bullet++;
////		  draw_beam(x_beam, 8);
//	  }

	  //udapte matrix for draw
	  disp1color_FillScreenbuff(0);
	  for(int16_t i = 0; i < 16; i++){
		  for(int16_t j = 0; j < 32; j++){
			  if (led_matrix[i][j])
			  disp1color_DrawLine(j, i, j, i);
		  }
	  }

	  disp1color_UpdateFromBuff();
	  prepare_data();
	  disp_row(0);
	  disp_row(1);
	  disp_row(2);
	  disp_row(3);





	  //text matrix_led
//	  draw_aircraft(3,8);
//	  draw_enemy_aircraft(16,8);
//	  draw_beam(10,8);
//	  update_led();
	  //main_code
//	  while(!game_over){
//		  switch (stage)
//		  {
//		  case 1:
//			  number_enemy = 3;
//			  break;
//		  case 2:
//			  number_enemy = 5;
//			  break;
//		  }
//		  for ( int i = 0; i < number_enemy; i++){
//			  draw_enemy_aircraft(24, 3 + 4*i);
//		  }
//		  update_led();
//	  }




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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
