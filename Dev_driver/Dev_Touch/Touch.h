#pragma once
#include "sys.h"

typedef struct 
{
    u8 (*init)(void);               //触摸屏初始化
	u8 (*scan)(void);				//扫描触摸屏.0,屏幕扫描;1,物理坐标;
	u16 X[5]; 		//当前坐标
	u16 Y[5];		//电容屏有最多5组坐标X[0],Y[0]表示第一组坐标
								
	u8  sta;					//笔的状态 
								//b7:按下1/松开0; 
	                            //b6:0,没有按键按下;1,有按键按下. 
								//b5:保留
								//b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
                                //b4~b0每一位代表一个点，共5个点，该位为0时代表该点未被按下，为1时为被按下
}_m_tp_dev;

extern _m_tp_dev Tp_dev;//Touch.c中定义,touch设备句柄，全局生效
