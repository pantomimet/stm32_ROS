#include "transport.h"
int openmv_number = 0;
int openmv_direction;
int move_list[10],move_cnt,near_cnt;
int openmv_state = 0;
u32 start_number;
int image_err;
int turn_flag = 1;
void Light_KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//KEY0-KEY2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOC.2	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.0

}

void wait_to_start(void)
{
//	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) != 0)
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) != 0)
	{
		while((openmv_number == 0 || openmv_state != 0x03) && start_number != 0)//等待openmv读取数字
		{
			TX_BUF[2] = state_1;	//未出发，且未接收到数字
		}
//		
		/*保存数字*/
//		openmv_number = openmv_number_read;
		start_number = openmv_number;

		TX_BUF[2] = state_2;		//收到数字
		oled_show();
		
		/*延迟消抖*/
		delay_ms(100);
		
	}
	
}

void go_to_target(void)
{
//	if(openmv_number == 1 || openmv_number == 2)//近十字
//	{
		/*直行至接近十字位置，然后识别*/
		
		go_forward(near_distance);
		move_list[move_cnt] = straight_near;
		move_cnt++;
		near_cnt++;
	
		while(openmv_state != 0x01)//当openmv发来的状态不是虚线时
		{
			/*等待openmv判断后发出指令*/
//			while(openmv_state != 0x00)
//			{}			
		
			
			
			/*通过数字判断左右转*/
			if(next_move == 0)//这里的数字待定
			{
				next_move = 255;
				turn(left);
				move_list[move_cnt] = right;
				move_cnt++;
				if(near_cnt != 3)
				{
					go_to_patient(patient_distance);
					move_list[move_cnt] = straight_patient;
					move_cnt++;
					return;
				}
				else 
				{
					go_forward(near_distance);
					move_list[move_cnt] = straight_near;
					move_cnt++;
					near_cnt++;
				}
			}
			else if(next_move == 1)
			{
				next_move = 255;
				turn(right);
				move_list[move_cnt] = left;
				move_cnt++;
				if(near_cnt != 3)
				{
					go_to_patient(patient_distance);
					move_list[move_cnt] = straight_patient;
					move_cnt++;
					return;
				}
				else 
				{
					go_forward(near_distance);
					move_list[move_cnt] = straight_near;
					move_cnt++;
					near_cnt++;
				}
			}
			else if(next_move == 2)
			{
				next_move = 255;
				go_forward(near_distance);
				move_list[move_cnt] = straight_near;
				move_cnt++;
				near_cnt++;
			}
		}
		/*前往病房的大致位置*/
//		go_to_patient(patient_distance);
////	}
////	else if(openmv_number == 3 || openmv_number == 4)
////	{
//		/*直行至接近十字位置，然后识别*/
//		go_forward(mid_distance);
//		
//		/*等待openmv读取数字*/
//		while(openmv_number == 0)
//		{}
//			
//		turn(openmv_direction);
//			
//		/*存储转弯信息*/
//        if(openmv_direction == 0)
//			move_list[0] = 1;//反向存储
//		else move_list[0] = 0;
//		
//		/*前往病房的大致位置*/
//		go_to_patient(patient_distance);
////	}
////	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
////	{
//		/*直行至接近十字位置，然后识别*/
//		go_forward(far_distance);
//		
//		/*等待openmv读取数字*/
////		while(openmv_number == 0)
////		{}
//			
//		/*根据openmv的指令转向*/			
////		turn(openmv_direction);
//		turn(right);
//			
//		/*存储转弯信息*/
////        if(openmv_direction == 0)
////			move_list[0] = 1;//反向存储
////		else move_list[0] = 0;
//		move_list[0] = left;
//		
//		/*向前行进至第二个十字*/
//		go_forward(far_distance_2);
//		
//		/*等待openmv读取数字*/
////		while(openmv_number == 0)
////		{}
//			
//		/*根据openmv的指令转向*/
////		turn(openmv_direction);
//		turn(left);

//		/*存储转弯信息*/
////        if(openmv_direction == 0)
////			move_list[1] = 1;//反向存储
////		else move_list[1] = 0;
//			move_list[1] = right;
//		
//		/*前往病房的大致位置*/
//		go_to_patient(patient_distance);
////	}
////	else	//什么都没检测到，蜂鸣器响
////	{
////		//Beep();
////	}
	
	/*亮红灯*/
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
//		/*通过串口发送的状态变为识别数字*/
//		TX_BUF[2]=state_3;	//即将抵达十字，识别数字
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
		while(abs(encoder_left_cnt) - abs(encoder_right_cnt)<= right_90)
		{}
	}
	Target_straight = 0;
	Target_Right = 0;
	Target_Left = 0;
	turn_speed =0;
	turn_flag = 1;
	TX_BUF[2] = state_5;//转弯结束
}

