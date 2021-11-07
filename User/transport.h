#ifndef _TRANSPORT_H
#define _TRANSPORT_H

#include "sys.h"

#define near_distance 0.5
#define mid_distance 1.6
#define far_distance 2.5
#define far_distance_2 0.9
#define patient_distance 0.15

#define left 1
#define right 2
#define straight_near 2
#define straight_patient 3

#define left_90 700
#define right_90 700


#define state_1 0		//??????????
#define state_2 1		//?????
#define state_3 2		//??????,????
#define state_4 3		//??
#define state_5 4  		//????
#define state_6 5		//????
#define state_7 6		//????

extern float turn_speed;
extern long encoder_right_turn_cnt,encoder_left_turn_cnt;
extern int openmv_number;
extern int openmv_state;
extern int image_err;
extern u32 start_number;
extern int turn_flag;

extern int car1_cmd;//小车1控制标志位

void Light_KEY_Init(void);

void car2_wait_to_start(void);

void car2_mode1_go_to_suspend(void);
void car2_mode1_wait_to_continue(void);
void car2_mode1_go_to_target(void);

void car2_mode2_go_to_suspend(void);
void car2_mode2_wait_to_continue(void);
void car2_mode2_go_to_target(void);


void go_forward(float distance);
void turn(int direction);
void turn_round(void);
void control_delay(void);


#endif



