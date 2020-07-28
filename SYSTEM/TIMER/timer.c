/* --------TIMER库使用说明-----------
适用于f103系列
对于F103ZET6等大容量芯片共TIM1 ~ TIM8 八个定时器
对于F103C8T6只有TIM1 ~ TIM4 四个定时器


TIM1 ~ TIM8时钟源默认均为72MH
void timer_init(Timer_InitDef *Timer)函数说明
	typedef struct 
	{
		TIM_TypeDef* TIMx;	//选择定时器（TIM1 ~ TIM4）
		u16 ms;       		//最大定时时间6553ms,Timer->ms范围0-6553
		u16 psc;      		//you can specify ms to set a timer,the psc and arr will be set automatically
		u16 arr;        	//you can also set psc and arr value manually to set a timer,BUT leave ms to 0
		u8 NVIC_Priority;	//中断优先级,可选值为0~15，0优先级最高
		void (*event_handler)(void); //函数指针，中断回调函数
	} Timer_InitDef;
	
demo：
	//声明函数：
		void tim_interrupt_user1(void);
	//定义结构体：
		Timer_InitDef TIM_USER1=
		{
			.TIMx = TIM2,
			.ms   = 500,
			.NVIC_Priority = 0,
			.event_handler = tim_interrupt_user1,
		};
	//调用：
		timer_init(&TIM_USER1);
	//写函数体：
		void tim_interrupt_user1()
		{
			LED1 = !LED1;
		}
	//然后LED1就会以500ms为间隔闪烁
	//end
	

*/


#include "timer.h"

#define NULL 0

void (*TIM1_arr_overflow_Irq)() = NULL;
void (*TIM2_arr_overflow_Irq)() = NULL;
void (*TIM3_arr_overflow_Irq)() = NULL;
void (*TIM4_arr_overflow_Irq)() = NULL;
void (*TIM5_arr_overflow_Irq)() = NULL;
void (*TIM6_arr_overflow_Irq)() = NULL;
void (*TIM7_arr_overflow_Irq)() = NULL;
void (*TIM8_arr_overflow_Irq)() = NULL;

void timer_init(Timer_InitDef *Timer)
{
	u8 n = 0;
	
	if(Timer->TIMx == TIM1)
		RCC->APB2ENR |= 1<<11;
	else if(Timer->TIMx == TIM8)
		RCC->APB2ENR |= 1<<13;
	else
		RCC->APB1ENR |= 1<<(((uint32_t)(Timer->TIMx)-(uint32_t)(TIM2))/0X400);
	
	if(Timer->ms)
	{
		Timer->psc = 7200;    		//默认时钟频率0.01M，100us计数
		Timer->arr = Timer->ms * 10;//最大定时时间6553ms,Timer->ms范围0-6553
	}
	
	Timer->TIMx->PSC = Timer->psc - 1;
	Timer->TIMx->ARR = Timer->arr - 1;
	
	Timer->TIMx->CR1  &= 0XFFEF;     //向上计数
	
	
	switch ((u32)Timer->TIMx)
    {
		case (u32)TIM1:
            n=TIM1_UP_IRQn;
            TIM1_arr_overflow_Irq = Timer->event_handler;
        break;
		
        case (u32)TIM2:
            n=TIM2_IRQn;
            TIM2_arr_overflow_Irq = Timer->event_handler;
        break;
    
        case (u32)TIM3:
            n=TIM3_IRQn;
            TIM3_arr_overflow_Irq = Timer->event_handler;
        break;
		
        case (u32)TIM4:
            n=TIM4_IRQn;
            TIM4_arr_overflow_Irq = Timer->event_handler;
        break;
#if F103HD		
        case (u32)TIM5:
            n=TIM5_IRQn;
            TIM5_arr_overflow_Irq = Timer->event_handler;
        break;
		
        case (u32)TIM6:
            n=TIM6_IRQn;
            TIM6_arr_overflow_Irq = Timer->event_handler;
        break;
		
        case (u32)TIM7:
            n=TIM7_IRQn;
            TIM7_arr_overflow_Irq = Timer->event_handler;
        break;
		
		case (u32)TIM8:
            n=TIM8_UP_IRQn;
            TIM8_arr_overflow_Irq = Timer->event_handler;
        break;
#endif
    default:
        break;
    }
	NVIC->IP[n]  = (Timer->NVIC_Priority)<<4;
	NVIC->ISER[n/32] |= 1<<(n%32);
	
	Timer->TIMx->SR   &= 0XFFFE;
	Timer->TIMx->DIER |= 1;
	
	
	Timer->TIMx->CR1 |= 0x01;
	
	
}

void TIM1_UP_IRQHandler()
{
    if((TIM1->SR)&0X0001)
    {
        (*TIM1_arr_overflow_Irq)();
       
    }
    TIM1->SR &= 0XFFFE;
}

void TIM2_IRQHandler()
{
    if((TIM2->SR)&0X0001)
    {
        TIM2_arr_overflow_Irq();
       
    }
    TIM2->SR &= 0XFFFE;
}

void TIM3_IRQHandler()
{
    if(TIM3->SR&0x0001)
    {
        (*TIM3_arr_overflow_Irq)();
    }

    TIM3->SR &= 0XFFFE;
}

void TIM4_IRQHandler()
{
    if(TIM4->SR & 0x0001)
    {
        (*TIM4_arr_overflow_Irq)();
    }
    
    TIM4->SR &= 0XFFFE;    
}

// void TIM5_IRQHandler()    //用于编码器驱动
// {
//     if(TIM5->SR&0x0001)
//     {
//         (*TIM5_arr_overflow_Irq)();
//     }
    
//     TIM5->SR &= 0XFFFE;        
// }

void TIM6_IRQHandler()
{
   if(TIM6->SR & 0x0001)
   {
       (*TIM6_arr_overflow_Irq)();
   }
   
   TIM6->SR &= 0XFFFE;        
}

void TIM7_IRQHandler()
{
    if(TIM7->SR & 0x0001)
    {
        (*TIM7_arr_overflow_Irq)();
    }

    TIM7->SR &= 0XFFFE;        
}

void TIM8_UP_IRQHandler()
{
	if(TIM8->SR & 0x0001)
    {
        (*TIM8_arr_overflow_Irq)();
    }

    TIM8->SR &= 0XFFFE; 
}

/*
	TIM1_UP_IRQn                = 25
	TIM2_IRQn                   = 28,                                    
	TIM3_IRQn                   = 29,                                 
	TIM4_IRQn                   = 30,
	TIM5_IRQn                   = 50,
	TIM6_IRQn                   = 54, 
	TIM7_IRQn                   = 55,
	TIM8_UP_IRQn                = 44,
*/

