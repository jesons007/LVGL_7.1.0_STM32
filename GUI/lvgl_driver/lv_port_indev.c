#if 1
#include "lv_port_indev.h"
#include "Touch.h"

static bool touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

void lv_port_indev_init(void)
{
    lv_indev_drv_t indev_drv;

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

static bool touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
		static uint16_t last_x = 0;
		static uint16_t last_y = 0;
		Tp_dev.scan();
		if(Tp_dev.sta&0x80)//触摸按下了
		{
			last_x = Tp_dev.X[0];
			last_y = Tp_dev.Y[0];//竖屏
			
			data->point.x = last_x;
			data->point.y = last_y;
			data->state = LV_INDEV_STATE_PR;
		}else{
			data->point.x = last_x;
			data->point.y = last_y;
			data->state = LV_INDEV_STATE_REL;
		}
    return false;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
