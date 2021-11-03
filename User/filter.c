#include "filter.h"
/*
float K1 =0.02;
float angle, angle_dot; // 
float Q_angle=0.001; // 
float Q_gyro=0.003; //
float R_angle=0.5; // 
float dt=0.005; // 
char C_0 = 1; // 
float Q_bias, Angle_err; // 
float PCt_0, PCt_1, E; 
float K_0, K_1, t_0, t_1; 
float Pdot[4] ={0,0,0,0}; 
float PP[2][2] = {{ 1, 0 },{ 0, 1 } }; 
*/
double Angle=0,Angley=0,Anglez = 0;
float  dt=0.005;    //定时器周期
//******卡尔曼滤波参数-X轴*************************************
float  Q_angle=0.001;     //对加速度计的信任度
float  Q_gyro=0.003;      //对陀螺仪的信任度
float  R_angle=0.5;
char   C_0 = 1;
float  Q_bias, Angle_err;
float  PCt_0, PCt_1, E;
float  K_0, K_1, t_0, t_1;
float  Pdot[4] ={0,0,0,0};
float  PP[2][2] = { { 1, 0 },{ 0, 1 } }; 
//******卡尔曼滤波参数-Y轴*************************************
float  Q_biasy, Angle_erry;
float  PCt_0y, PCt_1y, Ey;
float  K_0y, K_1y, t_0y, t_1y;
float  Pdoty[4] ={0,0,0,0};
float  PPy[2][2] = { { 1, 0 },{ 0, 1 } };
float Roll=0,Pitch=0;
float Roll_g=0,Pitch_g=0;
/*
float Kalman_Filter(float Accel,float Gyro) 
{
	angle+=(Gyro - Q_bias) * dt; 
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; 
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;
	PP[0][1] += Pdot[1] * dt; 
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	
	Angle_err = Accel - angle;
	PCt_0 = C_0 * PP[0][0]; 
	PCt_1 = C_0 * PP[1][0]; 
	E = R_angle + C_0 * PCt_0; 
	K_0 = PCt_0 / E; 
	K_1 = PCt_1 / E;

	t_0 = PCt_0; 
	t_1 = C_0 * PP[0][1]; 
	PP[0][0] -= K_0 * t_0; 
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	angle += K_0 * Angle_err; 
	Q_bias += K_1 * Angle_err; 
	angle_dot = Gyro - Q_bias; 
	return angle;
}
*/

