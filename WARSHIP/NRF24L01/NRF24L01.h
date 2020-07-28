#ifndef _24l01_H_
#define _24l01_H_
#include "sys.h"


#define MAX_TX  0X10
#define TX_OK   0X20

/* ------------NRF24L01指令--------------*/
#define R_RX_PAYLOAD  0X61          					//读RX缓冲区
#define W_TX_PAYLOAD  0XA0								//写TX缓冲区
#define FLUSH_TX      0XE1								//清除TX缓冲区
#define FLUSH_RX      0XE2 								//清除RX缓冲区


/* ------------NRF24L01寄存器--------------*/
#define CONFIG_REG    0X00									//配置寄存器
#define TX_ADDR       0X10  								//数据发送地址
#define STATUS        0X07									//状态寄存器地址

#define RX_ADDR_P0    0x0A  							//数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1    0x0B  							//数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2    0x0C  							//数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3    0x0D  							//数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4    0x0E  							//数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5    0x0F  							//数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;

#define EN_RXADDR     0x02								//数据通道使能寄存器
#define EN_AA         0x01								//自动应答使能寄存器
#define RF_CH         0x05  							//RF通道,bit6:0,工作通道频率
#define RF_SETUP      0x06                              //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define SETUP_RETR    0x04 								//自动重发设置寄存器

#define RX_PW_P0      0x11  							//接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1      0x12  							//接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2      0x13  							//接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3      0x14  							//接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4      0x15  							//接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5      0x16  							//接收数据通道5有效数据宽度(1~32字节),设置为0则非法


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
