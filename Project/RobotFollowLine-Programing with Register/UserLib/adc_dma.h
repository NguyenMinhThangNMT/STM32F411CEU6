#ifndef ADC_DMA_H
#define ADC_DMA_H

#include "stm32f4xx.h"

void ADC_Init(void);
void ADC1_DMA_Init(uint32_t srcAdd,uint32_t destAdd,uint16_t size);


#endif