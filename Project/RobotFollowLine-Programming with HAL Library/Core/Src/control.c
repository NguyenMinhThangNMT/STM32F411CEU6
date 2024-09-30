#include "control.h"

extern TIM_HandleTypeDef htim3;
extern uint16_t data[5];
int8_t err,err_p;
int16_t Speedchange,stop=0;


void follow_line(){
	 uint8_t i=0;
	 uint16_t average_sensor[5]={1961,1958,1959,1957,1959};
	 uint16_t sensor=0,prev_sensor,digital[5];
	 for (i = 0; i < 5; i++ )
		{
			if (data[i]<average_sensor[i])
				digital[i]=1;
			else digital[i]=0;
		}
		sensor=digital[0]+digital[1]*2+digital[2]*4+digital[3]*8+digital[4]*16;
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
				stop=0;
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

//Ham PD dieu khien vi tri
void PD_Control(int Kp,int Kd){
	uint16_t Basespeedleft=100,Basespeedright=100,leftspeed[1],rightspeed[1];
	Speedchange=Kp*err+Kd*(err-err_p);
	if(Speedchange>100) Speedchange=100;
	else if(Speedchange<-100) Speedchange=-100;
	else{}
	
	if(err>0){
		leftspeed[0]=(uint16_t)(Basespeedright-Speedchange);
		rightspeed[0]=(uint16_t)(Basespeedleft);
	}
	else if(err<0){
		leftspeed[0]=(uint16_t)Basespeedright;
		rightspeed[0]=(uint16_t)(Basespeedleft+Speedchange);
	}
	else{
		leftspeed[0]=50;
		rightspeed[0]=50;
	}
	if(stop==2){
		leftspeed[0]=0;
		rightspeed[0]=0;
	}
	//Dieu xung PWM qua DMA
	HAL_TIM_PWM_Start_DMA(&htim3,TIM_CHANNEL_1,(uint32_t*)leftspeed,1);
	HAL_TIM_PWM_Start_DMA(&htim3,TIM_CHANNEL_2,(uint32_t*)rightspeed,1);

	err_p=err;
}

void tien(){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
}
void lui(){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
}



