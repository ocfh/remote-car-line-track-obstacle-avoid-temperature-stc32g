#include <STC32G.H>
#include "intrins.h"
#include "delay.h"  
#include "motor.h" 
#include "timer.h"  
#include "ds18B20.h"   
#include "adc.h"    
#include "uart1.h" 
#include "sr04.h" 
#include "oledfont.h" 
#include "hw.h"  
sbit   OLED_SDA = P4^4;
sbit   OLED_SCL = P4^5;
int state=2;

char csb_toggle = 0;
char hwxj_toggle = 0;
char oled = 1;
//-----------------OLED函数定义----------------  					   
void IIC_Start();
void delay(void);//声明延时函数
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();

void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ColorTurn(unsigned char oled_Color);
void OLED_DisplayTurn(unsigned char oled_Display);

void OLED_ShowChar6X8(unsigned char chr_y,unsigned char chr_x,unsigned char chr);
void OLED_ShowChar8X16(unsigned char chr_y,unsigned char chr_x,unsigned char chr);
void OLED_Show_HZ16X16(unsigned char hz_y,unsigned char hz_x,unsigned char no);   
//-----------------OLED函数声明----------------  					   

/******************************************/
//起始信号
/******************************************/
void IIC_Start()
{
	OLED_SCL = 1;
	OLED_SDA = 1;
	OLED_SDA = 0;
	OLED_SCL = 0;
}
/******************************************/
//停止信号
/******************************************/
void IIC_Stop()
{
	OLED_SCL = 1;
	OLED_SDA = 0;
	OLED_SDA = 1;
}
/******************************************/
//等待I2信号响应
/******************************************/
void IIC_Wait_Ack()
{
	OLED_SCL = 1;
	OLED_SCL = 0;
}
/******************************************/
//写入一个字节
/******************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	static unsigned char i;
	static unsigned char m,da;
	
	da=IIC_Byte;
	OLED_SCL = 0;
	for(i=0;i<8;i++)		
	{
		m=da;
		OLED_SCL = 0;
		m=m&0x80;
        
		if(m==0x80)
        {
            OLED_SDA = 1;
        }
		else OLED_SDA = 0;
        {
               da=da<<1;
        }
			
		OLED_SCL = 1;//将时钟信号设置为高电平
		OLED_SCL = 0;//将时钟信号设置为低电平
	}
}

/******************************************/
//发送一个字节
//向屏的控制器SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
/******************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}
/**********************************************
// IIC写数据
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}
//**************************************************************************
//坐标设置
//**************************************************************************
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	Write_IIC_Command(0xb0+y);
	Write_IIC_Command(((x&0xf0)>>4)|0x10);
	Write_IIC_Command(x&0x0f); 
} 
//**************************************************************************
//开启OLED显示  
//**************************************************************************
void OLED_Display_On(void)
{
	Write_IIC_Command(0x8d);//电荷泵使能
	Write_IIC_Command(0x14);//开启电荷泵
	Write_IIC_Command(0xaf);//点亮屏幕
}
//**************************************************************************
//关闭OLED显示
//**************************************************************************
void OLED_Display_Off(void)
{
	Write_IIC_Command(0X8D);  //电荷泵使能
	Write_IIC_Command(0X10); //关闭电荷泵
	Write_IIC_Command(0XAE); //关闭屏幕 
}
//**************************************************************************
//初始化SSD1306			
//**************************************************************************
void OLED_Init(void)
{ 	
 
	Write_IIC_Command(0xAE);//--display off
	Write_IIC_Command(0x00);//---set low column address
	Write_IIC_Command(0x10);//---set high column address
	Write_IIC_Command(0x40);//--set start line address  
	Write_IIC_Command(0xB0);//--set page address
	Write_IIC_Command(0x81); // contract control
	Write_IIC_Command(0xFF);//--128   
	Write_IIC_Command(0xA1);//set segment remap 
	Write_IIC_Command(0xA6);//--normal / reverse
	Write_IIC_Command(0xA8);//--set multiplex ratio(1 to 64)
	Write_IIC_Command(0x3F);//--1/32 duty
	Write_IIC_Command(0xC8);//Com scan direction
	Write_IIC_Command(0xD3);//-set display offset
	Write_IIC_Command(0x00);//
	
	Write_IIC_Command(0xD5);//set osc division
	Write_IIC_Command(0x80);//
	
	Write_IIC_Command(0xD8);//set area color mode off
	Write_IIC_Command(0x05);//
	
	Write_IIC_Command(0xD9);//Set Pre-Charge Period
	Write_IIC_Command(0xF1);//
	
	Write_IIC_Command(0xDA);//set com pin configuartion
	Write_IIC_Command(0x12);//
	
	Write_IIC_Command(0xDB);//set Vcomh
	Write_IIC_Command(0x30);//
	
	Write_IIC_Command(0x8D);//set charge pump enable
	Write_IIC_Command(0x14);//
	
	Write_IIC_Command(0xAF);//--turn on oled panel
}
//**************************************************************************
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
//**************************************************************************
void OLED_Clear(void)  
{  
	static unsigned char i,n;		    
	
	for(i=0;i<8;i++)  
	{  
		Write_IIC_Command (0xb0+i);    //设置行起始地址（0~7）
		Write_IIC_Command (0x00);      //设置低列起始地址
		Write_IIC_Command (0x10);      //设置高列起始地址  
		for(n=0;n<128;n++)
        {
            Write_IIC_Data(0); 
        }//更新显示
    }
}

	
/******************************************/
//反显函数
//参数：oled_Color，值：0正常显示，1 反色显示
/******************************************/
void OLED_ColorTurn(unsigned char oled_Color)
{
	if(oled_Color==0)
	{
		Write_IIC_Command(0xA6);//正常显示
	}
	if(oled_Color==1)
	{
		Write_IIC_Command(0xA7);//反色显示
	}
}
/******************************************/
//屏幕旋转180度
//参数：oled_Display，值：0正常显示，1 反转显示
/******************************************/
void OLED_DisplayTurn(unsigned char oled_Display)
{
	if(oled_Display==0)
	{
		Write_IIC_Command(0xC8);//正常显示
		Write_IIC_Command(0xA1);
	}
	if(oled_Display==1)
	{
		Write_IIC_Command(0xC0);//反转显示
		Write_IIC_Command(0xA0);
	}
}
//**************************************************************************
//此函数功能：在指定位置显示一个8X8点阵的字符、数字，
//一行共有128点，显示8点的字符可以有chr_x：0-15,分别代表每行的列数，0-15共16个8*8点阵的内容
//一列共有 64点，显示8点的字符可以有chr_y：0-7,分别代表行数，0-7共8行个8*8点阵的内容 
//**************************************************************************
void OLED_ShowChar6X8(unsigned char chr_y,unsigned char chr_x,unsigned char chr)
{      	
	static unsigned char c,i;	
	static unsigned char x,y;
	
	x = chr_x*8;   //规定占用8点阵
	y = chr_y;     //8点阵只能占用4行
	
	c = chr - ' ';	//得到偏移后的值			
	if(x>127)
	{
		x = 0;
		y = y+2;
	}
	OLED_Set_Pos(x,y);
	for(i=0;i<6;i++)
	{
		Write_IIC_Data(F6x8[c][i]);
	}	
}

