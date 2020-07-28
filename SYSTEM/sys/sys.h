#ifndef __SYS_H
#define __SYS_H	  
#include <stm32f10x.h>  
#include "stdio.h"
#include "GPIO.h"

#define PRINTF_UART1 1
#define PRINTF_UART2 2

#define PRINTF_USE_UART1  PRINTF_UARTx=PRINTF_UART1
#define PRINTF_USE_UART2  PRINTF_UARTx=PRINTF_UART2

extern u8 PRINTF_UARTx;

//0,不支持OS
//1,支持OS
#define SYSTEM_SUPPORT_OS		0		//定义系统文件夹是否支持OS
																	    
#define PA(n)    (0X00+n)
#define PB(n)    (0X10+n)
#define PC(n)    (0X20+n)
#define PD(n)    (0X30+n)
#define PE(n)    (0X40+n)
#define PF(n)    (0X50+n)
#define PG(n)    (0X60+n) 

#define $STR (u8*)

/////////////////////////////////////////////////////////////////
//Ex_NVIC_Config专用定义
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发
								   

//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	

/////////////////////////////////////////////////////////////////  
void Stm32_Clock_Init(void);  //时钟初始化  
void Sys_Soft_Reset(void);      //系统软复位
void Sys_Standby(void);         //待机模式 	
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);//设置偏移地址
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);//设置NVIC分组
//void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//设置中断
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);//外部中断配置函数(只对GPIOA~G)
void JTAG_Set(u8 mode);
//////////////////////////////////////////////////////////////////////////////
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址
///////////////////////////////////////////////////////////////////////////////////////////////////////
//   NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
//   MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
//   BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
//   UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
//   SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
//   DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
//   PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
//   SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */
//   WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
//   PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
//   TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
//   RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
//   FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
//   RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
//   EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
//   EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
//   EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
//   EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
//   EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
//   DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
//   DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
//   DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
//   DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
//   DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
//   DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
//   DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */
//   ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
//   USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
//   USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
//   CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
//   CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
//   EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
//   TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
//   TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
//   TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
//   TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
//   TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
//   TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
//   TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
//   I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
//   I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
//   I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
//   I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
//   SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
//   SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
//   USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
//   USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
//   USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
//   EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
//   RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
//   USBWakeUp_IRQn              = 42,     /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
//   TIM8_BRK_IRQn               = 43,     /*!< TIM8 Break Interrupt                                 */
//   TIM8_UP_IRQn                = 44,     /*!< TIM8 Update Interrupt                                */
//   TIM8_TRG_COM_IRQn           = 45,     /*!< TIM8 Trigger and Commutation Interrupt               */
//   TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                       */
//   ADC3_IRQn                   = 47,     /*!< ADC3 global Interrupt                                */
//   FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                */
//   SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                */
//   TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
//   SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
//   UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
//   UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
//   TIM6_IRQn                   = 54,     /*!< TIM6 global Interrupt                                */
//   TIM7_IRQn                   = 55,     /*!< TIM7 global Interrupt                                */
//   DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
//   DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
//   DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
//   DMA2_Channel4_5_IRQn        = 59      /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
//默认中断分组：组4:4位抢占优先级,0位响应优先级     
//NVIC_Channel:             中断编号
//NVIC_PreemptionPriority:  优先级 0~15
void MY_NVIC_Init(u8 NVIC_Channel, u8 NVIC_PreemptionPriority);

#endif











