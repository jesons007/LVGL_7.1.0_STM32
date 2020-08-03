# LVGL 移植到STM32示例工程
- Blank_Temp分支为移植前的工程
- demo_prj分支为移植后的工程
- master分支为后续开发主分支
- 博客教程: [https://blog.csdn.net/qq_40831286/article/details/107633216](https://blog.csdn.net/qq_40831286/article/details/107633216)

# 移植过程
## 一、前言
littlevgl是一个小型开源嵌入式 GUI 库(简称LVGL)，界面精美,消耗资源小，可移植度高，支持响应式布局，全库采用纯 c 语言开发，移植上手简单。
littleVGL 的主要特性如下:
- 具有非常丰富的内置控件,像 buttons, charts, lists, sliders, images 等
- 高级图形效果：动画，反锯齿，透明度，平滑滚动
- 支持多种输入设备,像 touchpad, mouse, keyboard, encoder 等
- 支持多语言的 UTF-8 编码
- 支持多个和多种显示设备，例如同步显示在多个彩色屏或单色屏上
- 完全自定制的图形元素
- 硬件独立于任何微控制器或显示器
- 可以缩小到最小内存 (64 kB Flash, 16 kB RAM)
- 支持操作系统、外部储存和 GPU（非必须）
- 仅仅单个帧缓冲设备就可以呈现高级视觉特效
- 使用 C 编写以获得最大兼容性(兼容 C++)
- 支持 PC 模拟器
- 为加速 GUI 设计,提供教程,案例和主题,支持响应式布局
- 提供了在线和离线文档
- 基于自由和开源的 MIT 协议
效果图以及更多详细的说明请见官网
littlevgl中文官网：[https://littlevgl.cn/](https://littlevgl.cn/)
littlevgl英文官网：[https://lvgl.io/](https://lvgl.io/)
## 二、学习资料
- 官方文档：[https://docs.lvgl.io/latest/en/html/index.html](https://docs.lvgl.io/latest/en/html/index.html)(注:官网打不开的话可能会需要梯子)
* Github: [https://github.com/lvgl/lvgl](https://github.com/lvgl/lvgl)
## 三、移植前的准备工作
#### 开发环境：
- Keil MDK5  v5.28
- ARM Compiler 6.0版本或以上

关于编译器，老版本的5.x编译器也能编译，但是新的6版本的编译器在编译速度与效率上有很大的提升，并且解决了一些莫名其妙的bug，强烈建议升级到6版本的编译器。笔者使用的版本是v6.14.1。
#### 硬件：
- STM32开发板
- 屏幕一块（单色点阵屏或者是LCD彩屏都可以）

笔者使用的开发板是正点原子的战舰V3开发板，芯片是STM32F103ZET6，72M主频+512k flash+64k ram，这样的性能足够跑LVGL了，当然，你的芯片性能越高肯定越好。哦，板上还外挂了一颗1M字节的SRAM，这对提升性能很有帮助。如果你用的不是这款芯片或者这块开发板，那也没关系，笔者会详细指出移植过程中不同的芯片移植需要注意的地方。
#### 获取源码：
源码地址：[https://github.com/lvgl/lvgl](https://github.com/lvgl/lvgl)
源码有很多的发行版，不同的发行版对应着不同的LVGL版本，注意不同的版本之间是不同的，比如版本V6与版本V7之间就变化很大，具体的版本变化信息可以看它的release note，这里我以v7.1.0版本为例来移植，并且今后的学习都会使用这个版本。
![LVGL v7.1.0源码下载](https://img-blog.csdnimg.cn/20200728160558742.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
打开下载好的文件夹
![LVGL v7.1.0源码结构](https://img-blog.csdnimg.cn/2020072816091118.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
我们重点关注src/目录和lv_conf_template.h, lvgl.h 这几个文件，其它的暂时不用管。
#### 准备模板工程：
- 为你的开发板准备好一个模板工程
要求：这个模板工程至少能够驱动一块屏幕(做GUI必须得有屏幕呀= =)
如果你的屏幕带有触摸功能，那你的工程还需要有触摸屏的驱动，其实触摸屏驱动需要但非必须，没有触摸功能的话你也可以做一些展示的界面。事实上LVGL也支持键盘，鼠标，触控板等外设，因为在LVGL核心那里，它会将这些全部抽象为输入设备，它只需要从输入设备那里取得数据，并不关心你所用的是何种设备。

当然，触摸屏已经用得相当广泛，笔者使用的屏幕同样是正点原子家开发板配套的屏幕，型号：4.3英寸MCU屏，800*480分辨率，带电容触摸功能，其接口为16位并口，驱动IC：NT35510，触摸控制IC：GT9147，触摸控制接口为IIC。
因此我需要提前准备好并且测试好这些驱动，对于屏幕控制器，除了初始化API（就是函数），还需要提供填充LCD的API，像这样：
```c
LCD_Color_Fill(x1,y1,x2,y2,(u16*)color_p);
```
(x1,y1), (x2,y2)为俩坐标点，color_p为显存指针，该API要实现的功能为：将显存区域内的像素数据点，依次填充到以(x1,y1)为起点， (x2,y2)为重点的对角矩形区域内。
注意，这个API是LVGL渲染屏幕的唯一API，哈哈哈简单吧。同样LVGL内部也不关心使用的何种屏幕，他们之间仅通过一个填充色块儿的API耦合，这种高内聚、低耦合的模块儿化设计思想是非常值得我们学习借鉴的。
## 四、开始移植
#### ①修改MDK工程(默认你已经会使用keil了)
用keil mdk打开我们的模板工程，添加4个Groups:GUI_core, GUI_drv, GUI_demo, GUI_app，这些组用于添加我们的c代码到工程中，不同的组名意味着会存放不同功能的c代码。(注意：工程中的组与实际文件目录并不相同)
- GUI_core：GUI核心文件层
- GUI_drv：GUI驱动设备层，包括显示设备与输入设备
- GUI_demo：存放官方demo
- GUI_app：存放我们自己写的GUI应用
修改后的工程结构：
![修改MDK工程](https://img-blog.csdnimg.cn/20200728212410300.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
#### ②添加源文件
在项目的根路径下新建GUI目录，以及GUI_app目录，然后将准备工作第三步下载到的源码直接解压到GUI/目录下，然后把解压出来的文件夹改名为lvgl；在GUI/目录下新建lvgl_driver目录，然后：
```c
//从GUI\lvgl\examples\porting\目录拷贝：
lv_port_disp_template.c
lv_port_disp_template.h
lv_port_indev_template.c
lv_port_indev_template.h
// 到GUI/lvgl_driver/目录下并分别改名为：
lv_port_disp.c
lv_port_disp.h
lv_port_indev.c
lv_port_indev.h

//添加：
dma.c
dma.h
//这两个文件时我编写的使用DMA进行加速的代码，如果你不使用DMA请忽略
```
然后把GUI/lvgl/lv_conf_template.h拷贝到GUI/目录下并改名为lv_conf.h。
修改后的目录结构：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200728220159277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200728220213422.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200728220229874.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
#### ③添加源文件到工程中
在GUI_core组中添加以下文件夹中所有的.c文件：
```c
GUI/lvgl/src/lv_core
GUI/lvgl/src/lv_draw
GUI/lvgl/src/lv_font
GUI/lvgl/src/lv_hal
GUI/lvgl/src/lv_misc
GUI/lvgl/src/lv_themes
GUI/lvgl/src/lv_widgets
//注意不要添加 GUI/lvgl/src/lv_gpu中的文件，除非你用到了相关功能
```
在GUI_drv组中添加以下.c文件：
```c
GUI/lvgl_driver/dma.c
GUI/lvgl_driver/lv_port_disp.c
GUI/lvgl_driver/lv_port_indev.c
```
#### ④添加头文件路径
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200728232702860.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
#### ⑤修改配置文件
打开GUI/lv_conf.h
首先第十行设置为#if 1 使能整个配置文件
找到以下几个宏定义并修改
```c
#define LV_HOR_RES_MAX          (480)//定义屏幕的最大水平分辨率
#define LV_VER_RES_MAX          (800)//定义屏幕的最大垂直分辨率
//修改为你的屏幕分辨率即可

/* Color depth:
 * - 1:  1 byte per pixel
 * - 8:  RGB332
 * - 16: RGB565
 * - 32: ARGB8888
 */
#define LV_COLOR_DEPTH     16
//定义颜色深度，如果是单色屏的话就改为1

#  define LV_MEM_SIZE    (30U * 1024U)
//给LVGL分配内存的大小，至少需要2k
```
这个文件很重要，控制着整个LVGL的功能，但我们暂时就先修改这几个参数，其它参数暂时保持默认即可。等到我们深入了解了LVGL或者需要用到其它特性时再去研究修改也无妨。
然后我们需要修改GUI/lvgl_driver/目录下的lv_port_disp.c, lv_port_disp.h,lv_port_indev.c, lv_port_indev.h这4个文件，这里直接给处修改好之后的文件。
- lv_port_disp.c
```c
#if 1
#include "lv_port_disp.h"
#include "LCD.h"
#include "dma.h"

#define COLOR_BUF_SIZE (LV_HOR_RES_MAX*LV_VER_RES_MAX)
//分配到外部 1MB sram 的最起始处
static lv_color_t *color_buf;

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

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
```
- lv_port_disp.h

```c
#if 1
#ifndef LV_PORT_DISP_H
#define LV_PORT_DISP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

void lv_port_disp_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_PORT_DISP_TEMPL_H*/
#endif /*Disable/Enable content*/
```
- lv_port_indev.c

```c
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
```
- lv_port_indev.h

```c
#if 1
#ifndef LV_PORT_INDEV_H
#define LV_PORT_INDEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

void lv_port_indev_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/
#endif /*Disable/Enable content*/
```
#### ⑥完善main函数
至此，移植工作基本完成，接下来只需要完善main函数，初始化LVGL，便可放心玩耍。
修改main.c如下：
```c
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
```

#### ⑦编译并运行
 设置编译器版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729122120228.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
注意：这一步应该在移植之前就应该执行，模板工程也使用V6编译器编译通过，免得移植后再编译各种报错。这一步非强制的。

设置编译参数![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729123709495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)
C99：LVGL要求 C99 或更新的编译器,否则编译是会报错的
勾选Execute-only Code, 会自动去除未使用到的代码函数，大大减小了编译后的固件大小，建议勾选，其它默认即可。
- 编译
- 烧写运行

效果图：![在这里插入图片描述](https://img-blog.csdnimg.cn/20200729124851915.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODMxMjg2,size_16,color_FFFFFF,t_70)

## 五、总结
> 到这里移植工作已经结束，接下来我会介绍一下移植需要修改的那几个重点文件，假如你要移植到其它平台，请看这里。
- lv_port_disp.c
```c
//定义显存的大小
//我使用了刚好一整块屏幕的显存
//如果你的内存没有那么大，那么定义10行的显存也是可以的   LV_HOR_RES_MAX*10
//LV_HOR_RES_MAX，LV_VER_RES_MAX定义于lv_conf.h
#define COLOR_BUF_SIZE (LV_HOR_RES_MAX*LV_VER_RES_MAX)

static lv_color_t *color_buf;   //定义显存指针


//lv_port_disp_init函数
//显存初始化，将显存指针指向我指定的内存啊区域
//0X68000000是我板子上外挂的SRAM启始地址
//显存需要根据你自己的板子合理分配
color_buf = (lv_color_t *)0X68000000;


//把指定区域的显示缓冲区内容写入到屏幕上,你可以使用 DMA 或者其他的硬件加速器
//在后台去完成这个操作但是在完成之后,你必须得调用 lv_disp_flush_ready()
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    //把指定区域的显示缓冲区内容写入到屏幕
    //LCD_Color_Fill(area->x1,area->y1,area->x2,area->y2,(u16*)color_p);//TODO
    DMA_Fill_Color(area->x1,area->y1,area->x2,area->y2,(u16*)color_p);
    lv_disp_flush_ready(disp_drv);//最后必须得调用,通知 lvgl 库你已经 flushing 拷贝完成了
}
//DMA_Fill_Color或者LCD_Color_Fill函数需要根据你自己的平台实现
```
- lv_port_indev.c
```c
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
//Tp_dev.scan() 是触摸屏驱动层的函数
//其作用是扫描整块儿屏幕，得到触摸状态写入Tp_dev.sta中，如果有触摸点按下，得到其坐标并存入Tp_dev.X和Tp_dev.Y数组中
//因此这个函数需要根据你自己的板子和屏幕提供
```
## 六、源码
- 本篇文章中移植的源码
移植前的工程模板：[https://github.com/jesons007/LVGL_7.1.0_STM32/tree/Blank_Temp](https://github.com/jesons007/LVGL_7.1.0_STM32/tree/Blank_Temp)
移植好的代码：[https://github.com/jesons007/LVGL_7.1.0_STM32](https://github.com/jesons007/LVGL_7.1.0_STM32)