//**************************************************************************
//此函数功能：在指定位置显示一个8X16点阵的字符、数字，占用半个汉字位置
//一行共有128点，显示8点的字符可以有X：0-15,分别代表每行的列数，0-15共16个8*16点阵的内容
//一列共有 64点，显示16点的字符可以有X：0-3,分别代表行数，0-3共4行个8*16点阵的内容 
//**************************************************************************
void OLED_ShowChar8X16(unsigned char chr_y,unsigned char chr_x,unsigned char chr)
{      	
	static unsigned char c,i;	
	static unsigned char x,y;
	
	x = chr_x*8;   //规定占用8点阵
	y = chr_y*2;   //16点阵只能占用4行
	
	c = chr - ' ';	//得到偏移后的值			
	if(x>127)
	{
		x = 0;
		y = y+2;
	}
	OLED_Set_Pos(x,y);	
	for(i=0;i<8;i++)
	{
		Write_IIC_Data(F8X16[c*16+i]);
	}
	
	OLED_Set_Pos(x,y+1);
	
	for(i=0;i<8;i++)
	{
		Write_IIC_Data(F8X16[c*16+i+8]);
	}
}
//**************************************************************************
//此函数功能：在指定位置显示一个16X16点阵的汉字（字符、数字）
//显示汉字hz_x:0-7,分别代表每行的列数，0-7共8个16*16点阵汉字 
//显示汉字hz_y:0-3,分别代表每页行数，因为64点阵最多显示4行汉字
//**************************************************************************
void OLED_Show_HZ16X16(unsigned char hz_y,unsigned char hz_x,unsigned char no)
{      			    
	static unsigned char t,adder=0;
	static unsigned char x,y;
	
	x = hz_x*16;   //规定占用16点阵
	y = hz_y*2;    //规定占用4行
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
	{
		Write_IIC_Data(Hzk[2*no][t]);
		adder+=1;
    }	
	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		Write_IIC_Data(Hzk[2*no+1][t]);
		adder+=1;
    }					
}
 
