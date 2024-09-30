#ifndef TIM3_PWM_DMA_H
#define TIM3_PWM_DMA_H
#include "stm32f4xx.h"
#define TIM_CHANEL_1  1
#define TIM_CHANEL_2  2
#define TIM_CHANEL_3  3
#define TIM_CHANEL_4  4

void TIM3_PWM_Init(uint16_t psc,uint16_t arr);
void TIM3_DMA_Init(DMA_Stream_TypeDef *DMA_Stream);
void TIM3_PWM_DMA_Set_Compare(DMA_Stream_TypeDef *DMA_Stream,uint32_t TIM_CHANEL, uint32_t SrcAdd,uint32_t DesAdd,uint16_t Size);

#endif