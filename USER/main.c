#include "include_config.h"
#include "sys.h"
#include "delay.h"	


void setup()
{
	JTAG_Set(SWD_ENABLE);   //关闭JTAG,只用SWD
	delay_init(72);
	led_init();
	myuart_init(USART1,1152000,PA(10),PA(9));
	
	LCD_init(BKOR); //默认竖屏
	if(Tp_dev.init())printf("Tp_dev failed!\r\n");
	else printf("Tp_dev success!\r\n");
	
	FSMC_SRAM_Init();
}

int main(void)
{			
	setup();

	printf("set up success!\r\n");


	while(1)
	{
		
	}
}
