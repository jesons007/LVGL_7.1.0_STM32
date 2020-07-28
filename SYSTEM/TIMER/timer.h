#ifndef _TIMER_H_
#define _TIMER_H_
#include "sys.h"

#define F103HD 1			//为1则支持103大容量设备，为0不支持

typedef struct 
{
    TIM_TypeDef* TIMx;
    u16 ms;       		//you can specify ms to set a timer,the psc and arr will be set automatically
    u16 psc;      		//you can also set psc and arr value manually to set a timer,BUT leave ms to 0
    u16 arr;        	
    u8 NVIC_Priority;	//中断优先级
    void (*event_handler)(void);
} Timer_InitDef;

void timer_init(Timer_InitDef *Timer);


#endif
