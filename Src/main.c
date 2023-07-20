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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "printf.h"
#include "mpu6050.h"
#include "PID.h"
#include "motor.h"
#include "oled.h"
#include "stdio.h"
#include "ADC_XJ.h"
#include "string.h"
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

extern PID_struct pid_motor_L;
extern PID_struct pid_motor_R;
extern uint8_t  _10ms_flag ,_50ms_flag;
extern int ADC_Value[5];
extern float target_angle;
extern float Direction[18];
extern float Motor_speed_L;
extern float Motor_speed_R;
extern uint8_t  zhixing , xuanzhuan;

extern int turn_cnt;

uint8_t sbuf[21] = {0};

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
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);

  MPU_Init();
  while(mpu_dmp_init());

  OLED_Init();
  OLED_DisplayTurn(1);
  OLED_Clear();


  for(int i; i < 100; i++) {
      mpu_dmp_get_data(&pitch, &roll, &yaw);
      HAL_Delay(5);
  }
  PID_Init(&pid_motor_L);
  PID_Init(&pid_motor_R);

  target_angle = 0.0;

  zhixing = 1;

  PID_Set(&pid_motor_R,target_angle,300,0.001,0.5);
  PID_Set(&pid_motor_L,target_angle,-250,-0.001,-0.5);





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    memset(sbuf,0,21);
    sprintf((char*)sbuf,"Now turn: %.1f",Direction[turn_cnt]);
    OLED_ShowString(0,0,sbuf,12);
    memset(sbuf,0,21);
    sprintf((char*)sbuf,"yaw: %.2f",yaw);
    OLED_ShowString(0,1,(uint8_t*)sbuf,12);
    memset(sbuf,0,21);
    sprintf((char*)sbuf,"pid_L:%d",(int)Motor_speed_L);
    OLED_ShowString(0,2,(uint8_t*)sbuf,12);
    memset(sbuf,0,21);
    sprintf((char*)sbuf,"pid_R:%d",(int)Motor_speed_R);
    OLED_ShowString(0,3,(uint8_t*)sbuf,12);
    memset(sbuf,0,21);
    sprintf((char*)sbuf,"%d  %d     ",ADC_Value[0],ADC_Value[1]);
    OLED_ShowString(0,5,(uint8_t*)sbuf,12);
    memset(sbuf,0,21);
    sprintf((char*)sbuf,"trun_cnt: %d",turn_cnt);
    OLED_ShowString(0,6,(uint8_t*)sbuf,12);


      HAL_Delay(100);

    if(zhixing == 1)//直线行驶
    {
        Motor(forward,default_dpeed,default_dpeed);
    }

    else if(xuanzhuan == 1)//路口旋转
    {
        HAL_Delay(600);//冲出赛道一丢丢
        Motor(stop,0,0);//停车
        MPU_Init();
        while (mpu_dmp_init());//等待陀螺仪初始化
        yaw = 0;//偏航角清零


        pid_motor_L.err_sum = 0;
        pid_motor_R.err_sum = 0;

        target_angle = Direction[turn_cnt];
        pid_motor_R.target = Direction[turn_cnt];
        pid_motor_L.target = Direction[turn_cnt];

        printf("角度已重置\r\n");

        if(yaw>80||yaw<-80)
        {
            xuanzhuan = 0;
            zhixing = 1;
        }
    }

    if(_10ms_flag)//微调角度
    {
        _10ms_flag = 0;
        if(yaw < (target_angle - 2.0) || yaw > (target_angle + 2.0))
        {
          if(Motor_speed_R > 0)
          {
            MOTOR_R_F((int)Motor_speed_R);
          }
          else
          {
            MOTOR_R_B((int)(-Motor_speed_R));
          }
          if(Motor_speed_L > 0)
          {
            MOTOR_L_F((int)Motor_speed_L);
          }
          else
          {
            MOTOR_L_B((int)(-Motor_speed_L));
          }
        }
    }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
