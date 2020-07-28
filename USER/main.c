#include "include_config.h"
#include "sys.h"
#include "delay.h" 
#include "dma.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

void tim_lv_tick(void);
void lvgl_first_demo_start(void);
void setup()
{
	JTAG_Set(SWD_ENABLE);   //关闭JTAG,只用SWD
	delay_init(72);
	led_init();
	myuart_init(USART1,1152000,PA(10),PA(9));
	Timer_InitDef TIM_2_conf=
	{
		.TIMx = TIM2,
		.ms   = 1,
		.NVIC_Priority = 0,
		.event_handler = tim_lv_tick,
	};
	timer_init(&TIM_2_conf);
	
	LCD_init(BKOR); //默认竖屏
	if(Tp_dev.init())printf("Tp_dev failed!\r\n");
	else printf("Tp_dev success!\r\n");
	
	FSMC_SRAM_Init();
	DMA_fsmc_Init();
}

int main(void)
{			
	setup();
	printf("set up success!\r\n");

	lv_init(); //lvgl 系统初始化
	lv_port_disp_init(); //lvgl 显示接口初始化,放在 lv_init()的后面
	lv_port_indev_init(); //lvgl 输入接口初始化,放在 lv_init()的后面


	lvgl_first_demo_start();
	while(1)
	{
		lv_task_handler();
	}
}

void tim_lv_tick()
{
	lv_tick_inc(1);//lvgl 的 1ms 心跳
}


static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, NULL);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}
void lvgl_first_demo_start(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn, NULL);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/


	lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label1, "Hello world!"); 
	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_align(btn, label1, LV_ALIGN_OUT_TOP_MID, 0, -10);
}