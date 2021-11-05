#include "transport.h"

void KEY_Init(void) //IO��ʼ��
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
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) != 0)
	{
		while(openmv_number_read == 0)//�ȴ�openmv��ȡ����
		{
		}
		
		/*��������*/
		openmv_number = openmv_number_read;
		
		/*�ӳ�����*/
		delay_ms(1000);
	}
	
}

void go_to_target(void)
{
	if(openmv_number == 1 || openmv_number == 2)//��ʮ��
	{
		/*ֱ�����ӽ�ʮ��λ�ã�Ȼ��ʶ��*/
		go_forward(near_distance);
				
		/*ͨ�������ж�����ת*/
		if(openmv_number == 1)
		{
			turn(left);
		}
		else if(openmv_number == 2)
		{
			turn(right);
		}
		/*ǰ�������Ĵ���λ��*/
		go_to_patient(patient_distance);
	}
	else if(openmv_number == 3 || openmv_number == 4)
	{
		/*ֱ�����ӽ�ʮ��λ�ã�Ȼ��ʶ��*/
		go_forward(mid_distance);
		
		/*�ȴ�openmv��ȡ����*/
		while(openmv_number == 0)
		{}
			
		turn(openmv_direction);
			
		/*�洢ת����Ϣ*/
        turn_list[0] = ...//����洢
		/*ǰ�������Ĵ���λ��*/
		go_to_patient(patient_distance);
	}
	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
	{
		/*ֱ�����ӽ�ʮ��λ�ã�Ȼ��ʶ��*/
		go_forward(far_distance);
		
		/*�ȴ�openmv��ȡ����*/
		while(openmv_number == 0)
		{}
			
		/*����openmv��ָ��ת��*/			
		turn(openmv_direction);
			
		/*�洢ת����Ϣ*/
        turn_list[0] = ...//����洢
			
		/*��ǰ�н����ڶ���ʮ��*/
		go_forward(far_distance_2);
		
		/*�ȴ�openmv��ȡ����*/
		while(openmv_number == 0)
		{}
			
		/*����openmv��ָ��ת��*/
		turn(openmv_direction);

		/*�洢ת����Ϣ*/
        turn_list[1] = ...//����洢			
			
		/*ǰ�������Ĵ���λ��*/
		go_to_patient(patient_distance);
	}
	else	//ʲô��û��⵽����������
	{
		Beep();
	}
	
	/*�����*/
	
}

void go_forward(float distance)	
{
	/*���ܾ����������*/
	total_distance = 0;
	
	/*���ٶȸ���ֵ*/
	Target_Left = 1.0;
	Target_Right = 1.0;
	
	/*���벻������ѭ��*/
	while(total_distance <= distance)
	{}
	
	/*ֹͣ*/
	Target_Left = 0;
	Target_Right = 0;
	
	/*ͨ�����ڷ��͵�״̬��Ϊʶ������*/
	TX_BUF[5]=recognize_number;
	
	/*�ȴ�����ʶ��*/
	delay_ms(1000);
}

u32 encoder_right_turn_cnt,encoder_left_turn_cnt;
void turn(int direction)
{
	/*ת��ʼʱ��������ת��������㣬�ǵ�д�ڶ�ʱ�����ۼ�*/
	encoder_right_turn_cnt = 0;
	encoder_left_turn_cnt = 0;
	
	/*�ж�ת��*/
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
	/*���ܾ����������*/
	total_distance = 0;
	
	/*���ٶȸ���ֵ*/
	Target_Left = 1.0;
	Target_Right = 1.0;
	
	/*���벻������ѭ��*/
	while(total_distance <= distance)
	{}
	
	/*ֹͣ*/
	Target_Left = 0;
	Target_Right = 0;
	
	/*ͨ�����ڷ��͵�״̬��Ϊ���ﲡ��*/
	TX_BUF[5]=reach_patient;
	
	/*�ȴ�����ʶ��*/
	delay_ms(1000);
	
}

void wait_to_return(void)
{
	/*�ȴ���翪�ؼ���Ƿ�ж��*/
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == 0)
	{
		delay_ms(1000);
	}
	
	/*����*/
	
}

void return_to_home()
{
	/*С��2�Ĵ���*/
	
	
	/*ԭ�ص�ͷ*/
	turn_round();
	
	/*�س�*/
	if(openmv_number == 1 || openmv_number == 2)
	{
		/*�ؼң����뿪����*/
		go_forward(patient_distance);
				
		/*ͨ�������ж�����ת*/
		if(openmv_number == 1)
		{
			turn(right);
		}
		else if(openmv_number == 2)
		{
			turn(letf);
		}
		
		/*ʮ�ֵ��ҵĴ���λ��*/
		go_to_home(near_distance);
	}
	else if(openmv_number == 3 || openmv_number == 4)
	{
		/*�ؼң����뿪����*/
		go_forward(patient_distance);
		
		/*�ؼұ�����ת��*/
		turn(turn_list[0]);
		
		/*ʮ�ֵ��ҵĴ���λ��*/
		go_to_home(mid_distance)
	else if(openmv_number == 5 || openmv_number == 6 || openmv_number == 7 || openmv_number == 8)
	{
		/*�ؼң����뿪����*/
		go_forward(patient_distance);
		
		/*ת��һ����*/
		turn(turn_list[1]);
			
			
		/*��ǰ�н����ڶ���ʮ��*/
		go_to_home(far_distance_2);
		
			
		/*ת�ڶ�����*/
		turn(turn_list[0]);
		
			
		/*ֱ�ߵ���*/
		go_to_home(far_distance);
	}
	else	//ʲô��û��⵽����������
	{
		Beep();
	}
	}
}

void turn_round()
{
	/*ת��ʼʱ��������ת��������㣬�ǵ�д�ڶ�ʱ�����ۼ�*/
	encoder_right_turn_cnt = 0;
	encoder_left_turn_cnt = 0;
	
	/*��ʼת��*/
	turn_speed = turn_speed;
	while(abs(encoder_right_cnt) - abs(encoder_left_cnt)<= 2*left_90);
}

void go_to_home(float distance)
{
	/*�ܾ�������*/
	total_distance = 0;
	while(total_distance <= distance && openmv_state != reach_DottedLine);
	
}