/************************************************/
/***第1类函数：无参数返回、    同时无参数传递********/
/**              void     delay   (void)**/
/************************************************/
void delay(void)//定义延时函数
{
	 //定义静态局部变量，加上静态static 
	static unsigned int i,j;

	for (i=0;i<60000;i++)//
	{
		for (j=0;j<60000;j++);//
		{
			;
		}
	}
}


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
			if(!csb_toggle){
				Car_State_number=5;delay_ms(1500);Car_State_number=5;P42=0;P43=0;P41=0;
			}
			break;
			case 0x09: //红外循迹
			 hwxj_toggle = !hwxj_toggle;
			if(!hwxj_toggle){
			Car_State_number=5;delay_ms(1500);Car_State_number=5;P42=0;P43=0;P41=0;
			}
			break;
			case 0x10: //左灯
			 P42 = !P42;
			break;
			case 0x11: //右灯
			 P43 = !P43;
			break;
			case 0x12: //开蜂鸣器
			 P41 = 1;
			break;
			case 0x13: //关蜂鸣器
			 P41=0;
			break;
			case 0x14: //SR04
			 User_Get_ADC_data();
			break;
		}
				//SendDataByUart1(SBUF+0X30);
    //单片机将接收数据直接发送给电脑端（串口助手）
	}

}