void Kalman_Filter(float Accel,float Gyro)		
{
	Angle+=(Gyro - Q_bias) * dt; //陀螺仪积分角度（测量值-陀螺仪偏差)*dt  
//Angle相当于是系统的预测值
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // 先验估计误差协方差的微分
	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=  Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // 先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - Angle;//估计值与预测值之间的偏差
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;
	K_0 = PCt_0 / E;//卡尔曼增益1 用于计算最优估计值
	K_1 = PCt_1 / E;//卡尔曼增益2 用于计算最后估计的偏差
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		 //更新协方差矩阵
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	Angle	+= K_0 * Angle_err;//根据卡尔曼增益1算出最优角度
	Q_bias	+= K_1 * Angle_err;//根据卡尔曼增益2算出预测值偏差
	
// 	Angle+=(Gyro - Q_bias) * dt; //先验估计
// 	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

// 	Pdot[1]=-PP[1][1];
// 	Pdot[2]=-PP[1][1];
// 	Pdot[3]=Q_gyro;
// 	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
// 	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
// 	PP[1][0] += Pdot[2] * dt;
// 	PP[1][1] += Pdot[3] * dt;
// 		
// 	Angle_err = Accel - Angle;	//zk-先验估计
// 	
// 	PCt_0 = C_0 * PP[0][0];
// 	PCt_1 = C_0 * PP[1][0];
// 	
// 	E = R_angle + C_0 * PCt_0;
// 	
// 	K_0 = PCt_0 / E;
// 	K_1 = PCt_1 / E;
// 	
// 	t_0 = PCt_0;
// 	t_1 = C_0 * PP[0][1];

// 	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
// 	PP[0][1] -= K_0 * t_1;
// 	PP[1][0] -= K_1 * t_0;
// 	PP[1][1] -= K_1 * t_1;
// 		
// 	Angle	+= K_0 * Angle_err;	 //后验估计
// 	Q_bias	+= K_1 * Angle_err;	 //后验估计
}
void Kalman_Filter_Y(float Accely,float Gyroy)		
{
	Angley+=(Gyroy - Q_biasy) * dt; 
	Pdoty[0]=Q_angle - PPy[0][1] - PPy[1][0]; 
	Pdoty[1]=- PPy[1][1];
	Pdoty[2]=- PPy[1][1];
	Pdoty[3]=Q_gyro;
	PPy[0][0] += Pdoty[0] * dt;   
	PPy[0][1] += Pdoty[1] * dt;   
	PPy[1][0] += Pdoty[2] * dt;
	PPy[1][1] += Pdoty[3] * dt;
	Angle_erry = Accely - Angley;	
	PCt_0y = C_0 * PPy[0][0];
	PCt_1y = C_0 * PPy[1][0];
	Ey = R_angle + C_0 * PCt_0y;
	K_0y = PCt_0y / Ey;
	K_1y = PCt_1y / Ey;
	t_0y = PCt_0y;
	t_1y = C_0 * PPy[0][1];
	PPy[0][0] -= K_0y * t_0y;		 
	PPy[0][1] -= K_0y * t_1y;
	PPy[1][0] -= K_1y * t_0y;
	PPy[1][1] -= K_1y * t_1y;
	Angley	+= K_0y * Angle_erry;	 
	Q_biasy	+= K_1y * Angle_erry;	
	
	
// 	Angley+=(Gyroy - Q_biasy) * dt; //先验估计
// 	Pdoty[0]=Q_angle - PPy[0][1] - PPy[1][0]; // Pk-先验估计误差协方差的微分

// 	Pdoty[1]=-PPy[1][1];
// 	Pdoty[2]=-PPy[1][1];
// 	Pdoty[3]=Q_gyro;
// 	PPy[0][0] += Pdoty[0] * dt;   // Pk-先验估计误差协方差微分的积分
// 	PPy[0][1] += Pdoty[1] * dt;   // =先验估计误差协方差
// 	PPy[1][0] += Pdoty[2] * dt;
// 	PPy[1][1] += Pdoty[3] * dt;
// 		
// 	Angle_erry = Accely - Angley;	//zk-先验估计
// 	
// 	PCt_0y = C_0 * PPy[0][0];
// 	PCt_1y = C_0 * PPy[1][0];
// 	
// 	Ey = R_angle + C_0 * PCt_0y;
// 	
// 	K_0y = PCt_0y / Ey;
// 	K_1y = PCt_1y / Ey;
// 	
// 	t_0y = PCt_0y;
// 	t_1y = C_0 * PPy[0][1];

// 	PPy[0][0] -= K_0y * t_0y;		 //后验估计误差协方差
// 	PPy[0][1] -= K_0y * t_1y;
// 	PPy[1][0] -= K_1y * t_0y;
// 	PPy[1][1] -= K_1y * t_1y;
// 		
// 	Angley	+= K_0y * Angle_erry;	 //后验估计
// 	Q_biasy	+= K_1y * Angle_erry;	 //后验估计
}
float T_gr = 3,T_gp =18.5;

void IMU_filter(void)
{
//	if(Angle_az<0){Angle_gy=-Angle_gy;}
//	Kalman_Filter(atan2(Angle_ax,Angle_az)*57.2957795f,-Angle_gy);       //卡尔曼滤波

//	
//	if(Angle_az<0){Angle_gx=-Angle_gx;}
//	Kalman_Filter_Y(atan2(Angle_ay,Angle_az)*57.2957795f,Angle_gx);      //卡尔曼滤波
	
//	if(Angle_az<0){Angle_gz=-Angle_gz;}
//	Kalman_Filter_Y(atan2(Angle_ax,Angle_ay)*57.2957795f,Angle_gz);      //卡尔曼滤波
	
//	Roll=Angle;
//	Pitch=Angley;
	Angle_gx = Angle_gx + T_gr * (Roll - Roll_g);
	Angle_gy = Angle_gy + T_gp * (Pitch - Pitch_g);
	Roll_g += Angle_gx*0.02;
	Pitch_g += Angle_gy*0.02;
	Pitch=atan(Angle_ax/sqrt(Angle_ay*Angle_ay+Angle_az*Angle_az))*57.2957795f; //后面的数字是180/PI 目的是弧度转角度
	Roll=atan(Angle_ay/sqrt(Angle_ax*Angle_ax+Angle_az*Angle_az))*57.2957795f;
	



}

