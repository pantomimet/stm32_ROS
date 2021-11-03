#ifndef __FILTER_H
#define __FILTER_H
#include "mpu9250.h"
extern float angle, angle_dot; 
extern float Roll,Pitch;
extern float Roll_g,Pitch_g;
//float Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo(float angle_m, float gyro_m);
void Kalman_Filter_Y(float Accely,float Gyroy);
void Kalman_Filter(float Accel,float Gyro);
void IMU_filter(void);
#endif
