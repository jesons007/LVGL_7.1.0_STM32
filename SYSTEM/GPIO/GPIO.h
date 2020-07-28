#ifndef _GPIO_H_
#define _GPIO_H_
#include "stm32f10x.h"

/*GPIO常用的5种模式*/
#define OUTPUT_PP 		0X3     //推挽输出
#define OUTPUT_FF 		0XB     //复用推挽
#define INPUT_UP  		0X9     //上拉输入
#define INPUT_DOWN		0X8     //下拉输入
#define INPUT_ANALOG    0X0	    //模拟输入

//////////////////////////////------------GPIO Fast operate------------///////////////////////////////////
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define  PAout(n)   *((volatile unsigned long  *)(0X42210180+4*n))
#define  PBout(n)   *((volatile unsigned long  *)(0X42218180+4*n))
#define  PCout(n)   *((volatile unsigned long  *)(0X42220180+4*n))
#define  PDout(n)   *((volatile unsigned long  *)(0X42228180+4*n))
#define  PEout(n)   *((volatile unsigned long  *)(0X42230180+4*n))
#define  PFout(n)   *((volatile unsigned long  *)(0X42238180+4*n))
#define  PGout(n)   *((volatile unsigned long  *)(0X42240180+4*n))


#define  PAin(n)    *((volatile unsigned long  *)(0X42210100+4*n))
#define  PBin(n)    *((volatile unsigned long  *)(0X42218100+4*n))
#define  PCin(n)    *((volatile unsigned long  *)(0X42220100+4*n))
#define  PDin(n)    *((volatile unsigned long  *)(0X42228100+4*n))
#define  PEin(n)    *((volatile unsigned long  *)(0X42230100+4*n))
#define  PFin(n)    *((volatile unsigned long  *)(0X42238100+4*n))
#define  PGin(n)    *((volatile unsigned long  *)(0X42240100+4*n))
/////////////////////////////////////////////////////////////////

//GPIO引脚编号定义
#define GPIO_PIN_0				1<<0
#define GPIO_PIN_1				1<<1
#define GPIO_PIN_2				1<<2
#define GPIO_PIN_3				1<<3
#define GPIO_PIN_4				1<<4
#define GPIO_PIN_5				1<<5
#define GPIO_PIN_6				1<<6
#define GPIO_PIN_7				1<<7
#define GPIO_PIN_8				1<<8
#define GPIO_PIN_9				1<<9
#define GPIO_PIN_10				1<<10
#define GPIO_PIN_11				1<<11
#define GPIO_PIN_12				1<<12
#define GPIO_PIN_13				1<<13
#define GPIO_PIN_14				1<<14
#define GPIO_PIN_15				1<<15 
#define GPIO_PIN_ALL            (0XFFFF)
#define GPIO_PIN_LSB            (0X00FF)
#define GPIO_PIN_MSB            (0XFF00)

void GPIO_Init(GPIO_TypeDef *GPIOx, u16 GPIO_PIN, u8 GPIO_MODE);
void GPIO_Set(GPIO_TypeDef *GPIOx, u16 GPIO_PIN);
void GPIO_Rset(GPIO_TypeDef *GPIOx, u16 GPIO_PIN);

void GPIO_WRITE(GPIO_TypeDef *GPIOx,u16 data);//一次设置16个io口的输出
u16 GPIO_READ(GPIO_TypeDef *GPIOx);          //一次读出16个io口的输入

void GPIO_PIN_Init(u8 PIN, u8 Mode);

#endif


