#include "stm32f10x.h"
#include "ds1302.h"
#include "SysTick.h"
#include "printf.h"


#define NOP() __NOP

#define DS1302_CLK_H()	(GPIOB->BSRR=GPIO_Pin_12)
#define DS1302_CLK_L()	(GPIOB->BRR=GPIO_Pin_12)

#define DS1302_RST_H()	(GPIOB->BSRR=GPIO_Pin_14)
#define DS1302_RST_L()	(GPIOB->BRR=GPIO_Pin_14)

#define DS1302_OUT_H()	(GPIOB->BSRR=GPIO_Pin_13)
#define DS1302_OUT_L()	(GPIOB->BRR=GPIO_Pin_13)
											
#define DS1302_IN_X		(GPIOB->IDR&GPIO_Pin_13)

#define Time_24_Hour	0x00	//24ʱ�ƿ���	
#define Time_Start		0x00	//��ʼ��ʱ

#define DS1302_SECOND	0x80	//DS1302���Ĵ������������
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_WEEK		0x8A  
#define DS1302_YEAR		0x8C
#define DS1302_WRITE	0x8E
#define DS1302_POWER	0x90


unsigned char tt[7];
unsigned char year,month,day,week,hour,minute,second;

static GPIO_InitTypeDef GPIO_InitStructure;

void DS1302_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* PE4,5,6��� */
	GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


static void DelayNOP(u32 count)
{
	while(count--) ;
}

static void DS1302_OUT(void)
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void DS1302_IN(void)
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS1302SendByte(u8 byte)
{
	u8	i;

	for(i=0x01;i;i<<=1)
	{
		if(byte&i)	DS1302_OUT_H();
		else	DS1302_OUT_L();
		DS1302_CLK_H();
		DelayNOP(50);		//����ʱ
		DS1302_CLK_L();
	}
}


u8 DS1302ReceiveByte(void)
{
	u8	i,byte=0;

	for(i=0x01;i;i<<=1)
	{
		if(DS1302_IN_X)	byte |= i;
		DS1302_CLK_H();
		DelayNOP(50);		//����ʱ
		DS1302_CLK_L();
	}
	return(byte);
}


void Write1302(u8 addr,u8 data)
{
  DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(addr);
	DS1302SendByte(data);
	DelayNOP(100);
	DS1302_RST_L();
}

u8 Read1302(u8 addr)
{
    u8 data=0;

  DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(addr|0x01);
	DS1302_IN();
	data = DS1302ReceiveByte();
	DelayNOP(100);
	DS1302_RST_L();
	return(data);
}

//��ȡʱ�亯��
void DS1302_GetTime(u8 *time)
{
	time[0] = Read1302(DS1302_YEAR);
	time[1] = Read1302(DS1302_WEEK);
	time[2] = Read1302(DS1302_MONTH);
	time[3] = Read1302(DS1302_DAY);
	time[4] = Read1302(DS1302_HOUR);
	time[5] = Read1302(DS1302_MINUTE);
	time[6] = Read1302(DS1302_SECOND);	
}

/*
��ȡDS1302�е�RAM
addr:��ַ,��0��30,��31���ֽڵĿռ�
����Ϊ����ȡ������
*/
u8 ReadDS1302Ram(u8 addr)
{
	u8	tmp,res;

	tmp = (addr<<1)|0xc0;
	res = Read1302(tmp);
	return(res);
}

/*
дDS1302�е�RAM
addr:��ַ,��0��30,��31���ֽڵĿռ�
data:Ҫд������
*/
void WriteDS1302Ram(u8 addr,u8 data)
{
	u8	tmp;

	Write1302(DS1302_WRITE,0x00);		//�ر�д����
	tmp = (addr<<1)|0xc0;
	Write1302(tmp,data);
	Write1302(DS1302_WRITE,0x80);		//��д����
}

void ReadDSRam(u8 *p,u8 add,u8 cnt)
{
	u8 i;
	
	if(cnt>30) return;
	for(i=0;i<cnt;i++)
	{
		*p = ReadDS1302Ram(add+1+i);
		p++;
	}
}

void WriteDSRam(u8 *p,u8 add,u8 cnt)
{
	u8 i;
	
	if(cnt>30) return;
	for(i=0;i<cnt;i++)
	{
		WriteDS1302Ram(add+1+i,*p++);
	}
}
  
