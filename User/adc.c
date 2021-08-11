#include "adc.h"

volatile uint16_t ADC_ConvertedValue;
void adc_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO,ENABLE);	
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;		 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);//ADC??
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue; //????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //??(??????)
	DMA_InitStructure.DMA_BufferSize = 1; //???????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //??????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; //??????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ; //??????
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;    //??????
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; //DMA??:????
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; //???:?
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   //??????????
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //??DMA1?4??
	DMA_Cmd(DMA1_Channel1,ENABLE);
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //??ADC??
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //??????
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//???????? 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //?????????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //???????
	ADC_InitStructure.ADC_NbrOfChannel = 1; //????????
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//??ADC??,?PCLK2?8??,?9Hz
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);//??ADC1??11?55.5????? 
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
 
	ADC_ResetCalibration(ADC1);//???????
	while(ADC_GetResetCalibrationStatus(ADC1));//???????????
 
	ADC_StartCalibration(ADC1);//ADC??
	while(ADC_GetCalibrationStatus(ADC1));//??????
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//??????????,????????ADC??
}


