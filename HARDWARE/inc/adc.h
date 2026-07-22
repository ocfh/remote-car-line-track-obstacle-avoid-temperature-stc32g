#ifndef	__ADC_H
#define	__ADC_H 

#include <STC32G.H> 

#define  uint32   unsigned long
#define  uint16   unsigned int  
#define  uint8    unsigned char  

#define VREFH_ADDR  0x07
#define VREFL_ADDR  0x08 

void	ADC_config(void); 
uint16	Get_ADC12bitResult(uint8 channel)	;
void ADC_Init_Port(void);
void User_Get_ADC_data(void);
void Uart_Send_ADC_data(uint16 ADC_DATA); 


#endif 
