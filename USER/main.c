//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2018-10-31
//  ����޸�   : 
//  ��������   : OLED I2C�ӿ���ʾ����(STM32F103ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND  ��Դ��
//              VCC  3.3v��Դ
//              D0   PA0��SCL��
//              D1   PA1��SDA��
//              RES  PA2��SPIģ��ĳ�IICģ����Ҫ�Ӵ����ţ�IICģ���û�����ԣ�
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2018-10-31
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
				
				//OLED_ShowPicture(0,0,64,64,BMP3,1); //�¶ȼ�
				//OLED_ShowChinese(70,20,0,16,1);//��
				//OLED_ShowChinese(90,20,1,16,1);//��
				//OLED_ShowChinese(70,40,2,16,1);//ģ
				//OLED_ShowChinese(90,40,3,16,1);//ʽ	
				OLED_Refresh();
				break;
			}
			case 2:{
				OLED_ShowPicture(0,0,64,64,BMP4,1); //ʱ��
				OLED_ShowChinese(70,20,4,16,1);//ʱ
				OLED_ShowChinese(90,20,5,16,1);//��
				OLED_ShowChinese(70,40,2,16,1);//ģ
				OLED_ShowChinese(90,40,3,16,1);//ʽ	
				OLED_Refresh();
				break;
			}
			case 3:{
				OLED_ShowPicture(0,0,64,64,BMP5,1); //����
				OLED_ShowChinese(70,20,6,16,1);//��
				OLED_ShowChinese(90,20,7,16,1);//��
				OLED_ShowChinese(70,40,2,16,1);//ģ
				OLED_ShowChinese(90,40,3,16,1);//ʽ	
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
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
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
		key=KEY_Scan(0);   //ɨ�谴��
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
		OLED_ShowChinese(70,20,0,16,1);//��
		OLED_ShowChinese(90,20,1,16,1);//��
		OLED_ShowChinese(70,40,2,16,1);//ģ
		OLED_ShowChinese(90,40,3,16,1);//ʽ		
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,11,16,1);//��
		OLED_ShowChinese(18,0,12,16,1);//��
		OLED_ShowChinese(36,0,13,16,1);//԰
		//OLED_ShowChinese(54,0,11,16,1);//��
		OLED_ShowChinese(72,0,14,16,1);//��
		OLED_ShowChinese(90,0,15,16,1);//��
		OLED_ShowChinese(108,0,16,16,1);//��
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
		OLED_ShowString(20,32,"2014/05/01",16,1);
		OLED_ShowString(0,48,"��",16,1);  
		OLED_ShowString(63,48,"CODE:",16,1);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16,1);  //16*16 ��
	  OLED_ShowChinese(16,0,0,24,1); //24*24 ��
		OLED_ShowChinese(24,20,0,32,1);//32*32 ��
	  OLED_ShowChinese(64,0,0,64,1); //64*64 ��
		OLED_Refresh();
	  delay_ms(500);
  	OLED_Clear();
		OLED_ShowString(0,0,"ABC",8,1);//6*8 ��ABC��
		OLED_ShowString(0,8,"ABC",12,1);//6*12 ��ABC��
	  OLED_ShowString(0,20,"ABC",16,1);//8*16 ��ABC��
		OLED_ShowString(0,36,"ABC",24,1);//12*24 ��ABC��
	  OLED_Refresh();
		delay_ms(500);
		OLED_ScrollDisplay(11,4,1);
		*/
	}
}

