#include "show.h"
#include "encoder.h"
#include "mpu9250.h"
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
int accz;
void oled_show(void)
{
	OLED_ShowNumber(15,00, (u32)start_number,3,12);
	OLED_ShowNumber(15,10, (u32)car1_cmd,3,12);
	//=============第3行显示左电机的状态=======================//	
	OLED_ShowNumber(15,10,(u32)start_number,5,12);
	if( encoder_left_cnt<0)		  OLED_ShowString(00,20,"-"),
												OLED_ShowNumber(15,20,-encoder_left_cnt,5,12);
	else                 	OLED_ShowString(0,20,"+"),
												OLED_ShowNumber(15,20, encoder_left_cnt,5,12); 			
	if( Encoder_Left<0)		  OLED_ShowString(80,20,"-"),
												OLED_ShowNumber(95,20,-Encoder_Left,4,12);
	else                 	OLED_ShowString(80,20,"+"),
												OLED_ShowNumber(95,20, Encoder_Left,4,12);
	//=============第4行显示右电机的状态=======================//	
		if( encoder_right_cnt<0)		  OLED_ShowString(00,30,"-"),
												OLED_ShowNumber(15,30,-encoder_right_cnt,5,12);
	else                 	OLED_ShowString(0,30,"+"),
												OLED_ShowNumber(15,30, encoder_right_cnt,5,12); 					
	if( Encoder_Right<0)		  OLED_ShowString(80,30,"-"),
												OLED_ShowNumber(95,30,-Encoder_Right,4,12);
	else                 	OLED_ShowString(80,30,"+"),
												OLED_ShowNumber(95,30, Encoder_Right,4,12);	
	
	
	//=============第5行显示换算成mm的距离=======================//	
	if( Encoder_Right<0)		  OLED_ShowString(80,40,"-"),
												OLED_ShowNumber(95,40,-(u32)(total_distance*1000),4,12);
	else                 	OLED_ShowString(80,40,"+"),
												OLED_ShowNumber(95,40, (u32)(total_distance*1000),4,12);	
	

	//=============第6行显示电压模式等=======================//	
//    //  电机使能/使能显示
//	if(Flag_Stop==0)//根据Flag_Stop标志位显示电机的状态
//	OLED_ShowString(103,50,"O-N");
//	if(Flag_Stop==1)
//	OLED_ShowString(80,50,"OFF");
//	//OLED_ShowNumber(80,50, accont,4,12);
//	if(mode == 0)
//		OLED_ShowString(0,50,"PS2 "); //遥控模式
//	else if(mode == 1)
//		OLED_ShowString(0,50,"AUT"); //遥控模式
	OLED_Refresh_Gram();	//刷新
}


