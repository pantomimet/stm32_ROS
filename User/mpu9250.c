#include "mpu9250.h"
#include "filter.h"

//short Accl[3];
//short Gyro[3];
//short Mag[3];
short gyroX,gyroY,gyroZ;
short accelX,accelY,accelZ; 
short magX,magY,magZ; 
float Angle_ax=0,Angle_ay=0,Angle_az=0;  	//由加速度计算的加速度(弧度制)
float Angle_gy=0,Angle_gx=0,Angle_gz=0;  	//由角速度计算的角速率(角度制)

void MPU9250_Write_Reg(u8 Slave_add,u8 reg_add,u8 reg_dat)
{
	IIC_Start();
	 
	IIC_Send_Byte(Slave_add);

	IIC_Wait_Ack();
	 
	IIC_Send_Byte(reg_add);

	IIC_Wait_Ack();
	 
	IIC_Send_Byte(reg_dat);

	IIC_Wait_Ack();
	 
	IIC_Stop();
	 
}

u8 MPU9250_Read_Reg(u8 Slave_add,u8 reg_add)
{
	u8 temp=0;
	IIC_Start();
	IIC_Send_Byte(Slave_add);
	temp=IIC_Wait_Ack();
	IIC_Send_Byte(reg_add);
	temp=IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(Slave_add+1);
	temp=IIC_Wait_Ack();
	temp=IIC_Read_Byte(0);
	IIC_Stop();
	return temp;
}

u8 MPU9250_Init(void)
{
	IIC_Init();
	if(MPU9250_Read_Reg(GYRO_ADDRESS,WHO_AM_I)==0x71)
	{
		MPU9250_Write_Reg(GYRO_ADDRESS,PWR_MGMT_1,0x00); //? ?????
		MPU9250_Write_Reg(GYRO_ADDRESS,SMPLRT_DIV,0x07);//????125Hz
		MPU9250_Write_Reg(GYRO_ADDRESS,CONFIG,0x06);//?????5Hz
		MPU9250_Write_Reg(GYRO_ADDRESS,GYRO_CONFIG,0x18); //?????, ??2000?
		MPU9250_Write_Reg(GYRO_ADDRESS,ACCEL_CONFIG,0x18); //?????,?? 16g
		return 0;
	}
	return 1;
}

void MPU6050_Init(void)
{	
	IIC_Init();
	MPU9250_Write_Reg(GYRO_ADDRESS,PWR_MGMT_1,0x80);
	delay_ms(100);
	MPU9250_Write_Reg(GYRO_ADDRESS,PWR_MGMT_1,0x00); //? ?????
	delay_ms(100);
	MPU9250_Write_Reg(GYRO_ADDRESS,SMPLRT_DIV,0x07);//????125Hz
	MPU9250_Write_Reg(GYRO_ADDRESS,CONFIG,0x06);//?????5Hz
	MPU9250_Write_Reg(GYRO_ADDRESS,GYRO_CONFIG,0x18); //?????, ??2000?
	MPU9250_Write_Reg(GYRO_ADDRESS,ACCEL_CONFIG,0x18); //?????,?? 16g
	//MPU9250_Write_Reg(MPU_INT_EN_REG,0X00);
} 

void MPU9250_READ_ACCEL()
{
	u8 BUF[6];
	BUF[0]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_XOUT_L); //? X??????
	BUF[1]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_XOUT_H); //? X??????
	accelX=(BUF[1]<<8)| BUF[0]; 
//	accelX = accelX / 2048;
	Angle_ax = (accelX) / 2048.0;
	BUF[2]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_YOUT_L); //? Y??????
	BUF[3]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_YOUT_H); //? Y??????
	accelY=(BUF[3]<<8)|BUF[2];
//	accelY = accelY / 2048;
	Angle_ay = (accelY) / 2048.0;	
	BUF[4]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_ZOUT_L); //? Z??????
	BUF[5]=MPU9250_Read_Reg(ACCEL_ADDRESS,ACCEL_ZOUT_H); //? Z??????
	accelZ=(BUF[5]<<8)| BUF[4]; 
//	accelZ = accelZ / 2048;
	Angle_az = (accelZ) / 2048.0;	
}

void MPU9250_READ_GYRO()
{
	u8 BUF[8];
	BUF[0]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_XOUT_L);
	BUF[1]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_XOUT_H);
	gyroX=(BUF[1]<<8)| BUF[0];
//	gyroX = gyroX / 16.4;
	Angle_gx=(gyroX)/16.4;
	BUF[2]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_YOUT_L);
	BUF[3]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_YOUT_H);
	gyroY=(BUF[3]<<8)| BUF[2];
//	gyroY = gyroY / 16.4;
	Angle_gy=(gyroY)/16.4;
	BUF[4]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_ZOUT_L);
	BUF[5]=MPU9250_Read_Reg(GYRO_ADDRESS,GYRO_ZOUT_H);
	gyroZ=(BUF[5]<<8)| BUF[4];
//	gyroZ = gyroZ / 16.4;
	Angle_gz=(gyroZ)/16.4;
}

void MPU9250_READ_MAG()
{
	u8 BUF[6];
	MPU9250_Write_Reg(GYRO_ADDRESS,INT_PIN_CFG,0x02); //turn on Bypass Mode
	delay_ms(10);
	MPU9250_Write_Reg(MAG_ADDRESS,0x0A,0x01); //?? ??????,????????????
	delay_ms(10); 
	BUF[0]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_XOUT_L); //? X??????
	BUF[1]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_XOUT_H); //? X??????
	magX=(BUF[1]<<8) |BUF[0];
	BUF[2]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_YOUT_L); //? Y??????
	BUF[3]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_YOUT_H); //? Y??????
	magY=(BUF[3]<<8)| BUF[2];
	BUF[4]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_ZOUT_L); //? Z??????
	BUF[5]=MPU9250_Read_Reg(MAG_ADDRESS,MAG_ZOUT_H); //? Z??????
	magZ=(BUF[5]<<8)| BUF[4];
}
void readimu() //
{
//	float Angle_AY;
	MPU9250_READ_ACCEL(); 
	MPU9250_READ_GYRO(); 
	//MPU9250_READ_MAG();
	IMU_filter();
//	Angle_AY = atan2(accelY , sqrt(accelZ*accelZ + accelX*accelX)) * (180 / 3.14159);
//	angle = Kalman_Filter(Angle_AY,gyroZ);
}


