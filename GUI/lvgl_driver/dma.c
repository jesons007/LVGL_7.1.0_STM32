#include "dma.h"
#include "LCD.h"

void DMA_fsmc_Init()
{
    RCC->AHBENR|=1<<0;			//开启DMA1时钟
	delay_ms(5);				//等待DMA时钟稳定

    DMA1_Channel1->CCR  = 0X00000000;   //复位
    DMA1_Channel1->CCR |= 1<<14; 	    //存储器到存储器模式(内部RAM->FSMC_TFT, FSMC_SRAM->FSMC_TFT,必须使用储存器到储存器模式)
    DMA1_Channel1->CPAR = 0X6C000800;   //外设地址，FSMC_TFT作为外设，地址: 0X6C000800
    DMA1_Channel1->CMAR = 0X68000000;   //存储器地址，FSMC_SRAM作为储存器，地址：0X68000000
    DMA1_Channel1->CCR |=1<<4;  		//从存储器读
                                        //即数据从储存器->外设(FSMC_TFT)
    DMA1_Channel1->CNDTR=100;    	    //DMA1,传输数据量4800
    DMA1_Channel1->CCR |=0<<5;  		//非循环模式

    DMA1_Channel1->CCR |=0<<6; 		    //外设地址非增量模式
	DMA1_Channel1->CCR |=1<<7; 	 	    //存储器增量模式
	DMA1_Channel1->CCR |=1<<8; 	 	    //外设数据宽度为16位
	DMA1_Channel1->CCR |=1<<10; 		//存储器数据宽度16位
	DMA1_Channel1->CCR |=0<<12; 		//高优先级
}
//开启一次DMA传输
void DMA_Enable(u16 num)
{
	DMA1_Channel1->CCR&=~(1<<0);       //关闭DMA传输 
	DMA1_Channel1->CNDTR=num;          //DMA1,传输数据量 
	DMA1_Channel1->CCR|=1<<0;          //开启DMA传输
}	

void DMA_Fill_Color(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 *color)
{
    int Point_Num = (X1-X0+1)*(Y1-Y0+1);    //要填充的点数
    u16 i=0;

    LCD_SetWin(X0,Y0,X1,Y1);
    for(;Point_Num>0;Point_Num-=65535)
    {
        if(Point_Num<65535)
        {
            DMA1_Channel1->CCR&=~(1<<0);                    //关闭DMA传输 
            DMA1_Channel1->CMAR = (u32)(color+i*65535);     //地址
            DMA1_Channel1->CNDTR=Point_Num;                 //DMA1,传输数据量 
            DMA1_Channel1->CCR|=1<<0;                       //开启DMA传输
            while(!(DMA1->ISR&0x02));                       //等待传输结束
            DMA1->IFCR|=0x02;                               //清除结束标志
        }
        else
        {
            DMA1_Channel1->CCR&=~(1<<0);                    //关闭DMA传输 
            DMA1_Channel1->CMAR = (u32)(color+i*65535);     //地址
            DMA1_Channel1->CNDTR=65535;                     //DMA1,传输数据量 
            DMA1_Channel1->CCR|=1<<0;                       //开启DMA传输
            while(!(DMA1->ISR&0x02));                       //等待传输结束
            DMA1->IFCR|=0x02;                               //清除结束标志
            i++;
        }
        
    }
}