//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              SCL   ��PD6��SCL��
//              SDA   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/


#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    

//OLEDģʽ����
//0:4�ߴ���ģʽ

//#define OLED_MODE 1
		    						  
//-----------------OLED�˿ڶ���----------------  					   

//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_8)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_8)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_10)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_10)

#define OLED_RS_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_9)//DC
#define OLED_RS_Set() GPIO_SetBits(GPIOD,GPIO_Pin_9)



#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_12)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOD,GPIO_Pin_12)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_11)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD,GPIO_Pin_11)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
//OLED�����ú���






#endif  
	 



