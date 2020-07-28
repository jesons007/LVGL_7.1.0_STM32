#include "W25Q128.h"

void W25Q128_Init()
{
	GPIO_Init(GPIOB,GPIO_PIN_12,OUTPUT_PP);
	W25Q128_CS = 1;
	GPIO_Init(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,OUTPUT_FF);
	GPIO_Set(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	SPI_init(SPI2,SPI_MODE_2,SPI_8Bits_Mode,1);
}

u16  W25Q128_Read_ID()
{
	u16 ID=0;
	W25Q128_CS = 0;
	SPI_Write_Read(SPI2,0X90);
	SPI_Write_Read(SPI2,0X00);
	SPI_Write_Read(SPI2,0X00);
	SPI_Write_Read(SPI2,0X00);
	ID |= SPI_Write_Read(SPI2,0XFF)<<8;
	ID |= SPI_Write_Read(SPI2,0XFF);
	
	W25Q128_CS = 1;
	return ID;
}

void W25Q128_Read(u8 *buff,u32 addr,u16 read_len)
{
	u16 i;
	W25Q128_CS = 0;
	SPI_Write_Read(SPI2,W25_ReadDataCMD);
	SPI_Write_Read(SPI2,(u8)((addr)>>16));  	//发送24bit地址    
    SPI_Write_Read(SPI2,(u8)((addr)>>8));   
    SPI_Write_Read(SPI2,(u8)addr);
	for(i=0;i<read_len;i++)
	{
		buff[i] = SPI_Write_Read(SPI2,0XFF);
	}
	
	W25Q128_CS = 1;
}

//在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
//只能在一页里面写（第一页：0 ~ 255， 第二页：256 ~ 511， 第三页：512 ~ 767 ....依次类推共65536页）
void W25Q128_Write_Page(u8* buff,u32 StartAddr,u16 WR_len)
{
 	u16 i;  
    W25Q128_Write_EN();                  	    //SET WEL 
	W25Q128_CS=0;                            	//使能器件   
    SPI_Write_Read(SPI2,W25_Wr_Page_CMD);      	//发送写页命令   
    SPI_Write_Read(SPI2,(u8)((StartAddr)>>16)); 	//发送24bit地址    
    SPI_Write_Read(SPI2,(u8)((StartAddr)>>8));   
    SPI_Write_Read(SPI2,(u8)StartAddr);   
    for(i=0;i<WR_len;i++)
		SPI_Write_Read(SPI2,buff[i]);           //循环写数  
	W25Q128_CS=1;                            	//取消片选 
	W25Q128_Wait_Busy();					   		//等待写入结束
} 

u8   W25Q128_Read_SR()
{
	u8 sr;
	W25Q128_CS = 0;
	SPI_Write_Read(SPI2,W25_ReadSRCMD);
	sr = SPI_Write_Read(SPI2,0xff);
	W25Q128_CS = 1;
	return sr;
}

void W25Q128_Write_EN()
{
	W25Q128_CS = 0;
	SPI_Write_Read(SPI2,W25_WriteEnable);
	W25Q128_CS = 1;
}

void W25Q128_Wait_Busy()
{
	while((W25Q128_Read_SR()&0x01)==0x01);
}

void W25Q128_Erase_Sector(u32 Dst_Addr)
{
	Dst_Addr*=4096;
    W25Q128_Write_EN();                  	//SET WEL 	 
    W25Q128_Wait_Busy();   
  	W25Q128_CS=0;                            	//使能器件   
    SPI_Write_Read(SPI2,0x20);      				//发送扇区擦除指令 
    SPI_Write_Read(SPI2,(u8)((Dst_Addr)>>16));  	//发送24bit地址    
    SPI_Write_Read(SPI2,(u8)((Dst_Addr)>>8));   
    SPI_Write_Read(SPI2,(u8)Dst_Addr);  
	W25Q128_CS=1;                            	//取消片选     	      
    W25Q128_Wait_Busy();
}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25Q128_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25Q128_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
u8 W25QXX_BUFFER[4096];		 
void W25Q128_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25Q128_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25Q128_Erase_Sector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q128_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25Q128_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};	 
}

u8   W25Q128_Check()      //w25q128检查是否存在，存在返回1，不存在返回0
{
	if(W25Q128_Read_ID()==W25Q128_ID)
		return 1;
	else 
		return 0;
}

//	while(!W25Q128_Check())
//	{
//		LCD_show_str(0,0,"W25Q128 Error!",RED,BKOR);
//		delay_ms(100);
//		LCD_show_str(0,0,"              ",RED,BKOR);
//		delay_ms(100);
//	}
//	LCD_show_str(0,0,"W25Q128 ID:",RED,BKOR);
//	LCD_show_str(11*12,0,HEX(W25Q128_Read_ID()),RED,BKOR);

//	u8 p[11];
//	LCD_show_str(0,24,"Read Data:     ",RED,BKOR);
//	W25Q128_Read(p,0,10);
//	p[10] = 0;
//	LCD_show_str(0,48,p,RED,BKOR);

//	u8 p[10] ={"0123456789"}; 
//	LCD_show_str(0,24,"Start Write!...",RED,BKOR);
//	W25Q128_Write(p,0,10);
//	LCD_show_str(0,24,"Write Down!... ",RED,BKOR);
//	LCD_show_str(0,48,"          ",RED,BKOR);
