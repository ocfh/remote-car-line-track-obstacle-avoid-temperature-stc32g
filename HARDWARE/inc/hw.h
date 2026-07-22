#ifndef	__HW_H
#define	__HW_H 

#include <STC32G.H> 
#include "timer.h"   

#define  Get_HW_R  P47

#define  Get_HW_L  P33 

#define  Get_HW_MR  P06

#define  Get_HW_ML  P32 



void HW_Init_Port(void);

void Get_HW_State(void);




#endif 
