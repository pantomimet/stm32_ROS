#include "dma.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DMA 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u32 dma_dir,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = dma_dir;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	  	
} 
//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	  


#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.
u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
//void DMA_USART1_INIT(void)
//{
//	DMA_InitTypeDef DMA_InitStructure; 
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
//    //NVIC_Config();                //??DMA??
//    //NVIC_Configuration();
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;    
//    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;      
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    
//    DMA_InitStructure.DMA_BufferSize = SEND_BUF_SIZE;     
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    //DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     
//    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;       
//    DMA_Init(DMA1_Channel2, &DMA_InitStructure);      
//        //DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);  //??DMA?????????
//    DMA_Cmd (DMA1_Channel2,ENABLE);
//	
//}


void UART_DMA_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Send_rasberry,DMA_DIR_PeripheralDST,60);//发送：DMA1通道4,外设为串口1,存储器为Send_rasberry,方向DMA_DIR_PeripheralDST,长度60.
	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)rxbuf,DMA_DIR_PeripheralSRC,60);//接收：DMA1通道5,外设为串口1,存储器为rxbuf,方DMA_DIR_PeripheralSRC,长度60.

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???

	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);	//?USART1 Tx DMA??
	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???

	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);	//?USART1 Tx DMA??
	
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  	//????3?DMA??
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  	//????3?DMA??
}


void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)	//判断通道4传输完成
	{
		DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道4传输完成标志
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	u8 * Urxbuf_ptr = Urxbuf;
	u8 * rxbuf_ptr = rxbuf;
	int i = 0;
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5)==SET)
    {
		DMA_ClearFlag(DMA1_FLAG_TC5);//清除中断标志位
		if(*(rxbuf_ptr+0) == 0xA5 && *(rxbuf_ptr+1) == 0x5A)
		{
			for(i = 0;i<8;i++)
			{
				*(Urxbuf_ptr + i) =  *(rxbuf_ptr+i);
			}
		}
    }
}














