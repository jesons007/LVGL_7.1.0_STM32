#pragma once
#include "sys.h"
#include "delay.h"

void DMA_fsmc_Init(void);
void DMA_Enable(u16 num);

void DMA_Fill_Color(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 *color);
