#include "stdio.h"
#include "SystemClock.h"
#include "delay.h"
#include "adc_dma.h"
#include "tim3_pwm_dma.h"
#include "gpio.h"
#include "uart.h"
#include "control.h"

uint16_t data[5];
int count;


//External Interrupt for button mode
void EXTI0_IRQHandler(void){
	if(EXTI->PR & (1<<0)){
		EXTI->PR |=(1<<0);//clear pending bit
		count++;
		if(count ==10){
			count=0;
		}
	}
}


int main(void){
	/*     Init     */
	SysClockConfig();
	ADC_Init();
	ADC1_DMA_Init((uint32_t)&ADC1->DR,(uint32_t)data,5);
	TIM3_PWM_Init(720,100);
  TIM3_DMA_Init(DMA1_Stream4);
	GPIO_Config();
	forward();
	
  while(1){		
		//Auto mode
		if(count%2==0){
			AutoMode();
		}
		// Manual mode
		else {
			ManualMode();
		}
	}
	return 0;
}