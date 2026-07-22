#include <STC32G.H>
#include "intrins.h"
#include "delay.h"  
#include "motor.h" 
#include "timer.h"    
#include "uart1.h" 
#include "sr04.h" 
int state=2;
char csb_toggle = 0;
char hwxj_toggle = 0;
/********************* 主函数 *************************/
void yanshi(float a) //延时 单位：秒
{
  int i,k,j,l;  
	a=a*8;
	for(k=0;k<a;k++)
	{
		for (i = 0; i < 50000; i++)    
		{    
			for (j = 0; j < 50000; j++)    
			{    
				for (l = 0; l < 50000; l++);
			} 
		}  
	}
}
void int_uart(void) interrupt  4	
{

if (TI)											// 发送中断标志置1时
	{
		TI = 0;										// 清零发送中断标志
	}
	
		if (RI)											// 接收中断标志置1时
	{
		RI = 0;	
		switch(SBUF)
		{
			//前进 01
			case 0x01:
			Car_State_number=1;
			state=1;
			break;
			//停止 02
			case 0x02:
			Car_State_number=5;
			state=2;
			break;
			//后退 03
			case 0x03:
			Car_State_number=2;
			state=3;
			break;
			//右转 04
			case 0x04:
			Car_State_number=4;
			state=4;
			break;
			//左转 05
			case 0x05:
			Car_State_number=3;
			state=5;
			break;
			//加速 06
			case 0x06:
			if(pwm==20)
				{
				pwm=40;
				}
			else if(pwm==40)
				{
				pwm=60;
				}
			else if(pwm==60)
				{
				pwm=80;
				}
			else if(pwm==80)
				{
				pwm=100;
				}

			break;
			//减速 07
			case 0x07:
			if(pwm==60)
				{
				pwm=40;
				}
			else if(pwm==80)
				{
				pwm=60;
				}
			else if(pwm==100)
				{
				pwm=80;
				}
			else if(pwm==40)
				{
				pwm=20;
				}	
			break;
			case 0x08: //超声波
			 csb_toggle = !csb_toggle;
			if(!csb_toggle){Car_State_number=5;delay_ms(500);Car_State_number=5;P42=0;P43=0;P41=0;}
			break;
			case 0x09: //红外循迹
			 hwxj_toggle = !hwxj_toggle;
			if(!hwxj_toggle){Car_State_number=5;delay_ms(500);Car_State_number=5;P42=0;P43=0;P41=0;}
			break;
			case 0x10: //左灯
			 P42 = !P42;
			break;
			case 0x11: //右灯
			 P43 = !P43;
			break;
			case 0x12: //蜂鸣器
			 P41 = !P41;
			break;
		}
				SBUF=SBUF+0X30;
    //单片机将接收数据直接发送给电脑端（串口助手）
	}

}



void main()
{
	
   WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
   EAXFR = 1; //扩展寄存器(XFR)访问使能
   CKCON = 0; //提高访问XRAM速度
	 
	 Motor_Init_Port(); //电机初始化
	
	 Timer0Init(); //定时器 初始化 ,超声波使用
	 
	 Timer4Init(); //定时器初始化,计数时间使用
	
	 Uart1_Init(); //串口初始化 

	 SR04_Init_Port(); // 超声波IO初始化
	//SendDataByUart1(0x38);
	/*
			case 1:  Car_Forword(25);break;
			case 2:  Car_Back(50);break;
			case 3:  Car_Turn_Left(90);break;
			case 4:  Car_Turn_Right(90);break;
			case 5:  Car_Stop();break;
	*/
	
while(1)
	{ 
	 //SendDataByUart1(0x37);
		if(csb_toggle){
		if(Time_Count>=10) //100ms更新一次超声波距离
			{				
				 Count_Number_DATA(); //测量距离	
				 Time_Count=0; //重新计时
	    }  
		if(S>=15) //直走
		{
			P41=0;
      Car_State_number=1; 
		}			
		else if(S<15&&S>=10)
					{ 
						
						Car_State_number=2;//后退
						P41=1;
						delay_ms(750);
						P41=0;
						P42=1;
						Car_State_number=3; //左转 
						delay_ms(750);
						P42=0;
					}			
		else if(S<10&&S>=5)
					{
						 Car_State_number=2;//后退
P41=1;
						delay_ms(1500);
P41=0;
P43=1;
						 Car_State_number=4; //右3转  
						 delay_ms(1500);
						P43=0;
					}else{
						P41=1;
           Car_State_number=2;//后退
					}
  }
		if(hwxj_toggle){
			
		}
	}
}























