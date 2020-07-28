#ifndef _LCD_H_
#define _LCD_H_
#include "stm32f10x.h"

#define BKOR    WHITE   //����������ɫ

typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_typedef;

#define LCD_BASE          ((u32)(0X6C000000|0X000007FE))
#define LCD               ((LCD_typedef *)LCD_BASE)
#define LCD_LED            PBout(0)    //�������

typedef struct
{
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//��ǰ��Ļ��ɨ�跽��	
	u16	wramcmd;		//��ʼдgramָ��
	u16 setxcmd;		//����x����ָ��
	u16 setycmd;		//����y����ָ�� 
}_lcd_dev;

extern _lcd_dev lcddev;

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ

//ɨ�跽����
//����ɨ�跽���ʵ�ֺ���/����, ����Ȳ���
//ɨ�跽�������Ļ(0,0)��λ���Լ�������
#define L2R_U2D  0 //������,���ϵ���(Ĭ��)
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���(����)
#define D2U_L2R  6 //���µ���,������(����)
#define D2U_R2L  7 //���µ���,���ҵ���

#define VER_SCREEN     	L2R_U2D      //����
#define HOR_SCREEN   	U2D_R2L      //����

void LCD_init(u16 color);
void LCD_write_reg(u16 cmd);
void LCD_write_ram(u16 dat);
vu16 LCD_read_ram(void);
void LCD_WriteReg(u16 reg, u16 vaul);
void LCD_Clear(u16 color);
void LCD_Set_Window(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 color);
void LCD_Color_Fill(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 *color);
void LCD_Start_Write(void);
void LCD_Draw_Point(u16 x, u16 y, u16 color);
void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_Show_OneChar(u16 x,u16 y,u8 chr,u16 color,u16 Back_Ground);
void LCD_show_char(u16 x,u16 y,u8 chr,u16 color,u16 Back_Ground);
void LCD_show_str(u16 x,u16 y,u8 *p,u16 color, u16 Back_Ground);
void LCD_show_number(u16 x,u16 y,uint64_t num,u16 color,u16 Back_Ground,u8 chs);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);
u16  LCD_read_id(void);
u16  LCD_Read_Point(u16 x, u16 y);
u8* HEX(u16 valu);

void LCD_SetCursor(u16 x, u16 y);
void LCD_Scan_Dir(u8 dir);
void LCD_Scan_Dir_tempChange(u8 dir);		
void LCD_SetWin(u16 X0, u16 Y0, u16 X1, u16 Y1);

void LCD_Draw_9478BMP(u16 X0,u16 Y0, const u8 *buf ,u16 color, u16 back_color);//��ʾ94x78pix�ĺڰ�ͼƬ
void LCD_Draw_Img(u16 x0,u16 y0, u16 width,u16 height, u8 *imgbuf);

#endif

