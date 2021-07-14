#ifndef __MPU9250_H__
#define __MPU9250_H__

#include "sys.h"
#include "delay.h"
#include "myiic.h"

#define ACCEL_ADDRESS 	0xD0
#define GYRO_ADDRESS 	0xD0
#define MAG_ADDRESS 	0x18
#define SMPLRT_DIV		0X19
#define CONFIG			0X1A
#define GYRO_CONFIG		0X1B
#define ACCEL_CONFIG	0X1C
#define ACCEL_CONFIG2	0X1D
#define WHO_AM_I 		0x75
#define PWR_MGMT_1		0x6B
#define INT_PIN_CFG 	0x37

//加速度地址
#define ACCEL_XOUT_H	0X3B
#define ACCEL_XOUT_L	0X3C
#define ACCEL_YOUT_H	0X3D
#define ACCEL_YOUT_L	0X3E
#define ACCEL_ZOUT_H	0X3F
#define ACCEL_ZOUT_L	0X40
//角速度地址
#define GYRO_XOUT_H	0X43
#define GYRO_XOUT_L	0X44
#define GYRO_YOUT_H	0X45
#define GYRO_YOUT_L	0X46
#define GYRO_ZOUT_H	0X47
#define GYRO_ZOUT_L	0X48
//磁力计地址
#define MAG_XOUT_L	0x03
#define MAG_XOUT_H	0x04
#define MAG_YOUT_L	0x05
#define MAG_YOUT_H	0x06
#define MAG_ZOUT_L	0x07
#define MAG_ZOUT_H	0x08

void MPU9250_Write_Reg(u8 Slave_add,u8 reg_add,u8 reg_dat);
u8 MPU9250_Read_Reg(u8 Slave_add,u8 reg_add);
void MPU6050_ReadData(u8 Slave_add,u8 reg_add,u8* Read,u8 num);
u8 MPU9250_Init(void);
//void MPU9250_READ_ACCEL(short *accData); 
//void MPU9250_READ_GYRO(short *gyroData);
//void MPU9250_READ_MAG(short *magData);
void MPU9250_READ_ACCEL(void); 
void MPU9250_READ_GYRO(void);
void MPU9250_READ_MAG(void);

void readimu(void);
extern short gyroX,gyroY,gyroZ;
extern short accelX,accelY,accelZ; 
extern short magX,magY,magZ; 

#endif 
