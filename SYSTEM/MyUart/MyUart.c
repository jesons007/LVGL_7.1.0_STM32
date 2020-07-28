#include "MyUart.h"

//USART1
//USART2
//USART3
//UART4
//UART5

/*
Ĭ�����ã�
����λ:8λ
ֹͣλ:1λ
��żУ��λ����
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
		RCC->APB2ENR |= 1<<14;    //ʹ��UART1ʱ��
		RCC->APB2RSTR |= 1<<14;   //��λUART1
		RCC->APB2RSTR &= ~(1<<14);
		n = USART1_IRQn;
	}
	else
	{
		RCC->APB1ENR |= 1<<(((u32)USARTx-(u32)USART2)/1024 + 17);   //ʹ��UARTxʱ��
		RCC->APB1RSTR |= 1<<(((u32)USARTx-(u32)USART2)/1024 + 17);
		RCC->APB1RSTR &= ~(1<<(((u32)USARTx-(u32)USART2)/1024 + 17));
		if((u32)USARTx<=(u32)USART3)
			n = ((u32)USARTx-(u32)USART2)/1024 + 38;
		else
			n = ((u32)USARTx-(u32)UART4)/1024 + 52;
	}
	
	temp=(float)(pclk*1000000)/(baud*16);	//�õ�USARTDIV
	mantissa=temp;				 			//�õ���������
	fraction=(temp-mantissa)*16; 			//�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	USARTx->BRR  = mantissa; //���ò�����
	USARTx->CR1 |= 0X000C;	 //Ĭ�ϲ�ʹ�ܽ����ж�
	USARTx->CR2 &= 0XCFFF;
	
	
	NVIC->IP[n]  = 2<<4;    //���ȼ�2
	NVIC->ISER[n/32] |= 1<<(n%32);
	
	USARTx->CR1 |= 0X2000;//ʹ��UART
	GPIO_PIN_Init(TX_PIN,OUTPUT_FF); //TX
}

void uart_send_byte(USART_TypeDef *USARTx, u8 data)				//����һ���ֽ�
{      
	while((USARTx->SR&0X40)==0);//�ȴ���һ�δ������ݷ������
	USARTx->DR = data;      	//дDR,���ڽ���������
	
}

void uart_send_str(USART_TypeDef *USARTx, u8 *buff)               //�����ַ���
{
	while((*buff)!='\0')
	{
		uart_send_byte(USARTx,*buff);
		buff++;
	}
}

void uart_send_data(USART_TypeDef *USARTx, u8 *buff, u16 send_len) //����ָ�����ȵ�����
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

void Uart_Rx_IRQENR(USART_TypeDef *USARTx, u8 enr)   //�����ж�ʹ�ܿ���
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
