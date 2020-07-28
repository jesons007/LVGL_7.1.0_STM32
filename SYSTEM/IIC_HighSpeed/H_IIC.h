#ifndef _H_IIC_H_
#define _H_IIC_H_
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

void H_IIC_init(u8 SCL,u8 SDA);
void _SCL_set(u8 val);
void _SDA_set(u8 val);
u8 _SDA_read(void);
void _SDA_IN(void);
void _SDA_OUT(void);
void H_IIC_start(void);
void H_IIC_stop(void);
void H_IIC_write(u8 val);
u8 H_IIC_read(u8 ack);
u8 H_IIC_Wait_ACK(void);


#endif


