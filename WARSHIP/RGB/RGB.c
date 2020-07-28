/*  
eg:
将RGB的三个控制脚分别接入TIM3的 TIM_CH1，TIM_CH2，TIM_CH4（PA6,PA7,PB1）
//调用
	RGB_Init()
//然后更改
	RGB_R = 500；
	RGB_G = 500；
	RGB_B = 500；
//便可以控制颜色及亮度

 */


#include "RGB.h"
#include "pwm.h"


void RGB_Init()
{
	PWM_InitTypeDef PWM_User_1 = 
	{
		.TIMx = TIM3,
		.channels = TIM_CH1|TIM_CH2|TIM_CH4,
		.frequency = 1000,
	};
	GPIO_Init(GPIOA,GPIO_PIN_6|GPIO_PIN_7,OUTPUT_FF);
	GPIO_PIN_Init(PB(1),OUTPUT_FF);
	RCC_APB1ENR(APB1_TIM3,ENABLE);
	PWM_init(&PWM_User_1);
	
}


//	RGB_Init();    //TIM4
//	RGB_R=1000;
//	RGB_G=1000;
//	RGB_B=1000;

/*
---------RGB Test-----------
		if(j==0)
		{
			RGB_R --;
			if(RGB_R==0)
			{
				j=1;
			}
		}
		else if(j==1)
		{
			RGB_G --;
			if(RGB_G==0)
			{
				j=2;
			}
		}
		else if(j==2)
		{
			RGB_B --;
			if(RGB_B==0)
			{
				j=0;
				RGB_R=1000;
				RGB_G=1000;
				RGB_B=1000;
			}
		}
--------------------------
*/

