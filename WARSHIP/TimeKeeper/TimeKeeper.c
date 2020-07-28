/*------TimeKeeper 程序计时器-------- */
/*
**TimeKeeper库使用说明:
作用：可用于程序运行精确计时，帧率分析等场合，最高精确度为1us

可自定义定时器
#define TimeKeeper_Obj  TIM7            //选择用于计时的定时器

TimeKeeper_Init(u16 TimeKeeper_nus_Count)     //初始化TimeKeeper,初始化后默认不开启计时
可选参数 TimeKeeper_nus_Count:
    TimeKeeper_1us_Count                    //1us计时，返回时间的单位为1us,最大计时时间     ≈ 71.5分钟
    TimeKeeper_10us_Count                   //10us计时，返回时间的单位为10us,最大计时时间   ≈ 12小时
    TimeKeeper_100us_Count                  //100us计时，返回时间的单位为0.1ms,最大计时时间 ≈ 5天
**在连续计时的场合需要注意计时溢出

void TimeKeeper_ON()            //TimeKeeper开启计时，开启时会将之前的计数值清零
void TimeKeeper_OFF()           //关闭计时，不清零
Get_TimeKeeper_Count()          //返回从调用TimeKeeper_ON()开始到当前的计时时间(单位 1/10/100 us)

demo:
    TimeKeeper_ON();	
	//function code
	TimeKeeper_OFF();
    printf("%d\r\n",Get_TimeKeeper_Count());    //打印function code执行时间
*/

#include "TimeKeeper.h"
#include "timer.h"
#include "delay.h"

#define TimeKeeper_Obj  TIM7            //选择用于计时的定时器
u8 TimeKeeper_Ready = 0;
u32 TimeKeeper_Count = 0;   //程序运行时间计数器，随时间累加
void TimeKeeper_interrupt_user1(void);

void TimeKeeper_Init(u16 TimeKeeper_nus_Count)
{
    	Timer_InitDef TimeKeeper=
		{
			.TIMx = TimeKeeper_Obj,
			.ms   = 0,
            .psc  = TimeKeeper_nus_Count,  //n us计数
            .arr  = 60000,//溢出时间6000*n us
			.NVIC_Priority = 2,
			.event_handler = TimeKeeper_interrupt_user1,
		};
		timer_init(&TimeKeeper);
		delay_ms(10);           //延时，防止首次初始化后立即进入中断对计时造成影响，还可以阻止一些该死的bug = =!
        TimeKeeper_OFF();
        TimeKeeper_Obj->CNT = 0;
        TimeKeeper_Ready = 1;
}

void TimeKeeper_ON()
{
    TimeKeeper_Obj->CR1 &= 0xFFFE;
    TimeKeeper_Obj->CNT = 0;
	TimeKeeper_Count    = 0;
    TimeKeeper_Obj->CR1 |= 0x01;
}

void TimeKeeper_OFF()
{
    TimeKeeper_Obj->CR1 &= 0xFFFE;
}

u32 Get_TimeKeeper_Count()    //返回计时时间(单位 1/10/100 us)
{
    u16 tem = TimeKeeper_Obj->CNT;
    return ((u32)tem+(u32)TimeKeeper_Count);
}

void TimeKeeper_interrupt_user1()
{
    if(TimeKeeper_Ready)
    {
        //PEout(5) = !PEout(5);
        TimeKeeper_Count += 60000;
    }
    
}
