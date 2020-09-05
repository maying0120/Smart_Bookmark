#include "system.h"
#include "SysTick.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "printf.h"
#include "ds1302.h"
#include "nfc.h"
#include "SysTick.h"
#include "ku.h"


struct tushu{
  int NO;
  int Day;
  int ReadTime;
  int Start;
};


int nn=1;
struct tushu book[140]={0};
int main(void)  
{ 
		




int Book[9]=  {92,42,12,41,62,0,12,34,12};
int h[9];
double H=0;
int ye=0;

int k;

int w;
int s;
int t=32;
int cardid=0;
int readtime=0;
int day=0;
int start=0;
int a=1,c=480,d=90;
int m;
int g=0;
int kai=1;
int guan=0;
int qie;
int qing;
a=(a-1)*16+16;
c=(c-(8*60))/12+21;
d=d/12;

	GPIO_Configuration1();
GPIO_Configuration2();
OLED_Init();

SysTick_Init(72000);
  SystemInit(); 	
	
 printf_init();	
ds1302init();
nfcinit();

Fill_RAM(0x00,0x00);	
	while(1) 
	{
		readtime=time;
		start=time1;
		
		ds1302while1();
		nfcwhile1();
		if(nn>140) nn=140;
		if(week==1 && hour>=0 && hour<=6)
		{
			for(qing=0;qing<140;qing++)
			   {
					book[qing].NO=0;
					book[qing].ReadTime=0;
				  book[qing].Day=0;
				  book[qing].Start=0;
					 
				 }
		}
		if(day==1 && hour>=0 && hour<=6)
		{
			for(qing=0;qing<9;qing++)
			{
				Book[qing]=0;
				h[qing]=0;
			}
			H=0;
		}
		switch(id){
	
  case 0:
		break;
	case 1:
		      Book[0]+=readtime;
					book[nn].NO=1;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	        nn+=1;
	        id=0;
				break;
	case 2:
		      Book[1]+=readtime;
					book[nn].NO=2;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
					
	case 3:
		Book[2]+=readtime;
					book[nn].NO=3;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
  case 4:
		Book[3]+=readtime;
					book[nn].NO=4;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
	case 5:	
		Book[4]+=readtime;
					book[nn].NO=5;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
  case 6:
		Book[5]+=readtime;
					book[nn].NO=6;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
  case 7:
		Book[6]+=readtime;
					book[nn].NO=7;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
	case 8:
		Book[7]+=readtime;
					book[nn].NO=8;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
	case 9:
		Book[8]+=readtime;
					book[nn].NO=9;
				  book[nn].ReadTime=readtime/12+1;
				  book[nn].Day=(week-1)*16+16;
				  book[nn].Start=(start-8*60)/12+21;
	nn+=1;
	 id=0;
				break;
			}
			
	//////////////////////////////////////	

for(m=0;m<9;m++)
{
	if(H<=Book[m]) H=Book[m];
	else continue;	
}
for(w=0;w<9;w++)
 {
	if(Book[w]!=0)
	{
		h[w]=(int)(((Book[w]/H)*80)+1);
	}	
	else h[w]=0;
 }

	///////////////////////////////显示：图二////////////////////////////////////
if((~GPIOF->IDR)& GPIO_IDR_IDR0)
{
	
	 if(g==0){
		      kaiji();
		      Delay(10);
		 	//Fill_RAM(0x00,0x00);
		// Show_64k_Pattern(gImage_zhuangshi,0x00,0x7F,0x4B,0x7F);
		qieping();
					}
	  g=1;

if((~GPIOE->IDR)& GPIO_IDR_IDR2)
	{
		if(ye!=1) 
	 {
		    ye=1;
				qieping();	
				
	 }		
			
switch(month)
	 {
  
	case 1:
				display_Chinese1616(0x10 , 0x00, 0, 0x1f);
				break;
	case 2:
				display_Chinese1616(0x10 , 0x00, 1, 0x1f);
				break;
					
	case 3:
					display_Chinese1616(0x10 , 0x00, 2 , 0x7e0);
				break;
  case 4:
          display_Chinese1616(0x10 , 0x00, 3, 0x7e0);
				break;
	case 5:	
					display_Chinese1616(0x10 , 0x00, 4, 0x7e0);
				break;
  case 6:
					display_Chinese1616(0x10 , 0x00, 5, 0xfff3);
				break;
  case 7:
				display_Chinese1616(0x10 , 0x00, 6, 0xfff3);
				break;
	case 8:
				display_Chinese1616(0x10 , 0x00, 7, 0xfff3);
				break;
	case 9:
				display_Chinese1616(0x10 , 0x00, 8, 0xff80);
				break;
	case 10:
					display_Chinese1616(0x10 , 0x00, 9, 0xff80);
				break;
	case 11:
					display_Chinese1616(0x00 , 0x00,9 , 0xff80);
	        display_Chinese1616(0x10 , 0x00,0 , 0xff80);
				break;
	case 12:
				display_Chinese1616(0x00 , 0x00, 9, 0x1f);
	      display_Chinese1616(0x10 , 0x00, 1, 0x1f);
				break;
	 }
		
			 display_Chinese1616(0x20 , 0x00, 10, 0xffff);
			 display_Chinese1616(0x30 , 0x00, 11, 0xffff);
			 display_Chinese1616(0x40 , 0x00, 12, 0xffff);
			 display_Chinese1616(0x50 , 0x00, 13, 0xffff);
			 display_Chinese1616(0x60 , 0x00, 14, 0xffff);
			  
			  
			  Fill_Block((u16)(5),(u16)(h[0]+5),(u16)(20),(u16)(28),0xF8,0x1F);			  
		OLED_ShowNum((u16)(h[0]+7),(u16)(20),Book[0],3,8,0xf81f);
	  		Fill_Block((u16)(5),(u16)(h[1]+5),(u16)(30),(u16)(38),0x07,0xE0);
		OLED_ShowNum((u16)(h[1]+7),(u16)(30),Book[1],3,8,0x07e0);
	  		Fill_Block((u16)(5),(u16)(h[2]+5),(u16)(40),(u16)(48),0xCE,0x7F);
		OLED_ShowNum((u16)(h[2]+7),(u16)(40),Book[2],3,8,0xce7f);
	  		Fill_Block((u16)(5),(u16)(h[3]+5),(u16)(50),(u16)(58),0xF4,0x00);
		OLED_ShowNum((u16)(h[3]+7),(u16)(50),Book[3],3,8,0xf400);
	  		Fill_Block((u16)(5),(u16)(h[4]+5),(u16)(60),(u16)(68),0xF6,0xF9);
		OLED_ShowNum((u16)(h[4]+7),(u16)(60),Book[4],3,8,0xf6f9);
	  		Fill_Block((u16)(5),(u16)(h[5]+5),(u16)(70),(u16)(78),0x0F,0xF7);
	  OLED_ShowNum((u16)(h[5]+7),(u16)(70),Book[5],3,8,0x0ff7);
	  		Fill_Block((u16)(5),(u16)(h[6]+5),(u16)(80),(u16)(88),0xF0,0x0F);
		OLED_ShowNum((u16)(h[6]+7),(u16)(80),Book[6],3,8,0xf00f);
	  		Fill_Block((u16)(5),(u16)(h[7]+5),(u16)(90),(u16)(98),0xBF,0xEB);
		OLED_ShowNum((u16)(h[7]+7),(u16)(90),Book[7],3,8,0xbfeb);
	   	  Fill_Block((u16)(5),(u16)(h[8]+5),(u16)(100),(u16)(108),0xC4,0x40);
	  OLED_ShowNum((u16)(h[8]+7),(u16)(100),Book[8],3,8,0xc440);
			

	}
	////////////////////////////////////显示：图一//////////////////////////////
	
else
	{
		  if(ye!=2) 
   {	
		 
		 ye=2;

			qieping();
					
   }

/////////////////////////////biaoge/////////////////////////////
  
	Show_biaoge();
///////////////////////////////////////////////////		
 // Fill_Block((u16)a,(u16)(a+14),(u16)c,(u16)(c+d),0xC4,0x40);
	  Fill_Block((u16)(16),(u16)(16+14),(u16)(21),(u16)(26),0xF8,0x1F);
	  Fill_Block((u16)(16),(u16)(16+14),(u16)(30),(u16)(38),0x07,0xE0);
	  Fill_Block((u16)(16),(u16)(16+14),(u16)(58),(u16)(67),0xF4,0x00);
	  Fill_Block((u16)(16),(u16)(16+14),(u16)(69),(u16)(78),0x0F,0xF7);
    Fill_Block((u16)(16),(u16)(16+14),(u16)(80),(u16)(86),0xF8,0x1F);
	  Fill_Block((u16)(32),(u16)(32+14),(u16)(36),(u16)(38),0x07,0xE0);
	  Fill_Block((u16)(32),(u16)(32+14),(u16)(53),(u16)(62),0xFE,0xEF);
	  Fill_Block((u16)(32),(u16)(32+14),(u16)(69),(u16)(78),0x0F,0xF7);
	  Fill_Block((u16)(48),(u16)(48+14),(u16)(30),(u16)(36),0xF8,0x1F);
	  Fill_Block((u16)(48),(u16)(48+14),(u16)(36),(u16)(40),0xF6,0xF9);
	  Fill_Block((u16)(48),(u16)(48+14),(u16)(53),(u16)(62),0xF4,0x00);
	  Fill_Block((u16)(48),(u16)(48+14),(u16)(69),(u16)(78),0xC4,0x40);
	  Fill_Block((u16)(64),(u16)(64+14),(u16)(21),(u16)(26),0xBF,0xEB);
	  Fill_Block((u16)(64),(u16)(64+14),(u16)(30),(u16)(38),0x07,0xE0);
	  Fill_Block((u16)(64),(u16)(64+14),(u16)(58),(u16)(67),0x0F,0xF7);
for(s=0;s<nn;s++)
		{
	
switch(book[s].NO)
	  {
	case 1:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0xF8,0x1F);
	  break;
	case 2:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0x07,0xE0);
	  break;
	case 3:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0xCE,0x7F );
	  break;
	case 4:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0xF4,0x00);
	  break;
	case 5:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0xF6,0xF9);
	  break;
	case 6:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0x0F,0xF7);
	  break;
	case 7:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0xFE,0xEF);
	  break;
	case 8:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0xBF,0xEB);
	  break;
	case 9:
		Fill_Block((u16)(book[s].Day),(u16)(book[s].Day+14),(u16)(book[s].Start),(u16)(book[s].Start+book[s].ReadTime),0xC4,0x40);
	  break;
	
   }
	 }

	}

 }
else  
	{
  		Fill_RAM(0x00,0x00);
					g=0;
		
	}
	}
}  



