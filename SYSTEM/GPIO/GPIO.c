#include "GPIO.h"

/*
	GPIO库使用说明：
		适用于STM32F103系列
		GPIO_Init():使能gpio时钟并初始化io口
		GPIOx:		GPIOA ~ GPIOG
		GPIO_PIN:	GPIO_PIN_0 ~ GPIO_PIN_15
		GPIO_MODE:
			OUTPUT_PP 	    //推挽输出
			OUTPUT_FF 	    //复用推挽
			INPUT_UP  	    //上拉输入
			INPUT_DOWN	    //下拉输入
			INPUT_ANALOG    //模拟输入


	demo:
		GPIO_Init(GPIOB, GPIO_PIN_10|GPIO_PIN_11, OUTPUT_PP);  	//使能GPIOB的时钟，并设置PB10和PB11口为推挽输出模式
		GPIO_Set(GPIOB,GPIO_PIN_10|GPIO_PIN_11);				//设置PB10和PB11口为高电平
*/

void GPIO_Init(GPIO_TypeDef *GPIOx, u16 GPIO_PIN, u8 GPIO_MODE)
{
	
	RCC->APB2ENR |= 1<<(((uint32_t)GPIOx-(uint32_t)GPIOA)/1024+2);//使能GPIOx的时钟
	u8 i;


	for(i=0;i<16;i++)
	{
		if((1<<i)&GPIO_PIN)
		{
			*(uint32_t *)((u32)GPIOx+((i>7)?4:0)) &= ~(0X0000000F<<(4*((i<8)?i:i-8)));//清零i口的配置位
			
			if(GPIO_MODE==INPUT_UP)
			{
				*(uint32_t *)((u32)GPIOx+((i>7)?4:0)) |=  (0X0000000F&0X8)<<(4*((i<8)?i:i-8));
				GPIOx->ODR |= 1<<i;
			}
			else if(GPIO_MODE==INPUT_DOWN)
			{
				*(uint32_t *)((u32)GPIOx+((i>7)?4:0)) |=  (0X0000000F&0X8)<<(4*((i<8)?i:i-8));
				GPIOx->ODR &= ~(1<<i);
			}
			else
				*(uint32_t *)((u32)GPIOx+((i>7)?4:0)) |=  (0X0000000F&GPIO_MODE)<<(4*((i<8)?i:i-8));
		}
	}	
	
}

void GPIO_Set(GPIO_TypeDef *GPIOx, u16 GPIO_PIN)
{
	GPIOx->ODR |= GPIO_PIN;
}

void GPIO_Rset(GPIO_TypeDef *GPIOx, u16 GPIO_PIN)
{
	GPIOx->ODR &= ~GPIO_PIN;
}

void GPIO_WRITE(GPIO_TypeDef *GPIOx,u16 data)
{
	GPIOx->ODR = data;
}

u16 GPIO_READ(GPIO_TypeDef *GPIOx)
{
	__IO u16 val;
    	val=GPIOx->IDR;
	return val;
}

void GPIO_PIN_Init(u8 PIN, u8 Mode)
{
	RCC->APB2ENR |= 1<<((PIN>>4)+2);//使能GPIO的时钟
	*(uint32_t *)(GPIOA_BASE+1024*(PIN>>4)+(((PIN&0X0F)>7)?4:0)) &= ~(0X0000000F<<(4*(((PIN&0X0F)<8)?(PIN&0X0F):(PIN&0X0F)-8)));
	
	if(Mode==INPUT_UP)
	{
		*(uint32_t *)(GPIOA_BASE+1024*(PIN>>4)+(((PIN&0X0F)>7)?4:0)) |=  0X08<<(4*(((PIN&0X0F)<8)?(PIN&0X0F):(PIN&0X0F)-8));
		*(uint32_t *)(GPIOA_BASE+1024*(PIN>>4)+0x0c) |= 1<<(PIN&0X0F);
	}
	else if(Mode==INPUT_DOWN)
	{
		*(uint32_t *)(GPIOA_BASE+1024*(PIN>>4)+(((PIN&0X0F)>7)?4:0)) |=  0X08<<(4*(((PIN&0X0F)<8)?(PIN&0X0F):(PIN&0X0F)-8));
		*(uint32_t *)(GPIOA_BASE+1024*(PIN>>4)+0x0c) &= ~(1<<(PIN&0X0F));
	}
	else
	{
		*(uint32_t *)(GPIOA_BASE+1024*(PIN>>4)+(((PIN&0X0F)>7)?4:0)) |=  Mode<<(4*(((PIN&0X0F)<8)?(PIN&0X0F):(PIN&0X0F)-8));
	}
}


