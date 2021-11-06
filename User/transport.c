#include "transport.h"
int openmv_number = 0;
int openmv_direction;
int move_list[10],move_cnt,near_cnt;
int openmv_state = 0;
u32 start_number;
int image_err;
int turn_flag = 1;
void Light_KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTEʱ��

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//KEY0-KEY2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	//��ʼ�� WK_UP-->GPIOC.2	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.0

}

void wait_to_start(void)
{
//	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) != 0)
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) != 0)
	{
		while((openmv_number == 0 || openmv_state != 0x03) && start_number != 0)//�ȴ�openmv��ȡ����
		{
			TX_BUF[2] = state_1;	//δ��������δ���յ�����
		}
//		
		/*��������*/
//		openmv_number = openmv_number_read;
		start_number = openmv_number;

		TX_BUF[2] = state_2;		//�յ�����
		oled_show();
		
		/*�ӳ�����*/
		delay_ms(100);
		
	}
	
}

void go_to_target(void)
{
//	if(openmv_number == 1 || openmv_number == 2)//��ʮ��
//	{
		/*ֱ�����ӽ�ʮ��λ�ã�Ȼ��ʶ��*/
		
		go_forward(near_distance);
		move_list[move_cnt] = straight_near;
		move_cnt++;
		near_cnt++;
	
		while(openmv_state != 0x01)//��openmv������״̬��������ʱ
		{
			/*�ȴ�openmv�жϺ󷢳�ָ��*/
//			while(openmv_state != 0x00)
//			{}			
		
			
			
			/*ͨ�������ж�����ת*/
			if(next_move == 0)//��������ִ���
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
		/*ǰ�������Ĵ���λ��*/
//		go_to_patient(patient_distance);
////	}
////	else if(openmv_number == 3 || openmv_number == 4)
////	{
//		/*ֱ�����ӽ�ʮ��λ�ã�Ȼ��ʶ��*/
//		go_forward(mid_distance);
//		
//		/*�ȴ�openmv��ȡ����*/
//		while(openmv_number == 0)
//		{}
//			
//		turn(openmv_direction);
//			
//		/*�洢ת����Ϣ*/
//        if(openmv_direction == 0)
//			move_list[0] = 1;//����洢
//		else move_list[0] = 0;
//		
//		/*ǰ�������Ĵ���λ��*/
//		go_to_patient(patient_distance);
////	}
////	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
////	{
//		/*ֱ�����ӽ�ʮ��λ�ã�Ȼ��ʶ��*/
//		go_forward(far_distance);
//		
//		/*�ȴ�openmv��ȡ����*/
////		while(openmv_number == 0)
////		{}
//			
//		/*����openmv��ָ��ת��*/			
////		turn(openmv_direction);
//		turn(right);
//			
//		/*�洢ת����Ϣ*/
////        if(openmv_direction == 0)
////			move_list[0] = 1;//����洢
////		else move_list[0] = 0;
//		move_list[0] = left;
//		
//		/*��ǰ�н����ڶ���ʮ��*/
//		go_forward(far_distance_2);
//		
//		/*�ȴ�openmv��ȡ����*/
////		while(openmv_number == 0)
////		{}
//			
//		/*����openmv��ָ��ת��*/
////		turn(openmv_direction);
//		turn(left);

//		/*�洢ת����Ϣ*/
////        if(openmv_direction == 0)
////			move_list[1] = 1;//����洢
////		else move_list[1] = 0;
//			move_list[1] = right;
//		
//		/*ǰ�������Ĵ���λ��*/
//		go_to_patient(patient_distance);
////	}
////	else	//ʲô��û��⵽����������
////	{
////		//Beep();
////	}
	
	/*�����*/
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
//		/*ͨ�����ڷ��͵�״̬��Ϊʶ������*/
//		TX_BUF[2]=state_3;	//�����ִ�ʮ�֣�ʶ������
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
	TX_BUF[2] = state_5;//ת�����
}

void go_to_patient(float distance)
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
//		if(total_distance > distance )
//		/*ͨ�����ڷ��͵�״̬��Ϊ���ﲡ��*/
//		TX_BUF[2] = state_7;//�������
	}
	
	/*ֹͣ*/
	Target_straight = 0;
	Target_Right = 0;
	Target_Left = 0;
	//Set_Pwm(0,-0,0);
	
	
	
	/*�ȴ�����ʶ��*/
	//delay_ms(1000);
	
}

void wait_to_return(void)
{
	/*�ȴ���翪�ؼ���Ƿ�ж��*/
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) != 0)
	{
		delay_ms(100);
	}
	
	/*����*/
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

void return_home(void)
{
	/*С��2�Ĵ���*/
	
	
	/*ԭ�ص�ͷ*/
	turn_round();
//	turn(right);
//	turn(right);
//	
	TX_BUF[2] = state_4;//�ؼұ�־λ
	/*�س�*/
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
//		while(openmv_state != 0x01)//��openmv������״̬��������ʱ
//		{
//			/*�ȴ�openmv�жϺ󷢳�ָ��*/
//			while(openmv_state != 0x00)
//			{}			
//		
//			/*ͨ�������ж�����ת*/
//			if(openmv_number == 0)//��������ִ���
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
		/*�ؼң����뿪����*/
//		go_home(move_list[move_cnt]);
//		move_cnt--;
		/*ͨ�������ж�����ת*/
//		if(openmv_number == 1)
//		{
//			turn(right);
//		}
//		else if(openmv_number == 2)
//		{
//			turn(left);
//		}
		
		/*ʮ�ֵ��ҵĴ���λ��*/
//		go_home(near_distance);
////	}
////	else if(openmv_number == 3 || openmv_number == 4)
////	{
//		/*�ؼң����뿪����*/
//		go_home(patient_distance);
//		
//		/*�ؼұ�����ת��*/
//		turn(move_list[0]);
//		
//		/*ʮ�ֵ��ҵĴ���λ��*/
//		go_home(mid_distance);
////	}
////	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
////	{
//		/*�ؼң����뿪����*/
//		go_home(patient_distance);
//		
//		/*ת��һ����*/
//		turn(move_list[1]);
//			
//			
//		/*��ǰ�н����ڶ���ʮ��*/
//		go_home(far_distance_2);
//		
//			
//		/*ת�ڶ�����*/
//		turn(move_list[0]);
//		
//			 
//		/*ֱ�ߵ���*/
//		go_home(far_distance);
//	}
//	else	//ʲô��û��⵽����������
//	{
//		//Beep();
//	}
}

void turn_round(void)
{
	/*ת��ʼʱ��������ת��������㣬�ǵ�д�ڶ�ʱ�����ۼ�*/
	encoder_right_turn_cnt = 0;
	encoder_left_turn_cnt = 0;
	
	/*��ʼת��*/
	turn_speed = 0.2;
	while(abs(encoder_left_cnt) - abs(encoder_right_cnt)<= 2*right_90)
	{}
		
	turn_speed = 0;
}

void go_home(float distance)
{
	/*���ܾ����������*/
	encoder_left_cnt = 0;
	encoder_right_cnt = 0;
	total_distance = 0;
	TX_BUF[2] = state_6;//�ؼ�Ѳ��
	
	/*���ٶȸ���ֵ*/
	Target_straight = 0.5;
	turn_speed = 0;
	
	/*���벻������ѭ��*/
	while(total_distance <= distance)
	{}
	
	
	
	Target_straight = 0;
	Target_Right = 0;
	Target_Left = 0;
	
	
}




