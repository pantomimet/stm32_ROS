#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED 端口定义
#define LED PBout(13)  //LED控制引脚
#define Red_LED_on GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define Red_LED_off GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define Green_LED_on GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define Green_LED_off GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define Yellow_LED_on GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define Yellow_LED_off GPIO_SetBits(GPIOB,GPIO_Pin_4)
void LED_Init(void);  //初始化
void Led_Flash(u16 time);
void Light_KEY_init(void);
void Beep_init(void);
void Beep_on(void);
void Beep_off(void);
#endif
