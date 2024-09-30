#include "control.h"
#include "uart.h"

int16_t err,err_p, stop = 0;
extern uint16_t data[5];

//Direction
void forward(void){
	GPIOB->ODR |= (1<<12)|(1<<14);
	GPIOB->ODR &=~((1<<13)|(1<<15));	
}
void back(void){
	GPIOB->ODR &=~ ((1<<12)|(1<<14));
	GPIOB->ODR |= (1<<13)|(1<<15);	
}

// Function for FollowLine
void follow_line(){
	uint16_t average_sensor[5]={1961,1958,1959,1957,1959};
	uint16_t digital[5];
	uint16_t sensor=0,prev_sensor;
	uint8_t i=0;
	
	
	// Convert ADC value to Digital
	for (i = 0; i < 5; i++ )
	{
		if (data[i]<average_sensor[i])
			digital[i]=1;
		else digital[i]=0;
	}
	
	// Calculator for value of sensor	
	sensor=digital[0]+digital[1]*2+digital[2]*4+digital[3]*8+digital[4]*16;
	// Error value for PID 
	switch (sensor){
			//11011
			case 4: 
				err=0;
				break;
			
			//01111
			case 1:
				err=-4;
				break;
			
			//00111
			case 3:
				err=-3;
				break;
			
			//10111
			case 2:
				err=-2;
				break;
			
			//10011
			case 6:
				err=-1;
				break;
			
			//11001
			case 12:
				err=1;
				break;
			
			//11101
			case 8:
				err=2;
				break;
			
			//11101
			case 24:
				err=3;
				break;
			
			//11110
			case 16:
				err=4;
				break;
			
			//00000
			case 31:
				if(prev_sensor != 31 ){
					stop++;
				}
				break;				
		}
		prev_sensor=sensor;		
}

void PD_Control(int Kp ,int Ki,int Kd){
  int16_t Speedchange;
  uint16_t Basespeedleft=100,Basespeedright=100,leftspeed,rightspeed;
	
	Speedchange=Kp*err+Kd*(err-err_p)/0.0001;
	if(Speedchange>100) Speedchange=100;
	else if(Speedchange<-100) Speedchange=-100;
	else{}
	
	if(err>0){
		leftspeed=(uint16_t)(Basespeedright-Speedchange);
		rightspeed=(uint16_t)(Basespeedleft);
	}
	else if(err<0){
		leftspeed=(uint16_t)Basespeedright;
		rightspeed=(uint16_t)(Basespeedleft+Speedchange);
	}
	else{
		leftspeed=50;
		rightspeed=50;
	}
	if(stop==2){
		leftspeed=0;
		rightspeed=0;
	}
	err_p=err;
	TIM3->CCR1 = (int)leftspeed;
	TIM3->CCR2 = (int)rightspeed;	
}

void AutoMode(void){
	GPIOC->ODR &=~ (1<<13);
	follow_line();
	PD_Control(20,0,5);
}

void ManualMode(void){
	uint8_t rxbuffer;
	rxbuffer = UART1_Get();
	GPIOC->ODR |=(1<<13);
	switch(rxbuffer) {
		case 'S':
				TIM3->CCR1 = 0;
				TIM3->CCR2 = 0;
				break;

		case 'F':
				forward();
				TIM3->CCR1 = 100;
				TIM3->CCR2 = 100;
				break;

		case 'B':
				back();
				TIM3->CCR1 = 100;
				TIM3->CCR2 = 100;
				break;

		case 'L':
				forward();
				TIM3->CCR1 = 100;
				TIM3->CCR2 = 0;
				break;

		case 'R':
				forward();
				TIM3->CCR1 = 0;
				TIM3->CCR2 = 100;
				break;

		default:
				TIM3->CCR1 = 0;
				TIM3->CCR2 = 0;
				break;
	}			
}