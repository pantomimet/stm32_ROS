#ifndef _MYIIC_H_
#define _MYIIC_H_
#include "sys.h"

#define SDA_IN() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

#define IIC_SCL PBout(10)//SCL
#define IIC_SDA PBout(11)//SDA
#define READ_SDA PBin(11)//输入SDA

//IIC所有操作
void IIC_Init(void);//初始化I2C的IO口
void IIC_Start(void);//发送开始数据
void IIC_Stop(void);//发送停止数据
void IIC_Send_Byte(u8 txd);//发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//读取一个字节
u8 IIC_Wait_Ack(void);//等待ACK信号
void IIC_Ack(void);//发送ACK信号
void IIC_NAck(void);//不发送ACK信号
void IIC_Write_One_Byte(u8 daddr,u8 add,u8 data);//写一个字节
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);//读一个字节

#endif
