#include "filter.h"	
#include "control.h"
#define ZERO 1E-6
u8 Flag_Target,Flag_Change;  //��ر�־λ
float Voltage_Count,Voltage_All;  //��ѹ������ر���
int j,sum;
u8 mode = 1; //�ֶ����Զ�ģʽ���ֶ�Ϊ0���Զ�Ϊ1
#define T 0.245f
#define L 0.29f
#define K 16.00f
/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ������ٶȺ�ת��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle)
{
		Servo=SERVO_INIT+angle*K; //���ת��   angle*
		if(Servo > 2050){
			Servo = 2050;
			angle = (double)(Servo - SERVO_INIT)/K;
		}
		else if(Servo < 1280){
			Servo = 1280;
			angle = (double)(Servo - SERVO_INIT)/K;
		}
		
		Tand = tan(angle/57.3);;//(int)tan(angle);
//		if(abs(velocity) < ZERO)
//		{
//			Target_Left = 0;
//			Target_Right = 0;
//		}
//		else
//		{
			Target_Left=velocity*(1-1.5*T*Tand/2/L); 
			Target_Right=-velocity*(1+1.5*T*Tand/2/L);      //���ֲ���
//		}
//		Servo=SERVO_INIT+angle*K; //���ת��   
}
float Target_Left_dream,Target_Right_dream;

void stm32_smooth(void)
{
	if(Target_Left < 0.000001 && (-Target_Left) > 0.000001)
	{
		Target_Left_dream = 0;
	}
	else if(Target_Left_dream < Target_Left)
	{
		Target_Left_dream = Target_Left_dream + 0.02;
	}
	else if(Target_Left_dream > Target_Left)
	{
		Target_Left_dream = Target_Left_dream - 0.02;
	}
	
	if(Target_Right < 0.000001 && (-Target_Right) > 0.000001)
	{
		Target_Right_dream = 0;
	}
	else if(Target_Right_dream < Target_Right)
	{
		Target_Right_dream = Target_Right_dream + 0.02;
	}
	else if(Target_Right_dream > Target_Right)
	{
		Target_Right_dream = Target_Right_dream - 0.02;
	}
}

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         ��ʱ�жϴ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
short accont = 0;
int LEFT=0;
int X=1600;
float Velocity_L,Velocity_R;
float Velocity_dream;
int time_flag = 0;
float v_now_l = 0,v_now_r = 0;
void TIM6_IRQHandler(void)   //TIM6�ж�
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 	
	{     	
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);          //����жϱ�־λ  	
//			Flag_Target=!Flag_Target; //��Ƶ��־λ
//			if(delay_flag==1)
//			{
//					if(++delay_50==2)	 delay_50=0,delay_flag=0; //���������ṩ50ms�ľ�׼��ʱ
//			}
//			if(Flag_Target==1)
//			{
//			  	Key();//ɨ�谴���仯	
//   									                                        
//			}
//			else if(Flag_Target == 0)
//			{   
		Encoder_Right=Read_Encoder(3);  //===��ȡ��������ֵ
		Encoder_Left=Read_Encoder(2);    //===��ȡ��������ֵ
		readimu();
		USART_TX();
		PS2_KEY=PS2_DataKey();
		if(PS2_KEY == PSB_START)
		{
//			delay_ms(200);
			if(PS2_DataKey() == PSB_START)
				mode = !mode;
		}
//		  if(mode == 0)
//		  {
//			
////			PS2_LX=PS2_AnologData(PSS_LX);    //PS2���ݲɼ�    
//			PS2_LY=PS2_AnologData(PSS_LY);
//			PS2_RX=PS2_AnologData(PSS_RX);
////			PS2_RY=PS2_AnologData(PSS_RY);
//		  }
					
