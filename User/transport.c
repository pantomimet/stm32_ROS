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
int game_mode = 0;//第几问
void car2_wait_to_start(void)
{
	//识别手持房间号
	while(start_number == 0)
	{
		if(openmv_state == 0x03)
		{
			start_number = openmv_number;
		}
	}
	
	//车1给发指令 任意一种都跳出
	while(car1_cmd == 0);
	
	//判断是发挥部分第几问
	if(car1_cmd == 1 || car1_cmd == 2)
	{
		game_mode = 1;
		//等待装货
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0);
	}
	else if(car1_cmd == 3)
	{
		game_mode = 2;
	}
	else
	{
		//警报
	}
	
	delay_ms(1000);
}


/*----------------mode1-----------------*/
void car2_mode1_go_to_suspend(void)
{
	//直行到中端路口
	go_forward(1.59);
	control_delay();
	
	//1左转，2右转
	turn(car1_cmd);
	control_delay();
	
	//走个小豁口
	go_forward(0.15);
	control_delay();
	
	//掉头
	turn_round();
	control_delay();
	
	//停车，亮黄灯
	Yellow_LED_on;
	
}
void car2_mode1_wait_to_continue(void)
{
	//判断标志位,小车1过了十字路口就发
	while(car1_cmd != 6);
	delay_ms(3000);
	
	Yellow_LED_off;
	
}
void car2_mode1_go_to_target(void)
{
	//直行即可
	go_forward(0.70);
	control_delay();
	
	Green_LED_on;
}

/*----------------mode2-----------------*/
void car2_mode2_go_to_suspend(void)
{
	//直行到中端路口
	go_forward(1.59);
	control_delay();
	
	//1左转，2右转
	turn(left);
	control_delay();
	
	//走个小豁口
	go_forward(0.15);
	control_delay();
	
	//掉头
	turn_round();
	control_delay();
	
	//停车，亮黄灯
	Yellow_LED_on;
}
void car2_mode2_wait_to_continue(void)
{
	//判断标志位,小车1过了十字路口就发
	while(car1_cmd != 7);
	delay_ms(5000);
	
	Yellow_LED_off;
}
void car2_mode2_go_to_target(void)
{
	//直行出路口
	go_forward(0.15);
	control_delay();
	
	//1左转，2右转
	turn(left);
	control_delay();
	
	//直行到远端路口前，接下来和模式1一样
	go_forward(0.50);
	control_delay();
	delay_ms(1000);
	
	go_forward(0.13);
	
	if(next_move == 0)
	{
		next_move = 255;
		turn(left);
		
		go_forward(0.5);
		delay_ms(1000);
		
		go_forward(0.13);
		if(next_move == 0)
		{
			next_move = 255;
			turn(left);
			
			go_forward(0.29);

		}
	}

	Red_LED_on;
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
	control_delay();
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
	else if(direction == right)
	{
//		Target_straight = 0.5;
		turn_speed = 0.2;
		while(-encoder_left_cnt - encoder_right_cnt<= right_90);
	}
	else
	{
	}
	Target_straight = 0;
	Target_Right = 0;
	Target_Left = 0;
	turn_speed =0;
	turn_flag = 1;
	control_delay();
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



