#ifndef _TIMEKEEPER_H_
#define _TIMEKEEPER_H_
#include "sys.h"

#define TimeKeeper_1us_Count    72          //TimeKeeper计时精度选择为1us
#define TimeKeeper_10us_Count   720         //TimeKeeper计时精度选择为10us
#define TimeKeeper_100us_Count  7200        //TimeKeeper计时精度选择为100us

void TimeKeeper_Init(u16 TimeKeeper_nus_Count);
void TimeKeeper_ON(void);
void TimeKeeper_OFF(void);
u32 Get_TimeKeeper_Count(void);


#endif
