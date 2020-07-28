#include "spi.h"

/*
SPIʱ��Դ˵����
	SPI2&SPI3: PCLK1 = 36M
	SPI1:      PCLK2 = 72M
	
	��Ƶ������(�ٶȵȼ�:1~8):
	SPI1_CLK = 72M/(2^�ٶȵȼ�)  ���36M
	SPI2_CLK = 36M/(2^�ٶȵȼ�)  ���18M
	SPI3_CLK = 36M/(2^�ٶȵȼ�)  ���18M

SPI_init(SPI_TypeDef *SPIx, u8 SPI_MODE, u8 width, u8 SPI_Speed_Grade)����˵����
Ĭ������:
	����ģʽ��    ����
	ͨ��ģʽ��	  ˫��˫��ȫ˫��
	Ӳ��CRCУ�飺 ������
	DMA��         ������
	������豸���� ���ã�SPI NSS�߲��ã�ʹ�����Ƭѡ��
	֡��ʽ��       �ȷ���MSB
����˵����
	SPIx��SPI1/SPI2/SPI3
	SPI_MODE:
		SPI_MODE_1		ʱ�ӿ���״̬Ϊ�ߣ��½��ز���
		SPI_MODE_2		ʱ�ӿ���״̬Ϊ�ߣ������ز���
		SPI_MODE_3		ʱ�ӿ���״̬Ϊ�ͣ��½��ز���
		SPI_MODE_4		ʱ�ӿ���״̬Ϊ�ͣ������ز���
	width: ���ݴ�����8 or 16
		SPI_8Bits_Mode
		SPI_16Bits_Mode
	SPI_Speed_Grade: ����spi�ٶȵȼ�����1~8�˸��ȼ���
		��ѡ����Ϊ����1 ~ 8
		����Ϊ1ʱ��죬�ɶ�̬�ı�spi�����ʣ���ͨ���ڼ䣩

SPI_Write_Read(SPI_TypeDef *SPIx, u8 data)����˵����
	SPIxдһ���ֽڵ�ͬʱ����һ���ֽ�
	data�� ��д����
	����ֵ�� ���ص��ֽ�

demo:ʹ��SPI1����NF24L01
	����ȷ�����豸������SPI���ĸ�����ģʽ��ȷ��ʱ�Ӽ��ԺͲ������ض�����ȷ��SPI_MODE����ΪSPI_MODE_4
	��������λ���ΪSPI_8Bits_Mode
	����SPIͨ���ٶȵȼ�Ϊ3��SPI1_CLK = PCLK2/(2^�ٶȵȼ�) = 72M / (2^3) = 9M      (NRF24L01���ͨ���ٶȲ��ܳ���10M)
		SPI_init(SPI1, SPI_MODE_4, SPI_8Bits_Mode, 3);
	��ʼ����ɣ�����SPI_Write_Read����������24L01����ͨ�š�
*/
void SPI_init(SPI_TypeDef *SPIx, u8 SPI_MODE, u8 width, u8 SPI_Speed_Grade)
{
	if(SPIx==SPI1)
		RCC->APB2ENR |= 1<<12;
	else if(SPIx==SPI2)
		RCC->APB1ENR |= 1<<14;
	else if(SPIx==SPI3)
		RCC->APB1ENR |= 1<<15;
	
	SPIx->CR1 &= ~(1<<6);
	SPIx->CR1 = 0X0304;
	SPIx->CR1 |= (width/8 -1)<<11;
	SPIx->CR1 |= SPI_MODE; 

	SPIx->CR1 &= 0XFFC7;
	SPIx->CR1 |= (SPI_Speed_Grade-1)<<3;
	SPIx->CR1 |= 1<<6;
	
	SPI_Write_Read(SPIx,0xFF);
}

//дһ���ֽڵ�ͬʱ����һ���ֽ�
u8 SPI_Write_Read(SPI_TypeDef *SPIx, u8 data)
{
		while(!(SPIx->SR&0x02));    //�ȴ���һ�η������
		SPIx->DR = data;
		while(!(SPIx->SR&0x01));    //�ȴ��������
		return SPIx->DR;
}

// u8 SPI_Write_Read(SPI_TypeDef *SPIx, u8 data)
// {
// 	u16 val=0;
// 	while(!(SPIx->SR&0x02))    //�ȴ���һ�η������
// 	{
// 		val++;
// 		if(val>=0XFFFE)
// 			return 0;//��ʱ�쳣
// 	}
// 	SPIx->DR = data;
// 	val = 0;
// 	while(!(SPIx->SR&0x01))    //�ȴ��������
// 	{
// 		val++;
// 		if(val>=0XFFFE)
// 			return 0;//��ʱ�쳣
// 	}
	
// 	return SPIx->DR;
// }