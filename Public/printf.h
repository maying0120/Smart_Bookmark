#ifndef _printf_H
#define _printf_H
#include "stm32f10x.h"
#include "stdio.h"

void printf_init(void);	//printf��ʼ������
int fputc(int ch,FILE *p);

#endif
