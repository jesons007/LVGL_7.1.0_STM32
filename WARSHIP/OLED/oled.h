#ifndef _oled_h_
#define _oled_h_
#include "H_IIC.h"
#include "delay.h"
#include "OLED_font.h"

#define $1206 12
#define $1608 16
#define $2412 36

#define Page7 0x01
#define Page6 0x02
#define Page5 0x04
#define Page4 0x08
#define Page3 0x10
#define Page2 0x20
#define Page1 0x40
#define Page0 0x80

#define OLED_REFRESH_FULL  oled_refresh(Page0|Page1|Page2|Page3|Page4|Page5|Page6|Page7,0,127)

void oled_wrcmd(u8 cmd);
void oled_wrdat(u8 dat);
void oled_init(void);
void oled_clear(u8 n);
void oled_refresh(u8 page, u8 x0, u8 x1);  //0 <= x0 <= x1 <= 127

void draw_point(u8 x, u8 y);
void clear_point(u8 x,u8 y);
void oled_show_char(u8 x,u8 y,u8 chr,u8 font);
void oled_show_str(u8 x, u8 y, u8* p,u8 font);
void oled_show_num(u8 x, u8 y, uint64_t num,u8 font,u8 chs);
void oled_show_img(u8 *img);

#endif
