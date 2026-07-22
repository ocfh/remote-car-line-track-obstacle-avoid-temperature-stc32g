#include <STC32G.H>
#include "intrins.h"
#include "delay.h"  
#include "motor.h" 
#include "timer.h"    
#include "uart1.h" 
#include "sr04.h" 
char buf;
sbit BEEF=P4^1;
int state=2;
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
void send(unsigned a) //延时 单位：秒
{
	ES   = 0;	
	SBUF = a;/*向串行口1发送数据寄存器SBUF发出1个字节*/
	while(TI==0);//等待发送完成
	TI = 0;		//清除发送中断标志位	
	ES   = 1;
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
				send(0x02);
				}
			else if(pwm==40)
				{
				pwm=60;
				send(0x03);
				}
			else if(pwm==60)
				{
				pwm=80;
				send(0x04);
				}
			else if(pwm==80)
				{
				pwm=100;
				send(0x05);
				}
			else if(pwm==100)
				{
				send(0x05);
				}

			break;
			//减速 07
			case 0x07:
			if(pwm==60)
				{
				pwm=40;
				send(0x02);
				}
			else if(pwm==80)
				{
				pwm=60;
				send(0x03);
				}
			else if(pwm==100)
				{
				pwm=80;
				send(0x04);
				}
			else if(pwm==40)
				{
				pwm=20;
				send(0x01);
				}
			else if(pwm==20)
				{
				Car_Stop();
				send(0x01);
				}
				
			break;
		}			
		buf = SBUF+0x30;
		SBUF = buf; 
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
		send(0x37); 

	 SR04_Init_Port(); // 超声波IO初始化
	/*
				case 1:  Car_Forword(25);break;
			case 2:  Car_Back(50);break;
			case 3:  Car_Turn_Left(90);break;
			case 4:  Car_Turn_Right(90);break;
			case 5:  Car_Stop();break;
	*/
	
  while (1)
	{ 
	 send(0x38); //pwm=20 一档

		/*
		//Car_Forword(50);	
		//后退
						delay_ms(2000);
						Car_State_number=7; //左转 
						delay_ms(2000);
						Car_State_number=8; //左转 
						delay_ms(2000);
		Car_State_number=5; //左转 
		delay_ms(2000);
		*/
		  if(Time_Count>=10) //100ms更新一次超声波距离
			{				
				 Count_Number_DATA(); //测量距离
								
				 Time_Count=0; //重新计时
	    }  
			
		if(S>=20) //直走
		{
      Car_State_number=1; 
		}			
		else if(S<20&&S>=10)
					{ 
						Car_State_number=2;//后退
						delay_ms(100);
						Car_State_number=3; //左转 
						delay_ms(2000);
					}			
		else if(S<10&&S>=3)
					{
						 Car_State_number=2;//后退
						 delay_ms(500);
						 Car_State_number=4; //右3转  
						 delay_ms(500);
					}else{

           Car_State_number=2;//后退

					}	
					
				
  } 
}























