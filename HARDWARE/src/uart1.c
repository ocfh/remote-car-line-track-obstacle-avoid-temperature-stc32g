#include "uart1.h" 
#include "delay.h" 
#include "delay.h"  
#include "timer.h"   
void Uart1_Init(void)
{	 
	P3M1 = 0x00; // 清除P3.6和P3.7的位，设置为准双向口	P3M0 &= 0xFC;	                  //设置P3.0 ,P3.1为准双向口  
	P3M0 = 0x00; 
	EA  = 1;       //开启总中断
	ET1 = 1;       //允许定时器1中断
	
	S1_S1 = 0;			//P3.6:RXD1_2 
	S1_S0 = 1;			//P3.7:TXD1_2 

	//AUXR = 0x01;		/*降低T1、T0、串行口的速度,选择T2作为定时器2作波特率发生器方式*/	
	S1M0x6 = 0;			//串行口1的模式0不加速

	SCON = 0x50;		/*初始化串行口。工作方式1，8位数据发送 */
	
	//用定时器2来做波特率发生器时的初始化代码
	S1BRT  = 1;			//选择T2作为定时器2作波特率发生器
	T2R   = 0;			//禁止定时器2
    T2_CT = 0;			//定时器2作为定时器用
    T2x12 = 0;			//定时器2工作在12T模式下
    T2H = 0xff;			//通信波特率为19200时，定时器初值为FFF4H
    T2L = 0xf4;
    ET2 = 0;    		//禁止中断
    T2R = 1;			//启动定时器2
	
	PCON = 0x80;		/*SMOD=1,波特率变为19200 */
	//波特率变为19200 ,表示传输一个字节，所耗的时间为0.5us(500ns)
	
	REN  = 1;			/*允许串行口接收*/
	ES   = 1;			/*开启串行口中断 */
	EA  = 1;    //总中断打开 
	//B_TX1_Busy = 0;
	delay_ms(100);
}
void SendDataByUart1(unsigned char dat)
{
  SBUF = dat;                 //写数据到UART数据寄存器
	while(TI==0);//等待发送完成
	TI = 0;		//清除发送中断标志位	
} 

void PrintString1(unsigned char *puts)
{
    for (; *puts != 0;  puts++)     //遇到停止符0结束
    {
        SBUF = *puts;
        //B_TX1_Busy = 1;
        //while(B_TX1_Busy);
    }
}











