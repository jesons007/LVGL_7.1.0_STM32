#ifndef _W25128_H_
#define _W25128_H_
#include "SPI.h"
#include "GPIO.h"

#define W25Q128_CS PBout(12)
#define W25Q128_ID 0XEF17

//W25Q128 CMD
#define W25_ReadDataCMD			0x03 
#define W25_ReadSRCMD		    0x05 
#define W25_Wr_Page_CMD         0x02 
#define W25_WriteEnable		    0x06 

void W25Q128_Init(void);
u16  W25Q128_Read_ID(void);
u8   W25Q128_Check(void);
void W25Q128_Read(u8 *buff,u32 addr,u16 read_len);
u8   W25Q128_Read_SR(void);
void W25Q128_Wait_Busy(void);
void W25Q128_Write_EN(void);
void W25Q128_Write_Page(u8* buff,u32 StartAddr,u16 WR_len);
void W25Q128_Erase_Sector(u32 Dst_Addr);	//ÉÈÇø²Á³ý
void W25Q128_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25Q128_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);


#endif
