#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "ds18b20.h"
#include "mq2.h"
#include "stm32f10x.h"

   
 int main(void)
 {	 
  u16 adcx;//adc数据
	float temp;
	u8 t=0;			   
	u8 num1=1;
	u8 num2=1;
	u8 num3=1;
	short temperature,threshold_T=0,flame=0;//定义温度
	u16 threshold=1;      //浓度阈值初始为1
	delay_init();	    	  //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			      //LED端口初始化
	LCD_Init();			 	    //LCD初始化
 	Adc_Init();		  		  //ADC初始化
	BEEP_Init();          //蜂鸣器初始化
	MOTOR_Init();				  //水泵初始化
	threshold=1;          //设定浓度阈值为1
	threshold_T=300;      //设定温度阈值为30度
	POINT_COLOR=RED;      //设置字体为红色
	LCD_ShowString(60,50,200,16,16,"Elite STM32");	
	LCD_ShowString(60,70,200,16,16,"FLAME:NO");	
	LCD_ShowString(60,90,200,16,16,"SMOKE:NO");
	LCD_ShowString(60,110,200,16,16,"2019/12/9");	
	/********显示提示信息************/
	while(DS18B20_Init())	//DS18B20初始化	
	{
		LCD_ShowString(60,170,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(60,170,239,130+16,WHITE);
 		delay_ms(200);
	}		
	LCD_ShowString(60,170,200,16,16,"DS18B20 OK");

	
	POINT_COLOR=BLUE;//设置字体为蓝色
  //	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VOL:0.000ppb");	
 	LCD_ShowString(60,190,200,16,16,"Temp:   . C");	 	
	while(1)
	{
		/************ADC读取电压*******************/
		adcx=Get_Adc_Average(ADC_Channel_1,10);
		temp=(float)adcx*(3.3/4096);
		adcx=temp;//adcx为电压值
		LCD_ShowxNum(156,130,adcx,1,16,0);//显示电压值
		
		
		/**************超过阈值报警***************/
		if(adcx>=threshold){
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		LCD_ShowString(60,90,200,16,16,"SMOKE:YES");
		delay_ms(50);
		num3=0;
		}
		else
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		if((adcx<threshold)&&!num3){
		LCD_Fill(60,90,240,90+16,WHITE);
		LCD_ShowString(60,90,200,16,16,"SMOKE:NO");
		num3=1;
		}
		/******************************************/
		
		
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(172,130,temp,3,16,0X80);
		LED0=!LED0;
		delay_ms(150);	
		/*********************************************/
		
		
		/*********************温度读取**********************/
		if(t%10==0)			//每100ms读取一次
		{									  
			temperature=DS18B20_Get_Temp();	
			if(temperature<0)
			{
				LCD_ShowChar(60+40,190,'-',16,0);			//显示负号
				temperature=-temperature;					//转为正数
			}else LCD_ShowChar(60+40,190,' ',16,0);			//去掉负号
			LCD_ShowNum(60+40+8,190,temperature/10,2,16);	//显示正数部分	    
   		LCD_ShowNum(60+40+32,190,temperature%10,1,16);	//显示小数部分 		   
		}				   
		
		
		/******************温度超过阈值报警************************/
		if((temperature>=threshold_T)){
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
			LCD_ShowString(60,190,200,16,16,"Temp:   . C TempOVER!");
			LCD_ShowNum(60+40+8,190,temperature/10,2,16);	//显示正数部分	    
   		LCD_ShowNum(60+40+32,190,temperature%10,1,16);	//显示小数部分 	
			num1=0;
}
			if((temperature<=threshold_T)&&(!num1)){
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
			LCD_Fill(0,170,239,319,WHITE);
			LCD_ShowString(60,190,200,16,16,"Temp:   . C");
			LCD_ShowString(60,170,200,16,16,"DS18B20 OK");
			num1=1;
				
}

		/*********************************************************/
		
		
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
		/**********************************************************/
		
		
		
		/**********************火焰报警**************************/
		if(!flame_read()){
			MOTOR_Z();
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
			LCD_ShowString(60,70,200,16,16,"FLAME:FIRE!");
			delay_ms(50);
			num2=0;
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
		}
		if(flame_read()&&!num2){
		LCD_Fill(60,70,240,70+16,WHITE);
		LCD_ShowString(60,70,200,16,16,"FLAME:NO");
		num2=1;}
			
		/********************************************************/
	}
}

 

