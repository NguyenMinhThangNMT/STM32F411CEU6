#include "SystemClock.h"

#define PLLM 25
#define PLLN 144
#define PLLP 0

void SysClockConfig(void){
	//Bat thach anh  ngoai
	RCC->CR |= RCC_CR_HSEON;  
	while (!(RCC->CR & RCC_CR_HSERDY));  
	//Bat PWR CLock va VOS
	RCC->APB1ENR |=(1<<28);
	PWR->CR |= PWR_CR_VOS_1;
	//Cau hinh FLASH
	FLASH->ACR = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_2WS;
	//Cau hinh bo chia tan AHB APB1 APB2
	RCC->CFGR |=RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |=RCC_CFGR_PPRE1_DIV2;
	RCC->CFGR |=RCC_CFGR_PPRE2_DIV1;
	//Cau hinh bo PLL
	RCC->PLLCFGR = (25<<0) | (144<<6) | (0<<16) | (1<<22) ;
	//Bat bo PLL
	RCC->CR |=(1<<24);
	while(!(RCC->CR & RCC_CR_PLLRDY));
	//Bat nguon bo PLLL
	RCC->CFGR |= (2<<0);
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	
}
