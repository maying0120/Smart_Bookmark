#ifndef DS1302_H
#define DS1302_H

void InitClock(void);
void ReadDS1302Clock(u8 *p);
extern void WriteDS1302Clock(u8 *p);
extern void ReadDSRam(u8 *p,u8 add,u8 cnt);
extern void WriteDSRam(u8 *p,u8 add,u8 cnt);
extern void TestDS1302(void);
extern void ds1302init(void);
extern void ds1302while1(void);

extern unsigned char year,month,day,week,hour,minute,second;

#endif

