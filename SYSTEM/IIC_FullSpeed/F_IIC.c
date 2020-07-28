#include "F_IIC.h"

/*    ȫ��IIC(F_IIC)ʹ��˵����    */
/*
	���ģ��IIC
	//----����˵��----//
		void F_IIC_init(F_IIC_TypeDef *F_IIC_Initer)��
			��ʼ��IIC�ӿ�����(����ʱ��ʹ��)
			����˵�� .SCL/.SDA: 
				PA(0) ~ PA(15)
				PB(0) ~ PB(15)
				PC(0) ~ PC(15)
				PD(0) ~ PD(15)
				PE(0) ~ PE(15)
				PF(0) ~ PF(15)
				PG(0) ~ PG(15)
			����F_IIC_init()����������ɶ�IIC�ӿ����ŵĳ�ʼ��
		void F_IIC_start(F_IIC_TypeDef *F_IIC_Initer)			//����IIC��ʼ�ź�
		void F_IIC_stop(F_IIC_TypeDef *F_IIC_Initer)			//����IIC�����ź�
		void F_IIC_write(F_IIC_TypeDef *F_IIC_Initer, u8 val)	//IICд�ֽں�����val:Ҫд�������
		u8 F_IIC_read(F_IIC_TypeDef *F_IIC_Initer, u8 ack)		//IIC���ֽں��������ض�ȡ��һ���ֽڣ�ack:�Ƿ���ack    0��������  1������
		u8 F_IIC_Wait_ACK(F_IIC_TypeDef *F_IIC_Initer)			//�ȴ��ظ�ack,����ֵ 1����ACK  0����ACK
																//F_IIC_TypeDef *F_IIC_Initer������ΪIIC�û�����

	eg:IIC���Ŷ�Ӧ��ϵΪSCL->PB0   SDA->PB1
		���ȶ���ṹ���������ֵ:
			F_IIC_TypeDef IIC_User_1 = 
			{
				.IIC_SCL = PB(0),
				.IIC_SDA = PB(1),
			};
		Ȼ����ó�ʼ������:
			F_IIC_init(&IIC_User_1);
		��ʼ�����!

2019/10/6:�޸�IIC_Read����

*/


void F_IIC_init(F_IIC_TypeDef *F_IIC_Initer)
{
	
	RCC->APB2ENR |= 1<<((F_IIC_Initer->IIC_SCL>>4)+2);//ʹ��GPIO��ʱ��
	RCC->APB2ENR |= 1<<((F_IIC_Initer->IIC_SDA>>4)+2);
	
	//����SCL��SDA�����ų�ʼģʽΪ�������ģʽ
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

void SDA_IN(F_IIC_TypeDef *F_IIC_Initer)//��������Ϊ����ģʽ
{
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) &= ~((u32)0X0000000F<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8)));
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) |=  ((u32)0X8<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8)));
	SDA_set(F_IIC_Initer, 1);
}

void SDA_OUT(F_IIC_TypeDef *F_IIC_Initer)//��������Ϊ���ģʽ
{
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) &= ~((u32)0X0000000F<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8)));
	*(uint32_t *)(GPIOA_BASE+1024*(F_IIC_Initer->IIC_SDA>>4)+(((F_IIC_Initer->IIC_SDA&0X0F)>7)?4:0)) |=  0X3<<(4*(((F_IIC_Initer->IIC_SDA&0X0F)<8)?(F_IIC_Initer->IIC_SDA&0X0F):(F_IIC_Initer->IIC_SDA&0X0F)-8));

}

u8 SDA_read(F_IIC_TypeDef *F_IIC_Initer)//��1��0
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

/*ack:�Ƿ��Ӧack    0������Ӧ  1����Ӧ*/
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
	SDA_OUT(F_IIC_Initer);      //----˳���������޸�2019/10/6
	SDA_set(F_IIC_Initer,!ack);
	delay_us(1);
	SCL_set(F_IIC_Initer,1);
	delay_us(1);
	SCL_set(F_IIC_Initer,0);
	
	return dat;
}


/*�ȴ�ACK�ظ�������ֵ 1����ACK  0����ACK*/
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
