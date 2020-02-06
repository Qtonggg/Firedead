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
  u16 adcx;//adc����
	float temp;
	u8 t=0;			   
	u8 num1=1;
	u8 num2=1;
	u8 num3=1;
	short temperature,threshold_T=0,flame=0;//�����¶�
	u16 threshold=1;      //Ũ����ֵ��ʼΪ1
	delay_init();	    	  //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			      //LED�˿ڳ�ʼ��
	LCD_Init();			 	    //LCD��ʼ��
 	Adc_Init();		  		  //ADC��ʼ��
	BEEP_Init();          //��������ʼ��
	MOTOR_Init();				  //ˮ�ó�ʼ��
	threshold=1;          //�趨Ũ����ֵΪ1
	threshold_T=300;      //�趨�¶���ֵΪ30��
	POINT_COLOR=RED;      //��������Ϊ��ɫ
	LCD_ShowString(60,50,200,16,16,"Elite STM32");	
	LCD_ShowString(60,70,200,16,16,"FLAME:NO");	
	LCD_ShowString(60,90,200,16,16,"SMOKE:NO");
	LCD_ShowString(60,110,200,16,16,"2019/12/9");	
	/********��ʾ��ʾ��Ϣ************/
	while(DS18B20_Init())	//DS18B20��ʼ��	
	{
		LCD_ShowString(60,170,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(60,170,239,130+16,WHITE);
 		delay_ms(200);
	}		
	LCD_ShowString(60,170,200,16,16,"DS18B20 OK");

	
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
  //	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VOL:0.000ppb");	
 	LCD_ShowString(60,190,200,16,16,"Temp:   . C");	 	
	while(1)
	{
		/************ADC��ȡ��ѹ*******************/
		adcx=Get_Adc_Average(ADC_Channel_1,10);
		temp=(float)adcx*(3.3/4096);
		adcx=temp;//adcxΪ��ѹֵ
		LCD_ShowxNum(156,130,adcx,1,16,0);//��ʾ��ѹֵ
		
		
		/**************������ֵ����***************/
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
		
		
		/*********************�¶ȶ�ȡ**********************/
		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			temperature=DS18B20_Get_Temp();	
			if(temperature<0)
			{
				LCD_ShowChar(60+40,190,'-',16,0);			//��ʾ����
				temperature=-temperature;					//תΪ����
			}else LCD_ShowChar(60+40,190,' ',16,0);			//ȥ������
			LCD_ShowNum(60+40+8,190,temperature/10,2,16);	//��ʾ��������	    
   		LCD_ShowNum(60+40+32,190,temperature%10,1,16);	//��ʾС������ 		   
		}				   
		
		
		/******************�¶ȳ�����ֵ����************************/
		if((temperature>=threshold_T)){
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
			LCD_ShowString(60,190,200,16,16,"Temp:   . C TempOVER!");
			LCD_ShowNum(60+40+8,190,temperature/10,2,16);	//��ʾ��������	    
   		LCD_ShowNum(60+40+32,190,temperature%10,1,16);	//��ʾС������ 	
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
		
		
		
		/**********************���汨��**************************/
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

 

