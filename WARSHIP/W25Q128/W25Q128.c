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
	SPI_Write_Read(SPI2,(u8)((addr)>>16));  	//����24bit��ַ    
    SPI_Write_Read(SPI2,(u8)((addr)>>8));   
    SPI_Write_Read(SPI2,(u8)addr);
	for(i=0;i<read_len;i++)
	{
		buff[i] = SPI_Write_Read(SPI2,0XFF);
	}
	
	W25Q128_CS = 1;
}

//��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
//ֻ����һҳ����д����һҳ��0 ~ 255�� �ڶ�ҳ��256 ~ 511�� ����ҳ��512 ~ 767 ....�������ƹ�65536ҳ��
void W25Q128_Write_Page(u8* buff,u32 StartAddr,u16 WR_len)
{
 	u16 i;  
    W25Q128_Write_EN();                  	    //SET WEL 
	W25Q128_CS=0;                            	//ʹ������   
    SPI_Write_Read(SPI2,W25_Wr_Page_CMD);      	//����дҳ����   
    SPI_Write_Read(SPI2,(u8)((StartAddr)>>16)); 	//����24bit��ַ    
    SPI_Write_Read(SPI2,(u8)((StartAddr)>>8));   
    SPI_Write_Read(SPI2,(u8)StartAddr);   
    for(i=0;i<WR_len;i++)
		SPI_Write_Read(SPI2,buff[i]);           //ѭ��д��  
	W25Q128_CS=1;                            	//ȡ��Ƭѡ 
	W25Q128_Wait_Busy();					   		//�ȴ�д�����
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
  	W25Q128_CS=0;                            	//ʹ������   
    SPI_Write_Read(SPI2,0x20);      				//������������ָ�� 
    SPI_Write_Read(SPI2,(u8)((Dst_Addr)>>16));  	//����24bit��ַ    
    SPI_Write_Read(SPI2,(u8)((Dst_Addr)>>8));   
    SPI_Write_Read(SPI2,(u8)Dst_Addr);  
	W25Q128_CS=1;                            	//ȡ��Ƭѡ     	      
    W25Q128_Wait_Busy();
}

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25Q128_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25Q128_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
u8 W25QXX_BUFFER[4096];		 
void W25Q128_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25Q128_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25Q128_Erase_Sector(secpos);		//�����������
			for(i=0;i<secremain;i++)	   		//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q128_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25Q128_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  				//ָ��ƫ��
			WriteAddr+=secremain;				//д��ַƫ��	   
		   	NumByteToWrite-=secremain;			//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;//��һ����������д����
			else secremain=NumByteToWrite;		//��һ����������д����
		}	 
	};	 
}

u8   W25Q128_Check()      //w25q128����Ƿ���ڣ����ڷ���1�������ڷ���0
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
