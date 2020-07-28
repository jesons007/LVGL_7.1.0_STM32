#include "ADC.h"
#include "delay.h"

/*------------ADC库使用说明----------------*/
//ADC控制器共三个ADC1 ADC2 ADC3，（小容量仅有ADC1&ADC2）
//ADC时钟来源均为APB2总线，ADC时钟均默认设为6分频（即12M）
//ADC通道IO对应关系见表（Adc_Channel.png）
/*
ADC函数说明：
ADC_Init(ADC_TypeDef* ADCx, u8 Channel)：初始化ADCx对应的通道
参数说明：
	ADC_TypeDef* ADCx;选择ADC控制器，有ADC1 ADC2 ADC3可选
	Channel 		 ;选择ADC通道，有ADC_CHANNEL_0 ~ ADC_CHANNEL_15 可选
	
ADC_Get_Raw(ADC_TypeDef* ADCx, u8 Channel):获取ADC通道转换的裸值
参数同上

u16 ADC_Get_Value(ADC_TypeDef* ADCx, u8 Channel):采样并滤波,使用中位值滤波法 
参数同上

demo:
	RCC_APB2ENR(APB2_ADC1,ENABLE);
	GPIO_Init(GPIOA,GPIO_PIN_0,INPUT_ANALOG);
	ADC_Init(ADC1,ADC_CHANNEL_0);
	u16 vaule = ADC_Get_Raw(ADC1,ADC_CHANNEL_0);
*/

//采样并滤波
//使用中位值滤波法 
u16 ADC_Get_Value(ADC_TypeDef* ADCx, u8 Channel)
{
	u16 buf[12];  //连续12次采样
	u16 temp;
	u16 sum=0;
	u8 i,j;
	
	ADCx->SQR3 |= Channel;
	for(i=0;i<11;i++)
	{
		ADCx->CR2 |= 1<<22;          //开始转换规则通道
		while(!(ADCx->SR&1<<1));	 //等待转换结束
		buf[i] = ADCx->DR;           //读取转换结果
		delay_us(100);
	}
	
	for(i=0;i<11;i++)         //增序排序
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
	RCC->CFGR |= 0X8000;    //ADC时钟12M
	
	if(adc_reset_enr)
	{
		adc_reset_enr = 0;
		RCC->APB2RSTR |= 0x43<<9;
		RCC->APB2RSTR &= ~(0x43<<9);    //ADC1,2,3复位
	}

	
	ADCx->CR1 &= 0XFFF0FFFF;     //独立模式
	ADCx->CR1 &= ~(1<<8);        //关闭扫描模式
	ADCx->CR2 &= ~(1<<1);        //单次转换模式
	ADCx->CR2 |= 1<<20;          //使用外部事件启动转换(软件触发也是外部事件)
	ADCx->CR2 |= 0X000E0000;     //选择启动规则通道组转换的外部事件为软件触发
	ADCx->CR2 &= ~(1<<11);       //转换数据右对齐
	ADCx->SQR1 &= 0XFF0FFFFF;    //规则通道转换序列中的通道数目为1
	
	//通道x的采样时间为239.5（+12.5）=252个周期= 21us
	if(Channel<=9)
		ADCx->SMPR2 |= 7<<(3*Channel);
	else
		ADCx->SMPR1 |= 7<<(3*Channel-10);
	
	ADCx->CR2 |= 1;				 //开启A/D转换器
	
	ADCx->CR2 |= 1<<3;           //复位校准
	while(ADCx->CR2&(1<<3));     //等待复位校准结束
	
	ADCx->CR2 |= 1<<2;           //A/D校准
	while(ADCx->CR2&(1<<2));
	
}


u16 ADC_Get_Raw(ADC_TypeDef* ADCx, u8 Channel)
{
	ADCx->SQR3 &=0XFFFFFFE0;  //清除第一个开始转换的通道号
	ADCx->SQR3 |= Channel;   //设置第一个开始转换的通道号
	ADCx->CR2 |= 1<<22;          //开始转换规则通道
	while(!(ADCx->SR&0x02));	 //等待转换结束
	
	return ADCx->DR;
}
