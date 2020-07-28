#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h"

#define ADC_CHANNEL_0    0
#define ADC_CHANNEL_1    1
#define ADC_CHANNEL_2    2
#define ADC_CHANNEL_3    3
#define ADC_CHANNEL_4    4
#define ADC_CHANNEL_5    5
#define ADC_CHANNEL_6    6
#define ADC_CHANNEL_7    7
#define ADC_CHANNEL_8    8
#define ADC_CHANNEL_9    9
#define ADC_CHANNEL_10  10
#define ADC_CHANNEL_11  11
#define ADC_CHANNEL_12  12
#define ADC_CHANNEL_13  13
#define ADC_CHANNEL_14  14
#define ADC_CHANNEL_15  15

void ADC_Init(ADC_TypeDef* ADCx, u8 Channel);

u16 ADC_Get_Value(ADC_TypeDef* ADCx, u8 Channel);
u16 ADC_Get_Raw(ADC_TypeDef* ADCx, u8 Channel);

#endif

