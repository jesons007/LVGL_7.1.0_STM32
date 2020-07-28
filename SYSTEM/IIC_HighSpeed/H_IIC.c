#include "H_IIC.h"

/*    高速IIC(H_IIC)使用说明：    */
/*
	软件模拟IIC
	//----函数说明----//
		H_IIC_init(u8 SCL,u8 SDA):
			初始化IIC接口引脚(包括时钟使能)
			参数说明 SCL/SDA: 
				PA(0) ~ PA(15)
				PB(0) ~ PB(15)
				PC(0) ~ PC(15)
				PD(0) ~ PD(15)
				PE(0) ~ PE(15)
				PF(0) ~ PF(15)
				PG(0) ~ PG(15)
			调用H_IIC_init()函数即可完成对IIC接口引脚的初始化
		void H_IIC_start(void)			//产生IIC启始信号
		void H_IIC_stop(void)			//产生IIC结束信号
		void H_IIC_write(u8 val)		//IIC写字节函数，val:要写入的数据
		u8 H_IIC_read(u8 ack)			//IIC读字节函数，返回读取的一个字节，ack:是否发送ack    0：不发送  1：发送
		u8 H_IIC_Wait_ACK(void)			//等待回复ack,返回值 1：有ACK  0：无ACK

	考虑到高速IIC的效率问题，使用前需要对以下函数进行改动(使用全速IIC不需要此步骤)：
		_SCL_set
		_SDA_set
		_SDA_IN
		_SDA_OUT
		_SDA_read
	将寄存器以及引脚编号修改为自己用的即可


	eg:IIC引脚对应关系为SCL->PB0   SDA->PB1
		首先调用
			H_IIC_init( PB(0), PB(1) );
		然后修改_SCL_set()为
			{
				if(val==0)
					GPIOB->ODR &= ~(1<<0);
				else
					GPIOB->ODR |= 1<<0;
			}

		修改_SDA_set()为
			{
				if(val==0)
					GPIOB->ODR &= ~(1<<1);
				else
					GPIOB->ODR |= 1<<1;
			}

		修改_SDA_IN()为
			{
				GPIOB->CRL &= 0XFFFFFF0F;
				GPIOB->CRL |= 0X00000080;
				GPIOB->ODR |= 1<<1;
			}

		修改_SDA_OUT()为
			{
				GPIOA->CRL &= 0XFFFFFF0F;
				GPIOA->CRL |= 0X00000030;
			}

		修改_SDA_read()为
			{
				if(GPIOB->IDR&(1<<1))
					return 1;
				else
					return 0;
			}


2019/10/6:修复IIC_Read函数
*/




void H_IIC_init(u8 SCL,u8 SDA)
{
	RCC->APB2ENR |= 1<<((SCL>>4)+2);//使能GPIO的时钟
	RCC->APB2ENR |= 1<<((SDA>>4)+2);
	
	//设置SCL和SDA的引脚初始模式为推挽输出模式
	*(uint32_t *)(GPIOA_BASE+1024*(SCL>>4)+(((SCL&0X0F)>7)?4:0)) &= ~(0X0000000F<<(4*(((SCL&0X0F)<8)?(SCL&0X0F):(SCL&0X0F)-8)));
	*(uint32_t *)(GPIOA_BASE+1024*(SDA>>4)+(((SDA&0X0F)>7)?4:0)) &= ~((u32)0X0000000F<<(4*(((SDA&0X0F)<8)?(SDA&0X0F):(SDA&0X0F)-8)));
	
	*(uint32_t *)(GPIOA_BASE+1024*(SCL>>4)+(((SCL&0X0F)>7)?4:0)) |=  0X3<<(4*(((SCL&0X0F)<8)?(SCL&0X0F):(SCL&0X0F)-8));
	*(uint32_t *)(GPIOA_BASE+1024*(SDA>>4)+(((SDA&0X0F)>7)?4:0)) |=  0X3<<(4*(((SDA&0X0F)<8)?(SDA&0X0F):(SDA&0X0F)-8));
	
	_SCL_set(1);
	_SDA_set(1);
}

void _SCL_set(u8 val)
{
	if(val==0)
		GPIOG->ODR &= ~(1<<13);
	else
		GPIOG->ODR |= 1<<13;
}

void _SDA_set(u8 val)
{
	if(val==0)
		GPIOC->ODR &= ~(1<<0);
	else
		GPIOC->ODR |= 1<<0;
	
}	

void _SDA_IN()//数据总线为输入模式
{
	GPIOC->CRL &= 0XFFFFFFF0;
	GPIOC->CRL |= 0X00000008;
	GPIOC->ODR |= 1<<0;
}

void _SDA_OUT()//数据总线为输出模式
{
	GPIOC->CRL &= 0XFFFFFFF0;
	GPIOC->CRL |= 0X00000003;
}

u8 _SDA_read()//读1或0
{
	if(GPIOC->IDR&(1<<0))
		return 1;
	else
		return 0;
}

void H_IIC_start()
{
	_SDA_OUT();
	_SCL_set(1);
	_SDA_set(1);
	//delay_us(1);
	_SDA_set(0);
	//delay_us(1);
	_SCL_set(0);
}

void H_IIC_stop()
{
	_SDA_OUT();
	_SCL_set(0);
	_SDA_set(0);
	//delay_us(1);
	_SCL_set(1);
	_SDA_set(1);
	//delay_us(1);
}

void H_IIC_write(u8 val)
{
	u8 mx;
	_SDA_OUT();
	_SCL_set(0);
	for(mx=0x80;mx!=0;mx>>=1)
	{
		if(mx&val)
			_SDA_set(1);
		else
			_SDA_set(0);
		//delay_us(1);
		_SCL_set(1);
		//delay_us(1);
		_SCL_set(0);
	}
	
}

/*ack:是否回应ack    0：不回应  1：回应*/
u8 H_IIC_read(u8 ack)
{
	u8 mx,dat=0;
	_SDA_IN();
	for(mx=0x80;mx!=0;mx>>=1)
	{
		_SCL_set(0);
		//delay_us(1);
		_SCL_set(1);
		if(_SDA_read())
			dat |= mx;
		else
			dat &= ~mx;
		//delay_us(1);
	}
	//ack
	_SCL_set(0);
	_SDA_OUT();    //----顺序问题已修复2019/10/6
	_SDA_set(!ack);
	//delay_us(1);
	_SCL_set(1);
	//delay_us(1);
	_SCL_set(0);
	
	return dat;
	
}


/*等待ACK回复，返回值 1：有ACK  0：无ACK*/
u8 H_IIC_Wait_ACK()
{
	u8 time=0;
	_SDA_IN();
	_SCL_set(1);
	//delay_us(2);
	while(_SDA_read())
	{
		time++;
		if(time>250)
		{
			H_IIC_stop();
			return 0;
		}
	}
	_SCL_set(0);
	return 1;
}
