#include "transport.h"
int openmv_number = 0;
int openmv_direction;
int move_list[10],move_cnt,near_cnt;
int openmv_state = 0;
u32 start_number;
int image_err;
int turn_flag = 1;

void Light_KEY_Init(void) //IO???
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//??PORTA,PORTE??

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//KEY0-KEY2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //???????
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);//???GPIOE2,3,4

	//??? WK_UP-->GPIOC.2	  ????
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0?????,????	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//???GPIOA.0

}

int car1_cmd = 0;//小车1控制标志位
void car2_wait_to_start(void)
{
	//装货 或 车1给发指令 任意一种都跳出
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) != 1 || car1_cmd == 0)
	{
		//识别手持房间号
		while(start_number == 0)
		{
			if(openmv_state == 0x03)
			{
				start_number = openmv_number;
			}
		}
		delay_ms(100);
		
	}
	delay_ms(1000);
	
}


/*----------------mode1-----------------*/
void car2_mode1_go_to_suspend(void)
{
	
}
void car2_mode1_wait_to_continue(void)
{
	
}
void car2_mode1_go_to_target(void)
{
	
}

/*----------------mode2-----------------*/
void car2_mode2_go_to_suspend(void)
{
}
void car2_mode2_wait_to_continue(void)
{
}
void car2_mode2_go_to_target(void)
{
}


void go_to_target(void)
{
	go_forward(1.59);
	control_delay();
	if(next_move == 0)		//????????
	{
		next_move = 255;
		turn(left);
		control_delay();
		go_forward(0.29);
		control_delay();
		Red_LED_on;
		turn_round();
		control_delay();
	}

}

void go_forward(float distance)	
{
	/*将总距离计数清零*/
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	total_distance = 0;
	
	/*给速度赋初值*/
	Target_straight = 0.5;
	
	/*距离不够，死循环*/
	while(total_distance <= distance)
	{
//		if(total_distance >  distance - 0.3)
//		/*???????????????*/
//		TX_BUF[2]=state_3;	//??????,????
	}
	
	/*停止*/
//	if(openmv_number != 1 && openmv_number != 2)
//	{
		Target_straight = 0;
		Target_Right = 0;
		Target_Left = 0;
		//Set_Pwm(0,-0,0);
//	}
	
	
	/*等待数字识别*/
//	if(openmv_number != 1 && openmv_number != 2)
//	{
//		delay_ms(1000);
//	}
	
}

long encoder_right_turn_cnt,encoder_left_turn_cnt;
float turn_speed = 0;
void turn(int direction)
{
	/*转向开始时将编码器转向计数清零，记得写在定时器里累加*/
	turn_flag = 0;
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	total_distance = 0;
	
	/*判断转向*/
	if(direction == left)
	{
//		Target_straight = 0.5;
		turn_speed = -0.2;
		while(encoder_right_cnt + encoder_left_cnt <= left_90)
		{}
	}
	else 
	{
//		Target_straight = 0.5;
		turn_speed = 0.2;
		while(-encoder_left_cnt - encoder_right_cnt<= right_90)
		{}
	}
	Target_straight = 0;
	Target_Right = 0;
	Target_Left = 0;
	turn_speed =0;
	turn_flag = 1;
	TX_BUF[2] = state_5;//转弯结束
}


void turn_round(void)
{
	/*转向开始时将编码器的转向计数清零，记得写在定时器里累加*/
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	
	/*开始转向*/
	turn_speed = 0.2;
	while(-encoder_left_cnt - encoder_right_cnt<= 1700);
		
	turn_speed = 0;
}

void control_delay(void)
{
	while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
	while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
}



