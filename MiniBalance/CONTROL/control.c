#include "filter.h"	
#include "control.h"
#define ZERO 1E-6
u8 Flag_Target,Flag_Change;  //相关标志位
float Voltage_Count,Voltage_All;  //电压采样相关变量
int j,sum;
int next_move = 255;
u8 mode = 1; //手动或自动模式。手动为0，自动为1
#define T 0.245f
#define L 0.29f
#define K 16.00f
#define Balance_angle 0.00

float pos_err,pos_err_pre,pos_err_sum;
float pos_kp = 0.0020,pos_ki=0,pos_kd=0.0035;
float pos_pid_output;
float Target_straight = 0;
float total_distance = 0;

/**************************************************************************
函数功能：小车运动数学模型
入口参数：速度和转角
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle)
{
		Servo=SERVO_INIT+angle*K; //舵机转向   angle*
		if(Servo > 2050){
			Servo = 2050;
			angle = (double)(Servo - SERVO_INIT)/K;
		}
		else if(Servo < 1220){
			Servo = 1220;
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
			Target_Right=-velocity*(1+1.5*T*Tand/2/L);      //后轮差速
//		}
//		Servo=SERVO_INIT+angle*K; //舵机转向   
}

float b_kp=-70,b_ki=0,b_kd=-10;
float angle_err_pre = 0;
float Balance_PWM_output =0;
void Balance_PID(float angle_set, float angle_real)
{
	float angle_err = 0;
	angle_err = angle_set - angle_real;
	Balance_PWM_output = b_kp * angle_err + b_ki * angle_err + b_kd * ( angle_err - angle_err_pre);
	angle_err_pre = angle_err;
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
函数功能：所有的控制代码都在这里面
         定时中断触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
short accont = 0;
int LEFT=0;
int X=1600;
float Velocity_L,Velocity_R;
float Velocity_dream;
int time_flag = 0;
float v_now_l = 0,v_now_r = 0;
float Balance_v = 0;
void TIM6_IRQHandler(void)   //TIM6中断
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 	
	{     	
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);          //清除中断标志位 
//			if(flag_50ms == 0)
//			{
//				flag_50ms =1;
//			}
//			Flag_Target=!Flag_Target; //分频标志位
//			if(delay_flag==1)
//			{
//					if(++delay_50==2)	 delay_50=0,delay_flag=0; //给主函数提供50ms的精准延时
//			}
//			if(Flag_Target==1)
//			{
//			  	Key();//扫描按键变化	
//   									                                        
//			}
//			else if(Flag_Target == 0)
//			{  

		/* 读取编码器的值*/
		Encoder_Right=Read_Encoder(3);  //===读取编码器的值
		Encoder_Left=Read_Encoder(2);    //===读取编码器的值
		
		encoder_right_turn_cnt += Encoder_Right;
		encoder_left_turn_cnt += Encoder_Left;
		
//		PS2_KEY=PS2_DataKey();
//		if(PS2_KEY == PSB_START)
//		{
////			delay_ms(200);
//			if(PS2_DataKey() == PSB_START)
//				mode = !mode;
//		}
//		readimu();
//		Balance_PID(Balance_angle,Pitch_g);
//		
//		Balance_v = 1.756e-4 * (float)Balance_PWM_output - 0.04;
		
////		USART_TX();
////		PS2_KEY=PS2_DataKey();
//			USART_TX();
//			usart3_send(0);
			USART2_TX();
////		if(PS2_KEY == PSB_START)
////		{
//////			delay_ms(200);
////			if(PS2_DataKey() == PSB_START)
////				mode = !mode;
////		}
//				Get_commands();
//				Kinematic_Analysis(Velocity_dream,-Target_Angle); 	//小车运动学分析
				
				/* 通过编码器解算当前两轮速度*/
				v_now_l = (float)-Encoder_Left*50/biaoding_1m;
				v_now_r = (float)Encoder_Right*50/biaoding_1m;
				total_distance = (encoder_right_cnt - encoder_left_cnt)/biaoding_1m;
				
