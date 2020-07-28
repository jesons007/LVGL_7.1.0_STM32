#ifndef _tim3_pwm_h_
#define _tim3_pwm_h_
#include "stm32f10x.h"

#define TIM_CH1 0X01
#define TIM_CH2 0X02
#define TIM_CH3 0X04
#define TIM_CH4 0X08

typedef struct 
{
    TIM_TypeDef* TIMx;			//
    u8  channels;				//
    u16 frequency;				//若设置频率，则表示使用默认的计数频率，范围：20 ~ 10khz
    u16 psc;					//
    u16 arr;					//
} PWM_InitTypeDef;

void PWM_init(PWM_InitTypeDef *PWM);
void PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, u16 CCRX);
void SYS_PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, float duty);

#endif
