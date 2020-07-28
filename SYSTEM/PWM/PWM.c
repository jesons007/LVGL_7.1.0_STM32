#include "pwm.h"

/*定时器3的输出通道与引脚的对应关系(不使用重映射)*/
//TIM3 CH1   - - -> PA6
//TIM3 CH2   - - -> PA7		   
//TIM3 CH3   - - -> PB0
//TIM3 CH4   - - -> PB1

/*---------------PWM库使用说明----------------*/
//具有PWM输出能力的定时器(小容量的只有TIM1 2 3 4):
//TIM2 TIM3 TIM4 TIM5
//TIM1 TIM8
//定时器时钟源均为默认内部时钟源72M
/*
PWM_init(PWM_InitTypeDef *PWM)函数说明：用于定时器的PWM初始化

结构体参数说明：
	typedef struct 
	{
	    TIM_TypeDef* TIMx;			//定时器指针，指定定时器
	    u8  channels;				//通道选择，共有4个通道，可选参数有 TIM_CH1|TIM_CH2|TIM_CH3|TIM_CH4, 支持或操作
	    u16 frequency;				//频率设置范围：20 ~ 10khz（若设置频率，则表示使用默认的计数时钟），若设置频率为0，可自定义PWM频率
	    u16 psc;					//当frequency=0时有效，用于自定义时钟分频系数    
	    u16 arr;					//当frequency=0时有效，用于自定义计数溢出周期
	} PWM_InitTypeDef;
	
PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, u16 CCRX):用户设置占空比函数，当自定义PWM频率时（frequency=0，设置psc和arr），使用此函数设置占空比，函数底层直接操作的TIMx->CCRx寄存器

	TIM_TypeDef* TIMx://定时器指针，指定定时器
	u8  channels;	  //通道选择，共有4个通道，可选参数有 TIM_CH1|TIM_CH2|TIM_CH3|TIM_CH4, 支持或操作
	u16 CCRX;         //TIMx->CCRx寄存器的值


void SYS_PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, float duty):系统设置占空比函数，使用默认的系统频率时（frequency!=0时），用于设置占空比
	TIM_TypeDef* TIMx://定时器指针，指定定时器
	u8  channels;	  //通道选择，共有4个通道，可选参数有 TIM_CH1|TIM_CH2|TIM_CH3|TIM_CH4, 支持或操作
	float duty;	      //占空比，参数为浮点数类型，默认单位%，参数范围 0.0 - 100.0, (例如duty = 25.5 ,则占空比为25.5%);
	
	
	
demo:
	PWM_InitTypeDef PWM_User_1 = 
	{
		.TIMx = TIM4,
		.channels = TIM_CH1|TIM_CH2,
		.frequency = 50,
	};
	GPIO_Init(GPIOB,GPIO_PIN_6|GPIO_PIN_7,OUTPUT_FF);
	RCC_APB1ENR(APB1_TIM4,ENABLE);
	PWM_init(&PWM_User_1);
	SYS_PWM_Duty_CHX(TIM4,TIM_CH1|TIM_CH2,25.0);
*/


void PWM_init(PWM_InitTypeDef *PWM)
{
	u8 i;
	if(PWM->frequency)
	{
		PWM->psc = 72;		//默认计数频率1Mh,1us计数
		PWM->arr = 1000000 / PWM->frequency;
	}
	 PWM->TIMx->PSC = PWM->psc - 1;
     PWM->TIMx->ARR = PWM->arr - 1;
	 PWM->TIMx->CR1 &= 0XFF6F;           //TIM3向上计数 TIMx_ARR寄存器没有缓冲
	 
	for(i=0;i<4;i++)
	{
		if(PWM->channels &(1<<i))
		{
			if(i==0||i==1)
			{
				PWM->TIMx->CCMR1 |= 0x64<<(i*8);    //PWM模式1 向上计数时，CNT<CCR1 :输出1   CNT>=CCR1 :输出0
			}
			else if(i==2||i==3)
			{
				PWM->TIMx->CCMR2 |= 0x64<<((i-2)*8);//PWM模式1 向上计数时，CNT<CCR1 :输出1   CNT>=CCR1 :输出0
			}
			*(uint32_t *)((u32)(PWM->TIMx) + 0x34 +i*4) = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
			PWM->TIMx->CCER |= 1<<(i*4);    //使能定时器通道
		}
	}
	
	PWM->TIMx->CNT = 0; 	  	   //计数器清零
	PWM->TIMx->CR1 |= 0X01;        //开始计数
	
}

void PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, u16 CCRX)
{
	u8 i;
	for(i=0;i<4;i++)
	{
		if(channels &(1<<i))
		{
			*(uint32_t *)((u32)TIMx + 0x34 +i*4) = CCRX;
		}
	}
}

void SYS_PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, float duty)
{
	u8 i;u16 CCRX=0;
	float temp;
	
	temp = ((float)(TIMx->ARR+1))*(duty/100.0);
	CCRX = (u16)temp;
	
	for(i=0;i<4;i++)
	{
		if(channels &(1<<i))
		{
			*(uint32_t *)((u32)TIMx + 0x34 +i*4) = CCRX;
		}
	}
}
