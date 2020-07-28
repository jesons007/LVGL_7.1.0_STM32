#ifndef _RGB_H_
#define _RGB_H_
#include "sys.h"
#include "GPIO.h"
#include "RCC.h"

//   RGB_R/RGB_G/RGB_B: 0 ~ 1000   0������ 1000Ϩ��
#define RGB_R TIM3->CCR1 
#define RGB_G TIM3->CCR2
#define RGB_B TIM3->CCR4 

void RGB_Init();


#endif
