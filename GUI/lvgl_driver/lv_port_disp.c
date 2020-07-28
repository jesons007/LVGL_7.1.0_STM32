#if 1
#include "lv_port_disp.h"
#include "LCD.h"
#include "dma.h"

#define COLOR_BUF_SIZE (LV_HOR_RES_MAX*LV_VER_RES_MAX)
//分配到外部 1MB sram 的最起始处
static lv_color_t *color_buf;

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
#if LV_USE_GPU
static void gpu_blend(lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa);
static void gpu_fill(lv_color_t * dest, uint32_t length, lv_color_t color);
#endif

void lv_port_disp_init(void)
{
    static lv_disp_buf_t disp_buf;
    color_buf = (lv_color_t *)0X68000000;//显示缓冲区初始化
    lv_disp_buf_init(&disp_buf, color_buf, NULL,COLOR_BUF_SIZE);
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    //注册显示驱动回调
    disp_drv.flush_cb = disp_flush;
    
    //注册显示缓冲区
    disp_drv.buffer = &disp_buf;
    
    //注册显示驱动到 lvgl 中
    lv_disp_drv_register(&disp_drv);
}

//把指定区域的显示缓冲区内容写入到屏幕上,你可以使用 DMA 或者其他的硬件加速器
//在后台去完成这个操作但是在完成之后,你必须得调用 lv_disp_flush_ready()
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    //把指定区域的显示缓冲区内容写入到屏幕
    //LCD_Color_Fill(area->x1,area->y1,area->x2,area->y2,(u16*)color_p);//TODO
    DMA_Fill_Color(area->x1,area->y1,area->x2,area->y2,(u16*)color_p);
    lv_disp_flush_ready(disp_drv);//最后必须得调用,通知 lvgl 库你已经 flushing 拷贝完成了
}

#else /* Enable this file at the top */
/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif