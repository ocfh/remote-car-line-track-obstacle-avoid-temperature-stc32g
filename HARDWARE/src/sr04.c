#include "sr04.h"    
#include "delay.h" 
#include "uart1.h"  
#include "motor.h"  
#include "timer.h"  
sbit Trig = P1^3;
sbit Echo = P4^0; 
uint8 Car_State_number = 0; //小车状态值
uint8 pwm=20;
int i,k,j,l,a=1; 
void SR04_Init_Port(void)
{
		P1M0 = 0x00;        //将P2.0,P2.1,P2.2,P2.3口单独设置为双向输出模式  0000 1111
  	P1M1 = 0x00;	
	
		P4M0 = 0x0E;        //将P1.4,P1.5,P1.6,P1.7口单独设置为双向输出模式  0000 1111
  	P4M1 = 0x00;	 
	P41=!P41;
	P42=!P42;
	P43=!P43;
}
 void  StartModule(void) 		         //启动模块
  {
	  Trig=1;			                     //启动一次模块
		delay_ms(5);
	  Trig=0;
  }
void Count_Number_DATA(void)
{
		   StartModule(); //启动模块
	     while(!Echo);		//当RX为零时等待
	     TR0=1;			    //开启计数
	     while(Echo);			//当RX为1计数并等待
	     TR0=0;				//关闭计数   
       Calculation_Data();	
}
unsigned int  time=0; 
unsigned int S=0; 
bit  flag =0;
void Calculation_Data(void)
{
	 unsigned char temp=0,temp1=0,temp2=0,temp3=0;
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	 S=(time*1.87)/100/2;     //算出来是CM  
	     temp=(S/1000)+'0'; 
	     temp1=(S/100%10)+'0'; 
	     temp2=(S/10%10)+'0';
	     temp3=(S%10)+'0';
			 SendDataByUart1(temp);
			 SendDataByUart1(temp1 );
			 SendDataByUart1(temp2);
			 SendDataByUart1(temp3); 				
}
void Adjust_Pwm_data(void)
{
		switch(Car_State_number)
		{
			case 1:  
				Car_Forword(pwm);
			break;
			case 2:  
				Car_Back(pwm);
			break;
			case 3:  
				Car_Turn_Left(pwm);
			break;
			case 4:  
				Car_Turn_Right(pwm);
			break;
			case 5:  
				Car_Stop();
			break;
			default :Car_Stop(); break;
		}	
}





















