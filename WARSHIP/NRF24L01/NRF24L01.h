#ifndef _24l01_H_
#define _24l01_H_
#include "sys.h"


#define MAX_TX  0X10
#define TX_OK   0X20

/* ------------NRF24L01ָ��--------------*/
#define R_RX_PAYLOAD  0X61          					//��RX������
#define W_TX_PAYLOAD  0XA0								//дTX������
#define FLUSH_TX      0XE1								//���TX������
#define FLUSH_RX      0XE2 								//���RX������


/* ------------NRF24L01�Ĵ���--------------*/
#define CONFIG_REG    0X00									//���üĴ���
#define TX_ADDR       0X10  								//���ݷ��͵�ַ
#define STATUS        0X07									//״̬�Ĵ�����ַ

#define RX_ADDR_P0    0x0A  							//����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1    0x0B  							//����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2    0x0C  							//����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3    0x0D  							//����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4    0x0E  							//����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5    0x0F  							//����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;

#define EN_RXADDR     0x02								//����ͨ��ʹ�ܼĴ���
#define EN_AA         0x01								//�Զ�Ӧ��ʹ�ܼĴ���
#define RF_CH         0x05  							//RFͨ��,bit6:0,����ͨ��Ƶ��
#define RF_SETUP      0x06                              //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define SETUP_RETR    0x04 								//�Զ��ط����üĴ���

#define RX_PW_P0      0x11  							//��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1      0x12  							//��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2      0x13  							//��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3      0x14  							//��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4      0x15  							//��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5      0x16  							//��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�


void NRF24L01_Init(void);
void NRF_Write_Reg(u8 reg,u8 val);
u8 NRF_Read_Reg(u8 reg);
void NRF_Read_Buff(u8 cmd,u8 *buff, u8 len);
void NRF_Write_Buff(u8 cmd,u8 *buff, u8 len);
u8 NRF_Check(void);
u8 NRF_RX_Packet(u8 *Packet_Buf);
u8 NRF_TX_Packet(u8 *Packet_Buf, u8 TX_Len);
void NRF_TX_MODE(void);
void NRF_RX_MODE(u8 RX_Len);


#endif
