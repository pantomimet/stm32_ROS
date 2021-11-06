#ifndef _TRANSPORT_H
#define _TRANSPORT_H

#include "sys.h"

#define near_distance 0.7
#define mid_distance 1.6
#define far_distance 2.5
#define far_distance_2 0.7
#define patient_distance 0.35

#define left 0
#define right 1
#define straight_near 2
#define straight_patient 3

#define left_90 980
#define right_90 880


#define state_1 7		//接收到数字但未出发
#define state_2 8		//接收到数字
#define state_3 9		//检测虚线
#define state_4 0x0a	//回家
#define state_5 0x0b  	//转弯结束
#define state_6 0x0c	//回家巡线

extern float turn_speed;
extern long encoder_right_turn_cnt,encoder_left_turn_cnt;
extern int openmv_number;
extern int openmv_state;
extern int image_err;
extern u32 start_number;
extern int turn_flag;

void Light_KEY_Init(void);
void wait_to_start(void);
void go_to_target(void);
void go_forward(float distance);
void turn(int direction);
void go_to_patient(float distance);
void wait_to_return(void);
void return_home(void);
void turn_round(void);
void go_home(float distance);


#endif

