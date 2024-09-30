#include "tim3_pwm_dma.h"




void TIM3_PWM_Init(uint16_t psc,uint16_t arr){
	/*      TIM3 - BASE     */
	// Enable Clock TIM3
	RCC->APB1ENR |= (1<<1);
	//Auto-reload preload enable,Update disable
	TIM3->CR1 = (1<<7) | (1<<1);
	// Set Prescaler
	TIM3->PSC = psc-1;
	// Set Arr
	TIM3->ARR = arr-1;
	// Update enable
	TIM3->CR1 &=~(1<<1);
	// Update Generation
	TIM3->EGR |= (1<<0);
	// Counter enable
	TIM3->CR1 |= (1<<0);
	
	/*   PWM TIM3 - CHANEL1 - CHANEL2   */
	// TIM3-CCR1
	TIM3->CCMR1 |=  (1<<3);// Output compare 1 preload enable
	TIM3->CCMR1 &=~ (7<<4);// Clear bit
	TIM3->CCMR1 |=  (6<<4);// PWM mode 1
	// TIM3-CCR2
	TIM3->CCMR1 |=  (1<<11);// Output compare 2 preload enable
	TIM3->CCMR1 &=~ (7<<12);// Clear bit
	TIM3->CCMR1 |=  (6<<12);// PWM mode 1
	
	TIM3->CCER  |=  (1<<0); // Capture/Compare 1 output enable
	TIM3->CCER  |=  (1<<4); // Capture/Compare 2 output enable

	/*      GPIO      */
	RCC->AHB1ENR   |= (1<<0);// Enable clock port A
	GPIOA->MODER   |= (2<<12)|(2<<14);//Alternate function mode for PA6 PA7
	GPIOA->OSPEEDR |= (3<<12)|(3<<14);//High speed for PA6 PA7
  GPIOA->AFR[0]  |= (2<<24)|(2<<28);//AF1 for PA6 PA7
	
}

//void TIM3_PWM_Start_DMA(DMA_Stream_TypeDef *DMA_Stream,uint32_t TIM_CHANEL, uint32_t SrcAdd,uint32_t DesAdd,uint16_t Size){
//	RCC->AHB1ENR |= (1<<21);// Enable DMA1
//	/*   PWM-CH1-DMA  */
//	DMA_Stream->CR |= (5<<25);// Select Chanel 3
//	DMA_Stream->CR |= (1<<13);// Memory size 16 bit
//	DMA_Stream->CR |= (1<<11);// Memory size 16 bit
//	DMA_Stream->CR |= (1<<10);// Memory increment mode 
//	DMA_Stream->CR &=~(1<<9);// Peripheral increment mode
//	DMA_Stream->CR &=~ (1<<8);// Circular mode enable
//	DMA_Stream->CR |= (1<<6);// Data tranfer direction Memory to peripheral
//	
//	DMA_Stream->NDTR = Size;
//	DMA_Stream->PAR  = SrcAdd;
//	DMA_Stream->M0AR = DesAdd;
//	
//	DMA_Stream->CR |= (1<<0);// DMA Stream enable
//	TIM3->DIER |= (1<<(TIM_CHANEL+8));// DMA
//}


void TIM3_DMA_Init(DMA_Stream_TypeDef *DMA_Stream){
	RCC->AHB1ENR |= (1<<21);// Enable DMA1
	/*   PWM-CH1-DMA  */
	DMA_Stream->CR |= (5<<25);// Select Chanel 3
	DMA_Stream->CR |= (1<<13);// Memory size 16 bit
	DMA_Stream->CR |= (1<<11);// Memory size 16 bit
	DMA_Stream->CR |= (1<<10);// Memory increment mode 
	DMA_Stream->CR &=~(1<<9);// Peripheral increment mode
	DMA_Stream->CR &=~ (1<<8);// Circular mode enable
	DMA_Stream->CR |= (1<<6);// Data tranfer direction Memory to peripheral
	
}

void TIM3_PWM_DMA_Set_Compare(DMA_Stream_TypeDef *DMA_Stream,uint32_t TIM_CHANEL, uint32_t SrcAdd,uint32_t DesAdd,uint16_t Size){
	
	DMA_Stream->NDTR = Size;
	DMA_Stream->PAR  = SrcAdd;
	DMA_Stream->M0AR = DesAdd;
	
	DMA_Stream->CR |= (1<<0);// DMA Stream enable
	
	TIM3->DIER |= (1<<(TIM_CHANEL+8));// DMA
}