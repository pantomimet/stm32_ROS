#include "stm32f10x.h"
#include "sys.h"

u8 Flag_Left,Flag_Right,Flag_Direction=0,Flag_Way,Flag_Next; //����ң����صı���
u8 operationMode;
u8 Flag_Stop=1,Flag_Show;         
int Encoder_Left,Encoder_Right;                   
long int Motor_Left,Motor_Right;  
long int Target_Left,Target_Right;
float Velocity,Angle,Servo;       
u8 delay_50,delay_flag;           
float Velocity_KP=62,Velocity_KI=62;	  
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY,lastPS3Key,Accel_Key; 
int Remoter_Ch1,Remoter_Ch2,Remoter_Ch3,Remoter_Ch4;
float Tand;
int main(void)
  { 
		delay_init();	    	            
		JTAG_Set(SWD_ENABLE);           
		LED_Init();   
			  
		KEY_Init();                     
		MY_NVIC_PriorityGroupConfig(2);	
		//MiniBalance_PWM_Init(7199,0);   
		//Servo_PWM_Init(9999,71);   		  
		OLED_Init();                    
		//Encoder_Init_TIM2();            
		//Encoder_Init_TIM3();            
//		Flag_Way=1;
//		Flag_Show = 0;
//		Flag_Stop=1;	
		delay_ms(500);                  //=====��ʱ�ȴ��ȶ�
		//IIC_Init();
		
		//TIM6_Int_Init(999,7199);
		MPU9250_Init();
		usart1_init(115200);
	  
	  
//		PS2_Init();											
//		PS2_SetInit();									
//	  
//		Target_Left = 0;
//		Target_Right = 0;

		TIM6_Int_Init(9999,7199);      		//=====10ms��ʱ�жϳ�ʼ��
		Accel_Key = 4;
    while(1)
	  {	
			//Led_Flash(1000);	
//			PS2_KEY=PS2_DataKey();

//				PS2_LX=PS2_AnologData(PSS_LX);    //PS2���ݲɼ�    
//				PS2_LY=PS2_AnologData(PSS_LY);
//				PS2_RX=PS2_AnologData(PSS_RX);
//				PS2_RY=PS2_AnologData(PSS_RY);
//			
				//oled_show();          		 //��ʾ����
				delay_flag=1;	
			
				delay_50=0;
				while(delay_flag);	       //ͨ����ʱ�ж�ʵ�ֵ�50ms��׼��ʱ	
	  } 
}

