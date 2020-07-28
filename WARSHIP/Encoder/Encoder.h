#ifndef __Encoder_H__
#define __Encoder_H__
#include "sys.h"

void Orth_Encoder_Init(TIM_TypeDef *TIMx);
int Orth_Get_EnCount(TIM_TypeDef *TIMx);
void Orth_Set_EnCount(TIM_TypeDef *TIMx,int count);

#endif