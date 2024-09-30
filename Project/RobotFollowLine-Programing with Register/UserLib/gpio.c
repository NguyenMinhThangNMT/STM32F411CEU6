#include "gpio.h"

\
void LED_ON(void){
	GPIOC->ODR &=~(1<<13);
}

void LED_OFF(void){
	GPIOC->ODR |=(1<<13);
}

// L298N 
void GPIO_Config(void){
	/*         LED-PC13       */
	RCC->AHB1ENR |= (1<<2);
	GPIOC->MODER |= (1<<26);
	/*       DIR of L298N     */
	RCC->AHB1ENR |= (1<<1);
	GPIOB->MODER |= (1<<24)|(1<<26)|(1<<28)|(1<<30);
	/*       Button Mode      */
	RCC->AHB1ENR |= (1<<0);//Enable clock port A
	GPIOA->MODER &=~(3<<0);//Mode input
	GPIOA->PUPDR |= (1<<0);//Pull up
	
	/*  EXTI 0   */
	RCC->APB2ENR |= (1<<14);//System configuration controller clock enable
	SYSCFG->EXTICR[0] &=~(0xf<<0);//EXTI0 PA0;
	EXTI->IMR |= (1<<0);//Disable Mask on EXTI0;
	EXTI->RTSR &=~ (1<<0);//Rising trigger enable
	EXTI->FTSR |=(1<<0);//Falling trigger disable
	
	NVIC_SetPriority(EXTI0_IRQn,1);//Set priority
	NVIC_EnableIRQ(EXTI0_IRQn);
}
