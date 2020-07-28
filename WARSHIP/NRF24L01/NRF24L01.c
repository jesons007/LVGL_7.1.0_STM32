/*-------------NRF24L01外设驱动库使用说明-----------------			
USER_Init:使用前需要更改对应的端口
默认只使用接收通道0

void NRF24L01_Init():  								NRF24L01初始化
u8 NRF_Check():	  									检查NRF24L01模块是否存在，存在返回 1， 不存在返回0
						
void NRF_TX_MODE():									设置NRF24L01为发送模式
u8 NRF_TX_Packet(u8 *Packet_Buf, u8 TX_Len)			发送数据包，长度为TX_Len字节,发送成功返回 TX_OK    失败返回 错误代码

void NRF_RX_MODE(u8 RX_Len)							设置NRF24L01为接收模式, RX_Len:选择通道0的有效数据宽度(0-32字节)
u8 NRF_RX_Packet(u8 *Packet_Buf)					检查是否有数据包到来，如果有将数据装入Packet_Buf中， 有数据包返回 1		没数据包返回 0


demo:
在NRF24L01_Init()中初始化对应端口和SPI:
	GPIO_Init(GPIOB,GPIO_PIN_7,INPUT_UP);      //NRF_IRQ
	GPIO_Init(GPIOB,GPIO_PIN_8,OUTPUT_PP);     //NRF_CS
	GPIO_Init(GPIOB,GPIO_PIN_9,OUTPUT_PP);     //NRF_CE
	GPIO_Set(GPIOB,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9); //PB 7 8 9 上拉	   	           
	
	GPIO_Init(GPIOA,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,OUTPUT_FF);     //初始化SPI端口，   PA5->SPI2_CLK	PA6->SPI2_MISO	PA7->SPI2_MOSI
	GPIO_Set(GPIOA,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);   	           //PA 5/6/7上拉
		
	SPI_init(SPI2,SPI_MODE_4,SPI_8Bits_Mode,3);			//SPI2初始化，并设置速度9M
修改宏定义为
	#define NRF_CS  PBout(8)
	#define NRF_CE  PBout(9)
	#define NRF_IRQ PBin(7)
设置发送接收地址
	const u8 CH0_RX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//通道0接收地址
	const u8 SET_TX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//模块发送地址
调用初始化函数
	NRF24L01_Init()
设置模式为发送或者接收
	NRF_RX_MODE(32) / NRF_TX_MODE()
*/

#include "NRF24L01.h"
#include "spi.h"
#include "GPIO.h"

/*------------------------------此部分由用户自定义设置------------------------------*/
#define NRF_CS  PGout(7)								//SPI片选信号					
#define NRF_CE  PGout(8)
#define NRF_IRQ PGin(6)
const u8 CH0_RX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//通道0接收地址
const u8 SET_TX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//模块发送地址

void NRF24L01_Init()
{
/*-------------------此部分由用户自定义设置---------------------*/
	GPIO_Init(GPIOG,GPIO_PIN_6,INPUT_UP);      //NRF_IRQ
	GPIO_Init(GPIOG,GPIO_PIN_7,OUTPUT_PP);     //NRF_CS
	GPIO_Init(GPIOG,GPIO_PIN_8,OUTPUT_PP);     //NRF_CE
	GPIO_Set(GPIOG,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_6); //PG 6 7 8 上拉	   	           
	
	GPIO_Init(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,OUTPUT_FF);     //初始化SPI端口，   
	GPIO_Set(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);   	           //PB 13/14/15上拉
	
	GPIO_Init(GPIOB,GPIO_PIN_12,OUTPUT_FF);      //PB12上拉 防止W25Qxx的干扰
	GPIO_Set(GPIOB,GPIO_PIN_12);
		
	SPI_init(SPI2,SPI_MODE_4,SPI_8Bits_Mode,2);			//SPI2初始化，并设置速度9M
/*----------------------------------------------------------------------------------------------------------------------*/							
	
	NRF_CS = 1;            //SPI片选取消
	NRF_CE = 0;            //使能24L01
}


//返回值：检查成功返回 1		失败返回 0
u8 NRF_Check()
{
	u8 i;
	u8 buf[5] = {0X78,0X78,0X78,0X78,0X78};
	NRF_Write_Buff(0x20+TX_ADDR,buf,5);
	NRF_Read_Buff(0x00+TX_ADDR,buf,5);
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0X78)break;
	}
	if(i==5)
		return 1;
	else 
		return 0;
}


//reg:指定寄存器地址
//val:要写入的值
void NRF_Write_Reg(u8 reg,u8 val)
{
	NRF_CS = 0;
	SPI_Write_Read(SPI2, 0x20+reg);
	SPI_Write_Read(SPI2, val);
	NRF_CS = 1;
}

//reg:指定要读的寄存器地址
u8 NRF_Read_Reg(u8 reg)
{
	u8 dat;
	NRF_CS = 0;
	SPI_Write_Read(SPI2, 0x00+reg);
	dat = SPI_Write_Read(SPI2, 0xff);
	NRF_CS = 1;
	
	return dat;
}

