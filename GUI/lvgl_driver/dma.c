#include "dma.h"
#include "LCD.h"

void DMA_fsmc_Init()
{
    RCC->AHBENR|=1<<0;			//����DMA1ʱ��
	delay_ms(5);				//�ȴ�DMAʱ���ȶ�

    DMA1_Channel1->CCR  = 0X00000000;   //��λ
    DMA1_Channel1->CCR |= 1<<14; 	    //�洢�����洢��ģʽ(�ڲ�RAM->FSMC_TFT, FSMC_SRAM->FSMC_TFT,����ʹ�ô�������������ģʽ)
    DMA1_Channel1->CPAR = 0X6C000800;   //�����ַ��FSMC_TFT��Ϊ���裬��ַ: 0X6C000800
    DMA1_Channel1->CMAR = 0X68000000;   //�洢����ַ��FSMC_SRAM��Ϊ����������ַ��0X68000000
    DMA1_Channel1->CCR |=1<<4;  		//�Ӵ洢����
                                        //�����ݴӴ�����->����(FSMC_TFT)
    DMA1_Channel1->CNDTR=100;    	    //DMA1,����������4800
    DMA1_Channel1->CCR |=0<<5;  		//��ѭ��ģʽ

    DMA1_Channel1->CCR |=0<<6; 		    //�����ַ������ģʽ
	DMA1_Channel1->CCR |=1<<7; 	 	    //�洢������ģʽ
	DMA1_Channel1->CCR |=1<<8; 	 	    //�������ݿ��Ϊ16λ
	DMA1_Channel1->CCR |=1<<10; 		//�洢�����ݿ��16λ
	DMA1_Channel1->CCR |=0<<12; 		//�����ȼ�
}
//����һ��DMA����
void DMA_Enable(u16 num)
{
	DMA1_Channel1->CCR&=~(1<<0);       //�ر�DMA���� 
	DMA1_Channel1->CNDTR=num;          //DMA1,���������� 
	DMA1_Channel1->CCR|=1<<0;          //����DMA����
}	

void DMA_Fill_Color(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 *color)
{
    int Point_Num = (X1-X0+1)*(Y1-Y0+1);    //Ҫ���ĵ���
    u16 i=0;

    LCD_SetWin(X0,Y0,X1,Y1);
    for(;Point_Num>0;Point_Num-=65535)
    {
        if(Point_Num<65535)
        {
            DMA1_Channel1->CCR&=~(1<<0);                    //�ر�DMA���� 
            DMA1_Channel1->CMAR = (u32)(color+i*65535);     //��ַ
            DMA1_Channel1->CNDTR=Point_Num;                 //DMA1,���������� 
            DMA1_Channel1->CCR|=1<<0;                       //����DMA����
            while(!(DMA1->ISR&0x02));                       //�ȴ��������
            DMA1->IFCR|=0x02;                               //���������־
        }
        else
        {
            DMA1_Channel1->CCR&=~(1<<0);                    //�ر�DMA���� 
            DMA1_Channel1->CMAR = (u32)(color+i*65535);     //��ַ
            DMA1_Channel1->CNDTR=65535;                     //DMA1,���������� 
            DMA1_Channel1->CCR|=1<<0;                       //����DMA����
            while(!(DMA1->ISR&0x02));                       //�ȴ��������
            DMA1->IFCR|=0x02;                               //���������־
            i++;
        }
        
    }
}