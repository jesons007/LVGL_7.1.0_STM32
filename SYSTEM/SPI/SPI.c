#include "spi.h"

/*
SPI时钟源说明：
	SPI2&SPI3: PCLK1 = 36M
	SPI1:      PCLK2 = 72M
	
	分频后波特率(速度等级:1~8):
	SPI1_CLK = 72M/(2^速度等级)  最快36M
	SPI2_CLK = 36M/(2^速度等级)  最快18M
	SPI3_CLK = 36M/(2^速度等级)  最快18M

SPI_init(SPI_TypeDef *SPIx, u8 SPI_MODE, u8 width, u8 SPI_Speed_Grade)函数说明：
默认配置:
	主从模式：    主机
	通信模式：	  双线双向全双工
	硬件CRC校验： 不启用
	DMA：         不启用
	软件从设备管理： 启用（SPI NSS线不用，使用软件片选）
	帧格式：       先发送MSB
参数说明：
	SPIx：SPI1/SPI2/SPI3
	SPI_MODE:
		SPI_MODE_1		时钟空闲状态为高，下降沿采样
		SPI_MODE_2		时钟空闲状态为高，上升沿采样
		SPI_MODE_3		时钟空闲状态为低，下降沿采样
		SPI_MODE_4		时钟空闲状态为低，上升沿采样
	width: 数据传输宽度8 or 16
		SPI_8Bits_Mode
		SPI_16Bits_Mode
	SPI_Speed_Grade: 设置spi速度等级（共1~8八个等级）
		可选参数为数字1 ~ 8
		设置为1时最快，可动态改变spi波特率（非通信期间）

SPI_Write_Read(SPI_TypeDef *SPIx, u8 data)函数说明：
	SPIx写一个字节的同时读回一个字节
	data： 待写数据
	返回值： 读回的字节

demo:使用SPI1驱动NF24L01
	首先确定从设备工作在SPI的哪个工作模式（确定时钟极性和采样边沿儿），确定SPI_MODE参数为SPI_MODE_4
	设置数据位宽度为SPI_8Bits_Mode
	设置SPI通信速度等级为3，SPI1_CLK = PCLK2/(2^速度等级) = 72M / (2^3) = 9M      (NRF24L01最大通信速度不能超过10M)
		SPI_init(SPI1, SPI_MODE_4, SPI_8Bits_Mode, 3);
	初始化完成！调用SPI_Write_Read函数即可与24L01进行通信。
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

//写一个字节的同时读回一个字节
u8 SPI_Write_Read(SPI_TypeDef *SPIx, u8 data)
{
		while(!(SPIx->SR&0x02));    //等待上一次发送完成
		SPIx->DR = data;
		while(!(SPIx->SR&0x01));    //等待接收完成
		return SPIx->DR;
}

// u8 SPI_Write_Read(SPI_TypeDef *SPIx, u8 data)
// {
// 	u16 val=0;
// 	while(!(SPIx->SR&0x02))    //等待上一次发送完成
// 	{
// 		val++;
// 		if(val>=0XFFFE)
// 			return 0;//超时异常
// 	}
// 	SPIx->DR = data;
// 	val = 0;
// 	while(!(SPIx->SR&0x01))    //等待接收完成
// 	{
// 		val++;
// 		if(val>=0XFFFE)
// 			return 0;//超时异常
// 	}
	
// 	return SPIx->DR;
// }