/*
��ʱ�亯��,˳��Ϊ:��������ʱ����
*/
void ReadDS1302Clock(u8 *p)
{
	DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(0xbf);			//ͻ��ģʽ
	DS1302_IN();
	p[6] = DS1302ReceiveByte();		//��
	p[5] = DS1302ReceiveByte();		//��
	p[4] = DS1302ReceiveByte();		//ʱ
	p[3] = DS1302ReceiveByte();		//��
	p[2] = DS1302ReceiveByte();		//��
	p[1] = DS1302ReceiveByte();	  //��
	p[0] = DS1302ReceiveByte();		//��
	DS1302ReceiveByte();			//������־�ֽ�
	DelayNOP(100);
	DS1302_RST_L();
}

/*
дʱ�亯��,˳��Ϊ:��������ʱ����
*/
void WriteDS1302Clock(u8 *p)
{
	Write1302(DS1302_WRITE,0x00);		//�ر�д����
	DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(0xbe);				//ͻ��ģʽ
	DS1302SendByte(p[6]);				//��
	DS1302SendByte(p[5]);				//��
	DS1302SendByte(p[4]);				//ʱ
	DS1302SendByte(p[3]);				//��
	DS1302SendByte(p[2]);				//��
	DS1302SendByte(p[1]);				//�ܣ����ó���һ��û��ʹ��
	DS1302SendByte(p[0]);				//��
	DS1302SendByte(0x80);				//������־�ֽ�
	DelayNOP(100);
	DS1302_RST_L();
}



void InitClock(void)
{
	u8	tmp;
	DS1302_Configuration();
	tmp = ReadDS1302Ram(0);
	if(tmp^0xa5)
	{
		WriteDS1302Ram(0,0xa5);
		Write1302(DS1302_WRITE,0x00);		//�ر�д����
		Write1302(0x90,0x03);				//��ֹ������
		Write1302(DS1302_HOUR,0x00);		//���ó�24Сʱ��
		Write1302(DS1302_SECOND,0x00);		//ʹ��ʱ������
		Write1302(DS1302_WRITE,0x80);		//��д����
	}
}


void TestDS1302(void)
{
	u8 i,tt[7],dd1[30],dd2[30];
	
	DS1302_Configuration();
	InitClock();
	tt[0] = 0x17;
	tt[1] = 0x08;
	tt[2] = 0x24;
	tt[3] = 0x15;
	tt[4] = 0x06;
	tt[5] = 0x00;
	WriteDS1302Clock(tt);
	for(i=0;i<30;i++)
	{
		dd1[i] = i;
		dd2[i] = 0;
	}
	WriteDSRam(dd1,0,30);
	ReadDSRam(dd2,0,30);
	while(1) 
	{
		ReadDS1302Clock(tt);
		printf("%d",tt[5]);
		delay_ms(1000);
		
	}
}
#define DS1302_SECOND	0x80	//DS1302���Ĵ������������
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_WEEK		0x8A  
#define DS1302_YEAR		0x8C
#define DS1302_WRITE	0x8E
#define DS1302_POWER	0x90

void ds1302init(void)
{	
	InitClock();
  /*if(Read1302(0x86)==0x14 && Read1302(0x84)==0x13 && Read1302(0x82)==0x30 && Read1302(0x80)==0x00)
	{	
	*/
	tt[0] = 0x17;    //���趨
	tt[1] = 0x06;    //���趨
	tt[2] = 0x10;    //���趨
	tt[3] = 0x14;    //���趨
	tt[4] = 0x13;    //ʱ�趨
	tt[5] = 0x40;    //���趨
	tt[6] = 0x00;    //���趨
	//}
	WriteDS1302Clock(tt);
}
	

void	ds1302while1(void) 
	{
		ReadDS1302Clock(tt);	
//	printf("20%d%d��%d%d��%d%d�� ����%d   %d%d : %d%d : %d%d \n",tt[0]/16,tt[0]%16,tt[2]/16,tt[2]%16,tt[3]/16,tt[3]%16,tt[1],tt[4]/16,tt[4]%16,tt[5]/16,tt[5]%16,tt[6]/16,tt[6]%16);
//  delay_ms(1000);
		year   = tt[0]/16*10+tt[0]%16;
		month  = tt[2]/16*10+tt[2]%16;
		day    = tt[3]/16*10+tt[3]%16;
		week   = tt[1];
		hour   = tt[4]/16*10+tt[4]%16;
		minute = tt[5]/16*10+tt[5]%16;
		second = tt[6]/16*10+tt[6]%16;
	}		
