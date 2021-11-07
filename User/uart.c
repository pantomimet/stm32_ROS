#include "uart.h"

u8 rxbuf[8],Urxbuf[8],CAN_ON_Flag,Usart_ON_Flag,Usart_Flag,Usart_End_Flag,PID_Send;  //CAN和串口控制相关变量
u8 txbuf[8],txbuf2[8];  //CAN发送相关变量
u8 RX_BUF[16] = {0};
u8 TX_BUF[16] = {0};
void usart1_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_9;//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10; //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;

	USART_InitStructure.USART_Parity = USART_Parity_No;

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能串口接收中断
//	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); //使能串口1的DMA接收   
	USART_Cmd(USART1, ENABLE);
}

int send_cnt= 0;
u8 Send_rasberry[60];
int re_Encoder_Left,re_Encoder_Right;

void USART_TX(void)
{
	u8 * Send_rasberry_ptr = &Send_rasberry[0];
	*(Send_rasberry_ptr+0) = 0xA5; 

	*(Send_rasberry_ptr+1) = 0x5A;

	*(Send_rasberry_ptr+2) = 0x2E;

	re_Encoder_Left = -Encoder_Left;
	re_Encoder_Right = -Encoder_Right;
	
	//左编码器
	for(send_cnt=0; send_cnt<4; send_cnt++)	
		*(Send_rasberry_ptr+3+send_cnt) = ((unsigned char *)&re_Encoder_Left)[send_cnt];
	//右编码器
	for(send_cnt=0; send_cnt<4; send_cnt++)
		*(Send_rasberry_ptr+7+send_cnt) = ((unsigned char *)&re_Encoder_Right)[send_cnt];
	
	
//	for(send_cnt=0; send_cnt<4; send_cnt++) //电池采样
//		Send_rasberry[11+send_cnt] = ((unsigned char *)&Voltage)[send_cnt]; 
	//没有电压采集，暂时取消电压
	
	for(send_cnt=0; send_cnt<2; send_cnt++) // X轴加速度计
		*(Send_rasberry_ptr+15+send_cnt) = ((unsigned char *)&accelX)[send_cnt];
	for(send_cnt=0; send_cnt<2; send_cnt++) // Y轴加速度计
		*(Send_rasberry_ptr+17+send_cnt) = ((unsigned char *)&accelY)[send_cnt];
	for(send_cnt=0; send_cnt<2; send_cnt++) // Z轴加速度计
		*(Send_rasberry_ptr+19+send_cnt) = ((unsigned char *)&accelZ)[send_cnt];
	
	//send gyroXYZ
	for(send_cnt=0; send_cnt<2; send_cnt++) //x轴角速度计
		*(Send_rasberry_ptr+21+send_cnt) = ((unsigned char *)&gyroX)[send_cnt];
	for(send_cnt=0; send_cnt<2; send_cnt++) //Y轴角速度计
		*(Send_rasberry_ptr+23+send_cnt) = ((unsigned char *)&gyroY)[send_cnt];
	for(send_cnt=0; send_cnt<2; send_cnt++) //z轴角速度计
		*(Send_rasberry_ptr+25+send_cnt) = ((unsigned char *)&gyroZ)[send_cnt];

	//send MAG X YZ
	for(send_cnt=0; send_cnt<2; send_cnt++) //X ?????
		*(Send_rasberry_ptr+27+send_cnt) = ((unsigned char *)&magX)[send_cnt];
	for(send_cnt=0; send_cnt<2; send_cnt++) //Y ?????
		*(Send_rasberry_ptr+29+send_cnt) = ((unsigned char *)&magY)[send_cnt];
	for(send_cnt=0; send_cnt<2; send_cnt++) //z ?????
		*(Send_rasberry_ptr+31+send_cnt) = ((unsigned char *)&magZ)[send_cnt];
	
	//send ultrasonicABCD
//	for(send_cnt=0; send_cnt<4;send_cnt++) // ??????A
//	Send_rasberry[33+send_cnt] = ((unsigned char *)&Distance_A)[send_cnt];
//	for(send_cnt=0; send_cnt<4; send_cnt++) // ??????B
//	Send_rasberry[37+send_cnt] = ((unsigned char *)&Distance_B)[send_cnt];
//	for(send_cnt=0; send_cnt<4;send_cnt++) // ??????C
//	Send_rasberry[41+send_cnt] = ((unsigned char *)&Distance_C)[send_cnt];
//	for(send_cnt=0; send_cnt<4; send_cnt++) // ??????D
//	Send_rasberry[45+send_cnt] = ((unsigned char *)&Distance_D)[send_cnt];
	
	//send Send_rasberry串口发送的方式
	for(send_cnt = 0;send_cnt < 49;send_cnt++)
	{
		USART_SendData(USART2,*(Send_rasberry_ptr+send_cnt));
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
		//(Send_rasberry[send_cnt]);
	}
	memset(Send_rasberry_ptr, 0, sizeof(u8)*33);
	
	
	//DMA发送
//	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送      
//	MYDMA_Enable(DMA1_Channel4);//开始一次DMA传输！	

}