//在指定地点读数据
//cmd:为寄存器地址或读缓冲区指令
//buff：数据存放的指针
//len:读取数据的长度
void NRF_Read_Buff(u8 cmd,u8 *buff, u8 len)
{
	u8 i;
	
	NRF_CS = 0;
	SPI_Write_Read(SPI2, cmd);
	for(i=0;i<len;i++)
		buff[i] = SPI_Write_Read(SPI2, 0xff);
	
	NRF_CS = 1;
}


//在指定地点写数据
//cmd:为寄存器地址或写缓冲区指令
//buff：数据存放的指针
//len:写入数据的长度
void NRF_Write_Buff(u8 cmd,u8 *buff, u8 len)
{
	u8 i;
	
	NRF_CS = 0;
	SPI_Write_Read(SPI2, cmd);
	for(i=0;i<len;i++)
		SPI_Write_Read(SPI2, *buff++);
	
	NRF_CS = 1;
}

//检查是否有数据包到来，如果有将数据装入Packet_Buf中
//每个通道每次接收的长度固定，由NRF24L01内部寄存器(RX_PW_P0 - RX_PW_P5)设置
//有数据包返回 1		没数据包返回 0
u8 NRF_RX_Packet(u8 *Packet_Buf)
{
	u8 sta;
	sta = NRF_Read_Reg(STATUS);
	NRF_Write_Reg(STATUS,sta);
	if(sta&0x40)
	{
		NRF_Read_Buff(R_RX_PAYLOAD,Packet_Buf,1);      //从缓冲区读取1个字节
		NRF_CS = 0;
		SPI_Write_Read(SPI2, FLUSH_RX);
		SPI_Write_Read(SPI2, 0xff);
		NRF_CS = 1;
		return 1;
	}
	else
		return 0;
}

//发送一个数据包，数据包长度为1-32字节
//发送成功返回 TX_OK    失败返回 0
//TX_Len:发送数据包长度(单位字节)，需要根据接收方的接收包大小来设定
u8 NRF_TX_Packet(u8 *Packet_Buf, u8 TX_Len)
{
	u8 sta;
	NRF_CE = 0;
	NRF_Write_Buff(W_TX_PAYLOAD,Packet_Buf,TX_Len);      		//向缓冲区写1个字节
	NRF_CE = 1;												//启动发送
	while(NRF_IRQ!=0);										//等待发送完成
	sta = NRF_Read_Reg(STATUS);
	NRF_Write_Reg(STATUS,sta);
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF_CS = 0;
		SPI_Write_Read(SPI2, FLUSH_TX);							//清除TX FIFO寄存器 
		SPI_Write_Read(SPI2, 0xff);
		NRF_CS = 1;
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}


//设置NRF24L01为发送模式
void NRF_TX_MODE()
{
	NRF_CE=0;    
	
  	NRF_Write_Buff(0x20+TX_ADDR,(u8*)SET_TX_ADDR,5);    	//写TX节点地址 
  	NRF_Write_Buff(0x20+RX_ADDR_P0,(u8*)CH0_RX_ADDR,5); 	//设置TX节点地址,主要为了使能ACK	  

  	NRF_Write_Reg(EN_AA,0x01);     					//使能通道0的自动应答    
  	NRF_Write_Reg(EN_RXADDR,0x01);    				//使能通道0的接收地址  
  	NRF_Write_Reg(SETUP_RETR,0x1a);   				//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF_Write_Reg(RF_CH,40);                        //设置RF通道为40
  	NRF_Write_Reg(RF_SETUP,0x0f);			        //设置TX发射参数,0db增益,2Mbps,低噪声增益开启    
  	NRF_Write_Reg(CONFIG_REG, 0x0e);    			//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发送模式,开启所有中断
	
	NRF_CE=1;//CE为高,10us后启动发送
	
}

//设置NRF24L01为接收模式
//RX_Len:选择通道0的有效数据宽度(0-32字节)
void NRF_RX_MODE(u8 RX_Len)
{
	NRF_CE = 0;
	
	NRF_Write_Buff(0x20+RX_ADDR_P0,(u8 *)CH0_RX_ADDR,5);
	NRF_Write_Reg(EN_AA,0x01);         				//使能通道0的自动应答    
  	NRF_Write_Reg(EN_RXADDR,0x01); 					//使能通道0的接收地址  	 
  	NRF_Write_Reg(RF_CH,40);	     		 		//设置RF通信频率 (2400+40)M =2.44GHZ		  
  	NRF_Write_Reg(RX_PW_P0,RX_Len);						//选择通道0的有效数据宽度: RX_Len    
  	NRF_Write_Reg(RF_SETUP,0x0f);			        //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF_Write_Reg(CONFIG_REG, 0x0f);		        //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	
	
	NRF_CE = 1;     //CE为高,进入接收模式 
}

//	LCD_show_str(0,0,"NRF24L01_Test by: Jesons007",BLACK,BKOR);
//	LCD_show_str(0,24,"NRF24L01_Check:",BLACK,BKOR);
//	while(NRF_Check()!=1)
//	{
//		//LED0 = !LED0;
//		LCD_show_str(15*12,24,"Error!",BLACK,BKOR);
//		delay_ms(200);
//		LCD_show_str(15*12,24,"      ",BLACK,BKOR);
//		delay_ms(200);
//	}
