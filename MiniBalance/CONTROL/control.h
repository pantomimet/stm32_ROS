#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern u8 mode;
#define PI 3.14159265
#define ZHONGZHI 0 
#define DIFFERENCE 100
#define biaoding_1m_l 3808
#define biaoding_1m_r 3498
extern	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
extern float total_distance;
extern float Target_straight;
extern int next_move;

void Get_openmv(void);
void Kinematic_Analysis(float velocity,float angle);
int EXTI15_10_IRQHandler(void);
void Set_Pwm(int motor_a,int motor_b,int servo);
void Key(void);
void Xianfu_Pwm(int amplitude);
void Xianfu_Velocity(int amplitude_A,int amplitude_B);
u8 Turn_Off( int voltage);
int myabs(int a);
void Incremental_PI_Left (float Encoder,float Target);
void Incremental_PI_Right (float Encoder,float Target);
void Get_RC(void);
void  Find_CCD_Zhongzhi(void);
void Get_commands(void);
void stm32_smooth(void);
void Position_PID(float image_err);
//void PID_Servo(float target_angle,float current_angle);
#endif
