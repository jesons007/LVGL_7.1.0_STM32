/*-------------NRF24L01����������ʹ��˵��-----------------			
USER_Init:ʹ��ǰ��Ҫ���Ķ�Ӧ�Ķ˿�
Ĭ��ֻʹ�ý���ͨ��0

void NRF24L01_Init():  								NRF24L01��ʼ��
u8 NRF_Check():	  									���NRF24L01ģ���Ƿ���ڣ����ڷ��� 1�� �����ڷ���0
						
void NRF_TX_MODE():									����NRF24L01Ϊ����ģʽ
u8 NRF_TX_Packet(u8 *Packet_Buf, u8 TX_Len)			�������ݰ�������ΪTX_Len�ֽ�,���ͳɹ����� TX_OK    ʧ�ܷ��� �������

void NRF_RX_MODE(u8 RX_Len)							����NRF24L01Ϊ����ģʽ, RX_Len:ѡ��ͨ��0����Ч���ݿ��(0-32�ֽ�)
u8 NRF_RX_Packet(u8 *Packet_Buf)					����Ƿ������ݰ�����������н�����װ��Packet_Buf�У� �����ݰ����� 1		û���ݰ����� 0


demo:
��NRF24L01_Init()�г�ʼ����Ӧ�˿ں�SPI:
	GPIO_Init(GPIOB,GPIO_PIN_7,INPUT_UP);      //NRF_IRQ
	GPIO_Init(GPIOB,GPIO_PIN_8,OUTPUT_PP);     //NRF_CS
	GPIO_Init(GPIOB,GPIO_PIN_9,OUTPUT_PP);     //NRF_CE
	GPIO_Set(GPIOB,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9); //PB 7 8 9 ����	   	           
	
	GPIO_Init(GPIOA,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,OUTPUT_FF);     //��ʼ��SPI�˿ڣ�   PA5->SPI2_CLK	PA6->SPI2_MISO	PA7->SPI2_MOSI
	GPIO_Set(GPIOA,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);   	           //PA 5/6/7����
		
	SPI_init(SPI2,SPI_MODE_4,SPI_8Bits_Mode,3);			//SPI2��ʼ�����������ٶ�9M
�޸ĺ궨��Ϊ
	#define NRF_CS  PBout(8)
	#define NRF_CE  PBout(9)
	#define NRF_IRQ PBin(7)
���÷��ͽ��յ�ַ
	const u8 CH0_RX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//ͨ��0���յ�ַ
	const u8 SET_TX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//ģ�鷢�͵�ַ
���ó�ʼ������
	NRF24L01_Init()
����ģʽΪ���ͻ��߽���
	NRF_RX_MODE(32) / NRF_TX_MODE()
*/

#include "NRF24L01.h"
#include "spi.h"
#include "GPIO.h"

/*------------------------------�˲������û��Զ�������------------------------------*/
#define NRF_CS  PGout(7)								//SPIƬѡ�ź�					
#define NRF_CE  PGout(8)
#define NRF_IRQ PGin(6)
const u8 CH0_RX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//ͨ��0���յ�ַ
const u8 SET_TX_ADDR[5] = {0xe5,0xe4,0xe3,0xe2,0xe1};	//ģ�鷢�͵�ַ

void NRF24L01_Init()
{
/*-------------------�˲������û��Զ�������---------------------*/
	GPIO_Init(GPIOG,GPIO_PIN_6,INPUT_UP);      //NRF_IRQ
	GPIO_Init(GPIOG,GPIO_PIN_7,OUTPUT_PP);     //NRF_CS
	GPIO_Init(GPIOG,GPIO_PIN_8,OUTPUT_PP);     //NRF_CE
	GPIO_Set(GPIOG,GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_6); //PG 6 7 8 ����	   	           
	
	GPIO_Init(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,OUTPUT_FF);     //��ʼ��SPI�˿ڣ�   
	GPIO_Set(GPIOB,GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);   	           //PB 13/14/15����
	
	GPIO_Init(GPIOB,GPIO_PIN_12,OUTPUT_FF);      //PB12���� ��ֹW25Qxx�ĸ���
	GPIO_Set(GPIOB,GPIO_PIN_12);
		
	SPI_init(SPI2,SPI_MODE_4,SPI_8Bits_Mode,2);			//SPI2��ʼ�����������ٶ�9M
/*----------------------------------------------------------------------------------------------------------------------*/							
	
	NRF_CS = 1;            //SPIƬѡȡ��
	NRF_CE = 0;            //ʹ��24L01
}


//����ֵ�����ɹ����� 1		ʧ�ܷ��� 0
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


//reg:ָ���Ĵ�����ַ
//val:Ҫд���ֵ
void NRF_Write_Reg(u8 reg,u8 val)
{
	NRF_CS = 0;
	SPI_Write_Read(SPI2, 0x20+reg);
	SPI_Write_Read(SPI2, val);
	NRF_CS = 1;
}

//reg:ָ��Ҫ���ļĴ�����ַ
u8 NRF_Read_Reg(u8 reg)
{
	u8 dat;
	NRF_CS = 0;
	SPI_Write_Read(SPI2, 0x00+reg);
	dat = SPI_Write_Read(SPI2, 0xff);
	NRF_CS = 1;
	
	return dat;
}

