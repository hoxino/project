#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
 
u8 vUSART_Data[8];    //用来储存八位数据
u8 vUSART_CL;         //函数计算位
unsigned int vUSART_PPM;       //PPM的值

void uart_init(u32 bound){

	NVIC_InitTypeDef NVIC_InitTypeDefs;
	USART_InitTypeDef USART_InitTypeDefs;
	GPIO_InitTypeDef GPIO_InitTypeDefs;
	USART_TypeDef USART_TypeDefs;
	//PA9,10??????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//GPIOA????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    //GPIOA PA10,9???????USART1,??1
      //PA9
	GPIO_InitTypeDefs.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitTypeDefs.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitTypeDefs.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefs);
	  //PA10
	GPIO_InitTypeDefs.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //接收引脚设置为浮空输入模式
	GPIO_InitTypeDefs.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitTypeDefs.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitTypeDefs);
	//???????
	USART_InitTypeDefs.USART_BaudRate=9600;
	USART_InitTypeDefs.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  //?????,??????
	USART_InitTypeDefs.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;      //????????
	USART_InitTypeDefs.USART_Parity=USART_Parity_No;       // ?????
	USART_InitTypeDefs.USART_StopBits=USART_StopBits_1;    //??????1
	USART_InitTypeDefs.USART_WordLength=USART_WordLength_8b; //????8
	USART_Init(USART1,&USART_InitTypeDefs);
	//????
	USART_Cmd(USART1,ENABLE);
	//??????????
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //???????,??????????????
	//???????????????????
	NVIC_InitTypeDefs.NVIC_IRQChannel=USART1_IRQn;          
	NVIC_InitTypeDefs.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_InitTypeDefs.NVIC_IRQChannelPreemptionPriority=0; 
	NVIC_InitTypeDefs.NVIC_IRQChannelSubPriority=0;   
	NVIC_Init(&NVIC_InitTypeDefs);

}
#endif

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
u8 res;
	static int i=0; 
	static u8 USART_i; 
	float PPM;
	unsigned int PPB;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); //?????,???????,????????
		res=USART_ReceiveData(USART1);
		if(res==0xFF)  USART_i=1;
		if((USART_i==1)&&(res != 0xFF))
		{
			vUSART_Data[i]=res;
			i++;
			if(i==8) 
			{
				i=0;
				USART_i=0;
				vUSART_CL=1;
			}
			if(vUSART_CL)
			{
				PPB=(vUSART_Data[3]*256)+vUSART_Data[4];
				PPM=PPB/1000.0;  
				PPM *= 1.25;         
				vUSART_PPM=PPM*1000; 
				vUSART_CL=0;
			}
		}
	}

}