void USART1_IRQHandler(void)
{
	u8 temp;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		
		static u8 count,last_data,last_last_data,Usart_ON_Count;
		if(Usart_ON_Flag==0)
		{
			if(++Usart_ON_Count>10) Usart_ON_Flag=1;
		}
		temp=USART1->DR;
		if(Usart_Flag==0)
		{
			if(last_data==0x5A && last_last_data==0xA5)
			Usart_Flag=1;
			Urxbuf[0] = 0xA5;
			Urxbuf[1] = 0X5A;
			count=2;
		}
		if(Usart_Flag==1) 
		{
			Urxbuf[count]=temp;
			count++;
			if(Urxbuf[count] == 0x55 && Urxbuf[count-1] == 0xAA)
			{
				Usart_Flag=0;
//				Get_commands();
			}
		}
		last_last_data=last_data;
		last_data=temp;
	}
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET){
//        USART_ReceiveData(USART1);
//        USART_ClearFlag(USART1, USART_FLAG_ORE);
//    }   
    USART_ClearFlag(USART1, USART_IT_RXNE);
}

void usart2_init(u32 bound)
{  	 
	  //GPIO????
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	//GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);//引脚重定义
	//USART_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //A2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	//USART_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//UsartNVIC ??
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
   //USART ?????
	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????
	USART_Init(USART2, &USART_InitStructure);     //?????3
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//????????
	USART_Cmd(USART2, ENABLE);                    //????3 
}


void USART2_IRQHandler(void)
{	
	u8 temp;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //?????
	{
		static u8 count,last_data,last_last_data,Usart_ON_Count;
		if(Usart_ON_Flag==0)
		{
			if(++Usart_ON_Count>10) Usart_ON_Flag=1;
		}
		temp=USART2->DR;
		if(Usart_Flag==0)
		{
			if(last_data==0x5A && last_last_data==0xA5)
			{
				Usart_Flag=1;
				RX_BUF[0] = 0xA5;
				RX_BUF[1] = 0X5A;
			}
			count=2;
		}
		if(Usart_Flag==1) 
		{
			RX_BUF[count]=temp;
			
			if(RX_BUF[count] == 0x55 && RX_BUF[count-1] == 0xAA)
			{
				Usart_Flag=0;
//				Get_commands();
				Get_openmv();
			}
			count++;
		}
		last_last_data=last_data;
		last_data=temp;
	}
	USART_ClearFlag(USART2, USART_IT_RXNE);
} 

u8 usart2_Send_rasberry[10];
int send_i=0;
int u2_cnt;
void USART2_TX(void)
{
	u8 * TX_BUF_ptr = &TX_BUF[0];

	*(TX_BUF_ptr + 0) = 0XA5;
	*(TX_BUF_ptr + 1) = 0X5A;
	*(TX_BUF_ptr + 2) = state_7;
	*(TX_BUF_ptr + 3) = 0XAA;
	*(TX_BUF_ptr + 4) = 0X00;

	for(u2_cnt = 0;u2_cnt<5;u2_cnt++)
	{
		USART_SendData(USART2,*(TX_BUF_ptr+u2_cnt));
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
	
		//(Send_rasberry[send_cnt]);
//	}
//	memset(usart2_Send_rasberry_ptr, 0, sizeof(u8)*10);
	
	
	//DMA发送
//	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送      
//	MYDMA_Enable(DMA1_Channel4);//开始一次DMA传输！	

}


void usart3_init(u32 bound)
{  	 
	  //GPIO????
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// ????AFIO??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//??GPIO??
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//??USART??
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);//?????
	//USART_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //C10 O_Pin_4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
	GPIO_Init(GPIOC, &GPIO_InitStructure);   
  //USART_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11  GPIO_Pin_3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  //UsartNVIC ??
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
   //USART ?????
	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????
	USART_Init(USART3, &USART_InitStructure);     //?????3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//????????
	USART_Cmd(USART3, ENABLE);                    //????3 
}
u8 USART3_TX[16] ;
void usart3_send(u8 data) 
{
	int u3_cnt;
	USART3_TX[0] = 0X5A;
	USART3_TX[1] = car1_cmd;
	USART3_TX[2] = 0X55;
	for(u3_cnt=0;u3_cnt < 6;u3_cnt++)
	{
		USART3->DR = USART3_TX[u3_cnt];
		while((USART3->SR&0x40)==0);
	}
	
}

void USART3_IRQHandler(void)
{
	u8 temp;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		
		static u8 count,last_data,last_last_data,Usart_ON_Count;
//		if(Usart_ON_Flag==0)
//		{
//			if(++Usart_ON_Count>10) Usart_ON_Flag=1;
//		}
		temp=USART3->DR;
		if(Usart_Flag==0)
		{
			if(last_data==0x5A)
			{
				Usart_Flag=1;
				RX_BUF[0] = 0x5A;
			}
			count=1;
		}
		else if(Usart_Flag==1) 
		{
			RX_BUF[count]=temp;
			
			if(count == 2 && RX_BUF[count] == 0x55 && RX_BUF[count-2] == 0x5A)
			{
				Usart_Flag=0;
//				Get_commands();
				car1_cmd = RX_BUF[1];
			}
			count++;
		}
		last_data=temp;
	}
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET){
//        USART_ReceiveData(USART1);
//        USART_ClearFlag(USART1, USART_FLAG_ORE);
//    }   
    USART_ClearFlag(USART3, USART_IT_RXNE);
}


