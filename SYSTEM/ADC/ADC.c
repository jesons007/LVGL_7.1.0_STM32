#include "ADC.h"
#include "delay.h"

/*------------ADC��ʹ��˵��----------------*/
//ADC������������ADC1 ADC2 ADC3����С��������ADC1&ADC2��
//ADCʱ����Դ��ΪAPB2���ߣ�ADCʱ�Ӿ�Ĭ����Ϊ6��Ƶ����12M��
//ADCͨ��IO��Ӧ��ϵ����Adc_Channel.png��
/*
ADC����˵����
ADC_Init(ADC_TypeDef* ADCx, u8 Channel)����ʼ��ADCx��Ӧ��ͨ��
����˵����
	ADC_TypeDef* ADCx;ѡ��ADC����������ADC1 ADC2 ADC3��ѡ
	Channel 		 ;ѡ��ADCͨ������ADC_CHANNEL_0 ~ ADC_CHANNEL_15 ��ѡ
	
ADC_Get_Raw(ADC_TypeDef* ADCx, u8 Channel):��ȡADCͨ��ת������ֵ
����ͬ��

u16 ADC_Get_Value(ADC_TypeDef* ADCx, u8 Channel):�������˲�,ʹ����λֵ�˲��� 
����ͬ��

demo:
	RCC_APB2ENR(APB2_ADC1,ENABLE);
	GPIO_Init(GPIOA,GPIO_PIN_0,INPUT_ANALOG);
	ADC_Init(ADC1,ADC_CHANNEL_0);
	u16 vaule = ADC_Get_Raw(ADC1,ADC_CHANNEL_0);
*/

//�������˲�
//ʹ����λֵ�˲��� 
u16 ADC_Get_Value(ADC_TypeDef* ADCx, u8 Channel)
{
	u16 buf[12];  //����12�β���
	u16 temp;
	u16 sum=0;
	u8 i,j;
	
	ADCx->SQR3 |= Channel;
	for(i=0;i<11;i++)
	{
		ADCx->CR2 |= 1<<22;          //��ʼת������ͨ��
		while(!(ADCx->SR&1<<1));	 //�ȴ�ת������
		buf[i] = ADCx->DR;           //��ȡת�����
		delay_us(100);
	}
	
	for(i=0;i<11;i++)         //��������
	{
		for(j=i+1;j<12;j++)
		{
			if(buf[i]>buf[j])
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	
	return ((buf[5]+buf[6])/2);
	
}

u8 adc_reset_enr=1;
void ADC_Init(ADC_TypeDef* ADCx, u8 Channel)
{	
	RCC->CFGR |= 0X8000;    //ADCʱ��12M
	
	if(adc_reset_enr)
	{
		adc_reset_enr = 0;
		RCC->APB2RSTR |= 0x43<<9;
		RCC->APB2RSTR &= ~(0x43<<9);    //ADC1,2,3��λ
	}

	
	ADCx->CR1 &= 0XFFF0FFFF;     //����ģʽ
	ADCx->CR1 &= ~(1<<8);        //�ر�ɨ��ģʽ
	ADCx->CR2 &= ~(1<<1);        //����ת��ģʽ
	ADCx->CR2 |= 1<<20;          //ʹ���ⲿ�¼�����ת��(�������Ҳ���ⲿ�¼�)
	ADCx->CR2 |= 0X000E0000;     //ѡ����������ͨ����ת�����ⲿ�¼�Ϊ�������
	ADCx->CR2 &= ~(1<<11);       //ת�������Ҷ���
	ADCx->SQR1 &= 0XFF0FFFFF;    //����ͨ��ת�������е�ͨ����ĿΪ1
	
	//ͨ��x�Ĳ���ʱ��Ϊ239.5��+12.5��=252������= 21us
	if(Channel<=9)
		ADCx->SMPR2 |= 7<<(3*Channel);
	else
		ADCx->SMPR1 |= 7<<(3*Channel-10);
	
	ADCx->CR2 |= 1;				 //����A/Dת����
	
	ADCx->CR2 |= 1<<3;           //��λУ׼
	while(ADCx->CR2&(1<<3));     //�ȴ���λУ׼����
	
	ADCx->CR2 |= 1<<2;           //A/DУ׼
	while(ADCx->CR2&(1<<2));
	
}


u16 ADC_Get_Raw(ADC_TypeDef* ADCx, u8 Channel)
{
	ADCx->SQR3 &=0XFFFFFFE0;  //�����һ����ʼת����ͨ����
	ADCx->SQR3 |= Channel;   //���õ�һ����ʼת����ͨ����
	ADCx->CR2 |= 1<<22;          //��ʼת������ͨ��
	while(!(ADCx->SR&0x02));	 //�ȴ�ת������
	
	return ADCx->DR;
}
