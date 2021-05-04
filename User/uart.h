#ifndef __UART_H
#define __UART_H

#include "sys.h"

void usart1_init(u32 bound);
void USART_TX(void);
void USART1_IRQHandler(void);


#endif
