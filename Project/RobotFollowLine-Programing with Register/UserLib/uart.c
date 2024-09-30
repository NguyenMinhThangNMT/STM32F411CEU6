#include "uart.h"

void UART1_Init(void){
	// Enable Clock
	RCC->APB2ENR |= (1<<4);//Enable clock USART1
	RCC->AHB1ENR |= (1<<0);//Enable clock port A
	// GPIO Config  
	GPIOA->MODER  |= (2<<18)|(2<<20);//Alternate function mode for PA9 PA10
	GPIOA->AFR[1] |= (7<<4) |(7<<8); //AF7 for PA9 PA10
	
	// USART1 Configure 
	USART1->CR1 |=  (1<<13);// USART1 enable
	USART1->CR1 &=~ (1<<12);// Word lenghth: 1 Start bit,8 Data bit,n Stop bit
	USART1->CR1 |=  (1<<3); // Transmitter enable
	USART1->CR1 |=  (1<<2); // Receiver enable
	USART1->BRR |=  (8<<0)|(19<<4);// Baud rate of 115200,PCLK1 at 36MHz
}

uint8_t UART1_Get(void){
	uint8_t temp;
	while(!(USART1->SR & (1<<5)));// Wait for RXNE to set
	temp= USART1->DR;// Read the data
	return temp;
}