//				if(Encoder_Right < 210 && Encoder_Right > -210){
//					Encoder_Right = 0;
//					Velocity_R = 0;
//				}
//				else 
//				{
//					Velocity_R = (Encoder_Right + 200) / 0.6 / 26.9;
//				}
//				if(Encoder_Left < 210 && Encoder_Left > -210){
//					Encoder_Left = 0;
//					Velocity_L = 0;
//				}
//				else{
//					
//					Velocity_L = (Encoder_Left + 200) / 0.6 / 26.9;
//				}
//				if(mode == 0)
//				{
//				
//					//Key();//ɨ�谴���仯	
//					Get_RC();   //===���տ���ָ��
//				}
				Get_commands();
				/*�������٣�����������	6.14ע�ͣ���PS2�ֱ�ͨ���ƴ���*/
//				if(Velocity_dream < Velocity)
//				{
//					Velocity_dream = Velocity_dream + 0.5;
//				}
//				else if(Velocity_dream > Velocity)
//				{
//					Velocity_dream = Velocity_dream - 0.5;
//				}
				Kinematic_Analysis(Velocity_dream,-Target_Angle); 	//С���˶�ѧ����   
				v_now_l = (float)Encoder_Left*20/biaoding_1m;
				v_now_r = (float)Encoder_Right*20/biaoding_1m;
				if(mode == 0)
				{
//					stm32_smooth();
					Incremental_PI_Left(v_now_l,Target_Left);  
					Incremental_PI_Right(v_now_r,Target_Right);//    *11/17
				}
//				stm32_smooth();
				else if(mode == 1)
				{
					Incremental_PI_Left(v_now_l,Target_Left);  
					Incremental_PI_Right(v_now_r,Target_Right);//    *11/17
				}
//				Incremental_PI_Left(Encoder_Left,Target_Left);  
//				Incremental_PI_Right(Encoder_Right,Target_Right);//    *11/17
				Xianfu_Pwm(6900);                          //===PWM�޷�
				Set_Pwm(Motor_Left,Motor_Right,Servo);     //===��ֵ��PWM�Ĵ���  Servo
//				Set_Pwm(LEFT,-LEFT,1650);
				
				//accont += gyroX;
				//oled_show();
