#include "F_IIC.h"

/*    全速IIC(F_IIC)使用说明：    */
/*
	软件模拟IIC
	//----函数说明----//
		void F_IIC_init(F_IIC_TypeDef *F_IIC_Initer)：
			初始化IIC接口引脚(包括时钟使能)
			参数说明 .SCL/.SDA: 
				PA(0) ~ PA(15)
				PB(0) ~ PB(15)
				PC(0) ~ PC(15)
				PD(0) ~ PD(15)
				PE(0) ~ PE(15)
				PF(0) ~ PF(15)
				PG(0) ~ PG(15)
			调用F_IIC_init()函数即可完成对IIC接口引脚的初始化
		void F_IIC_start(F_IIC_TypeDef *F_IIC_Initer)			//产生IIC启始信号
		void F_IIC_stop(F_IIC_TypeDef *F_IIC_Initer)			//产生IIC结束信号
		void F_IIC_write(F_IIC_TypeDef *F_IIC_Initer, u8 val)	//IIC写字节函数，val:要写入的数据
		u8 F_IIC_read(F_IIC_TypeDef *F_IIC_Initer, u8 ack)		//IIC读字节函数，返回读取的一个字节，ack:是否发送ack    0：不发送  1：发送
		u8 F_IIC_Wait_ACK(F_IIC_TypeDef *F_IIC_Initer)			//等待回复ack,返回值 1：有ACK  0：无ACK
																//F_IIC_TypeDef *F_IIC_Initer参数均为IIC用户变量

	eg:IIC引脚对应关系为SCL->PB0   SDA->PB1
		首先定义结构体变量并赋值:
			F_IIC_TypeDef IIC_User_1 = 
			{
				.IIC_SCL = PB(0),
				.IIC_SDA = PB(1),
			};
		然后调用初始化函数:
			F_IIC_init(&IIC_User_1);
		初始化完成!

2019/10/6:修复IIC_Read函数

*/


void F_IIC_init(F_IIC_TypeDef *F_IIC_Initer)
{
	
	RCC->APB2ENR |= 1<<((F_IIC_Initer->IIC_SCL>>4)+2);//使能GPIO的时钟
	RCC->APB2ENR |= 1<<((F_IIC_Initer->IIC_SDA>>4)+2);
	
	//设置SCL和SDA的引脚初始模式为推挽输出模式
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SCL>>4)+(((F_IIC_Initer->IIC_SCL&0X0F)>7)?4:0)) &= ~(0X0000000F<<(4*(((F_IIC_Initer->IIC_SCL&0X0F)<8)?(F_IIC_Initer->IIC_SCL&0X0F):(F_IIC_Initer->IIC_SCL&0X0F)-8)));
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) &= ~((u32)0X0000000F<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8)));
	
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SCL>>4)+(((F_IIC_Initer->IIC_SCL&0X0F)>7)?4:0)) |=  0X3<<(4*(((F_IIC_Initer->IIC_SCL&0X0F)<8)?(F_IIC_Initer->IIC_SCL&0X0F):(F_IIC_Initer->IIC_SCL&0X0F)-8));
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) |=  0X3<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8));
	
	SCL_set(F_IIC_Initer, 1);
	SDA_set(F_IIC_Initer, 1);
	
}

void SCL_set(F_IIC_TypeDef *F_IIC_Initer, u8 val)
{
	if(val==0)
		((GPIO_TypeDef *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SCL>>4)))->ODR &= ~(1<<(F_IIC_Initer->IIC_SCL&0X0F));
	else
		((GPIO_TypeDef *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SCL>>4)))->ODR |=   1<<(F_IIC_Initer->IIC_SCL&0X0F);
	
}

void SDA_set(F_IIC_TypeDef *F_IIC_Initer, u8 val)
{
	if(val==0)
		((GPIO_TypeDef *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)))->ODR &= ~(1<<(F_IIC_Initer->IIC_SDA&0X0F));
	else
		((GPIO_TypeDef *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)))->ODR |=   1<<(F_IIC_Initer->IIC_SDA&0X0F);
}	

void SDA_IN(F_IIC_TypeDef *F_IIC_Initer)//数据总线为输入模式
{
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) &= ~((u32)0X0000000F<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8)));
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) |=  ((u32)0X8<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8)));
	SDA_set(F_IIC_Initer, 1);
}

void SDA_OUT(F_IIC_TypeDef *F_IIC_Initer)//数据总线为输出模式
{
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) &= ~((u32)0X0000000F<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8)));
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) |=  0X3<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8));

}

u8 SDA_read(F_IIC_TypeDef *F_IIC_Initer)//读1或0
{
	if(((GPIO_TypeDef *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)))->IDR&(1<<(F_IIC_Initer->IIC_SDA&0X0F)))
		return 1;
	else
		return 0;
}

void F_IIC_start(F_IIC_TypeDef *F_IIC_Initer)
{
	SDA_OUT(F_IIC_Initer);
	SDA_set(F_IIC_Initer,1);
	SCL_set(F_IIC_Initer,1);
	delay_us(2);
	SDA_set(F_IIC_Initer,0);
	delay_us(2);
	SCL_set(F_IIC_Initer,0);
}

void F_IIC_stop(F_IIC_TypeDef *F_IIC_Initer)
{
	SDA_OUT(F_IIC_Initer);
	SCL_set(F_IIC_Initer,0);
	SDA_set(F_IIC_Initer,0);
	delay_us(2);
	SCL_set(F_IIC_Initer,1);
	SDA_set(F_IIC_Initer,1);
	delay_us(2);
}

void F_IIC_write(F_IIC_TypeDef *F_IIC_Initer, u8 val)
{
	u8 mx;
	SDA_OUT(F_IIC_Initer);
	SCL_set(F_IIC_Initer,0);
	for(mx=0x80;mx!=0;mx>>=1)
	{
		if(mx&val)
			SDA_set(F_IIC_Initer,1);
		else
			SDA_set(F_IIC_Initer,0);
		delay_us(1);
		SCL_set(F_IIC_Initer,1);
		delay_us(1);
		SCL_set(F_IIC_Initer,0);
	}
}

/*ack:是否回应ack    0：不回应  1：回应*/
u8 F_IIC_read(F_IIC_TypeDef *F_IIC_Initer, u8 ack)
{
	u8 mx,dat=0;
	SDA_IN(F_IIC_Initer);
	for(mx=0x80;mx!=0;mx>>=1)
	{
		SCL_set(F_IIC_Initer,0);
		delay_us(1);
		SCL_set(F_IIC_Initer,1);
		if(SDA_read(F_IIC_Initer))
			dat |= mx;
		else
			dat &= ~mx;
		delay_us(1);
	}
	//ack
	SCL_set(F_IIC_Initer,0);
	SDA_OUT(F_IIC_Initer);      //----顺序问题已修复2019/10/6
	SDA_set(F_IIC_Initer,!ack);
	delay_us(1);
	SCL_set(F_IIC_Initer,1);
	delay_us(1);
	SCL_set(F_IIC_Initer,0);
	
	return dat;
}


/*等待ACK回复，返回值 1：有ACK  0：无ACK*/
u8 F_IIC_Wait_ACK(F_IIC_TypeDef *F_IIC_Initer)
{
	u8 time=0;
	SDA_IN(F_IIC_Initer);
	delay_us(1);
	SCL_set(F_IIC_Initer,1);
	delay_us(1);
	while(SDA_read(F_IIC_Initer))
	{
		time++;
		if(time>250)
		{
			F_IIC_stop(F_IIC_Initer);
			return 0;
		}
	}
	SCL_set(F_IIC_Initer,0);
	return 1;
}
