#include "gt9147.h"
#include "GPIO.h"
#include "delay.h"
#include "string.h"
/*
	while(gt9147_init())
	{
		LCD_show_str(0,0,$STR"GT9147 init error!",BLACK,BKOR);
		delay_ms(200);
		LCD_show_str(0,0,$STR"                  ",BLACK,BKOR);
		delay_ms(200);
	}
	LCD_show_str(0,0,    $STR"GT9147 init ok   !",BLACK,BKOR);
	Tp_NumPad_Init(1);

	
	u8 i=0;
	while(1)
	{
		delay_ms(10);
		Tp_dev.scan();
		i++;
		if(i>=20)
		{
			i=0;
			LED1 = !LED1;
			//printf("system running!\r\n");
		}
	}





	u8 last_Tp_dev_sta = 0x00;
	u8 tem[8];

	while(1)
	{
		key_moniter();
		GT9147_Scan();
		// lv_task_handler();
		if(Tp_dev.sta!=last_Tp_dev_sta)
		{
			ToBinary(Tp_dev.sta,tem);
			printf("Tp_dev.sta: %s\r\n",tem);
			last_Tp_dev_sta = Tp_dev.sta;
		}
	}
} 

void ToBinary(u8 n,u8 *p)
{
	for(u8 mask=0x80;mask!=0;mask>>=1)
	{
		*p = (mask&n)==0?'0':'1';
		p++;
	}
}
*/


//GT9147配置参数表
//第一个字节为版本号(0X60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
const u8 GT9147_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
};  
//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
u8 GT9147_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
	buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//计算校验和
    buf[0]=(~buf[0])+1;
	GT9147_Writ_REG(GT_CFGS_REG,(u8*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
	GT9147_Writ_REG(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
	return 0;
} 


//返回值:0,初始化成功;1,初始化失败 
u8 Gt9147_init()
{
    u8 temp[5];
    GPIO_PIN_Init(PF(11), OUTPUT_PP);
    GPIO_PIN_Init(PF(10),  INPUT_UP);
    GPIO_Set(GPIOF, GPIO_PIN_10|GPIO_PIN_11);
    CT_IIC_Init();
    
    GT_RST = 0; //复位
    delay_ms(10);
    GT_RST = 1;    
    delay_ms(10); //使用 0X14 作为地址在此时确定
    GPIO_PIN_Init(PF(10),  INPUT_DOWN);
    delay_ms(100);

    GT9147_Read_REG(GT_PID_REG, temp, 4);       //读取产品id
    temp[4] = '\0';
    //printf("CTP ID:%s\r\n",temp);		        //打印ID
    if(strcmp((char*)temp, "9147")==0)
    {       //初始化成功
        temp[0] = 0x02;
        GT9147_Writ_REG(GT_CTRL_REG, temp, 1);  //软复位
        GT9147_Read_REG(GT_CFGS_REG, temp, 1);  //读取GT_CFGS_REG寄存器
        //printf("Now Ver:%d\r\n",temp[0]);
        if(temp[0]<96)                        //默认版本比较低,需要更新flash配置
		{
			//printf("Default Ver:%d\r\n",temp[0]);
			GT9147_Send_Cfg(1);//更新并保存配置
		}
		delay_ms(10);
		temp[0]=0X00;	 
		GT9147_Writ_REG(GT_CTRL_REG,temp,1);	//结束复位   	
		return 0;
    }
    return 1;
}


//向GT9147写入一次数据
//REG_addr:起始寄存器地址
//buff:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 GT9147_Writ_REG(u16 REG_addr, u8 *buff, u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(REG_addr>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(REG_addr&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buff[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}


//从GT9147读出一次数据
//REG_addr:起始寄存器地址
//buff:数据缓缓存区
//len:读数据长度
void GT9147_Read_REG(u16 REG_addr, u8 *buff, u8 len)
{
    u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(REG_addr>>8);   	//发送高8位地址
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(REG_addr&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buff[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件    
}

const u16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};  //触摸点坐标寄存器表


#include "Touch.h"
//返回0 :没有按下，1 :有按下
u8 GT9147_Scan()
{
	u8 i;
	u8 mode;
	u8 temp=0;
	u8 pbuff[4];   

	GT9147_Read_REG(GT_GSTID_REG,&mode,1);	//读取触摸点的状态
	if(mode&0X80&&((mode&0XF)<6))
	{
		temp=0;
		GT9147_Writ_REG(GT_GSTID_REG,&temp,1);  //清除标志
	}
	if((mode&0XF)&&((mode&0XF)<6))     //有触摸点按下
	{
		temp = 0xFF<<(mode&0xF);
		Tp_dev.sta=(~temp)|0x80;

		for(i=0;i<5;i++)        //遍历更新当前被按下的坐标点
		{
			if(Tp_dev.sta & (1<<i))
			{
				GT9147_Read_REG(GT9147_TPX_TBL[i],pbuff,4);	//读取XY坐标值
				Tp_dev.X[i]=((u16)pbuff[1]<<8)+pbuff[0];
				Tp_dev.Y[i]=(((u16)pbuff[3]<<8)+pbuff[2]);  //更新当前坐标值
				// printf("x[%d]:%d, y[%d]:%d\r\n",i,Tp_dev.X[i],i,Tp_dev.Y[i]);
			}
		}
	}
	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		Tp_dev.sta = 0; 
	}
	
	return 0;
}
