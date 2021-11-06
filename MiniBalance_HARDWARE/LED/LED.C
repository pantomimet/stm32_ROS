#include "led.h"

/**************************************************************************
函数功能：LED接口初始化
入口参数：无 
返回  值：无
**************************************************************************/
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //使能端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	            //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	            //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	            //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOC, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	            //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
  GPIO_SetBits(GPIOB,GPIO_Pin_4);
  GPIO_SetBits(GPIOC,GPIO_Pin_4);
  GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

void Light_KEY_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
}


void Beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}
void Beep_on(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

void Beep_off(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}
/**************************************************************************
函数功能：LED闪烁
入口参数：闪烁频率 
返回  值：无
**************************************************************************/
void Led_Flash(u16 time)
{
	  static int temp;
	  if(0==time) LED=0;
	  else		if(++temp==time)	LED=~LED,temp=0;
}
