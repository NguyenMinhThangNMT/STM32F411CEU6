#ifndef CONTROL_H_
#define CONTROL_H_

#include "stdint.h"
#include "main.h"

void follow_line(void);
void PD_Control(int Kp,int Kd);
void tien(void);
void lui(void);
void AutoMode(void);
void ManualMode(void);

#endif
