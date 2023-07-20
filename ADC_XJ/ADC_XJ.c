//
// Created by LMTX on 2023/7/18.
//
#include "ADC_XJ.h"
#include "main.h"
#include "adc.h"



int ADC_Value[2] = {0};

#define YX 2.0


void GET_ADC_Value(void)
{
  for (int i = 0; i < 2; i ++)
  {
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
      printf("ADC开启失败\r\n");
    }
    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)//等待转换完成，第二个参数表示超时时间，单位ms
    {
      printf("ADC转换失败\r\n");
    }
    if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
    {
      if(i == 1)
      {
        if((float)(HAL_ADC_GetValue(&hadc1)*3.3/4096) > YX)
          ADC_Value[i] = 0;
        else ADC_Value[i] = 1;
      }
      else
      {
        if((float)(HAL_ADC_GetValue(&hadc1)*3.3/4096) > YX)
          ADC_Value[i] = 0;
        else ADC_Value[i] = 1;
      }
    }
  }
  HAL_ADC_Stop(&hadc1);
}


