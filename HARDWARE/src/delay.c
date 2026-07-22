#include "delay.h" 
#define MAIN_Fosc        24000000UL   //定义主时钟 
void delay_ms(unsigned int ms)
{
    unsigned int  i;
    do{
        i = MAIN_Fosc / 6000;
        while(--i);   //6T per loop
      }while(--ms);
}






