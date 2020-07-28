#ifndef _MyUart_H_
#define _MyUart_H_
#include "stm32f10x.h"
#include "GPIO.h"


void myuart_init(USART_TypeDef *USARTx, u32 baud, u8 RX_PIN, u8 TX_PIN);
void uart_send_byte(USART_TypeDef *USARTx, u8 data);
void uart_send_str(USART_TypeDef *USARTx, u8 *buff);
void uart_send_data(USART_TypeDef *USARTx, u8 *buff, u16 send_len);
void Uart_Rx_IRQENR(USART_TypeDef *USARTx, u8 enr);


#endif
