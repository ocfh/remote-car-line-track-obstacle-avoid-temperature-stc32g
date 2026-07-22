#include "motor.h" 
#include "delay.h"  
#include "timer.h"  

/***************************电机端口定义*************************************/
sbit LH_B=P2^0;
sbit LH_F=P2^1; 

sbit RH_F=P2^2;
sbit RH_B=P2^3;

sbit RQ_F=P1^4;
sbit RQ_B=P1^5;

sbit LQ_F=P1^6;
sbit LQ_B=P1^7;  

void Motor_Init_Port(void)
{
		P2M0 = 0x0F;        
  	P2M1 = 0x00;	
	
		P1M0 = 0xF0;        
  	P1M1 = 0x00;	
	
		P3M0 = 0x20;         //将P3.6口单独设置为推挽输出模式  0100 0000
		P3M1 = 0x00;	
	  delay_ms(50); //等待稳定
}
void Car_Forword(uint8 pwm_perent)//前进
{  
	
 PWM_Count = pwm_perent;
	
	LH_B=0;  
	LH_F=1;
	
	RH_F=1;  
	RH_B=0;
	
	RQ_F=0;
	RQ_B=1;
	
	LQ_F=0;
	LQ_B=1;
}
void Car_Back(uint8 pwm_perent)//后退
{ 
 PWM_Count = pwm_perent; 
	
	LH_B=1;  
	LH_F=0; 
	
	LQ_F=1;
	LQ_B=0;	 
	
	RH_F=0;  
	RH_B=1;
	
	RQ_F=1;
	RQ_B=0;
	

}
void Car_Turn_Left(uint8 pwm_perent)//左转
{
  PWM_Count = pwm_perent;  
	
	LH_B=1;  
	LH_F=0; 
	
	LQ_F=1;
	LQ_B=0;	 
	
	RH_F=1;  
	RH_B=0;
	
	RQ_F=0;
	RQ_B=1;
	
}
void Car_Turn_Right(uint8 pwm_perent)//右转
{
  PWM_Count = pwm_perent; 
	
	LQ_F=0;
	LQ_B=1;	
	
	LH_B=0;  
	LH_F=1;
	
	RH_F=0;  
	RH_B=1;
	
	RQ_F=1;
	RQ_B=0;

}
void Car_Stop(void)//停止
{ 
	
	LH_B=0;  
	LH_F=0;
	
	RH_F=0;  
	RH_B=0;
	
	RQ_F=0;
	RQ_B=0;
	
	LQ_F=0;
	LQ_B=0;
}




































