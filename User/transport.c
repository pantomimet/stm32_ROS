#include "transport.h"

void KEY_Init(void) //IO初始化
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
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) != 0)
	{
		while(openmv_number_read == 0)//等待openmv读取数字
		{
		}
		
		/*保存数字*/
		openmv_number = openmv_number_read;
		
		/*延迟消抖*/
		delay_ms(1000);
	}
	
}

void go_to_target(void)
{
	if(openmv_number == 1 || openmv_number == 2)//近十字
	{
		/*直行至接近十字位置，然后识别*/
		go_forward(near_distance);
				
		/*通过数字判断左右转*/
		if(openmv_number == 1)
		{
			turn(left);
		}
		else if(openmv_number == 2)
		{
			turn(right);
		}
		/*前往病房的大致位置*/
		go_to_patient(patient_distance);
	}
	else if(openmv_number == 3 || openmv_number == 4)
	{
		/*直行至接近十字位置，然后识别*/
		go_forward(mid_distance);
		
		/*等待openmv读取数字*/
		while(openmv_number == 0)
		{}
			
		turn(openmv_direction);
			
		/*存储转弯信息*/
        turn_list[0] = ...//反向存储
		/*前往病房的大致位置*/
		go_to_patient(patient_distance);
	}
	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
	{
		/*直行至接近十字位置，然后识别*/
		go_forward(far_distance);
		
		/*等待openmv读取数字*/
		while(openmv_number == 0)
		{}
			
		/*根据openmv的指令转向*/			
		turn(openmv_direction);
			
		/*存储转弯信息*/
        turn_list[0] = ...//反向存储
			
		/*向前行进至第二个十字*/
		go_forward(far_distance_2);
		
		/*等待openmv读取数字*/
		while(openmv_number == 0)
		{}
			
		/*根据openmv的指令转向*/
		turn(openmv_direction);

		/*存储转弯信息*/
        turn_list[1] = ...//反向存储			
			
		/*前往病房的大致位置*/
		go_to_patient(patient_distance);
	}
	else	//什么都没检测到，蜂鸣器响
	{
		Beep();
	}
	
	/*亮红灯*/
	
}

void go_forward(float distance)	
{
	/*将总距离计数清零*/
	total_distance = 0;
	
	/*给速度赋初值*/
	Target_Left = 1.0;
	Target_Right = 1.0;
	
	/*距离不够，死循环*/
	while(total_distance <= distance)
	{}
	
	/*停止*/
	Target_Left = 0;
	Target_Right = 0;
	
	/*通过串口发送的状态变为识别数字*/
	TX_BUF[5]=recognize_number;
	
	/*等待数字识别*/
	delay_ms(1000);
}

u32 encoder_right_turn_cnt,encoder_left_turn_cnt;
void turn(int direction)
{
	/*转向开始时将编码器转向计数清零，记得写在定时器里累加*/
	encoder_right_turn_cnt = 0;
	encoder_left_turn_cnt = 0;
	
	/*判断转向*/
	if(direction == left)
	{
		turn_speed = turn_speed;
		while(abs(encoder_right_cnt) - abs(encoder_left_cnt)<= left_90);
	}
	else 
	{
		rurn_speed = -turn_speed;
		while(abs(encoder_left_cnt) - abs(encoder_right_cnt)<= right_90);
	}
}

void go_to_patient(float distance)
{	
	/*将总距离计数清零*/
	total_distance = 0;
	
	/*给速度赋初值*/
	Target_Left = 1.0;
	Target_Right = 1.0;
	
	/*距离不够，死循环*/
	while(total_distance <= distance)
	{}
	
	/*停止*/
	Target_Left = 0;
	Target_Right = 0;
	
	/*通过串口发送的状态变为到达病房*/
	TX_BUF[5]=reach_patient;
	
	/*等待数字识别*/
	delay_ms(1000);
	
}

void wait_to_return(void)
{
	/*等待光电开关检测是否卸货*/
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == 0)
	{
		delay_ms(1000);
	}
	
	/*灭红灯*/
	
}

void return_to_home()
{
	/*小车2的代码*/
	
	
	/*原地掉头*/
	turn_round();
	
	/*回程*/
	if(openmv_number == 1 || openmv_number == 2)
	{
		/*回家，先离开病房*/
		go_forward(patient_distance);
				
		/*通过数字判断左右转*/
		if(openmv_number == 1)
		{
			turn(right);
		}
		else if(openmv_number == 2)
		{
			turn(letf);
		}
		
		/*十字到家的大致位置*/
		go_to_home(near_distance);
	}
	else if(openmv_number == 3 || openmv_number == 4)
	{
		/*回家，先离开病房*/
		go_forward(patient_distance);
		
		/*回家别忘了转弯*/
		turn(turn_list[0]);
		
		/*十字到家的大致位置*/
		go_to_home(mid_distance)
	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
	{
		/*回家，先离开病房*/
		go_forward(patient_distance);
		
		/*转第一个弯*/
		turn(turn_list[1]);
			
			
		/*向前行进至第二个十字*/
		go_to_home(far_distance_2);
		
			
		/*转第二个弯*/
		turn(turn_list[0]);
		
			
		/*直走到家*/
		go_to_home(far_distance);
	}
	else	//什么都没检测到，蜂鸣器响
	{
		Beep();
	}
	}
}

void turn_round()
{
	/*转向开始时将编码器转向计数清零，记得写在定时器里累加*/
	encoder_right_turn_cnt = 0;
	encoder_left_turn_cnt = 0;
	
	/*开始转向*/
	turn_speed = turn_speed;
	while(abs(encoder_right_cnt) - abs(encoder_left_cnt)<= 2*left_90);
}

void go_to_home(float distance)
{
	/*总距离清零*/
	total_distance = 0;
	while(total_distance <= distance && openmv_state != reach_DottedLine);
	
}




