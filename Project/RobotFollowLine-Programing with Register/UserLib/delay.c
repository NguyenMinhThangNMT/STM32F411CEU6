#include "delay.h"

void delay(uint32_t time){
	while(time){
		SysTick->LOAD = 72000-1;
		SysTick-> VAL =0;
		SysTick->CTRL = 5;
		while(!(SysTick->CTRL & (1<<16))){}
		--time;
		}
}

