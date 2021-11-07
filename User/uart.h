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
void usart3_send(void);
void USART3_IRQHandler(void);

extern u8 TX_BUF[16];
extern u8 RX_BUF[16];
extern u8 car1_cmd;
#endif
