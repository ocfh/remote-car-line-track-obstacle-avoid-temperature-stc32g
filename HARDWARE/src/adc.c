#include "adc.h"  
#include "uart1.h" 
#include "delay.h" 
#include "stdio.h" 
#include <stdio.h>
int BGV,BGP,VCC;
extern char atemp=0,atemp1=0,atemp2=0,atemp3=0;
void	ADC_config(void)
{	
	P_SW2 |= 0x80;		      //将EAXFR位置1，以访问在XDATA区域的扩展SFR
	ADCTIM = 0x3f;		      //设置ADC内部时序，ADC采样时间建议设最大值
	P_SW2 &= 0x7F;		      //将EAXFR位置0，恢复访问XRAM	

	ADCCFG|=0x20;           //ADC转换结果ADC_RES存高4位，ADC_RESL存低8位
	ADCCFG|=0x0F;           //设置ADC时钟为系统时钟/2/16/16

	ADC_CONTR&=0xDF;        //清AD转换完成标志
	ADC_CONTR|=0x80;      	//开AD转换电源
	
}
uint16	Get_ADC12bitResult(uint8 channel)	
{
	  ADC_RES = 0;        //将ADC转换结果高位寄存器ADC_RES清空
    ADC_RESL = 0;       //将ADC转换结果低位寄存器ADC_RESL清空

    ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | channel;  //启动AD转换并选择通道
   
	  while((ADC_CONTR & 0x20) == 0) ;   //等待AD转换完成
    ADC_CONTR &=0xDF;                    //清AD转换完成标志
    return  (((uint16)ADC_RES << 8) | ADC_RESL);
	
}
void ADC_Init_Port(void)
{                                              
	
  P0M0 = 0x00;	                  //设置P0.1为高阻输入  0000 0010 
	P0M1 = 0x02;	
	ADC_config();
}
void User_Get_ADC_data(void)
{  
	   uint16 ADCdata=0 ; 
	
	   float g_voltage=0;	
	
     ADCdata = Get_ADC12bitResult(9);     //读外部电压ADC   
	
	   //2V5为ADC基准电压,所以计算电压值公式：V=Temp_signal/4096*2.5 //	  
    
	   if(ADCdata>=4096)//超量程
		 {
			 ADCdata=4096;
		 }
	 
	   g_voltage=((2.5*ADCdata)/4096)*1000;	 //扩大1000倍，mv打印
	   
	   g_voltage=g_voltage*3; //电阻分压了
	
		 Uart_Send_ADC_data(g_voltage); 
	
	   delay_ms(200); 
}	 
void Uart_Send_ADC_data(uint16 ADC_DATA)
{  
	     atemp=(ADC_DATA/1000)+'0'; 
	     atemp1=(ADC_DATA/100%10)+'0'; 
	     atemp2=(ADC_DATA/10%10)+'0';
	     atemp3=(ADC_DATA%10)+'0';
}






















