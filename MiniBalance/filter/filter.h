#ifndef __FILTER_H
#define __FILTER_H

extern float angle, angle_dot; 	
float Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo(float angle_m, float gyro_m);
#endif