void go_to_patient(float distance)
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
//		if(total_distance > distance )
//		/*通过串口发送的状态变为到达病房*/
//		TX_BUF[2] = state_7;//检测虚线
	}
	
	/*停止*/
	Target_straight = 0;
	Target_Right = 0;
	Target_Left = 0;
	//Set_Pwm(0,-0,0);
	
	
	
	/*等待数字识别*/
	//delay_ms(1000);
	
}

void wait_to_return(void)
{
	/*等待光电开关检测是否卸货*/
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) != 0)
	{
		delay_ms(100);
	}
	
	/*灭红灯*/
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

void return_home(void)
{
	/*小车2的代码*/
	
	
	/*原地掉头*/
	turn_round();
//	turn(right);
//	turn(right);
//	
	TX_BUF[2] = state_4;//回家标志位
	/*回程*/
//	if(openmv_number == 1 || openmv_number == 2)
//	{
	for(move_cnt = move_cnt;move_cnt >=0;move_cnt --)
	{
		if(move_list[move_cnt] == straight_near || move_list[move_cnt] == straight_patient)
		{
			go_home(move_list[move_cnt]);
		}
		else if(move_list[move_cnt] == left || move_list[move_cnt] == right)
		{
			turn(move_list[move_cnt]);
		}
	}	
//		go_home(move_list[move_cnt]);
//		move_cnt--;
//		while(openmv_state != 0x01)//当openmv发来的状态不是虚线时
//		{
//			/*等待openmv判断后发出指令*/
//			while(openmv_state != 0x00)
//			{}			
//		
//			/*通过数字判断左右转*/
//			if(openmv_number == 0)//这里的数字待定
//			{
//				turn(left);
//				move_cnt--;
////				if(near_cnt != 3)
////				{
////					go_to_patient(patient_distance);
////					move_list[move_cnt] = straight_patient;
////					move_cnt++;
////					return;
////				}
////				else 
////				{
////					go_forward(near_distance);
////					move_list[move_cnt] = straight_near;
////					move_cnt++;
////					near_cnt++;
////				}
//			}
//			else if(openmv_number == 1)
//			{
//				turn(right);
//				move_cnt--;
////				if(near_cnt != 3)
////				{
////					go_to_patient(patient_distance);
////					move_list[move_cnt] = straight_patient;
////					move_cnt++;
////					return;
////				}
////				else 
////				{
////					go_forward(near_distance);
////					move_list[move_cnt] = straight_near;
////					move_cnt++;
////					near_cnt++;
////				}
//			}
//			else if(openmv_number == 3)
//			{
//				go_home(near_distance);
//				move_cnt--;
//				near_cnt--;
//			}
//			if(near_cnt <= 3)
//			{
//				go_home( near_cnt * near_distance + (near_cnt - 1) * 0.6);
//				break;
//			}
//		}
//		do
//		{
//			Target_straight = 0.5;
//			turn_speed = 0;
//		}while(openmv_state != 0x01);
		/*回家，先离开病房*/
//		go_home(move_list[move_cnt]);
//		move_cnt--;
		/*通过数字判断左右转*/
//		if(openmv_number == 1)
//		{
//			turn(right);
//		}
//		else if(openmv_number == 2)
//		{
//			turn(left);
//		}
		
		/*十字到家的大致位置*/
//		go_home(near_distance);
////	}
////	else if(openmv_number == 3 || openmv_number == 4)
////	{
//		/*回家，先离开病房*/
//		go_home(patient_distance);
//		
//		/*回家别忘了转弯*/
//		turn(move_list[0]);
//		
//		/*十字到家的大致位置*/
//		go_home(mid_distance);
////	}
////	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
////	{
//		/*回家，先离开病房*/
//		go_home(patient_distance);
//		
//		/*转第一个弯*/
//		turn(move_list[1]);
//			
//			
//		/*向前行进至第二个十字*/
//		go_home(far_distance_2);
//		
//			
//		/*转第二个弯*/
//		turn(move_list[0]);
//		
//			 
//		/*直走到家*/
//		go_home(far_distance);
//	}
//	else	//什么都没检测到，蜂鸣器响
//	{
//		//Beep();
//	}
}

void turn_round(void)
{
	/*转向开始时将编码器转向计数清零，记得写在定时器里累加*/
	encoder_right_turn_cnt = 0;
	encoder_left_turn_cnt = 0;
	
	/*开始转向*/
	turn_speed = 0.2;
	while(abs(encoder_left_cnt) - abs(encoder_right_cnt)<= 2*right_90)
	{}
		
	turn_speed = 0;
}

void go_home(float distance)
{
	/*将总距离计数清零*/
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	total_distance = 0;
	TX_BUF[2] = state_6;//回家巡线
	
	/*给速度赋初值*/
	Target_straight = 0.5;
	turn_speed = 0;
	
	/*距离不够，死循环*/
	while(total_distance <= distance)
	{}
	
	
	
	Target_straight = 0;
	Target_Right = 0;
	Target_Left = 0;
	
	
}




