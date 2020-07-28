#include "H_IIC.h"

/*    ����IIC(H_IIC)ʹ��˵����    */
/*
	���ģ��IIC
	//----����˵��----//
		H_IIC_init(u8 SCL,u8 SDA):
			��ʼ��IIC�ӿ�����(����ʱ��ʹ��)
			����˵�� SCL/SDA: 
				PA(0) ~ PA(15)
				PB(0) ~ PB(15)
				PC(0) ~ PC(15)
				PD(0) ~ PD(15)
				PE(0) ~ PE(15)
				PF(0) ~ PF(15)
				PG(0) ~ PG(15)
			����H_IIC_init()����������ɶ�IIC�ӿ����ŵĳ�ʼ��
		void H_IIC_start(void)			//����IIC��ʼ�ź�
		void H_IIC_stop(void)			//����IIC�����ź�
		void H_IIC_write(u8 val)		//IICд�ֽں�����val:Ҫд�������
		u8 H_IIC_read(u8 ack)			//IIC���ֽں��������ض�ȡ��һ���ֽڣ�ack:�Ƿ���ack    0��������  1������
		u8 H_IIC_Wait_ACK(void)			//�ȴ��ظ�ack,����ֵ 1����ACK  0����ACK

	���ǵ�����IIC��Ч�����⣬ʹ��ǰ��Ҫ�����º������иĶ�(ʹ��ȫ��IIC����Ҫ�˲���)��
		_SCL_set
		_SDA_set
		_SDA_IN
		_SDA_OUT
		_SDA_read
	���Ĵ����Լ����ű���޸�Ϊ�Լ��õļ���


	eg:IIC���Ŷ�Ӧ��ϵΪSCL->PB0   SDA->PB1
		���ȵ���
			H_IIC_init( PB(0), PB(1) );
		Ȼ���޸�_SCL_set()Ϊ
			{
				if(val==0)
					GPIOB->ODR &= ~(1<<0);
				else
					GPIOB->ODR |= 1<<0;
			}

		�޸�_SDA_set()Ϊ
			{
				if(val==0)
					GPIOB->ODR &= ~(1<<1);
				else
					GPIOB->ODR |= 1<<1;
			}

		�޸�_SDA_IN()Ϊ
			{
				GPIOB->CRL &= 0XFFFFFF0F;
				GPIOB->CRL |= 0X00000080;
				GPIOB->ODR |= 1<<1;
			}

		�޸�_SDA_OUT()Ϊ
			{
				GPIOA->CRL &= 0XFFFFFF0F;
				GPIOA->CRL |= 0X00000030;
			}

		�޸�_SDA_read()Ϊ
			{
				if(GPIOB->IDR&(1<<1))
					return 1;
				else
					return 0;
			}


2019/10/6:�޸�IIC_Read����
*/




void H_IIC_init(u8 SCL,u8 SDA)
{
	RCC->APB2ENR |= 1<<((SCL>>4)+2);//ʹ��GPIO��ʱ��
	RCC->APB2ENR |= 1<<((SDA>>4)+2);
	
	//����SCL��SDA�����ų�ʼģʽΪ�������ģʽ
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

void _SDA_IN()//��������Ϊ����ģʽ
{
	GPIOC->CRL &= 0XFFFFFFF0;
	GPIOC->CRL |= 0X00000008;
	GPIOC->ODR |= 1<<0;
}

void _SDA_OUT()//��������Ϊ���ģʽ
{
	GPIOC->CRL &= 0XFFFFFFF0;
	GPIOC->CRL |= 0X00000003;
}

u8 _SDA_read()//��1��0
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

/*ack:�Ƿ��Ӧack    0������Ӧ  1����Ӧ*/
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
	_SDA_OUT();    //----˳���������޸�2019/10/6
	_SDA_set(!ack);
	//delay_us(1);
	_SCL_set(1);
	//delay_us(1);
	_SCL_set(0);
	
	return dat;
	
}


/*�ȴ�ACK�ظ�������ֵ 1����ACK  0����ACK*/
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
