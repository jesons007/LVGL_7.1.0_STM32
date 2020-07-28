#include "Touch.h"
#include "gt9147/gt9147.h"


_m_tp_dev Tp_dev=
{
    .init = Gt9147_init,     //注册触摸芯片初始化函数
    .scan = GT9147_Scan,     //注册触摸屏扫描函数
    .X    = 0,
    .Y    = 0,
    .sta  = 0,
};
