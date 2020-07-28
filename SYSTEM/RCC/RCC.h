#ifndef _RCC_H_
#define _RCC_H_
#include "stm32f10x.h"

//APB1总线外设
//APB1总线默认时钟36MH
//对于TIM2~TIM7时钟为72MH
#define APB1_TIM2    1<<0
#define APB1_TIM3    1<<1
#define APB1_TIM4    1<<2
#define APB1_TIM5    1<<3
#define APB1_TIM6    1<<4
#define APB1_TIM7    1<<5
#define APB1_WWDG    1<<11
#define APB1_SPI2    1<<14
#define APB1_SPI3    1<<15
#define APB1_USART2  1<<17
#define APB1_USART3  1<<18
#define APB1_USART4  1<<19
#define APB1_USART5  1<<20
#define APB1_I2C1    1<<21
#define APB1_I2C2    1<<22
#define APB1_USB     1<<23
#define APB1_CAN     1<<25
#define APB1_BKP     1<<27
#define APB1_PWR     1<<28
#define APB1_DAC     1<<29


//APB2总线外设
//APB2总线默认时钟72MH
#define APB2_AFIO    1<<0
#define APB2_GPIOA   1<<2
#define APB2_GPIOB   1<<3
#define APB2_GPIOC   1<<4
#define APB2_GPIOD   1<<5
#define APB2_GPIOE   1<<6
#define APB2_GPIOF   1<<7
#define APB2_GPIOG   1<<8
#define APB2_ADC1    1<<9
#define APB2_ADC2    1<<10
#define APB2_TIM1    1<<11
#define APB2_SPI1    1<<12
#define APB2_TIM8    1<<13
#define APB2_USART1  1<<14
#define APB2_ADC3    1<<15

//AHB总线外设
//AHB总线时钟默认72MH
#define AHB_DMA1     1<<0
#define AHB_DMA2     1<<1
#define AHB_SRAM     1<<2
#define AHB_FLITF    1<<4
#define AHB_CRC      1<<6
#define AHB_FSMC     1<<8
#define AHB_SDIO     1<<10

void RCC_APB1ENR(u32 RCC_APB1Periph, u8 encmd);
void RCC_APB2ENR(u32 RCC_APB2Periph, u8 encmd);
void RCC_AHBENR(u32 RCC_AHBPeriph, u8 encmd);


#endif




