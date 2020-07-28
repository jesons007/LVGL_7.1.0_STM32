#ifndef _F_IIC_H_
#define _F_IIC_H_
#include "delay.h"
#include "sys.h"

typedef struct
{
	u8 IIC_SCL;
	u8 IIC_SDA;
} F_IIC_TypeDef;

void F_IIC_init(F_IIC_TypeDef *F_IIC_Initer);
void SCL_set(F_IIC_TypeDef *F_IIC_Initer, u8 val);
void SDA_set(F_IIC_TypeDef *F_IIC_Initer, u8 val);
u8 SDA_read(F_IIC_TypeDef *F_IIC_Initer);
void SDA_IN(F_IIC_TypeDef *F_IIC_Initer);
void SDA_OUT(F_IIC_TypeDef *F_IIC_Initer);
void F_IIC_start(F_IIC_TypeDef *F_IIC_Initer);
void F_IIC_stop(F_IIC_TypeDef *F_IIC_Initer);
void F_IIC_write(F_IIC_TypeDef *F_IIC_Initer, u8 val);
u8 F_IIC_read(F_IIC_TypeDef *F_IIC_Initer, u8 ack);
u8 F_IIC_Wait_ACK(F_IIC_TypeDef *F_IIC_Initer);


#endif


