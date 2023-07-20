//
// Created by LMTX on 2023/7/17.
//
#include "motor.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "ADC_XJ.h"
#include "mpu6050.h"
#include "PID.h"

extern int ADC_Value[5];
int MOVE = 0;

float target_angle = 0;
float Direction[20] = {0,90,90,-90,-90,90,-90,90,-90,\
                  0,90,-90,90,-90,90,-90,90,90,-90,-90};
extern PID_struct pid_motor_L;
extern PID_struct pid_motor_R;
extern int turn_cnt;



void MOTOR_L_F(int speedl)
{
  HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speedl);
}

void MOTOR_L_B(int speedl)
{
  HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speedl);
}

void MOTOR_R_F(int speedr)
{
  HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speedr);
}

void MOTOR_R_B(int speedr)
{
  HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speedr);
}


void Motor(char F_B,int speed_L,int speed_R)
{
  if(F_B == forward)
  {
    MOVE = forward;
    MOTOR_L_F(speed_L);
    MOTOR_R_F(speed_R);
  }
  else if(F_B == back)
  {
    MOVE = back;
    MOTOR_L_B(speed_L);
    MOTOR_R_B(speed_R);
  }
  else if(F_B == left)
  {
    MOVE = left;
    MOTOR_L_B(speed_L);
    MOTOR_R_F(speed_R);
  }
  else if(F_B == right)
  {
    MOVE = right;
    MOTOR_L_B(speed_L);
    MOTOR_R_F(speed_R);
  }
  else if(F_B == stop)
  {
    MOVE = stop;
    MOTOR_L_F(speed_L);
    MOTOR_R_F(speed_R);
  }
}

void Car_Turn(float angle)
{
    Motor(stop,0,0);
    while (mpu_dmp_init());
    yaw = 0;
    for(int i; i < 100; i++) {
        mpu_dmp_get_data(&pitch, &roll, &yaw);
        HAL_Delay(5);
    }
    while(yaw < (angle - 2.0) || yaw > (angle + 2.0))
    {
        ;
    }

}



