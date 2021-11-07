#include "stm32f10x.h"
#include "sys.h"

u8 Flag_Left,Flag_Right,Flag_Direction=0,Flag_Way,Flag_Next; //蓝牙遥控相关的变量
u8 operationMode;
u8 Flag_Stop=1,Flag_Show;         
int Encoder_Left,Encoder_Right;                   
long int Motor_Left,Motor_Right;
float Target_Left,Target_Right;
float Final_Target_Left,Final_Target_Right;
float Velocity,Target_Angle,Servo;
u8 delay_50,delay_flag;
float Velocity_KP=-11500,Velocity_KI=-2500;
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY,lastPS3Key,Accel_Key;
int Remoter_Ch1,Remoter_Ch2,Remoter_Ch3,Remoter_Ch4;
float Tand;
int flag_50ms = 0;
float ADC_ConvertedValueLocal = 0;
int A0;
int main(void)
  { 
	delay_init();	    	            
	JTAG_Set(SWD_ENABLE);           
	LED_Init();   
		  
	KEY_Init();
	Light_KEY_init();
	Beep_init();	  
	MY_NVIC_PriorityGroupConfig(2);	
	MiniBalance_PWM_Init(7199,0);   
//		Servo_PWM_Init(9999,71);   		  
	OLED_Init();                    
	Encoder_Init_TIM2();            
	Encoder_Init_TIM3();            
	Flag_Way=1;
	Flag_Show = 0;
	Flag_Stop=1;	
	delay_ms(500);                  //=====延时等待稳定

//	MPU9250_Init();
//		MPU6050_Init();
//		usart1_init(115200);
		usart2_init(115200);
//		usart3_init(115200);
//		MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA1通道4,外设为串口1的发送,存储器为SendBuff,长度SEND_BUF_SIZE.
	KEY_Init();
//	PS2_Init();											
//	PS2_SetInit();									
	Target_Left = 0;
	Target_Right = 0;
//		TIM5_Int_Init(4999,71);
//		TIM4_Int_Init(19999,71);			//=====20ms定时中断初始化 #
	TIM6_Int_Init(19999,71);      		//=====定时中断初始化 #20ms--19999	10ms--9999	50ms--49999	60ms--59999
	Accel_Key = 4;
//		UART_DMA_Config();
//		adc_init();
//		MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Send_rasberry,DMA_DIR_PeripheralDST,60);//发送：DMA1通道4,外设为串口1,存储器为Send_rasberry,方向DMA_DIR_PeripheralDST,长度SEND_BUF_SIZE.
//		MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)Urxbuf,DMA_DIR_PeripheralSRC,10);//发送：DMA1通道4,外设为串口1,存储器为Send_rasberry,方向DMA_DIR_PeripheralDST,长度SEND_BUF_SIZE.
	
	/*测试用*/
	while(1){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0)
		{
			delay_ms(100);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0 )
			go_forward(0.69);
			while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
			while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
//			delay_ms(1000);
			turn(left);
			while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
			while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
//			delay_ms(1000);
			go_forward(0.28);
			while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
			while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
//			delay_ms(1000);
//			turn(right);
			turn_round();
			while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
			while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
			go_forward(0.28);
			while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
			while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
//			delay_ms(1000);
			turn(right);
			while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
			while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
//			delay_ms(1000);
			go_forward(0.69);
			while(Final_Target_Right > 1e-3 || -Final_Target_Right > 1e-3 );
			while(Final_Target_Left > 1e-3 || -Final_Target_Left > 1e-3 );
//			delay_ms(1000);
//			else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0 && Target_straight != 0)
//				Target_straight = 0;
		}
		
//		
//		
	A0=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		
	}

	/*控制主函数*/
	

	/*装货阻塞，等待展示数字*/
	wait_to_start();

	/*前往十字*/
	go_to_target();

	/*等待回家*/
//	wait_to_return();
	
	/*回家*/
//	return_home();
	
	while(1)
	  {	
//		  if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)	//判断通道4传输完成
//		  {
//					DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道4传输完成标志
//		  }
//			//Led_Flash(1000);	
//		PS2_KEY=PS2_DataKey();
//		if(PS2_KEY == PSB_START)
//		{
//			mode = !mode;
//		}
//		  if(mode == 0)
//		  {
//			
//			PS2_LX=PS2_AnologData(PSS_LX);    //PS2数据采集    
//			PS2_LY=PS2_AnologData(PSS_LY);
//			PS2_RX=PS2_AnologData(PSS_RX);
//			PS2_RY=PS2_AnologData(PSS_RY);
//		  }
			
//		if(flag_50ms == 1)
//		{
//			readimu();
//			USART_TX();
//			
//			flag_50ms = 0;
//		}
//		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3*11;
//		oled_show();          		 //显示屏打开
				delay_flag=1;	
			
				delay_50=0;
//				while(delay_flag);	       //通过定时中断实现的50ms精准延时	
	  } 
}




