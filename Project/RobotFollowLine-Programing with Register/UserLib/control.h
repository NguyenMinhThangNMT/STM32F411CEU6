#ifndef CONTROL_H_
#define CONTROL_H_

#include "stm32f4xx.h"

void follow_line(void);
void PD_Control(int Kp ,int Ki,int Kd);
void AutoMode(void);
void ManualMode(void);
void forward(void);
void back(void);

#endif