//				if(mode == 0)
//				{
////					stm32_smooth();
//					Incremental_PI_Left(v_now_l,Target_Left);  
//					Incremental_PI_Right(v_now_r,Target_Right);//    *11/17
//				}
////				stm32_smooth();
//				else if(mode == 1)
//				{
//					Incremental_PI_Left(v_now_l,Target_Left);  
//					Incremental_PI_Right(v_now_r,Target_Right);//    *11/17
//				}
////				Incremental_PI_Left(Encoder_Left,Target_Left);  
////				Incremental_PI_Right(Encoder_Right,Target_Right);//    *11/17
//				Position_PID(image_err);
				if(TX_BUF[2] == state_1 || TX_BUF[2] == state_2)
					image_err =0;
				Position_PID((float)image_err);
				
				Target_Left = Target_straight +  turn_flag * pos_pid_output + turn_speed;
				Target_Right = Target_straight - turn_flag * pos_pid_output - turn_speed;
				
				if(Target_Left > 1.5) Target_Left = 1.5;
				else if(Target_Left < -1.5) Target_Left = -1.5;
				if(Target_Right > 1.5) Target_Right = 1.5;
				else if(Target_Right < -1.5) Target_Right = -1.5;
				
				
//				if(Final_Target_Left - Target_Left < 0.005 && Target_Left - Final_Target_Left  < 0.005 && Target_Left == 0)
//				{
//					Final_Target_Left = 0;
//				}
//				else if(Final_Target_Left < Target_Left)
//				{
//					Final_Target_Left = Target_Left + 0.005;
//				}
//				else if(Final_Target_Left > Target_Left)
//				{
//					Final_Target_Left = Target_Left - 0.005;
//				}
//				
//				if(Final_Target_Right - Target_Right < 0.005 && Target_Right - Final_Target_Right < 0.005 && Target_Right == 0)
//				{
//					Final_Target_Right = 0;
//				}
//				else if(Final_Target_Right < Target_Right)
//				{
//					Final_Target_Right = Target_Right + 0.005;
//				}
//				else if(Final_Target_Right > Target_Right)
//				{
//					Final_Target_Right = Target_Right - 0.005;
//				}
				
				Incremental_PI_Left(v_now_l,Target_Left);  
				Incremental_PI_Right(v_now_r,Target_Right);//    *11/17
//				Motor_Left = -Balance_PWM_output;
//				Motor_Right = Balance_PWM_output;
				Xianfu_Pwm(6900);                          //===PWM限幅
				Set_Pwm(Motor_Left,-Motor_Right,Servo);     //===赋值给PWM寄存器  Servo
				
	
	}
} 


void Position_PID(float err)
{
	pos_err = err;
	pos_err_sum += pos_err;
	pos_pid_output = pos_kp * pos_err + pos_ki * pos_err_sum + pos_ki * (pos_err - pos_err_pre);
	pos_err_pre = pos_err;
}



