/*
2019/10/6:新增函数void LCD_Show_OneChar(u16 x,u16 y,u8 chr,u16 color,u16 Back_Ground);
显示单个字符函数

*/

#include "lcd.h"
#include "delay.h"
#include "LCD_Font.h"

_lcd_dev lcddev;

void LCD_write_reg(u16 cmd)
{
	LCD->LCD_REG = cmd;
}

void LCD_write_ram(u16 dat)
{
	LCD->LCD_RAM = dat;
}

vu16 LCD_read_ram(void)
{
	vu16 ram;
	ram = LCD->LCD_RAM;
	return ram;
}

void LCD_WriteReg(u16 reg, u16 vaul)
{
	LCD->LCD_REG = reg;
	LCD->LCD_RAM = vaul;
}

void LCD_Start_Write()
{
	LCD_write_reg(0x2c00);
}

void LCD_SetCursor(u16 x, u16 y)       //设置光标指针
{
	LCD_WriteReg(0x2a00, x>>8);
	LCD_WriteReg(0x2a01, x&0X00FF);
	
	LCD_WriteReg(0x2b00, y>>8);
	LCD_WriteReg(0x2b01, y&0X00FF);
}

void LCD_Scan_Dir_tempChange(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0X3600;
	u16 temp;  
	

	switch(dir)
	{
		case L2R_U2D://从左到右,从上到下
			regval|=(0<<7)|(0<<6)|(0<<5);
			break;
		case L2R_D2U://从左到右,从下到上
			regval|=(1<<7)|(0<<6)|(0<<5);
			break;
		case R2L_U2D://从右到左,从上到下
			regval|=(0<<7)|(1<<6)|(0<<5);
			break;
		case R2L_D2U://从右到左,从下到上
			regval|=(1<<7)|(1<<6)|(0<<5);
			break;	 
		case U2D_L2R://从上到下,从左到右
			regval|=(0<<7)|(0<<6)|(1<<5);
			break;
		case U2D_R2L://从上到下,从右到左
			regval|=(0<<7)|(1<<6)|(1<<5);
			break;
		case D2U_L2R://从下到上,从左到右
			regval|=(1<<7)|(0<<6)|(1<<5);
			break;
		case D2U_R2L://从下到上,从右到左
			regval|=(1<<7)|(1<<6)|(1<<5);
			break;	 
	}
	LCD_WriteReg(dirreg,regval);
	
	if(regval&0X20)
	{
		if(lcddev.width<lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}
	else  
	{
		if(lcddev.width>lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}  
	
	LCD_write_reg(0X2A00)  ;  	LCD_write_ram(0); 
	LCD_write_reg(0X2A00+1);	LCD_write_ram(0); 
	LCD_write_reg(0X2A00+2);	LCD_write_ram((lcddev.width-1)>>8); 
	LCD_write_reg(0X2A00+3);	LCD_write_ram((lcddev.width-1)&0XFF); 
	LCD_write_reg(0X2B00)  ;  	LCD_write_ram(0); 
	LCD_write_reg(0X2B00+1);	LCD_write_ram(0); 
	LCD_write_reg(0X2B00+2);	LCD_write_ram((lcddev.height-1)>>8); 
	LCD_write_reg(0X2B00+3);	LCD_write_ram((lcddev.height-1)&0XFF); 
		
}

void LCD_Scan_Dir(u8 dir)	//设置屏扫描方向,扫描方向不同，屏幕坐标系会随之变化
{
	u16 regval=0;
	u16 dirreg=0X3600;
	u16 temp;  
	

	switch(dir)
	{
		case L2R_U2D://从左到右,从上到下
			regval|=(0<<7)|(0<<6)|(0<<5);lcddev.dir=L2R_U2D;
			break;
		case L2R_D2U://从左到右,从下到上
			regval|=(1<<7)|(0<<6)|(0<<5);lcddev.dir=L2R_D2U;
			break;
		case R2L_U2D://从右到左,从上到下
			regval|=(0<<7)|(1<<6)|(0<<5);lcddev.dir=R2L_U2D;
			break;
		case R2L_D2U://从右到左,从下到上
			regval|=(1<<7)|(1<<6)|(0<<5);lcddev.dir=R2L_D2U;
			break;	 
		case U2D_L2R://从上到下,从左到右
			regval|=(0<<7)|(0<<6)|(1<<5);lcddev.dir=U2D_L2R;
			break;
		case U2D_R2L://从上到下,从右到左
			regval|=(0<<7)|(1<<6)|(1<<5);lcddev.dir=U2D_R2L;
			break;
		case D2U_L2R://从下到上,从左到右
			regval|=(1<<7)|(0<<6)|(1<<5);lcddev.dir=D2U_L2R;
			break;
		case D2U_R2L://从下到上,从右到左
			regval|=(1<<7)|(1<<6)|(1<<5);lcddev.dir=D2U_R2L;
			break;	 
	}
	LCD_WriteReg(dirreg,regval);
	
	if(regval&0X20)
	{
		if(lcddev.width<lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}
	else  
	{
		if(lcddev.width>lcddev.height)//交换X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}  
	
	LCD_write_reg(0X2A00)  ;  	LCD_write_ram(0); 
	LCD_write_reg(0X2A00+1);	LCD_write_ram(0); 
	LCD_write_reg(0X2A00+2);	LCD_write_ram((lcddev.width-1)>>8); 
	LCD_write_reg(0X2A00+3);	LCD_write_ram((lcddev.width-1)&0XFF); 
	LCD_write_reg(0X2B00)  ;  	LCD_write_ram(0); 
	LCD_write_reg(0X2B00+1);	LCD_write_ram(0); 
	LCD_write_reg(0X2B00+2);	LCD_write_ram((lcddev.height-1)>>8); 
	LCD_write_reg(0X2B00+3);	LCD_write_ram((lcddev.height-1)&0XFF); 
		
}

void LCD_Set_Window(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 color)
{
	u32 i,j;
	
	LCD_WriteReg(0x2a00, X0>>8);
	LCD_WriteReg(0x2a01, X0&0X00FF);
	LCD_WriteReg(0x2a02, X1>>8);
	LCD_WriteReg(0x2a03, X1&0X0FF);
	
	LCD_WriteReg(0x2b00, Y0>>8);
	LCD_WriteReg(0x2b01, Y0&0X00FF);
	LCD_WriteReg(0x2b02, Y1>>8);
	LCD_WriteReg(0x2b03, Y1&0X00FF);
	
	LCD_Start_Write();
	j=(X1-X0+1)*(Y1-Y0+1);
	for(i=0;i<j;i++)
	{
		LCD_write_ram(color);
	}
}

void LCD_Color_Fill(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 *color)
{
	u32 i,j;
	
	LCD_WriteReg(0x2a00, X0>>8);
	LCD_WriteReg(0x2a01, X0&0X00FF);
	LCD_WriteReg(0x2a02, X1>>8);
	LCD_WriteReg(0x2a03, X1&0X0FF);
	
	LCD_WriteReg(0x2b00, Y0>>8);
	LCD_WriteReg(0x2b01, Y0&0X00FF);
	LCD_WriteReg(0x2b02, Y1>>8);
	LCD_WriteReg(0x2b03, Y1&0X00FF);
	
	LCD_Start_Write();
	j=(X1-X0+1)*(Y1-Y0+1);
	for(i=0;i<j;i++)
	{
		LCD_write_ram(*color++);
	}
}

void LCD_SetWin(u16 X0, u16 Y0, u16 X1, u16 Y1)
{
	LCD_WriteReg(0x2a00, X0>>8);
	LCD_WriteReg(0x2a01, X0&0X00FF);
	LCD_WriteReg(0x2a02, X1>>8);
	LCD_WriteReg(0x2a03, X1&0X0FF);
	
	LCD_WriteReg(0x2b00, Y0>>8);
	LCD_WriteReg(0x2b01, Y0&0X00FF);
	LCD_WriteReg(0x2b02, Y1>>8);
	LCD_WriteReg(0x2b03, Y1&0X00FF);
	
	LCD_Start_Write();
}

void LCD_Draw_Point(u16 x, u16 y, u16 color)    //在给定的坐标打点
{
	LCD_WriteReg(0x2a00, x>>8);
	LCD_WriteReg(0x2a01, x&0X00FF);
	
	LCD_WriteReg(0x2b00, y>>8);
	LCD_WriteReg(0x2b01, y&0X00FF);
	LCD_Start_Write();
	LCD_write_ram(color);
}

void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出
	{  
		LCD_Draw_Point(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

void LCD_Show_OneChar(u16 x,u16 y,u8 chr,u16 color,u16 Back_Ground)
{
	LCD_Scan_Dir(U2D_L2R);
	LCD_show_char(x,y,chr,color,Back_Ground);
	LCD_Scan_Dir(L2R_U2D);
}

void LCD_show_char(u16 x,u16 y,u8 chr,u16 color,u16 Back_Ground)
{
	u8 i,j;
	u8 temp;
	u16 y0=y;
	chr -= ' ';
	LCD_SetWin(y,x,y+23,x+11);
	for(i=0;i<36;i++)
	{
		temp = LCD_asc2_2412[chr][i];
		for(j=0x80;j!=0;j>>=1)
		{
			if(temp&j)LCD->LCD_RAM = color;    //LCD_Draw_Point(x,y,color);
			else LCD->LCD_RAM = Back_Ground;         // LCD_Draw_Point(x,y,Back_Ground);
			y++;
			if((y-y0)==24)
			{
				x++;
				y = y0;
			}
			
		}
	}
}

void LCD_show_str(u16 x,u16 y,u8 *p,u16 color, u16 Back_Ground)
{
	u8 temp;
	if     (lcddev.dir==L2R_U2D)temp=U2D_L2R;
	else if(lcddev.dir==L2R_D2U)temp=D2U_L2R;
	else if(lcddev.dir==R2L_U2D)temp=U2D_R2L;
	else if(lcddev.dir==R2L_D2U)temp=D2U_R2L;
	else if(lcddev.dir==U2D_L2R)temp=L2R_U2D;
	else if(lcddev.dir==U2D_R2L)temp=R2L_U2D;
	else if(lcddev.dir==D2U_L2R)temp=L2R_D2U;
	else if(lcddev.dir==D2U_R2L)temp=R2L_D2U;
	LCD_Scan_Dir_tempChange(temp);
	while(*p!='\0')
	{
		if(x>479)
		{
			x=0;
			y+=24;
		}
		if(y>799)
		{
			x=y=0;
		}
		LCD_show_char(x,y,*p,color,Back_Ground);
		x+=12;
		p++;
	}
	LCD_Scan_Dir(lcddev.dir);
}

u16  LCD_read_id()
{
	u16 id;
	LCD_write_reg(0XDA00);
	id = LCD_read_ram();
	LCD_write_reg(0XDB00);
	id = LCD_read_ram();
	if(id==0X0080)
		id = 5510;
	else
		id = 0;
	return id;
}

void LCD_show_number(u16 x,u16 y,uint64_t num,u16 color,u16 Back_Ground,u8 chs)
{
	uint64_t n = num;
	u8 str[20];
	signed char len=0,i;
	do
	{
		len++;
		num/=10;
	}while(num!=0);
	
	for(i=len-1;i>=0;i--)
	{
		str[i]=n%10+'0';
		n/=10;
	}
	
	if(len<=chs)
	{
		while(len<chs)
		{
			str[len++] = ' ';
		}
		str[len]='\0';
	}
	else
	{
		str[chs]='\0';
	}
	
	LCD_show_str(x,y,str,color,Back_Ground);
	
}

u16  LCD_Read_Point(u16 x, u16 y)
{
	u16 R,G,RGB;
	
	LCD_WriteReg(0x2a00, x>>8);
	LCD_WriteReg(0x2a01, x&0X00FF);
	
	LCD_WriteReg(0x2b00, y>>8);
	LCD_WriteReg(0x2b01, y&0X00FF);
	
	LCD_write_reg(0X2E00);
	R=LCD_read_ram();
	
	R=LCD_read_ram();
	G=LCD_read_ram();
	RGB = (R&0XF800)|((R&0X00FC)<<3)|(G>>11);
	
	return RGB;
}

u8* HEX(u16 a)
{
	static u8 buf[7];
	u8 i;
	u16 n;
	
	buf[0]='0';
	buf[1]='X';
	for(i=5;i>=2;i--)
	{
		n=a%16;
		a/=16;
		if(n>=10)
			buf[i] = n-10+'A';
		else
			buf[i] = n+'0';
	}
	buf[6]='\0';
	
	return buf;
}

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_Draw_Point(x0+a,y0-b,color);             //5
 		LCD_Draw_Point(x0+b,y0-a,color);             //0           
		LCD_Draw_Point(x0+b,y0+a,color);             //4               
		LCD_Draw_Point(x0+a,y0+b,color);             //6 
		LCD_Draw_Point(x0-a,y0+b,color);             //1       
 		LCD_Draw_Point(x0-b,y0+a,color);             
		LCD_Draw_Point(x0-a,y0-b,color);             //2             
  		LCD_Draw_Point(x0-b,y0-a,color);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 				
//显示94x78pix的黑白图片
void LCD_Draw_9478BMP(u16 X0,u16 Y0, const u8 *buf ,u16 color, u16 back_color)
{
    u16 i;
    u16 point_num = 3666;

    LCD_SetWin(X0,Y0,X0+93,Y0+77);
    for(i=0;i<point_num;i++)
    {
        if((*buf)&0xf0)LCD_write_ram(color);
        else LCD_write_ram(back_color);

        if((*buf)&0x0f)LCD_write_ram(color);
        else LCD_write_ram(back_color);

        buf++;
    }
}

void LCD_Clear(u16 color)
{
	LCD_Set_Window(0,0,479,799,color);
}
void LCD_init(u16 color)
{

	RCC->AHBENR  |= 0X0100;     //使能FSMC时钟
	RCC->APB2ENR |= 0X0168;     //使能GPIO时钟
	
	GPIOB->CRL   &= 0XFFFFFFF0;
	GPIOB->CRL   |= 0X00000003; //PB0推挽输出
	
	GPIOD->CRL   &= 0XFF00FF00;
	GPIOD->CRL   |= 0X00BB00BB;
	GPIOD->CRH   &= 0X00FFF000;
	GPIOD->CRH   |= 0XBB000BBB; //PD复用推挽
	
	GPIOE->CRL   &= 0X0FFFFFFF;
	GPIOE->CRL   |= 0XB0000000;
	GPIOE->CRH   &= 0X00000000;
	GPIOE->CRH   |= 0XBBBBBBBB; //PE复用推挽
	
	GPIOG->CRL   &= 0XFFFFFFF0;
	GPIOG->CRL   |= 0X0000000B;
	GPIOG->CRH   &= 0XFFF0FFFF;
	GPIOG->CRH   |= 0X000B0000; //PG复用推挽


///////////////////////////////////////////////////////////////////////////
	// FSMC_Bank1->BTCR[6] = 0X00005010;
	// FSMC_Bank1->BTCR[7] = 0X00000F01;
	// FSMC_Bank1E->BWTR[6]= 0X00000301; //FSMC配置
	// FSMC_Bank1->BTCR[6] |= 0X00000001;

	//寄存器清零
	//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
	//这里我们使用NE4 ，也就对应BTCR[6],[7]。				    
	FSMC_Bank1->BTCR[6]=0X00000000;
	FSMC_Bank1->BTCR[7]=0X00000000;
	FSMC_Bank1E->BWTR[6]=0X00000000;
	//操作BCR寄存器	使用异步模式
	FSMC_Bank1->BTCR[6]|=1<<12;		//存储器写使能
	FSMC_Bank1->BTCR[6]|=1<<14;		//读写使用不同的时序
	FSMC_Bank1->BTCR[6]|=1<<4; 		//存储器数据宽度为16bit 	    
	//操作BTR寄存器	
	//读时序控制寄存器 							    
	FSMC_Bank1->BTCR[7]|=0<<28;		//模式A 	 							  	 
	FSMC_Bank1->BTCR[7]|=0<<0; 		//地址建立时间（ADDSET）为2个HCLK 1/36M=27ns(实际>200ns)	 //todo	 old 1<<0
	//因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
	FSMC_Bank1->BTCR[7]|=0X0<<8;  	//数据保存时间为16个HCLK	 	//todo old 0xf<<8 
	//写时序控制寄存器  
	FSMC_Bank1E->BWTR[6]|=0<<28; 	//模式A 	 							    
	FSMC_Bank1E->BWTR[6]|=0<<0;		//地址建立时间（ADDSET）为1个HCLK 
 	//4个HCLK（HCLK=72M）因为液晶驱动IC的写信号脉宽，最少也得50ns。72M/4=24M=55ns  	 
	FSMC_Bank1E->BWTR[6]|=1<<8; 	//数据保存时间为4个HCLK	       //todo old 3<<8     
	//使能BANK1,区域4
	FSMC_Bank1->BTCR[6]|=1<<0;		//使能BANK1，区域4

	delay_ms(50);
	
		LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x01);
		//AVDD Set AVDD 5.2V
		LCD_WriteReg(0xB000,0x0D);
		LCD_WriteReg(0xB001,0x0D);
		LCD_WriteReg(0xB002,0x0D);
		//AVDD ratio
		LCD_WriteReg(0xB600,0x34);
		LCD_WriteReg(0xB601,0x34);
		LCD_WriteReg(0xB602,0x34);
		//AVEE -5.2V
		LCD_WriteReg(0xB100,0x0D);
		LCD_WriteReg(0xB101,0x0D);
		LCD_WriteReg(0xB102,0x0D);
		//AVEE ratio
		LCD_WriteReg(0xB700,0x34);
		LCD_WriteReg(0xB701,0x34);
		LCD_WriteReg(0xB702,0x34);
		//VCL -2.5V
		LCD_WriteReg(0xB200,0x00);
		LCD_WriteReg(0xB201,0x00);
		LCD_WriteReg(0xB202,0x00);
		//VCL ratio
		LCD_WriteReg(0xB800,0x24);
		LCD_WriteReg(0xB801,0x24);
		LCD_WriteReg(0xB802,0x24);
		//VGH 15V (Free pump)
		LCD_WriteReg(0xBF00,0x01);
		LCD_WriteReg(0xB300,0x0F);
		LCD_WriteReg(0xB301,0x0F);
		LCD_WriteReg(0xB302,0x0F);
		//VGH ratio
		LCD_WriteReg(0xB900,0x34);
		LCD_WriteReg(0xB901,0x34);
		LCD_WriteReg(0xB902,0x34);
		//VGL_REG -10V
		LCD_WriteReg(0xB500,0x08);
		LCD_WriteReg(0xB501,0x08);
		LCD_WriteReg(0xB502,0x08);
		LCD_WriteReg(0xC200,0x03);
		//VGLX ratio
		LCD_WriteReg(0xBA00,0x24);
		LCD_WriteReg(0xBA01,0x24);
		LCD_WriteReg(0xBA02,0x24);
		//VGMP/VGSP 4.5V/0V
		LCD_WriteReg(0xBC00,0x00);
		LCD_WriteReg(0xBC01,0x78);
		LCD_WriteReg(0xBC02,0x00);
		//VGMN/VGSN -4.5V/0V
		LCD_WriteReg(0xBD00,0x00);
		LCD_WriteReg(0xBD01,0x78);
		LCD_WriteReg(0xBD02,0x00);
		//VCOM
		LCD_WriteReg(0xBE00,0x00);
		LCD_WriteReg(0xBE01,0x64);
		//Gamma Setting
		LCD_WriteReg(0xD100,0x00);
		LCD_WriteReg(0xD101,0x33);
		LCD_WriteReg(0xD102,0x00);
		LCD_WriteReg(0xD103,0x34);
		LCD_WriteReg(0xD104,0x00);
		LCD_WriteReg(0xD105,0x3A);
		LCD_WriteReg(0xD106,0x00);
		LCD_WriteReg(0xD107,0x4A);
		LCD_WriteReg(0xD108,0x00);
		LCD_WriteReg(0xD109,0x5C);
		LCD_WriteReg(0xD10A,0x00);
		LCD_WriteReg(0xD10B,0x81);
		LCD_WriteReg(0xD10C,0x00);
		LCD_WriteReg(0xD10D,0xA6);
		LCD_WriteReg(0xD10E,0x00);
		LCD_WriteReg(0xD10F,0xE5);
		LCD_WriteReg(0xD110,0x01);
		LCD_WriteReg(0xD111,0x13);
		LCD_WriteReg(0xD112,0x01);
		LCD_WriteReg(0xD113,0x54);
		LCD_WriteReg(0xD114,0x01);
		LCD_WriteReg(0xD115,0x82);
		LCD_WriteReg(0xD116,0x01);
		LCD_WriteReg(0xD117,0xCA);
		LCD_WriteReg(0xD118,0x02);
		LCD_WriteReg(0xD119,0x00);
		LCD_WriteReg(0xD11A,0x02);
		LCD_WriteReg(0xD11B,0x01);
		LCD_WriteReg(0xD11C,0x02);
		LCD_WriteReg(0xD11D,0x34);
		LCD_WriteReg(0xD11E,0x02);
		LCD_WriteReg(0xD11F,0x67);
		LCD_WriteReg(0xD120,0x02);
		LCD_WriteReg(0xD121,0x84);
		LCD_WriteReg(0xD122,0x02);
		LCD_WriteReg(0xD123,0xA4);
		LCD_WriteReg(0xD124,0x02);
		LCD_WriteReg(0xD125,0xB7);
		LCD_WriteReg(0xD126,0x02);
		LCD_WriteReg(0xD127,0xCF);
		LCD_WriteReg(0xD128,0x02);
		LCD_WriteReg(0xD129,0xDE);
		LCD_WriteReg(0xD12A,0x02);
		LCD_WriteReg(0xD12B,0xF2);
		LCD_WriteReg(0xD12C,0x02);
		LCD_WriteReg(0xD12D,0xFE);
		LCD_WriteReg(0xD12E,0x03);
		LCD_WriteReg(0xD12F,0x10);
		LCD_WriteReg(0xD130,0x03);
		LCD_WriteReg(0xD131,0x33);
		LCD_WriteReg(0xD132,0x03);
		LCD_WriteReg(0xD133,0x6D);
		LCD_WriteReg(0xD200,0x00);
		LCD_WriteReg(0xD201,0x33);
		LCD_WriteReg(0xD202,0x00);
		LCD_WriteReg(0xD203,0x34);
		LCD_WriteReg(0xD204,0x00);
		LCD_WriteReg(0xD205,0x3A);
		LCD_WriteReg(0xD206,0x00);
		LCD_WriteReg(0xD207,0x4A);
		LCD_WriteReg(0xD208,0x00);
		LCD_WriteReg(0xD209,0x5C);
		LCD_WriteReg(0xD20A,0x00);

		LCD_WriteReg(0xD20B,0x81);
		LCD_WriteReg(0xD20C,0x00);
		LCD_WriteReg(0xD20D,0xA6);
		LCD_WriteReg(0xD20E,0x00);
		LCD_WriteReg(0xD20F,0xE5);
		LCD_WriteReg(0xD210,0x01);
		LCD_WriteReg(0xD211,0x13);
		LCD_WriteReg(0xD212,0x01);
		LCD_WriteReg(0xD213,0x54);
		LCD_WriteReg(0xD214,0x01);
		LCD_WriteReg(0xD215,0x82);
		LCD_WriteReg(0xD216,0x01);
		LCD_WriteReg(0xD217,0xCA);
		LCD_WriteReg(0xD218,0x02);
		LCD_WriteReg(0xD219,0x00);
		LCD_WriteReg(0xD21A,0x02);
		LCD_WriteReg(0xD21B,0x01);
		LCD_WriteReg(0xD21C,0x02);
		LCD_WriteReg(0xD21D,0x34);
		LCD_WriteReg(0xD21E,0x02);
		LCD_WriteReg(0xD21F,0x67);
		LCD_WriteReg(0xD220,0x02);
		LCD_WriteReg(0xD221,0x84);
		LCD_WriteReg(0xD222,0x02);
		LCD_WriteReg(0xD223,0xA4);
		LCD_WriteReg(0xD224,0x02);
		LCD_WriteReg(0xD225,0xB7);
		LCD_WriteReg(0xD226,0x02);
		LCD_WriteReg(0xD227,0xCF);
		LCD_WriteReg(0xD228,0x02);
		LCD_WriteReg(0xD229,0xDE);
		LCD_WriteReg(0xD22A,0x02);
		LCD_WriteReg(0xD22B,0xF2);
		LCD_WriteReg(0xD22C,0x02);
		LCD_WriteReg(0xD22D,0xFE);
		LCD_WriteReg(0xD22E,0x03);
		LCD_WriteReg(0xD22F,0x10);
		LCD_WriteReg(0xD230,0x03);
		LCD_WriteReg(0xD231,0x33);
		LCD_WriteReg(0xD232,0x03);
		LCD_WriteReg(0xD233,0x6D);
		LCD_WriteReg(0xD300,0x00);
		LCD_WriteReg(0xD301,0x33);
		LCD_WriteReg(0xD302,0x00);
		LCD_WriteReg(0xD303,0x34);
		LCD_WriteReg(0xD304,0x00);
		LCD_WriteReg(0xD305,0x3A);
		LCD_WriteReg(0xD306,0x00);
		LCD_WriteReg(0xD307,0x4A);
		LCD_WriteReg(0xD308,0x00);
		LCD_WriteReg(0xD309,0x5C);
		LCD_WriteReg(0xD30A,0x00);

		LCD_WriteReg(0xD30B,0x81);
		LCD_WriteReg(0xD30C,0x00);
		LCD_WriteReg(0xD30D,0xA6);
		LCD_WriteReg(0xD30E,0x00);
		LCD_WriteReg(0xD30F,0xE5);
		LCD_WriteReg(0xD310,0x01);
		LCD_WriteReg(0xD311,0x13);
		LCD_WriteReg(0xD312,0x01);
		LCD_WriteReg(0xD313,0x54);
		LCD_WriteReg(0xD314,0x01);
		LCD_WriteReg(0xD315,0x82);
		LCD_WriteReg(0xD316,0x01);
		LCD_WriteReg(0xD317,0xCA);
		LCD_WriteReg(0xD318,0x02);
		LCD_WriteReg(0xD319,0x00);
		LCD_WriteReg(0xD31A,0x02);
		LCD_WriteReg(0xD31B,0x01);
		LCD_WriteReg(0xD31C,0x02);
		LCD_WriteReg(0xD31D,0x34);
		LCD_WriteReg(0xD31E,0x02);
		LCD_WriteReg(0xD31F,0x67);
		LCD_WriteReg(0xD320,0x02);
		LCD_WriteReg(0xD321,0x84);
		LCD_WriteReg(0xD322,0x02);
		LCD_WriteReg(0xD323,0xA4);
		LCD_WriteReg(0xD324,0x02);
		LCD_WriteReg(0xD325,0xB7);
		LCD_WriteReg(0xD326,0x02);
		LCD_WriteReg(0xD327,0xCF);
		LCD_WriteReg(0xD328,0x02);
		LCD_WriteReg(0xD329,0xDE);
		LCD_WriteReg(0xD32A,0x02);
		LCD_WriteReg(0xD32B,0xF2);
		LCD_WriteReg(0xD32C,0x02);
		LCD_WriteReg(0xD32D,0xFE);
		LCD_WriteReg(0xD32E,0x03);
		LCD_WriteReg(0xD32F,0x10);
		LCD_WriteReg(0xD330,0x03);
		LCD_WriteReg(0xD331,0x33);
		LCD_WriteReg(0xD332,0x03);
		LCD_WriteReg(0xD333,0x6D);
		LCD_WriteReg(0xD400,0x00);
		LCD_WriteReg(0xD401,0x33);
		LCD_WriteReg(0xD402,0x00);
		LCD_WriteReg(0xD403,0x34);
		LCD_WriteReg(0xD404,0x00);
		LCD_WriteReg(0xD405,0x3A);
		LCD_WriteReg(0xD406,0x00);
		LCD_WriteReg(0xD407,0x4A);
		LCD_WriteReg(0xD408,0x00);
		LCD_WriteReg(0xD409,0x5C);
		LCD_WriteReg(0xD40A,0x00);
		LCD_WriteReg(0xD40B,0x81);

		LCD_WriteReg(0xD40C,0x00);
		LCD_WriteReg(0xD40D,0xA6);
		LCD_WriteReg(0xD40E,0x00);
		LCD_WriteReg(0xD40F,0xE5);
		LCD_WriteReg(0xD410,0x01);
		LCD_WriteReg(0xD411,0x13);
		LCD_WriteReg(0xD412,0x01);
		LCD_WriteReg(0xD413,0x54);
		LCD_WriteReg(0xD414,0x01);
		LCD_WriteReg(0xD415,0x82);
		LCD_WriteReg(0xD416,0x01);
		LCD_WriteReg(0xD417,0xCA);
		LCD_WriteReg(0xD418,0x02);
		LCD_WriteReg(0xD419,0x00);
		LCD_WriteReg(0xD41A,0x02);
		LCD_WriteReg(0xD41B,0x01);
		LCD_WriteReg(0xD41C,0x02);
		LCD_WriteReg(0xD41D,0x34);
		LCD_WriteReg(0xD41E,0x02);
		LCD_WriteReg(0xD41F,0x67);
		LCD_WriteReg(0xD420,0x02);
		LCD_WriteReg(0xD421,0x84);
		LCD_WriteReg(0xD422,0x02);
		LCD_WriteReg(0xD423,0xA4);
		LCD_WriteReg(0xD424,0x02);
		LCD_WriteReg(0xD425,0xB7);
		LCD_WriteReg(0xD426,0x02);
		LCD_WriteReg(0xD427,0xCF);
		LCD_WriteReg(0xD428,0x02);
		LCD_WriteReg(0xD429,0xDE);
		LCD_WriteReg(0xD42A,0x02);
		LCD_WriteReg(0xD42B,0xF2);
		LCD_WriteReg(0xD42C,0x02);
		LCD_WriteReg(0xD42D,0xFE);
		LCD_WriteReg(0xD42E,0x03);
		LCD_WriteReg(0xD42F,0x10);
		LCD_WriteReg(0xD430,0x03);
		LCD_WriteReg(0xD431,0x33);
		LCD_WriteReg(0xD432,0x03);
		LCD_WriteReg(0xD433,0x6D);
		LCD_WriteReg(0xD500,0x00);
		LCD_WriteReg(0xD501,0x33);
		LCD_WriteReg(0xD502,0x00);
		LCD_WriteReg(0xD503,0x34);
		LCD_WriteReg(0xD504,0x00);
		LCD_WriteReg(0xD505,0x3A);
		LCD_WriteReg(0xD506,0x00);
		LCD_WriteReg(0xD507,0x4A);
		LCD_WriteReg(0xD508,0x00);
		LCD_WriteReg(0xD509,0x5C);
		LCD_WriteReg(0xD50A,0x00);
		LCD_WriteReg(0xD50B,0x81);

		LCD_WriteReg(0xD50C,0x00);
		LCD_WriteReg(0xD50D,0xA6);
		LCD_WriteReg(0xD50E,0x00);
		LCD_WriteReg(0xD50F,0xE5);
		LCD_WriteReg(0xD510,0x01);
		LCD_WriteReg(0xD511,0x13);
		LCD_WriteReg(0xD512,0x01);
		LCD_WriteReg(0xD513,0x54);
		LCD_WriteReg(0xD514,0x01);
		LCD_WriteReg(0xD515,0x82);
		LCD_WriteReg(0xD516,0x01);
		LCD_WriteReg(0xD517,0xCA);
		LCD_WriteReg(0xD518,0x02);
		LCD_WriteReg(0xD519,0x00);
		LCD_WriteReg(0xD51A,0x02);
		LCD_WriteReg(0xD51B,0x01);
		LCD_WriteReg(0xD51C,0x02);
		LCD_WriteReg(0xD51D,0x34);
		LCD_WriteReg(0xD51E,0x02);
		LCD_WriteReg(0xD51F,0x67);
		LCD_WriteReg(0xD520,0x02);
		LCD_WriteReg(0xD521,0x84);
		LCD_WriteReg(0xD522,0x02);
		LCD_WriteReg(0xD523,0xA4);
		LCD_WriteReg(0xD524,0x02);
		LCD_WriteReg(0xD525,0xB7);
		LCD_WriteReg(0xD526,0x02);
		LCD_WriteReg(0xD527,0xCF);
		LCD_WriteReg(0xD528,0x02);
		LCD_WriteReg(0xD529,0xDE);
		LCD_WriteReg(0xD52A,0x02);
		LCD_WriteReg(0xD52B,0xF2);
		LCD_WriteReg(0xD52C,0x02);
		LCD_WriteReg(0xD52D,0xFE);
		LCD_WriteReg(0xD52E,0x03);
		LCD_WriteReg(0xD52F,0x10);
		LCD_WriteReg(0xD530,0x03);
		LCD_WriteReg(0xD531,0x33);
		LCD_WriteReg(0xD532,0x03);
		LCD_WriteReg(0xD533,0x6D);
		LCD_WriteReg(0xD600,0x00);
		LCD_WriteReg(0xD601,0x33);
		LCD_WriteReg(0xD602,0x00);
		LCD_WriteReg(0xD603,0x34);
		LCD_WriteReg(0xD604,0x00);
		LCD_WriteReg(0xD605,0x3A);
		LCD_WriteReg(0xD606,0x00);
		LCD_WriteReg(0xD607,0x4A);
		LCD_WriteReg(0xD608,0x00);
		LCD_WriteReg(0xD609,0x5C);
		LCD_WriteReg(0xD60A,0x00);
		LCD_WriteReg(0xD60B,0x81);

		LCD_WriteReg(0xD60C,0x00);
		LCD_WriteReg(0xD60D,0xA6);
		LCD_WriteReg(0xD60E,0x00);
		LCD_WriteReg(0xD60F,0xE5);
		LCD_WriteReg(0xD610,0x01);
		LCD_WriteReg(0xD611,0x13);
		LCD_WriteReg(0xD612,0x01);
		LCD_WriteReg(0xD613,0x54);
		LCD_WriteReg(0xD614,0x01);
		LCD_WriteReg(0xD615,0x82);
		LCD_WriteReg(0xD616,0x01);
		LCD_WriteReg(0xD617,0xCA);
		LCD_WriteReg(0xD618,0x02);
		LCD_WriteReg(0xD619,0x00);
		LCD_WriteReg(0xD61A,0x02);
		LCD_WriteReg(0xD61B,0x01);
		LCD_WriteReg(0xD61C,0x02);
		LCD_WriteReg(0xD61D,0x34);
		LCD_WriteReg(0xD61E,0x02);
		LCD_WriteReg(0xD61F,0x67);
		LCD_WriteReg(0xD620,0x02);
		LCD_WriteReg(0xD621,0x84);
		LCD_WriteReg(0xD622,0x02);
		LCD_WriteReg(0xD623,0xA4);
		LCD_WriteReg(0xD624,0x02);
		LCD_WriteReg(0xD625,0xB7);
		LCD_WriteReg(0xD626,0x02);
		LCD_WriteReg(0xD627,0xCF);
		LCD_WriteReg(0xD628,0x02);
		LCD_WriteReg(0xD629,0xDE);
		LCD_WriteReg(0xD62A,0x02);
		LCD_WriteReg(0xD62B,0xF2);
		LCD_WriteReg(0xD62C,0x02);
		LCD_WriteReg(0xD62D,0xFE);
		LCD_WriteReg(0xD62E,0x03);
		LCD_WriteReg(0xD62F,0x10);
		LCD_WriteReg(0xD630,0x03);
		LCD_WriteReg(0xD631,0x33);
		LCD_WriteReg(0xD632,0x03);
		LCD_WriteReg(0xD633,0x6D);
		//LV2 Page 0 enable
		LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x00);
		//Display control
		LCD_WriteReg(0xB100, 0xCC);
		LCD_WriteReg(0xB101, 0x00);
		//Source hold time
		LCD_WriteReg(0xB600,0x05);
		//Gate EQ control
		LCD_WriteReg(0xB700,0x70);
		LCD_WriteReg(0xB701,0x70);
		//Source EQ control (Mode 2)
		LCD_WriteReg(0xB800,0x01);
		LCD_WriteReg(0xB801,0x03);
		LCD_WriteReg(0xB802,0x03);
		LCD_WriteReg(0xB803,0x03);
		//Inversion mode (2-dot)
		LCD_WriteReg(0xBC00,0x02);
		LCD_WriteReg(0xBC01,0x00);
		LCD_WriteReg(0xBC02,0x00);
		//Timing control 4H w/ 4-delay
		LCD_WriteReg(0xC900,0xD0);
		LCD_WriteReg(0xC901,0x02);
		LCD_WriteReg(0xC902,0x50);
		LCD_WriteReg(0xC903,0x50);
		LCD_WriteReg(0xC904,0x50);
		LCD_WriteReg(0x3500,0x00);
		LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
		LCD_write_reg(0x1100);
		delay_us(120);
		LCD_write_reg(0x2900);
		///////////////////////
		LCD_WriteReg(0x3600,0x0000);
		
		LCD_LED = 1;
		LCD_Clear(color);
		lcddev.width  = 480;
		lcddev.height = 800; 
		lcddev.dir    = L2R_U2D;     //默认竖屏
		lcddev.id     = 0x5510;
}


/*
Func: 显示图片
x0,y0 :图片起始点坐标
width,height :分辨率（宽X高 = width X height）
imgbuf :图片取模指针
*/
void LCD_Draw_Img(u16 x0,u16 y0, u16 width,u16 height, u8 *imgbuf)
{
	int i,j=0;
	LCD_SetWin(x0,y0,x0+width-1,y0+height-1);

	for(i=0;i<width*height;i++)
	{
		LCD->LCD_RAM = (((u16)imgbuf[j+1])<<8) + imgbuf[j];
		j+=2;
	}

}

