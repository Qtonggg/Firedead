#include "mq2.h"

u16 Set_threshold(u16 concentration){

	return concentration;
}

void BEEP_Init(){

GPIO_InitTypeDef GPIO_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|
											 RCC_APB2Periph_GPIOG, ENABLE);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

GPIO_Init(GPIOB, &GPIO_InitStructure);
GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
GPIO_Init(GPIOG, &GPIO_InitStructure);
}

u8 flame_read(){//火焰报警器接口PG10

	return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_10);
	
}


void MOTOR_Init(){//水泵接PB3,PB4

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

void MOTOR_Z(){

	GPIO_ResetBits(GPIOB, GPIO_Pin_6);

}

void MOTOR_F(){

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	

}
