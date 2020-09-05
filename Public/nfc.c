#include "stm32f10x.h"
#include "platform_config.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stdarg.h"
#include "printf.h"
#include "ds1302.h"

USART_InitTypeDef USART_InitStructure;

unsigned char temp[50]={0};
unsigned char CardID[4];
unsigned char cardid=0,cardidlast=0;
int time1,time2,time;

unsigned char g_bReceOk=0;      //½ÓÊÕÕýÈ·±êÖ¾
unsigned char g_cReceBuf[50]; //½ÓÊÕÊý¾Ý³¤¶È
unsigned char g_bReceAA=0;
unsigned int  g_cReceNum;     //½ÓÊÕ×Ö½ÚÊý
unsigned int  g_cCommand;     //ÃüÁîÂë
unsigned char rece_data; 
int id=0;


/* Private function prototypes -----------------------------------------------*/
void nfcRCC_Configuration(void);
void nfcGPIO_Configuration(void);
void nfcNVIC_Configuration(void);
void Delay1(__IO uint32_t nCount);
void delay(unsigned int time);
void USART_Config(USART_TypeDef* USARTx);

void nfcinit(void);
void nfcwhile1(void);

unsigned char FindCard(unsigned char mode) ;//Ñ°¿¨º¯Êý
unsigned char Anticoll(void) ;//·À³åÍ»º¯Êý

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStruct;
USART_ClockInitTypeDef USART_ClockInitStruct;


void USART_Config(USART_TypeDef* USARTx){
  USART_InitStructure.USART_BaudRate = 9600;//115200;						//ËÙÂÊ115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//Êý¾ÝÎ»8Î»
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//Í£Ö¹Î»1Î»
  USART_InitStructure.USART_Parity = USART_Parity_No;				//ÎÞÐ£ÑéÎ»
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //ÎÞÓ²¼þÁ÷¿Ø
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½

  /* Configure USART1 */
  USART_Init(USARTx, &USART_InitStructure);							//ÅäÖÃ´®¿Ú²ÎÊýº¯Êý
 
  
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    //Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);						//Ê¹ÄÜ·¢ËÍ»º³å¿ÕÖÐ¶Ï   

  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);	
}

void Delay1(__IO uint32_t nCount)
	
{
  for(; nCount != 0; nCount--);
}


void nfcRCC_Configuration(void)
{
   SystemInit(); 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO  , ENABLE); 
}


void nfcGPIO_Configuration(void)
{
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //¸´ÓÃ¿ªÂ©ÊäÈë
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 //A¶Ë¿Ú 
}


void nfcNVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;  
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//ÉèÖÃ´®¿Ú1ÖÐ¶Ï
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     	//ÇÀÕ¼ÓÅÏÈ¼¶ 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//×ÓÓÅÏÈ¼¶Îª0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//Ê¹ÄÜ
  NVIC_Init(&NVIC_InitStructure);
}


//´®¿Ú·¢ËÍÊý¾Ý
void SendComData(unsigned char *Data,unsigned char Len)
{
	  unsigned char i;
	  for(i=0;i<Len;i++)
	  {
				 USART_SendData(USART1,*Data);
			   while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
				 Data++;
		}	
}


void delay(unsigned int time)
{
	   unsigned int a;
	   for(a=0;a<time;a++);
}

//Ñ°¿¨²Ù×÷
unsigned char FindCard(unsigned char mode) 
{
	  unsigned int i;
		temp[0]=0xAA;//STX1;
		temp[1]=0XBB;//STX2;
		temp[2]=0x06;
		temp[3]=0x00;
		temp[4]=0x00;
		temp[5]=0x00;		
		temp[6]=0x01;
		temp[7]=0x02;
		temp[8]=mode;	
		temp[9]=temp[3]^temp[4]^temp[5]^temp[6]^temp[7]^temp[8];
		SendComData(temp,10);
	  i=2000;
	  while(i)
		{
			if(g_bReceOk==1)
			{
				  if(g_cReceBuf[6]==0x00)
					{
						   g_bReceOk=0;
						   return 0;
          }	
      }
      delay(--i); 			
    }
		g_bReceOk=0; 
    return 1;		
}


//·À³åÍ»²Ù×÷
unsigned char Anticoll(void) 
{
	  unsigned int i;
		temp[0]=0xAA;
		temp[1]=0XBB;
		temp[2]=0x06;
		temp[3]=0x00;
		temp[4]=0x00;
		temp[5]=0x00;
		temp[6]=0x02;
		temp[7]=0x02;
		temp[8]=0x04;
	  temp[9]=temp[3]^temp[4]^temp[5]^temp[6]^temp[7]^temp[8];
		SendComData(temp,10);
	  i=2000;
	  while(i)
		{
			if(g_bReceOk==1)
			{
				  if(g_cReceBuf[6]==0x00)
					{
						   CardID[0]=g_cReceBuf[7];
						   CardID[1]=g_cReceBuf[8];
						   CardID[2]=g_cReceBuf[9];
						   CardID[3]=g_cReceBuf[10];
						   g_bReceOk=0;
						   return 0;
          }	
      }
      delay(--i); 			
    }
		g_bReceOk=0; 
    return 1;		
}


