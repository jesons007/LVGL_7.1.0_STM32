#include "pwm.h"

/*��ʱ��3�����ͨ�������ŵĶ�Ӧ��ϵ(��ʹ����ӳ��)*/
//TIM3 CH1   - - -> PA6
//TIM3 CH2   - - -> PA7		   
//TIM3 CH3   - - -> PB0
//TIM3 CH4   - - -> PB1

/*---------------PWM��ʹ��˵��----------------*/
//����PWM��������Ķ�ʱ��(С������ֻ��TIM1 2 3 4):
//TIM2 TIM3 TIM4 TIM5
//TIM1 TIM8
//��ʱ��ʱ��Դ��ΪĬ���ڲ�ʱ��Դ72M
/*
PWM_init(PWM_InitTypeDef *PWM)����˵�������ڶ�ʱ����PWM��ʼ��

�ṹ�����˵����
	typedef struct 
	{
	    TIM_TypeDef* TIMx;			//��ʱ��ָ�룬ָ����ʱ��
	    u8  channels;				//ͨ��ѡ�񣬹���4��ͨ������ѡ������ TIM_CH1|TIM_CH2|TIM_CH3|TIM_CH4, ֧�ֻ����
	    u16 frequency;				//Ƶ�����÷�Χ��20 ~ 10khz��������Ƶ�ʣ����ʾʹ��Ĭ�ϵļ���ʱ�ӣ���������Ƶ��Ϊ0�����Զ���PWMƵ��
	    u16 psc;					//��frequency=0ʱ��Ч�������Զ���ʱ�ӷ�Ƶϵ��    
	    u16 arr;					//��frequency=0ʱ��Ч�������Զ�������������
	} PWM_InitTypeDef;
	
PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, u16 CCRX):�û�����ռ�ձȺ��������Զ���PWMƵ��ʱ��frequency=0������psc��arr����ʹ�ô˺�������ռ�ձȣ������ײ�ֱ�Ӳ�����TIMx->CCRx�Ĵ���

	TIM_TypeDef* TIMx://��ʱ��ָ�룬ָ����ʱ��
	u8  channels;	  //ͨ��ѡ�񣬹���4��ͨ������ѡ������ TIM_CH1|TIM_CH2|TIM_CH3|TIM_CH4, ֧�ֻ����
	u16 CCRX;         //TIMx->CCRx�Ĵ�����ֵ


void SYS_PWM_Duty_CHX(TIM_TypeDef* TIMx, u8 channels, float duty):ϵͳ����ռ�ձȺ�����ʹ��Ĭ�ϵ�ϵͳƵ��ʱ��frequency!=0ʱ������������ռ�ձ�
	TIM_TypeDef* TIMx://��ʱ��ָ�룬ָ����ʱ��
	u8  channels;	  //ͨ��ѡ�񣬹���4��ͨ������ѡ������ TIM_CH1|TIM_CH2|TIM_CH3|TIM_CH4, ֧�ֻ����
	float duty;	      //ռ�ձȣ�����Ϊ���������ͣ�Ĭ�ϵ�λ%��������Χ 0.0 - 100.0, (����duty = 25.5 ,��ռ�ձ�Ϊ25.5%);
	
	
	
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
		PWM->psc = 72;		//Ĭ�ϼ���Ƶ��1Mh,1us����
		PWM->arr = 1000000 / PWM->frequency;
	}
	 PWM->TIMx->PSC = PWM->psc - 1;
     PWM->TIMx->ARR = PWM->arr - 1;
	 PWM->TIMx->CR1 &= 0XFF6F;           //TIM3���ϼ��� TIMx_ARR�Ĵ���û�л���
	 
	for(i=0;i<4;i++)
	{
		if(PWM->channels &(1<<i))
		{
			if(i==0||i==1)
			{
				PWM->TIMx->CCMR1 |= 0x64<<(i*8);    //PWMģʽ1 ���ϼ���ʱ��CNT<CCR1 :���1   CNT>=CCR1 :���0
			}
			else if(i==2||i==3)
			{
				PWM->TIMx->CCMR2 |= 0x64<<((i-2)*8);//PWMģʽ1 ���ϼ���ʱ��CNT<CCR1 :���1   CNT>=CCR1 :���0
			}
			*(uint32_t *)((u32)(PWM->TIMx) + 0x34 +i*4) = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
			PWM->TIMx->CCER |= 1<<(i*4);    //ʹ�ܶ�ʱ��ͨ��
		}
	}
	
	PWM->TIMx->CNT = 0; 	  	   //����������
	PWM->TIMx->CR1 |= 0X01;        //��ʼ����
	
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
