#include "show.h"
#include "encoder.h"
unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
						int accz;
void oled_show(void)
{
	
//					//=============��3����ʾ������״̬=======================//	
						if( encoder_left_cnt<0)		  OLED_ShowString(00,20,"-"),
																OLED_ShowNumber(15,20,-encoder_left_cnt,5,12);
					else                 	OLED_ShowString(0,20,"+"),
																OLED_ShowNumber(15,20, encoder_left_cnt,5,12); 			
//					if( Encoder_Left<0)		  OLED_ShowString(80,20,"-"),
//																OLED_ShowNumber(95,20,-Encoder_Left,4,12);
//					else                 	OLED_ShowString(80,20,"+"),
//																OLED_ShowNumber(95,20, Encoder_Left,4,12);
//					//=============��4����ʾ�ҵ����״̬=======================//	
						if( encoder_right_cnt<0)		  OLED_ShowString(00,30,"-"),
																OLED_ShowNumber(15,30,-encoder_right_cnt,5,12);
					else                 	OLED_ShowString(0,30,"+"),
																OLED_ShowNumber(15,30, encoder_right_cnt,5,12); 					
//					if( Encoder_Right<0)		  OLED_ShowString(80,30,"-"),
//																OLED_ShowNumber(95,30,-Encoder_Right,4,12);
//					else                 	OLED_ShowString(80,30,"+"),
//																OLED_ShowNumber(95,30, Encoder_Right,4,12);	
//					
//					//=============��5����ʾ�����״̬=======================//	
//				 	OLED_ShowString(00,40,"Servo:"),	//���״̬
//					OLED_ShowNumber(60,40, Servo,4,12);	
//					//=============��6����ʾ��ѹģʽ��=======================//	
//                       //  ���ʹ��/ʹ����ʾ
//											if(Flag_Stop==0)//����Flag_Stop��־λ��ʾ�����״̬
//											OLED_ShowString(103,50,"O-N");
//											if(Flag_Stop==1)
//											OLED_ShowString(80,50,"OFF");
//											//OLED_ShowNumber(80,50, accont,4,12);
					if(mode == 0)
                      OLED_ShowString(0,50,"PS2 "); //ң��ģʽ
					else if(mode == 1)
						OLED_ShowString(0,50,"AUTO"); //ң��ģʽ
											OLED_Refresh_Gram();	//ˢ��
}


