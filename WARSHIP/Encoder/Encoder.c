//only Time2 ~ Time5,Time1 & Time8 for STM32F103xxx
/*
正交编码器驱动库：
Orth_Encoder_Init(TIM_TypeDef *TIMx)			//初始化编码器模式TIMx
												//编码器的A,B 相应该接在TIMx的CH1和CH2
												//需要用户自行初始化TIMx的CH1和CH2 IO口

Orth_Get_EnCount(TIM_TypeDef *TIMx)    			//获取编码器的计数值
Orth_Set_EnCount(TIM_TypeDef *TIMx,u16 count) 	//设置编码器的计数值
说明：
	计数器是一个u16的寄存器，计数范围为0 ~ 65535，
	初始化之后，计数值默认为30000，
	当正交编码器接入TIMx的CH1和CH2之后并初始化定时器，编码器正转产生的脉冲会使得计数器以30000为初值(也可自定义初值)向上计数，一直计到65535之后会溢出.
	反转则会使计数方向相反.
**注意，当使用编码器模式时定时器不可用于PWM输出或定时器中断等其他模式！！！
eg:
500线欧姆龙编码器，每转一圈输出500个脉冲
计数器计数2000次，正转为增，反转为减（相对
初始化gpio:
	GPIO_Init(GPIOH,GPIO_PIN_10|GPIO_PIN_11,GPIO_MODE_AF,0,GPIO_SPEED_100M,GPIO_PUPD_PU);
	GPIO_AF_Set(GPIOH,GPIO_PIN_10|GPIO_PIN_11,2);							//TIM5-CH1   -> PH10   TIM5-CH2   -> PH11	
初始化编码器：													
	Orth_Encoder_Init(TIM5);
	Orth_Set_EnCount(TIM5,10000);   //设置计数初值10000
调用：
	Orth_Get_EnCount(TIM5);
便可获得当前计数值（当前位置）

定时1ms调用：
	En_counter = Orth_Get_EnCount(TIM5);
	En_Speed = 30000 - En_counter;
	Orth_Set_EnCount(TIM5,30000);
便可获得转速 En_Speed   (单位：个脉冲/ms)

2019/10/6更新说明：
	完善编码器功能，解决计数溢出问题
	新增函数：int Orth_Get_EnCount(TIM_TypeDef *TIMx);
	返回值为int类型（解决溢出问题）

        
*/
#include "Encoder.h"
#include "LED.h"

int upload_count = 0;

void Orth_Encoder_Init(TIM_TypeDef *TIMx)
{
	if(TIMx == TIM1)
		RCC->APB2ENR |= 1<<11;
	else if(TIMx == TIM8)
		RCC->APB2ENR |= 1<<13;
	else
		RCC->APB1ENR |= 1<<(((uint32_t)(TIMx)-(uint32_t)(TIM2))/0X400);
	
	TIMx->PSC   = 0;          
	TIMx->ARR   = 65535;		
	TIMx->CR1   &= 0XFF0F;      //TIMx向上计数 TIMx_ARR寄存器没有缓冲
	
	TIMx->CR1   &= 0XFCFF;		//TIMx时钟不分频
	
	//TIMx-CC1通道被配置为输入， IC1映射在TI1上；
	//TIMx-CC2通道被配置为输入， IC2映射在TI2上；
	//无预分频器，捕获输入口上检测到的每一个边沿都触发一次捕获；
	//无滤波器，以CK_INT采样
	TIMx->CCMR1 &= 0X0000;
	TIMx->CCMR1 |= 0X0101;	
	
	TIMx->CCER  |= 0X0011;   //使能TIMx-CC1&TIMx-CC2通道.  IC1&IC2不反相
	
	TIMx->SMCR  &= 0XFFF8;
	TIMx->SMCR  |= 3;       //编码器模式 3––计数器在 TI1FP1 和 TI2FP2 的边沿计数，计数的方向取决于另外一个信号的电平。
	
	TIMx->DIER |= 0x01;   //开启溢出中断
	
	NVIC->IP[TIM5_IRQn]  = (10)<<4;
	NVIC->ISER[TIM5_IRQn/32] |= 1<<(TIM5_IRQn%32);
	
	Orth_Set_EnCount(TIM5, 1000000);
	TIMx->CR1 |= 0X01;
}

int Orth_Get_EnCount(TIM_TypeDef *TIMx)
{
	int temp;
	if(upload_count>=0)
	{
		temp = upload_count*65535 + (TIMx->CNT);
	}
	else if(upload_count<0)
	{
		temp = upload_count*65536 + (TIMx->CNT);
	}
	return temp;
}

void Orth_Set_EnCount(TIM_TypeDef *TIMx,int count)
{
	int temp;
	if(count>=0)
	{
		upload_count = count / 65535;
		TIMx->CNT = count % 65535;
	}
	else
	{
		count = -count;
		temp = (count/65536) +1;
		upload_count = -temp;
		TIMx->CNT = 65536*temp - count;
	}
	
}

void TIM5_IRQHandler()
{
    if(TIM5->SR&0x0001)
    {
		LED1 = !LED1;
		if((TIM5->CNT)<100)  //上溢
		{
			upload_count ++;
		}
		else if((TIM5->CNT)>65400)   //下溢
		{
			upload_count --;
		}
    }
    TIM5->SR &= 0XFFFE;        
}
