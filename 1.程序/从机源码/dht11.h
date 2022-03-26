#ifndef _DHT11_H_
#define _DHT11_H_

#include "main.h"

sbit  P2_0  = P2^5;     //传感器1
sbit  P0_6  = P2^4 ;     //传感器2

void RH(void);
void  COM(void);
void RH2(void);
void  COM2(void);
void GetTemp1(void);
void GetTemp2(void);


#endif

