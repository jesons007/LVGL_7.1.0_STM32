#include "RCC.h"

/*
	针对F103大容量系列，小中容量只是在大容量的基础上进行缩减，因此小容量也适用
    RCC_APB1ENR() & RCC_APB2ENR() & RCC_AHBENR()
    用于使能内部外设的时钟使能，支持或操作
    可选参数如下：
        RCC_APB1Periph:
                APB1_TIM2    
                APB1_TIM3    
                APB1_TIM4    
                APB1_TIM5    
                APB1_TIM6    
                APB1_TIM7    
                APB1_WWDG    
                APB1_SPI2    
                APB1_SPI3    
                APB1_USART2  
                APB1_USART3  
                APB1_USART4  
                APB1_USART5  
                APB1_I2C1    
                APB1_I2C2    
                APB1_USB     
                APB1_CAN     
                APB1_BKP     
                APB1_PWR     
                APB1_DAC 

        RCC_APB2Periph:
                APB2_AFIO    
                APB2_GPIOA   
                APB2_GPIOB   
                APB2_GPIOC   
                APB2_GPIOD   
                APB2_GPIOE   
                APB2_GPIOF   
                APB2_GPIOG   
                APB2_ADC1    
                APB2_ADC2    
                APB2_TIM1    
                APB2_SPI1    
                APB2_TIM8    
                APB2_USART1  
                APB2_ADC3    

        RCC_AHBPeriph:
                AHB_DMA1     
                AHB_DMA2     
                AHB_SRAM     
                AHB_FLITF    
                AHB_CRC      
                AHB_FSMC     
                AHB_SDIO


    eg使能GPIOA,B,C的时钟：
        RCC_APB2ENR(APB2_GPIOA|APB2_GPIOB|APB2_GPIOC, ENABLE);        
*/

void RCC_APB1ENR(u32 RCC_APB1Periph, u8 encmd)
{
    if(encmd==ENABLE)
        RCC->APB1ENR |= RCC_APB1Periph;
    else
        RCC->APB1ENR &= ~RCC_APB1Periph;
    
}

void RCC_APB2ENR(u32 RCC_APB2Periph, u8 encmd)
{
    if(encmd==ENABLE)
        RCC->APB2ENR |= RCC_APB2Periph;
    else
        RCC->APB2ENR &= ~RCC_APB2Periph;
}

void RCC_AHBENR(u32 RCC_AHBPeriph, u8 encmd)
{
    if(encmd==ENABLE)
        RCC->AHBENR |= RCC_AHBPeriph;
    else
        RCC->AHBENR &= ~RCC_AHBPeriph;
}
