#ifndef DS18B20_H
#define DS18B20_H
#include "STC32G.h" 
typedef unsigned char uchar;
typedef unsigned int uint;
extern char dtemp,dtemp1,dtemp2,dtemp3;
void delay_us_ds18B20(unsigned int nus);
void Reset_ds18b20(void);
void get_ds18b20(void);
unsigned char DS18B20_Read_Bit(void);
unsigned char DS18B20_Read_Byte(void);
void write_byte_ds18b20(unsigned char data2);
void start_ds18b20(void);
float read_ds18b20_temperature(void);



#endif