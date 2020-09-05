#include "printf.h"
int fputc(int ch,FILE *p)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==Bit_RESET);
	return ch;	
}

void printf_init()	 //printf初始化函数
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO , ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;		 //tx
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;		//rx
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

 
	USART_InitStructure.USART_BaudRate = 9600; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; 	 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC); 
	
	/* 设置NVIC参数 */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	      //打开EXTI2的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		    //响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		      //使能
	NVIC_Init(&NVIC_InitStructure);	
}