void USART1_IRQHandler(void)      //´®¿Ú1 ÖÐ¶Ï·þÎñ³ÌÐò
{
  unsigned int i,j;
	unsigned char verify = 0;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	   //ÅÐ¶Ï¶Á¼Ä´æÆ÷ÊÇ·ñ·Ç¿Õ
  {	
    
       rece_data = USART_ReceiveData(USART1);   //½«¶Á¼Ä´æÆ÷µÄÊý¾Ý»º´æµ½½ÓÊÕ»º³åÇøÀï
			 if (g_bReceAA)                           //³õÊ¼Îª0
			 {  
					 g_bReceAA = 0;
					 switch (rece_data)
					 {    
						 case 0x00:
									break;
							 case 0xBB:
									g_cReceNum = 0;                //½ÓÊÕ×Ö½ÚÊý
									break;
							 default:
									i = g_cReceNum;
									g_cReceBuf[i] = rece_data;     //½ÓÊÕ×Ö½Ú³¤¶È
									break;
					 }
			 }
			 else
			 {    
					 switch (rece_data)
					 {    
						 case 0xAA:
									g_bReceAA = 1;
							 default:
									i = g_cReceNum++;
									g_cReceBuf[i] = rece_data;
									break;
					 }
			 }
			 
			 i = (((unsigned int)(g_cReceBuf[1]<<8)) + (unsigned int)(g_cReceBuf[0]));
			 if ((g_cReceNum == i + 2) && ( i != 0 )) 
				 {   
							for (j=1; j<g_cReceNum; j++)
							{   verify ^= g_cReceBuf[j];    }
							if (!verify)
						{   
							g_bReceOk  = 1;
							g_cCommand = (((unsigned int)(g_cReceBuf[5]<<8)) + (unsigned int)(g_cReceBuf[4]));
							g_bReceAA  = 0;
						}
					}

					if (g_cReceNum >= sizeof(g_cReceBuf))
					{   g_cReceNum=0;   }
	}

  
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                   //Õâ¶ÎÊÇÎªÁË±ÜÃâSTM32 USART µÚÒ»¸ö×Ö½Ú·¢²»³öÈ¥µÄBUG 
  { 
     USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					     //½ûÖ¹·¢»º³åÆ÷¿ÕÖÐ¶Ï£¬ 
  }	
	
	  
  
}




void nfcinit()
{
	 nfcRCC_Configuration();											  //ÏµÍ³Ê±ÖÓÉèÖÃ
   nfcNVIC_Configuration();											  //ÖÐ¶ÏÔ´ÅäÖÃ	
   nfcGPIO_Configuration();											  //¶Ë¿Ú³õÊ¼»¯
   USART_Config(USART1);											  //´®¿Ú1³õÊ¼»
	// printf_init();
}


void nfcwhile1()
{  	
	if(FindCard(0x52) ==0 && Anticoll()==0)
  {    	
	if(CardID[0]==0x54 && CardID[1]==0xEB && CardID[2]==0xFE && CardID[3]==0xDA)  cardid=1;
	if(CardID[0]==0x34 && CardID[1]==0x2A && CardID[2]==0xFB && CardID[3]==0xDA)  cardid=2;
	if(CardID[0]==0xB4 && CardID[1]==0xA5 && CardID[2]==0x0D && CardID[3]==0xDB)  cardid=3;
	if(CardID[0]==0xD4 && CardID[1]==0x35 && CardID[2]==0x15 && CardID[3]==0xDB)  cardid=4;
	if(CardID[0]==0x24 && CardID[1]==0xD3 && CardID[2]==0x0B && CardID[3]==0xDB)  cardid=5;
	if(CardID[0]==0x74 && CardID[1]==0x00 && CardID[2]==0xF8 && CardID[3]==0xDA)  cardid=6;
	if(CardID[0]==0x84 && CardID[1]==0xB6 && CardID[2]==0x02 && CardID[3]==0xDB)  cardid=7;
	if(CardID[0]==0xD4 && CardID[1]==0x8F && CardID[2]==0x12 && CardID[3]==0xDB)  cardid=8;
	if(CardID[0]==0xA4 && CardID[1]==0x24 && CardID[2]==0xF9 && CardID[3]==0xDA)  cardid=9; 	
  }
	else cardid=0;
	
	if(cardid!=0 && cardidlast==0)
	{
		time1=hour*60+minute;
		time2=0;
		time=0;
	}
 
  if(cardid==0 && cardidlast!=0)
  {
		time2=hour*60+minute;
		time=time2-time1;
		id=(int)cardidlast;
	}	
	
	cardidlast=cardid;
	
	//printf("cardid:%d   time1:%d   time2:%d   time:%d\n\n",cardid,time1,time2,time);
	
	delay(5000000);
}
