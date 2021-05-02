#ifndef _MYIIC_H_
#define _MYIIC_H_
#include "sys.h"

#define SDA_IN() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

#define IIC_SCL PBout(10)//SCL
#define IIC_SDA PBout(11)//SDA
#define READ_SDA PBin(11)//����SDA

//IIC���в���
void IIC_Init(void);//��ʼ��I2C��IO��
void IIC_Start(void);//���Ϳ�ʼ����
void IIC_Stop(void);//����ֹͣ����
void IIC_Send_Byte(u8 txd);//����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//��ȡһ���ֽ�
u8 IIC_Wait_Ack(void);//�ȴ�ACK�ź�
void IIC_Ack(void);//����ACK�ź�
void IIC_NAck(void);//������ACK�ź�
void IIC_Write_One_Byte(u8 daddr,u8 add,u8 data);//дһ���ֽ�
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);//��һ���ֽ�

#endif