//				readimu();	
//				USART_TX();
//				if(time_flag == 0)
//					oled_show(); 
//				
//				time_flag = !time_flag;
//			}	
	}
} 
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{
	  	if(motor_a>0)			PWMA1=7200,PWMA2=7200-motor_a;
			else 	            PWMA2=7200,PWMA1=7200+motor_a;
		
		  if(motor_b>0)			PWMB1=7200,PWMB2=7200-motor_b;
			else 	            PWMB2=7200,PWMB1=7200+motor_b;
	   SERVO=servo;	
}
/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(int amplitude)
{	
    if(Motor_Left<-amplitude) Motor_Left=-amplitude;	//������Сֵ
		if(Motor_Left>amplitude)  Motor_Left=amplitude;	  //�������ֵ
	  if(Motor_Right<-amplitude) Motor_Right=-amplitude;//������Сֵ	
		if(Motor_Right>amplitude)  Motor_Right=amplitude;	//�������ֵ		
}
/************************************************************************
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp,tmp2;
	tmp=click_N_Double(50); //˫����˫���ȴ�ʱ��500ms
	if(tmp==1)Flag_Stop=!Flag_Stop;//��������С������ͣ
	if(tmp==2)Flag_Show=!Flag_Show;//˫������С������ʾ״̬
	tmp2=Long_Press();  //����        
  if(tmp2==1)Flag_Show=!Flag_Show;//����С������ʾ״̬                 
}
/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<9500||Flag_Stop==1)//��ص�ѹ����9.5V�رյ��
			{	                                                
      temp=1;      
     	PWMA1=0; //�������λ����                                           
			PWMB1=0; //�������λ����
			PWMA2=0; //�������λ����
			PWMB2=0; //�������λ����					
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int PWM_LEFT,PWM_RIGHT;
float Bias_L,Pwm_L,Last_bias_L;
float Bias_R,Pwm_R,Last_bias_R;
float result;
void Incremental_PI_Left (float Encoder,float Target)
{ 	
	 
	 
	 Bias_L=Encoder - Target;                //����ƫ��
//	if(Bias_L < 0.05 && Bias_L > -0.05)
//		Bias_L = 0;
	 Pwm_L+=Velocity_KP*(Bias_L-Last_bias_L)+Velocity_KI*Bias_L;   //����ʽPI������11288
	 if(Pwm_L>7200)Pwm_L=7200;
	 if(Pwm_L<-7200)Motor_Left=-7200;
	 Motor_Left = Pwm_L;
	 Last_bias_L=Bias_L;	                   //������һ��ƫ�� 
//	 return Motor_Left;                         //�������
}

void Incremental_PI_Right (float Encoder,float Target)
{ 	
	 
	 Bias_R=Encoder - Target;                //����ƫ��
//	if(Bias_R < 0.05 && Bias_R > -0.05)
//		Bias_R = 0;
	 Pwm_R+=Velocity_KP*(Bias_R-Last_bias_R)+Velocity_KI*Bias_R;   //����ʽPI������
	 if(Pwm_R>7200)Pwm_R=7200;
	 if(Pwm_R<-7200)Pwm_R=-7200;
	 Motor_Right = Pwm_R;
	 Last_bias_R=Bias_R;	                   //������һ��ƫ�� 
//	 return Motor_Right;                         //�������
}
/**************************************************************************
�������ܣ�ͨ��ָ���С������ң��
��ڲ���������ָ��
����  ֵ���� 
**************************************************************************/
void Get_RC(void)
{
		int Yuzhi=2;  		
		float LY,RX;  
		PS2_LY=PS2_AnologData(PSS_LY);
		PS2_RX=PS2_AnologData(PSS_RX);
		LY=PS2_LY - 128;
		RX=PS2_RX - 128;
		if( LY>-Yuzhi && LY<Yuzhi )LY=0;
		if( RX>-Yuzhi && RX<Yuzhi )RX=0;
		Velocity_dream=(float)LY/256;	
//		Velocity_dream=-1 * X;
		Target_Angle=RX*0.25; 	
			
}

void Get_commands(void)
{
//	u8 Direction;
//	u8 Angle;
//	u8 Velocity;
//	u8 mode;
	u8 Position;
//	int8_t V_t;
	
	//��ȡģʽ
	//mode = Urxbuf[4];
	
	if(mode == 1)//�Զ�ģʽ
	{
		//��ȡ����ͽǶ�
//		Direction = Urxbuf[1];
//	
//		if(Direction == 0x00)
//			Angle = 0;
//		else if(Direction == 0x10)//����
		if(Urxbuf[2] == 0 )
			Target_Angle = 0;
		else if(Urxbuf[0] == 150)
			Target_Angle = 0;
		else 
			Target_Angle = 60 - Urxbuf[2];
//		else if(Direction == 0x20)//����
//			Angle = Urxbuf[2];
	
		//��ȡ��λ���ٶ�
		Position = Urxbuf[5];
		if(Position == 0)
			Velocity = 0;
		else if(Position == 0x01)
		{
			Velocity_dream = 0.03125*0.43* Urxbuf[3];
			
		}
//			Velocity_dream = -1.5*Urxbuf[3] ;
		else if(Position == 0x02)
		{
			Velocity_dream = -0.03125*0.43*Urxbuf[3] ;
		}
//			Velocity = -Urxbuf[3] * 0.25;
	}
	else if(mode == 0)
	{
		Get_RC();
		/*�������٣��ŷ���������*/
//		if(Velocity_dream < Velocity)
//		{
//			Velocity_dream = Velocity_dream + 1.0;
//		}
//		else if(Velocity_dream > Velocity)
//		{
//			Velocity_dream = Velocity_dream - 1.0;
//		}
	}
	
	
}
