#ifndef _TIMER_H_
#define _TIMER_H_
#include "sys.h"

#define F103HD 1			//Ϊ1��֧��103�������豸��Ϊ0��֧��

typedef struct 
{
    TIM_TypeDef* TIMx;
    u16 ms;       		//you can specify ms to set a timer,the psc and arr will be set automatically
    u16 psc;      		//you can also set psc and arr value manually to set a timer,BUT leave ms to 0
    u16 arr;        	
    u8 NVIC_Priority;	//�ж����ȼ�
    void (*event_handler)(void);
} Timer_InitDef;

void timer_init(Timer_InitDef *Timer);


#endif