/********************* 主函数 *************************/
void main()
{
	static unsigned int num,no,i,j;//定义静态局部变量，加上静态static 
   WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
   EAXFR = 1; //扩展寄存器(XFR)访问使能
   CKCON = 0; //提高访问XRAM速度
	 
	 Motor_Init_Port(); //电机初始化
	
	 Timer0Init(); //定时器 初始化 ,超声波使用
	 
	 Timer4Init(); //定时器初始化,计数时间使用
	
	 Uart1_Init(); //串口初始化 
		ADC_Init_Port(); //ADC初始化
	 SR04_Init_Port(); // 超声波IO初始化
	HW_Init_Port(); //红外端口
	OLED_Init();			//初始化OLED  
	OLED_Clear();//更新显示
	//SendDataByUart1(0x38);
	/*
			case 1:  Car_Forword(25);break;
			case 2:  Car_Back(50);break;
			case 3:  Car_Turn_Left(90);break;
			case 4:  Car_Turn_Right(90);break;
			case 5:  Car_Stop();break;
	*/
	//OLED_ColorTurn(1);
	
	start_ds18b20();
	User_Get_ADC_data();
	Count_Number_DATA();
	get_ds18b20();
while(1)
	{ 
		delay_ms(1000);
		OLED_Clear();
		if(oled){
	 //SendDataByUart1(0x37);
	OLED_ShowChar8X16(0,0,'A');
	OLED_ShowChar8X16(0,1,'D');
	OLED_ShowChar8X16(0,2,'C');
	OLED_ShowChar8X16(0,3,':');

	OLED_ShowChar8X16(0,8,'m');
	OLED_ShowChar8X16(0,9,'V');

	
	OLED_ShowChar8X16(1,0,'C');
	OLED_ShowChar8X16(1,1,'a');
	OLED_ShowChar8X16(1,2,'r');
	OLED_ShowChar8X16(1,3,':');
	OLED_ShowChar8X16(1,4,'L');
	OLED_ShowChar8X16(1,5,'v');



	OLED_ShowChar8X16(2,0,'R');
	OLED_ShowChar8X16(2,1,'a');
	OLED_ShowChar8X16(2,2,'n');
	OLED_ShowChar8X16(2,3,'g');
	OLED_ShowChar8X16(2,4,'e');
	OLED_ShowChar8X16(2,5,':');

	OLED_ShowChar8X16(2,10,'c');
	OLED_ShowChar8X16(2,11,'m');
	
	OLED_ShowChar8X16(3,0,'T');
	OLED_ShowChar8X16(3,1,'e');
	OLED_ShowChar8X16(3,2,'m');
	OLED_ShowChar8X16(3,3,'p');
	OLED_ShowChar8X16(3,4,':');
	OLED_ShowChar8X16(3,7,'.');
	OLED_ShowChar8X16(3,10,'^');
	OLED_ShowChar8X16(3,11,'C');
	
	OLED_ShowChar8X16(0,4,atemp);
	OLED_ShowChar8X16(0,5,atemp1);
	OLED_ShowChar8X16(0,6,atemp2);
	OLED_ShowChar8X16(0,7,atemp3);
	OLED_ShowChar8X16(2,6,stemp);
	OLED_ShowChar8X16(2,7,stemp1);
	OLED_ShowChar8X16(2,8,stemp2);
	OLED_ShowChar8X16(2,9,stemp3);
	OLED_ShowChar8X16(3,5,dtemp);
	OLED_ShowChar8X16(3,6,dtemp1);
	OLED_ShowChar8X16(3,8,dtemp2);
	OLED_ShowChar8X16(3,9,dtemp3);
	if(csb_toggle){
	OLED_ShowChar8X16(1,8,'W');
	OLED_ShowChar8X16(1,9,'a');
	OLED_ShowChar8X16(1,10,'v');
	OLED_ShowChar8X16(1,11,'e');
	OLED_ShowChar8X16(1,12,' ');
	}else if(hwxj_toggle){
	OLED_ShowChar8X16(1,8,'T');
	OLED_ShowChar8X16(1,9,'r');
	OLED_ShowChar8X16(1,10,'a');
	OLED_ShowChar8X16(1,11,'i');
	OLED_ShowChar8X16(1,12,'l');
	}else if(state==2){
	OLED_ShowChar8X16(1,8,'S');
	OLED_ShowChar8X16(1,9,'t');
	OLED_ShowChar8X16(1,10,'o');
	OLED_ShowChar8X16(1,11,'p');
	OLED_ShowChar8X16(1,12,' ');
	}else if(state==1){
	OLED_ShowChar8X16(1,8,'G');
	OLED_ShowChar8X16(1,9,'o');
	OLED_ShowChar8X16(1,10,' ');
	OLED_ShowChar8X16(1,11,' ');
	OLED_ShowChar8X16(1,12,' ');
	}else if(state==3){
	OLED_ShowChar8X16(1,8,'B');
	OLED_ShowChar8X16(1,9,'a');
	OLED_ShowChar8X16(1,10,'c');
	OLED_ShowChar8X16(1,11,'k');
	OLED_ShowChar8X16(1,12,' ');
	}else if(state==4){
	OLED_ShowChar8X16(1,8,'R');
	OLED_ShowChar8X16(1,9,'i');
	OLED_ShowChar8X16(1,10,'g');
	OLED_ShowChar8X16(1,11,'h');
	OLED_ShowChar8X16(1,12,'t');
	}else if(state==5){
	OLED_ShowChar8X16(1,8,'L');
	OLED_ShowChar8X16(1,9,'e');
	OLED_ShowChar8X16(1,10,'f');
	OLED_ShowChar8X16(1,11,'t');
	OLED_ShowChar8X16(1,12,' ');
	}
	
	if(pwm==20){
		OLED_ShowChar8X16(1,6,'1');
	}else if(pwm==40){
		OLED_ShowChar8X16(1,6,'2');
	}else if(pwm==60){
		OLED_ShowChar8X16(1,6,'3');
	}else if(pwm==80){
		OLED_ShowChar8X16(1,6,'4');
	}else if(pwm==100){
		OLED_ShowChar8X16(1,6,'5');
	}
	
}
	
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
			if(Get_HW_MR==1&&Get_HW_ML==1&&Get_HW_R==1&&Get_HW_L==1)
			 {   
					 Car_State_number=5;  //停车
			 }else{
		
										/******************状态一：直行状态****************************************/	
												//情况一：黑线在正中间，都没有识别到，直走
												if(Get_HW_L==0&&Get_HW_ML==0&&Get_HW_MR==0&&Get_HW_R==0)
												{
													pwm=15;
													Car_State_number=1; //慢速直走 
												}
												//情况二：中间两个识别到黑线，直走
												if(Get_HW_L==0&&Get_HW_ML==1&&Get_HW_MR==1&&Get_HW_R==0)
												{   				
													while(1) //防止由于拐弯过大，要纠正回来
													{  
														pwm=15;
														Car_State_number=1; //慢速直走 
														if(Get_HW_MR==0||Get_HW_ML==0)
														{  
															break; //跳出循环
														} 
													}	
												}
												//情况三：左中侧检测到，稍微左转后直走
												if(Get_HW_L==0&&Get_HW_ML==1&&Get_HW_MR==0&&Get_HW_R==0)// 慢左转
												{   							
													while(1) //防止由于拐弯过大，要纠正回来
													{  
														pwm=50;
														   Car_State_number=3; 					
														if(Get_HW_ML==0||Get_HW_MR==1||Get_HW_R==1)
														{  
															break; //跳出循环
														} 
													}	
												}
												
												//情况四：右中侧检测到，稍微右转后直走
												if(Get_HW_L==0&&Get_HW_ML==0&&Get_HW_MR==1&&Get_HW_R==0)//慢右转
												{   							
													while(1) //防止由于拐弯过大，要纠正回来
													{  
														pwm=50;
														   Car_State_number=4; //慢右转
														if(Get_HW_MR==0||Get_HW_ML==1||Get_HW_L==0)
														{  
															break; //跳出循环
														} 
													}	
												}	
												
										/******************状态二：左转状态****************************************/		
												//情况一：左外侧检测到，大拐弯
												if(Get_HW_L==1&&Get_HW_ML==0&&Get_HW_MR==0&&Get_HW_R==0)//
												{    										
													while(1) //防止由于拐弯过大，要纠正回来
													{  
														pwm=80;
														  Car_State_number=3; //快速左转				
														if(Get_HW_ML==1||Get_HW_MR==1||Get_HW_R==1)
														{  
															break; //跳出循环
														} 
													}	
												}
												//情况二：左外测和左中测检测到，大拐弯
												if(Get_HW_L==1&&Get_HW_ML==1&&Get_HW_MR==0&&Get_HW_R==0)
												{    										
													while(1) //防止由于拐弯过大，要纠正回来
													{  
														pwm=80;
														   Car_State_number=3; //快速左转
				
														if(Get_HW_MR==1||Get_HW_R==1)
														{  
															break; //跳出循环
														} 
													}	
												}
												
										/******************状态三：右转状态****************************************/						
												//右外侧检测到，大拐弯
												if(Get_HW_L==0&&Get_HW_ML==0&&Get_HW_MR==0&&Get_HW_R==1)
												{   						  										
													while(1) //防止由于拐弯过大，要纠正回来
													{  
														pwm=80;
														Car_State_number=4; //快速右转 

														if(Get_HW_MR==1||Get_HW_ML==1||Get_HW_L==1)
														{  
															break; //跳出循环
														} 
													}	
												}	
												//右侧和右外侧检测到，大拐弯
												if(Get_HW_L==0&&Get_HW_ML==0&&Get_HW_MR==1&&Get_HW_R==1)
												{   						  										
													while(1) //防止由于拐弯过大，要纠正回来
													{  
														pwm=80;
														Car_State_number=4; //快速右转 

														if(Get_HW_ML==1||Get_HW_L==1)
														{  
															break; //跳出循环
														} 
													}	
												}	
										/******************状态四：直角处理****************************************/						
												//右外侧检测到，大拐弯
												if(Get_HW_L==0&&Get_HW_ML==1&&Get_HW_MR==1&&Get_HW_R==1)
												{   						  																							 
													  while(1)
														{	
															pwm=85;
														    Car_State_number=4; //快速右转 
																if(Get_HW_L==1)
																{  
																	break; //跳出循环
																} 																
														}
												}	

	   }
		}
	}
}























