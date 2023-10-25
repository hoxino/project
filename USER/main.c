//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-10-31
//  最近修改   : 
//  功能描述   : OLED I2C接口演示例程(STM32F103系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND  电源地
//              VCC  3.3v电源
//              D0   PA0（SCL）
//              D1   PA1（SDA）
//              RES  PA2（SPI模块改成IIC模块需要接此引脚，IIC模块用户请忽略）
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2018-10-31
//All rights reserved
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "led.h"
#include "key.h"
#include "DHT11.h"
#include "stdio.h"


int menuStatus = 1;
unsigned int uiDht_Val;
unsigned char Dht_Hmd[21];
unsigned char Dht_Temp[21];

extern int vUSART_PPM;
void display()
{
		switch(menuStatus){
			case 1:{
				
				//OLED_ShowPicture(0,0,64,64,BMP3,1); //温度计
				//OLED_ShowChinese(70,20,0,16,1);//温
				//OLED_ShowChinese(90,20,1,16,1);//度
				//OLED_ShowChinese(70,40,2,16,1);//模
				//OLED_ShowChinese(90,40,3,16,1);//式	
				OLED_Refresh();
				break;
			}
			case 2:{
				OLED_ShowPicture(0,0,64,64,BMP4,1); //时钟
				OLED_ShowChinese(70,20,4,16,1);//时
				OLED_ShowChinese(90,20,5,16,1);//钟
				OLED_ShowChinese(70,40,2,16,1);//模
				OLED_ShowChinese(90,40,3,16,1);//式	
				OLED_Refresh();
				break;
			}
			case 3:{
				OLED_ShowPicture(0,0,64,64,BMP5,1); //闹铃
				OLED_ShowChinese(70,20,6,16,1);//闹
				OLED_ShowChinese(90,20,7,16,1);//铃
				OLED_ShowChinese(70,40,2,16,1);//模
				OLED_ShowChinese(90,40,3,16,1);//式	
				OLED_Refresh();
				break;
			}
	}
}
void DHT_Proc(void)
{
	
			uiDht_Val = dht11_read();
			sprintf((char *)Dht_Hmd,"Hmd : %2d.%d%%",uiDht_Val >> 24,(uiDht_Val >> 16)&0xff);
			OLED_ShowString(0,0,Dht_Hmd,16,1);
			sprintf((char *)Dht_Temp,"Temp: %2d.%dC",(uiDht_Val >> 8)&0xff,uiDht_Val&0xff);
			OLED_ShowString(0,16,Dht_Temp,16,1);		
	
}

int main(void)
{
	u8 key;
	u8 i=0;
	int OLED_PPM;
	uint8_t TEMP_I=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	uart_init();
	delay_init();
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	OLED_PPM=0;
	SysTick_Init(72);
	LED_Init();
	KEY_Init();
	
	display();
	
	while(1)
	{
		if(OLED_PPM!=vUSART_PPM)
		{
			OLED_PPM=vUSART_PPM;
		}
		OLED_ShowString(0,32,"PPM :",16,1);
		OLED_ShowNum(48,32,vUSART_PPM,3,16,1);
		OLED_ShowString(72,32,"um/m^3",16,1);		
		i++;
		if(i%100==0)
		{
			 DHT_Proc();	 
		}
		display();
		key=KEY_Scan(0);   //扫描按键
		switch(key)
		{
			case KEY1_VALUE: {
				led1=~led1;
				if(menuStatus<3)
					menuStatus++;
				else
					menuStatus=1;
				display();
				break; 
			}
			delay_ms(10);
			//case KEY2_VALUE: led1=1;menuStatus=1;display();break;    
		}
		
		delay_ms(1);	
		/*
		OLED_ShowPicture(0,0,64,64,BMP3,1);
		OLED_ShowChinese(70,20,0,16,1);//温
		OLED_ShowChinese(90,20,1,16,1);//度
		OLED_ShowChinese(70,40,2,16,1);//模
		OLED_ShowChinese(90,40,3,16,1);//式		
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,11,16,1);//中
		OLED_ShowChinese(18,0,12,16,1);//景
		OLED_ShowChinese(36,0,13,16,1);//园
		//OLED_ShowChinese(54,0,11,16,1);//电
		OLED_ShowChinese(72,0,14,16,1);//子
		OLED_ShowChinese(90,0,15,16,1);//技
		OLED_ShowChinese(108,0,16,16,1);//术
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
		OLED_ShowString(20,32,"2014/05/01",16,1);
		OLED_ShowString(0,48,"大",16,1);  
		OLED_ShowString(63,48,"CODE:",16,1);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16,1);  //16*16 中
	  OLED_ShowChinese(16,0,0,24,1); //24*24 中
		OLED_ShowChinese(24,20,0,32,1);//32*32 中
	  OLED_ShowChinese(64,0,0,64,1); //64*64 中
		OLED_Refresh();
	  delay_ms(500);
  	OLED_Clear();
		OLED_ShowString(0,0,"ABC",8,1);//6*8 “ABC”
		OLED_ShowString(0,8,"ABC",12,1);//6*12 “ABC”
	  OLED_ShowString(0,20,"ABC",16,1);//8*16 “ABC”
		OLED_ShowString(0,36,"ABC",24,1);//12*24 “ABC”
	  OLED_Refresh();
		delay_ms(500);
		OLED_ScrollDisplay(11,4,1);
		*/
	}
}