//��ָ���ص������
//cmd:Ϊ�Ĵ�����ַ���������ָ��
//buff�����ݴ�ŵ�ָ��
//len:��ȡ���ݵĳ���
void NRF_Read_Buff(u8 cmd,u8 *buff, u8 len)
{
	u8 i;
	
	NRF_CS = 0;
	SPI_Write_Read(SPI2, cmd);
	for(i=0;i<len;i++)
		buff[i] = SPI_Write_Read(SPI2, 0xff);
	
	NRF_CS = 1;
}


//��ָ���ص�д����
//cmd:Ϊ�Ĵ�����ַ��д������ָ��
//buff�����ݴ�ŵ�ָ��
//len:д�����ݵĳ���
void NRF_Write_Buff(u8 cmd,u8 *buff, u8 len)
{
	u8 i;
	
	NRF_CS = 0;
	SPI_Write_Read(SPI2, cmd);
	for(i=0;i<len;i++)
		SPI_Write_Read(SPI2, *buff++);
	
	NRF_CS = 1;
}

//����Ƿ������ݰ�����������н�����װ��Packet_Buf��
//ÿ��ͨ��ÿ�ν��յĳ��ȹ̶�����NRF24L01�ڲ��Ĵ���(RX_PW_P0 - RX_PW_P5)����
//�����ݰ����� 1		û���ݰ����� 0
u8 NRF_RX_Packet(u8 *Packet_Buf)
{
	u8 sta;
	sta = NRF_Read_Reg(STATUS);
	NRF_Write_Reg(STATUS,sta);
	if(sta&0x40)
	{
		NRF_Read_Buff(R_RX_PAYLOAD,Packet_Buf,1);      //�ӻ�������ȡ1���ֽ�
		NRF_CS = 0;
		SPI_Write_Read(SPI2, FLUSH_RX);
		SPI_Write_Read(SPI2, 0xff);
		NRF_CS = 1;
		return 1;
	}
	else
		return 0;
}

//����һ�����ݰ������ݰ�����Ϊ1-32�ֽ�
//���ͳɹ����� TX_OK    ʧ�ܷ��� 0
//TX_Len:�������ݰ�����(��λ�ֽ�)����Ҫ���ݽ��շ��Ľ��հ���С���趨
u8 NRF_TX_Packet(u8 *Packet_Buf, u8 TX_Len)
{
	u8 sta;
	NRF_CE = 0;
	NRF_Write_Buff(W_TX_PAYLOAD,Packet_Buf,TX_Len);      		//�򻺳���д1���ֽ�
	NRF_CE = 1;												//��������
	while(NRF_IRQ!=0);										//�ȴ��������
	sta = NRF_Read_Reg(STATUS);
	NRF_Write_Reg(STATUS,sta);
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF_CS = 0;
		SPI_Write_Read(SPI2, FLUSH_TX);							//���TX FIFO�Ĵ��� 
		SPI_Write_Read(SPI2, 0xff);
		NRF_CS = 1;
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}


//����NRF24L01Ϊ����ģʽ
void NRF_TX_MODE()
{
	NRF_CE=0;    
	
  	NRF_Write_Buff(0x20+TX_ADDR,(u8*)SET_TX_ADDR,5);    	//дTX�ڵ��ַ 
  	NRF_Write_Buff(0x20+RX_ADDR_P0,(u8*)CH0_RX_ADDR,5); 	//����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF_Write_Reg(EN_AA,0x01);     					//ʹ��ͨ��0���Զ�Ӧ��    
  	NRF_Write_Reg(EN_RXADDR,0x01);    				//ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF_Write_Reg(SETUP_RETR,0x1a);   				//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF_Write_Reg(RF_CH,40);                        //����RFͨ��Ϊ40
  	NRF_Write_Reg(RF_SETUP,0x0f);			        //����TX�������,0db����,2Mbps,���������濪��    
  	NRF_Write_Reg(CONFIG_REG, 0x0e);    			//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	
	NRF_CE=1;//CEΪ��,10us����������
	
}

//����NRF24L01Ϊ����ģʽ
//RX_Len:ѡ��ͨ��0����Ч���ݿ��(0-32�ֽ�)
void NRF_RX_MODE(u8 RX_Len)
{
	NRF_CE = 0;
	
	NRF_Write_Buff(0x20+RX_ADDR_P0,(u8 *)CH0_RX_ADDR,5);
	NRF_Write_Reg(EN_AA,0x01);         				//ʹ��ͨ��0���Զ�Ӧ��    
  	NRF_Write_Reg(EN_RXADDR,0x01); 					//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF_Write_Reg(RF_CH,40);	     		 		//����RFͨ��Ƶ�� (2400+40)M =2.44GHZ		  
  	NRF_Write_Reg(RX_PW_P0,RX_Len);						//ѡ��ͨ��0����Ч���ݿ��: RX_Len    
  	NRF_Write_Reg(RF_SETUP,0x0f);			        //����TX�������,0db����,2Mbps,���������濪��   
  	NRF_Write_Reg(CONFIG_REG, 0x0f);		        //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	
	
	NRF_CE = 1;     //CEΪ��,�������ģʽ 
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
