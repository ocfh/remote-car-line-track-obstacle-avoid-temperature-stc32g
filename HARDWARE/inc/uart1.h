#ifndef	__UART1_H
#define	__UART1_H 

#include <STC32G.H> 





void Uart1_Init(void);


void PrintString1(unsigned char *puts);
void program_automatic_update(void);
void SendDataByUart1(unsigned char dat);
void Uart_Send_ADC_data(unsigned int  ADC_DATA); 


#endif 
