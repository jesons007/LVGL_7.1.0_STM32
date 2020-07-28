#include "MyUart.h"

//USART1
//USART2
//USART3
//UART4
//UART5

/*
默认配置：
数据位:8位
停止位:1位
奇偶校验位：无
*/
void myuart_init(USART_TypeDef *USARTx, u32 baud, u8 RX_PIN, u8 TX_PIN)
{
	float temp;
	u16 mantissa;
	u16 fraction;
	u8  pclk = 36;	
	u8 n;
	
	GPIO_PIN_Init(RX_PIN,INPUT_UP);  //RX
	
	
	if(USARTx==USART1)
	{
		pclk = 72;
		RCC->APB2ENR |= 1<<14;    //使能UART1时钟
		RCC->APB2RSTR |= 1<<14;   //复位UART1
		RCC->APB2RSTR &= ~(1<<14);
		n = USART1_IRQn;
	}
	else
	{
		RCC->APB1ENR |= 1<<(((u32)USARTx-(u32)USART2)/1024 + 17);   //使能UARTx时钟
		RCC->APB1RSTR |= 1<<(((u32)USARTx-(u32)USART2)/1024 + 17);
		RCC->APB1RSTR &= ~(1<<(((u32)USARTx-(u32)USART2)/1024 + 17));
		if((u32)USARTx<=(u32)USART3)
			n = ((u32)USARTx-(u32)USART2)/1024 + 38;
		else
			n = ((u32)USARTx-(u32)UART4)/1024 + 52;
	}
	
	temp=(float)(pclk*1000000)/(baud*16);	//得到USARTDIV
	mantissa=temp;				 			//得到整数部分
	fraction=(temp-mantissa)*16; 			//得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	USARTx->BRR  = mantissa; //设置波特率
	USARTx->CR1 |= 0X000C;	 //默认不使能接收中断
	USARTx->CR2 &= 0XCFFF;
	
	
	NVIC->IP[n]  = 2<<4;    //优先级2
	NVIC->ISER[n/32] |= 1<<(n%32);
	
	USARTx->CR1 |= 0X2000;//使能UART
	GPIO_PIN_Init(TX_PIN,OUTPUT_FF); //TX
}

void uart_send_byte(USART_TypeDef *USARTx, u8 data)				//发送一个字节
{      
	while((USARTx->SR&0X40)==0);//等待上一次串口数据发送完成
	USARTx->DR = data;      	//写DR,串口将发送数据
	
}

void uart_send_str(USART_TypeDef *USARTx, u8 *buff)               //发送字符串
{
	while((*buff)!='\0')
	{
		uart_send_byte(USARTx,*buff);
		buff++;
	}
}

void uart_send_data(USART_TypeDef *USARTx, u8 *buff, u16 send_len) //发送指定长度的数据
{
	u16 i;
	for(i=0;i<send_len;i++)
		uart_send_byte(USARTx,buff[i]);
}

// void USART1_IRQHandler()
// {
// 	if(USART1->SR&(1<<5))
// 	{
// 		Usart_DR = USART1->DR;
// 		// uart_send_byte(USART1,Usart_DR);
// 	}
// }

void Uart_Rx_IRQENR(USART_TypeDef *USARTx, u8 enr)   //接收中断使能控制
{
	if(enr==ENABLE)
		USARTx->CR1 |= 1<<5;
	else
		USARTx->CR1 &= ~(1<<5);
}

//void USART3_IRQHandler()
//{
//	if(USART3->SR&(1<<5))
//	{
//		Usart_DR = USART3->DR;
//		
//	}
//}
