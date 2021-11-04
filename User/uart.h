#ifndef __UART_H
#define __UART_H

#include "sys.h"

void usart1_init(u32 bound);
void USART_TX(void);
void USART1_IRQHandler(void);
void usart2_init(u32 bound);
void USART2_IRQHandler(void);
void USART2_TX(void);
void usart3_init(u32 bound);
void usart3_send(u8 data);
void USART3_IRQHandler(void);


#endif
