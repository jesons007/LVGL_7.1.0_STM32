#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f10x.h"


#define SPI_MODE_1 0X02
#define SPI_MODE_2 0X03
#define SPI_MODE_3 0X01
#define SPI_MODE_4 0X00

#define SPI_8Bits_Mode  8
#define SPI_16Bits_Mode 16

void SPI_init(SPI_TypeDef *SPIx, u8 SPI_MODE, u8 width, u8 SPI_Speed_Grade);
u8 SPI_Write_Read(SPI_TypeDef *SPIx, u8 data);

#endif

