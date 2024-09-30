#ifndef _UART1_H_
#define _UART1_H_
#include "stm32f4xx.h"
#include "stdint.h"

void UART1_Init(void);
uint8_t UART1_Get(void);

#endif