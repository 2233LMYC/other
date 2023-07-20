//
// Created by LMTX on 2023/5/17.
//

#ifndef FLB_PRINTF_H
#define FLB_PRINTF_H

#include "usart.h"
#include "stdio.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 0xFFFF);
  return ch;
}

#endif //FLB_PRINTF_H
