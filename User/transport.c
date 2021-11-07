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

int car1_cmd = 0;//С��1���Ʊ�־λ
int game_mode = 0;//�ڼ���
void car2_wait_to_start(void)
{
	//ʶ���ֳַ����
	while(start_number == 0)
	{
		if(openmv_state == 0x03)
		{
			start_number = openmv_number;
		}
	}
	
	//��1����ָ�� ����һ�ֶ�����
	while(car1_cmd == 0);
	
	//�ж��Ƿ��Ӳ��ֵڼ���
	if(car1_cmd == 1 || car1_cmd == 2)
	{
		game_mode = 1;
		//�ȴ�װ��
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0);
	}
	else if(car1_cmd == 3)
	{
		game_mode = 2;
	}
	else
	{
		//����
	}
	
	delay_ms(1000);
}


/*----------------mode1-----------------*/
void car2_mode1_go_to_suspend(void)
{
	//ֱ�е��ж�·��
	go_forward(1.59);
	control_delay();
	
	//1��ת��2��ת
	turn(car1_cmd);
	control_delay();
	
	//�߸�С���
	go_forward(0.15);
	control_delay();
	
	//��ͷ
	turn_round();
	control_delay();
	
	//ͣ�������Ƶ�
	Yellow_LED_on;
	
}
void car2_mode1_wait_to_continue(void)
{
	//�жϱ�־λ,С��1����ʮ��·�ھͷ�
	while(car1_cmd != 6);
	delay_ms(3000);
	
	Yellow_LED_off;
	
}
void car2_mode1_go_to_target(void)
{
	//ֱ�м���
	go_forward(0.70);
	control_delay();
	
	Green_LED_on;
}

/*----------------mode2-----------------*/
void car2_mode2_go_to_suspend(void)
{
	//ֱ�е��ж�·��
	go_forward(1.59);
	control_delay();
	
	//1��ת��2��ת
	turn(left);
	control_delay();
	
	//�߸�С���
	go_forward(0.15);
	control_delay();
	
	//��ͷ
	turn_round();
	control_delay();
	
	//ͣ�������Ƶ�
	Yellow_LED_on;
}
void car2_mode2_wait_to_continue(void)
{
	//�жϱ�־λ,С��1����ʮ��·�ھͷ�
	while(car1_cmd != 7);
	delay_ms(5000);
	
	Yellow_LED_off;
}
void car2_mode2_go_to_target(void)
{
	//ֱ�г�·��
	go_forward(0.15);
	control_delay();
	
	//1��ת��2��ת
	turn(left);
	control_delay();
	
	//ֱ�е�Զ��·��ǰ����������ģʽ1һ��
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
	/*���ܾ����������*/
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	total_distance = 0;
	
	/*���ٶȸ���ֵ*/
	Target_straight = 0.5;
	
	/*���벻������ѭ��*/
	while(total_distance <= distance)
	{
//		if(total_distance >  distance - 0.3)
//		/*???????????????*/
//		TX_BUF[2]=state_3;	//??????,????
	}
	
	/*ֹͣ*/
//	if(openmv_number != 1 && openmv_number != 2)
//	{
		Target_straight = 0;
		Target_Right = 0;
		Target_Left = 0;
		//Set_Pwm(0,-0,0);
//	}
	
	
	/*�ȴ�����ʶ��*/
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
	/*ת��ʼʱ��������ת��������㣬�ǵ�д�ڶ�ʱ�����ۼ�*/
	turn_flag = 0;
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	total_distance = 0;
	
	/*�ж�ת��*/
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
	TX_BUF[2] = state_5;//ת�����
}


void turn_round(void)
{
	/*ת��ʼʱ����������ת��������㣬�ǵ�д�ڶ�ʱ�����ۼ�*/
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	
	/*��ʼת��*/
	turn_speed = 0.2;
	while(-encoder_left_cnt - encoder_right_cnt<= 1700);
		
	turn_speed = 0;
}

void control_delay(void)
{
	while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
	while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
}



