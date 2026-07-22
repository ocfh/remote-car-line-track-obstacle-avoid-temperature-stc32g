#include "STC32G.h"     //包含所用单片机的头文件
#include "ds18B20.h"   
#define uchar unsigned char
#define uint  unsigned int

sbit DQ = P4^4;				/*DQ = P07  P0.7 ds18b20的信号线*/
extern char dtemp=0,dtemp1=0,dtemp2=0,dtemp3=0;
void delay_us_ds18B20(unsigned int nus);		//DS18B20
void Reset_ds18b20(void);						//复位DS18B20    
unsigned char DS18B20_Read_Bit(void);			//读出一个位
unsigned char DS18B20_Read_Byte(void);			//读出一个字节
void write_byte_ds18b20(unsigned char data2);	//写入一个字节
float read_ds18b20_temperature(void);			//获取温度
void start_ds18b20(void);						//开始温度转换
void get_ds18b20(void);
float read_temperature_18b20;                  //温度值
void delay_us_ds18B20(unsigned int nus)
{
	nus = 2*nus;
	while(--nus);
}
void Reset_ds18b20(void)	   
{                 
	static unsigned int retry = 0;
	
    DQ = 0; 				/*复位脉冲开始*/
	delay_us_ds18B20(700);    	//延时750us
	DQ = 1; 				/*释放总线*/
	delay_us_ds18B20(50);     	//延时15US
	
	retry = 0;
    while (DQ&&(retry<5000))
	{
		retry++;
		delay_us_ds18B20(1);
	}	
	retry = 0;	 
	while (!DQ&&(retry<5000))
    {
		retry++;
		delay_us_ds18B20(1);
	}
}
unsigned char DS18B20_Read_Bit(void) 			 
{
    static unsigned char data1;
	
	DQ = 0; 
	delay_us_ds18B20(2);
    DQ = 1; 
	delay_us_ds18B20(12);
	if(DQ)
	{
		data1=1;
	}
    else 
	{
		data1=0;	 
	}
    delay_us_ds18B20(50);           
    return data1;
} 
unsigned char read_byte_ds18b20(void)  
{        
    static unsigned char i,j,dat;
	
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j = DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
void write_byte_ds18b20(unsigned char data2)     
 {             
    static  unsigned char j;
    static  unsigned char testb;
	 
    for (j=1;j<=8;j++) 
	{
        testb = data2&0x01;
        data2 = data2>>1;
        if (testb) 
        {
            DQ = 0;// DQ写0
            delay_us_ds18B20(2);                            
            DQ = 1;// DQ写1
            delay_us_ds18B20(60);             
        }
        else 
        {
            DQ = 0;// DQ写0
            delay_us_ds18B20(60);             
            DQ = 1;// DQ写1
            delay_us_ds18B20(2);                          
        }
    }
}
void start_ds18b20(void)
{
	Reset_ds18b20();	   
	write_byte_ds18b20(0xcc);
	write_byte_ds18b20(0x44);
} 
void get_ds18b20(void)
{
	float read_temperature_18b20; 
	int display_temper; 
	read_temperature_18b20=read_ds18b20_temperature();
	display_temper    = read_temperature_18b20*100;//得到只有整数的温度值
	dtemp=(display_temper/1000)+'0'; 
	dtemp1=(display_temper/100%10)+'0'; 
	dtemp2=(display_temper/10%10)+'0';
	dtemp3=(display_temper%10)+'0';
	start_ds18b20();
} 
float read_ds18b20_temperature(void)
{
    static unsigned char temp,TL,TH;
    static float Temper ;
	static short tem;
	
    Reset_ds18b20();         	//复位DS18B20
	
    write_byte_ds18b20(0xcc);	//写入读取命令
    write_byte_ds18b20(0xbe);	//写入读取命令
	
    TL = read_byte_ds18b20();   //读出温度8位 
    TH = read_byte_ds18b20();  	//读出温度8位
	
    if(TH>7)
    {
        TH = ~TH;
        TL = ~TL; 
        temp = 0;			//温度为负  
    }
	else 
	{
		temp  = 1;			//温度为正	  	
	}		
    tem  = TH; 				//获得高八位
    tem<<=8;    
    tem+=TL;				//获得底八位
    Temper = (float)tem*0.0625;	//转换     
	if(temp)
	{
		return Temper; 			//返回温度值
	}
	else 
	{
		return -Temper;    
	}
}