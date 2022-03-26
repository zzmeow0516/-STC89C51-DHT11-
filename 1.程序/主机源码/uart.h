#ifndef _UART_H_
#define _UART_H_

#include "reg52.h"

void UartInit(void);
void Send_string(unsigned char *c);
void Send_Data(unsigned char DAT);

#endif


