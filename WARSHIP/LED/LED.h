#ifndef _LED_H_
#define _LED_H_
#include "sys.h"
#define LED0 PBout(5)    //DS0
#define LED1 PEout(5)	 //DS1
#define BEEP PBout(8)

void led_init(void);

#endif
