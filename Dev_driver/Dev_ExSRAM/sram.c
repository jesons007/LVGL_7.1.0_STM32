#include "sram.h"

// u32 testsram_32[250000] __attribute__((at(0X68000000)));//测试用数组
// u32 *testsram_32 = (u32 *)0X68000000;
// u16 *testsram_16 = (u16 *)0X68000000;
// u8  *testsram__8 = (u8  *)0X68000000;

//若地址超过1M，会重新从SRAM的0地址处读/写
//即若往地址262144处写/读，则数据会从0处读/写
#define SRAM_U32_MAXADDR 262144     //addr: 0~262143   //刚好1M字节
#define SRAM_U16_MAXADDR 524288		//addr: 0~524287
#define SRAM_U8__MAXADDR 1048576    //addr: 0~1048575
//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))
//初始化外部SRAM
void FSMC_SRAM_Init(void)
{			 			    
	RCC->AHBENR|=1<<8;       //使能FSMC时钟	  
	RCC->APB2ENR|=1<<5;      //使能PORTD时钟
	RCC->APB2ENR|=1<<6;      //使能PORTE时钟
 	RCC->APB2ENR|=1<<7;      //使能PORTF时钟
	RCC->APB2ENR|=1<<8;      //使能PORTG时钟	   

	//PORTD复用推挽输出 	
	GPIOD->CRH&=0X00000000;
	GPIOD->CRH|=0XBBBBBBBB; 
	GPIOD->CRL&=0XFF00FF00;
	GPIOD->CRL|=0X00BB00BB;   	 
	//PORTE复用推挽输出 	
	GPIOE->CRH&=0X00000000;
	GPIOE->CRH|=0XBBBBBBBB; 
	GPIOE->CRL&=0X0FFFFF00;
	GPIOE->CRL|=0XB00000BB; 
	//PORTF复用推挽输出
	GPIOF->CRH&=0X0000FFFF;
	GPIOF->CRH|=0XBBBB0000;  	    	 											 
	GPIOF->CRL&=0XFF000000;
	GPIOF->CRL|=0X00BBBBBB; 

	//PORTG复用推挽输出 PG10->NE3     	 											 
	GPIOG->CRH&=0XFFFFF0FF;
	GPIOG->CRH|=0X00000B00;  
 	GPIOG->CRL&=0XFF000000;
	GPIOG->CRL|=0X00BBBBBB;  	 				  
  
	//寄存器清零
	//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
	//这里我们使用NE3 ，也就对应BTCR[4],[5]。				    
	FSMC_Bank1->BTCR[4]=0X00000000;
	FSMC_Bank1->BTCR[5]=0X00000000;
	FSMC_Bank1E->BWTR[4]=0X00000000;
	//操作BCR寄存器	使用异步模式,模式A(读写共用一个时序寄存器)
	//BTCR[偶数]:BCR寄存器;BTCR[奇数]:BTR寄存器
	FSMC_Bank1->BTCR[4]|=1<<12;//存储器写使能
	FSMC_Bank1->BTCR[4]|=1<<4; //存储器数据宽度为16bit 	    
	//操作BTR寄存器								    
	FSMC_Bank1->BTCR[5]|=3<<8; //数据保持时间（DATAST）为3个HCLK 4/72M=55ns(对EM的SRAM芯片)	 	 
	FSMC_Bank1->BTCR[5]|=0<<4; //地址保持时间（ADDHLD）未用到	  	 
	FSMC_Bank1->BTCR[5]|=0<<0; //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns	 	 
	//闪存写时序寄存器  
	FSMC_Bank1E->BWTR[4]=0x0FFFFFFF;//默认值
	//使能BANK1区域3
	FSMC_Bank1->BTCR[4]|=1<<0; 												
}

//在指定地址(WriteAddr+Bank1_SRAM3_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)  
	{										    
		*(vu8*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;	  
		WriteAddr++;
		pBuffer++;
	}   
}																			    
//在指定地址((WriteAddr+Bank1_SRAM3_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)  
	{											    
		*pBuffer++=*(vu8*)(Bank1_SRAM3_ADDR+ReadAddr);    
		ReadAddr++;
	}  
} 