/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
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
函数功能：限制PWM赋值 
入口参数：幅值
返回  值：无
**************************************************************************/
void Xianfu_Pwm(int amplitude)
{	
    if(Motor_Left<-amplitude) Motor_Left=-amplitude;	//限制最小值
		if(Motor_Left>amplitude)  Motor_Left=amplitude;	  //限制最大值
	  if(Motor_Right<-amplitude) Motor_Right=-amplitude;//限制最小值	
		if(Motor_Right>amplitude)  Motor_Right=amplitude;	//限制最大值		
}
/************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp,tmp2;
	tmp=click_N_Double(50); //双击，双击等待时间500ms
	if(tmp==1)Flag_Stop=!Flag_Stop;//单击控制小车的启停
	if(tmp==2)Flag_Show=!Flag_Show;//双击控制小车的显示状态
	tmp2=Long_Press();  //长按        
  if(tmp2==1)Flag_Show=!Flag_Show;//控制小车的显示状态                 
}
/**************************************************************************
函数功能：异常关闭电机
入口参数：电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<9500||Flag_Stop==1)//电池电压低于9.5V关闭电机
			{	                                                
      temp=1;      
     	PWMA1=0; //电机控制位清零                                           
			PWMB1=0; //电机控制位清零
			PWMA2=0; //电机控制位清零
			PWMB2=0; //电机控制位清零					
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int PWM_LEFT,PWM_RIGHT;
float Bias_L,Pwm_L,Last_bias_L;
float Bias_R,Pwm_R,Last_bias_R;
float result;
void Incremental_PI_Left (float Encoder,float Target)
{ 	
	 
	 
	 Bias_L=Target - Encoder;                //计算偏差
//	if(Bias_L < 0.05 && Bias_L > -0.05)
//		Bias_L = 0;
	 Pwm_L+=Velocity_KP*(Bias_L-Last_bias_L)+Velocity_KI*Bias_L;   //增量式PI控制器11288
	 if(Pwm_L>7200)Pwm_L=7200;
	 if(Pwm_L<-7200)Pwm_L=-7200;
	 Motor_Left = Pwm_L;
	 Last_bias_L=Bias_L;	                   //保存上一次偏差 
//	 return Motor_Left;                         //增量输出
}

void Incremental_PI_Right (float Encoder,float Target)
{ 	
	 
	 Bias_R = Target - Encoder;                //计算偏差
//	if(Bias_R < 0.05 && Bias_R > -0.05)
//		Bias_R = 0;
	 Pwm_R+=Velocity_KP*(Bias_R-Last_bias_R)+Velocity_KI*Bias_R;   //增量式PI控制器
	 if(Pwm_R>7200)Pwm_R=7200;
	 if(Pwm_R<-7200)Pwm_R=-7200;
	 Motor_Right = Pwm_R;
	 Last_bias_R=Bias_R;	                   //保存上一次偏差 
//	 return Motor_Right;                         //增量输出
}
/**************************************************************************
函数功能：通过指令对小车进行遥控
入口参数：串口指令
返回  值：无 
**************************************************************************/
float LY,RX; 
void Get_RC(void)
{
		int Yuzhi=2;  		
		 
		PS2_LY=PS2_AnologData(PSS_LY);
		PS2_RX=PS2_AnologData(PSS_RX);
		LY=PS2_LY - 128;
		RX=PS2_RX - 128;
		if( LY>-Yuzhi && LY<Yuzhi )LY=0;
		if( RX>-Yuzhi && RX<Yuzhi )RX=0;
		Velocity_dream=(float)LY/128;	
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
	
	//获取模式
	//mode = Urxbuf[4];
	
	if(mode == 1)//自动模式
	{
		//获取方向和角度
//		Direction = Urxbuf[1];
//	
//		if(Direction == 0x00)
//			Angle = 0;
//		else if(Direction == 0x10)//向左
		if(Urxbuf[2] == 0 )
			Target_Angle = 0;
		else if(Urxbuf[0] == 150)
			Target_Angle = 0;
		else 
			Target_Angle = 60 - Urxbuf[2];
//		else if(Direction == 0x20)//向右
//			Angle = Urxbuf[2];
	
		//获取档位和速度
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
		/*缓慢加速，放防抱死防打滑*/
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

void Get_openmv(void)
{
 	openmv_state = RX_BUF[2];
	if(openmv_state == 0x00)//十字
	{
		openmv_number = (RX_BUF[6]<<24) | (RX_BUF[5]<<16) | (RX_BUF[4]<<8) | RX_BUF[3];
		image_err = 0;
		if(next_move == 255)
			next_move = openmv_number;
	}
	else if(openmv_state == 0x01)//虚线
	{
		openmv_number = (RX_BUF[6]<<24) | (RX_BUF[5]<<16) | (RX_BUF[4]<<8) | RX_BUF[3];
		image_err = 0;
	}
	else if(openmv_state == 0x02)//寻线
	{
		image_err =  (RX_BUF[6]<<24) | (RX_BUF[5]<<16) | (RX_BUF[4]<<8) | RX_BUF[3];
		image_err = image_err - 80;
		openmv_number = 0;
	}
	else if(openmv_state == 0x03)//数字
	{
		openmv_number = (RX_BUF[6]<<24) | (RX_BUF[5]<<16) | (RX_BUF[4]<<8) | RX_BUF[3];
		image_err = 0;
	}
}


