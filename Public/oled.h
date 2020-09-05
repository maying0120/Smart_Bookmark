//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              SCL   接PD6（SCL）
//              SDA   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/


#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    

//OLED模式设置
//0:4线串行模式

//#define OLED_MODE 1
		    						  
//-----------------OLED端口定义----------------  					   

//-----------------OLED端口定义----------------  					   
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

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数






#endif  
	 



