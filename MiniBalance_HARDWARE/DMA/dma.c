#include "dma.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DMA ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u32 dma_dir,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = dma_dir;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	  	
} 
//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	  


#define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.
u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
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

	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Send_rasberry,DMA_DIR_PeripheralDST,60);//���ͣ�DMA1ͨ��4,����Ϊ����1,�洢��ΪSend_rasberry,����DMA_DIR_PeripheralDST,����60.
	MYDMA_Config(DMA1_Channel5,(u32)&USART1->DR,(u32)rxbuf,DMA_DIR_PeripheralSRC,60);//���գ�DMA1ͨ��5,����Ϊ����1,�洢��Ϊrxbuf,��DMA_DIR_PeripheralSRC,����60.

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
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)	//�ж�ͨ��4�������
	{
		DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ��4������ɱ�־
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	u8 * Urxbuf_ptr = Urxbuf;
	u8 * rxbuf_ptr = rxbuf;
	int i = 0;
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5)==SET)
    {
		DMA_ClearFlag(DMA1_FLAG_TC5);//����жϱ�־λ
		if(*(rxbuf_ptr+0) == 0xA5 && *(rxbuf_ptr+1) == 0x5A)
		{
			for(i = 0;i<8;i++)
			{
				*(Urxbuf_ptr + i) =  *(rxbuf_ptr+i);
			}
		}
    }
}














