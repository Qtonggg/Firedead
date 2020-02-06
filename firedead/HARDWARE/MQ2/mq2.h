#include "stm32f10x.h"
#include "sys.h"

#ifndef __MQ2_H
#define __MQ2_H

u16 Set_threshold(u16 concentration);//设定浓度阈值

extern void BEEP_Init();
extern u8 flame_read();
extern void MOTOR_Init();
extern void MOTOR_Z();
extern void MOTOR_F();

#endif
