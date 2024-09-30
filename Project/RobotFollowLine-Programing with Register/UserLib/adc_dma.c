#include "adc_dma.h"

void ADC_Init(void){
	// Enable Clock ADC1
	RCC->APB2ENR |= (1<<8);  // enable ADC1 clock
	// Set Prescalar for ADC1 clock
	ADC->CCR |= (1<<16);// ADC clock = 72/4 = 18MHz
	// Set scan mode and resolution 
	ADC1->CR1 |= (1<<8); // Scan mode enable
	ADC1->CR1 &=~(1<<24);// 12 bit RES (ADC value= 0 to 4095)
	// Enable continuous conversion
	ADC1->CR2 |= (1<<1);
	// Set EOC after each conversion
	ADC1->CR2 |= (1<<10);
	// Set Data Alignment
	ADC1->CR2 &=~ (1<<11);// Right alignment
	// Sample time for chanel: 1 2 3 4 5
	ADC1->SMPR2 |= (1<<3)|(1<<6)|(1<<9)|(1<<12)|(1<<15);
	// Set Regular chanel
	ADC1->SQR1 |= (4<<20);// 5 conversion
	
	/*  Enable DMA  */
	ADC1->CR2 |= (1<<8);// Enable DMA
	ADC1->CR2 |= (1<<9);// Enable Continouns Request
	
	// Chanel Sequence 
	ADC1->SQR3 |= (1<<0); // SEQ1 for for chanel 2
	ADC1->SQR3 |= (2<<5); // SEQ2 for for chanel 3
	ADC1->SQR3 |= (3<<10);// SEQ3 for for chanel 4
	ADC1->SQR3 |= (4<<15);// SEQ4 for for chanel 5
	ADC1->SQR3 |= (5<<20);// SEQ5 for for chanel 6
	
	/*   GPIO PIN     */
	RCC->AHB1ENR |= (1<<0);  // enable GPIOA clock
	GPIOA->MODER |= (3<<2)|(3<<4)|(3<<6)|(3<<8)|(3<<10);// Mode ADC for Chanel: 2 3 4 5 6
	
	// Enable A/D Coverter
	ADC1->CR2 |= (1<<0);
	uint32_t t=1000;
	while(t--);
	
}

void ADC1_DMA_Init(uint32_t srcAdd,uint32_t destAdd,uint16_t size){
	//Enable clock DMA2
	RCC->AHB1ENR |= (1<<22);
	// Set direction
	DMA2_Stream0->CR &=~ (3<<6);// Peripheral to memory
	// Enable Circular mode
	DMA2_Stream0->CR |= (1<<8);
	// Peripheral increment mode is fixed
	DMA2_Stream0->CR &=~(1<<9);
	// Memory increment mode 
	DMA2_Stream0->CR |= (1<<10);
	// Peripheral data size
	DMA2_Stream0->CR |= (1<<11);// 16 bit
	// Memory size data size
	DMA2_Stream0->CR |= (1<<13);// 16 bit
	// Chanel Selection for steam
	DMA2_Stream0->CR &=~ (7<<25);// Steam chanel 0
	
	// Start ADC 
	ADC1->SR = 0;// Clear the status register
	ADC1->CR2 |= (1<<30);// Start the conversion
	
	// Set the size of tranfer
	DMA2_Stream0->NDTR = size;
	// Source address is peripheral address
	DMA2_Stream0->PAR = srcAdd;
	// Destination Address is memory address
	DMA2_Stream0->M0AR = destAdd;
	//Enable the DMA stream
	DMA2_Stream0->CR |= (1<<0);
}
