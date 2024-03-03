/**
  * @file    stm32.h
  * @brief   Board Support Package
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  STM32_H
#define  STM32_H


#include <time.h>
#include "config.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_utils.h"
#include "trace/trace.h"


/*******************************************************************************
*
*******************************************************************************/
#define STM32_SYSTICK_HZ                        CONFIG_MCU_SYSTICK_HZ


#define STM32_NVIC_PRIO_GENERIC                 12

#define STM32_NVIC_PRIO_EXTI9_5                 11

#define STM32_NVIC_PRIO_FMC                     15

#define STM32_NVIC_PRIO_I2C3_EV                 STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_I2C3_ER                 STM32_NVIC_PRIO_GENERIC

#define STM32_NVIC_PRIO_LTDC                    15
#define STM32_NVIC_PRIO_LTDC_ER                 15

#define STM32_NVIC_PRIO_TIM6                    STM32_NVIC_PRIO_GENERIC

#define STM32_NVIC_PRIO_USART1                  STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_USART1_DMA_XMIT         STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_USART1_DMA_RECV         STM32_NVIC_PRIO_GENERIC

//#define STM32_NVIC_PRIO_USART2                  STM32_NVIC_PRIO_GENERIC
//#define STM32_NVIC_PRIO_USART2_DMA_RX           STM32_NVIC_PRIO_GENERIC
//#define STM32_NVIC_PRIO_USART2_DMA_TX           STM32_NVIC_PRIO_GENERIC

#define STM32_NVIC_PRIO_UART4                   STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_UART4_DMA_XMIT          STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_UART4_DMA_RECV          STM32_NVIC_PRIO_GENERIC

#define STM32_NVIC_PRIO_UART7                   STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_UART7_DMA_XMIT          STM32_NVIC_PRIO_GENERIC
#define STM32_NVIC_PRIO_UART7_DMA_RECV          STM32_NVIC_PRIO_GENERIC


/*******************************************************************************
* HAL TIMEBASE
*******************************************************************************/
void    stm32fxx_hal_timebase_tim_isr( void );

/*******************************************************************************
* CLOCK
*******************************************************************************/
int     stm32_clock_init( void );


/*******************************************************************************
* CORE
*******************************************************************************/
void    stm32_core_icache_enable( void );
void    stm32_core_icache_disable( void );
void    stm32_core_dcache_enable( void );
void    stm32_core_dcache_disable( void );

/*******************************************************************************
* DEVICE UID
*******************************************************************************/
int     stm32_device_uid( uint32_t *, size_t );
int     stm32_device_uid_str( char *, size_t );

/*******************************************************************************
* FMC
*******************************************************************************/
int     stm32_fmc_init( void );
void    stm32_fmc_isr( void );

/*******************************************************************************
* LTDC
*******************************************************************************/
void    stm32_ltdc_init( void );
void    stm32_ltdc_set_address( uint32_t, uint32_t );
void    stm32_ltdc_config_layer( uint32_t );
void    stm32_ltdc_config_lut( uint32_t *, uint32_t, uint32_t );
void    stm32_ltdc_enable( void );
void    stm32_ltdc_disable( void );
void    stm32_ltdc_layer_enable( uint32_t );
void    stm32_ltdc_layer_disable( uint32_t );
void    stm32_ltdc_reload_config( void );
void    stm32_ltdc_set_window_pos( int, int, uint32_t );
void    stm32_ltdc_set_alpha( int, uint32_t );
void    stm32_ltdc_enable_color_keying( uint32_t );
void    stm32_ltdc_disable_color_keying( uint32_t );
void    stm32_ltdc_config_color_keying( uint32_t, uint32_t );
void    stm32_ltdc_swap_x( int );
void    stm32_ltdc_swap_y( int );


/*******************************************************************************
* MPU
*******************************************************************************/
void    stm32_mpu_disable( void );
void    stm32_mpu_enable( void );
void    stm32_mpu_init( void );


/*******************************************************************************
* RESET
*******************************************************************************/
void    stm32_reset( void );


/*******************************************************************************
* RTC
*******************************************************************************/
int     stm32_rtc_init( void );
int     stm32_rtc_get_date( uint8_t * year, uint8_t * month, uint8_t * monthday );
int     stm32_rtc_get_time( uint8_t * hours, uint8_t * minutes, uint8_t * seconds );
int     stm32_rtc_get_second( void );
int     stm32_rtc_get_minute( void );
int     stm32_rtc_get_hour( void );
int     stm32_rtc_get_day( void );
int     stm32_rtc_get_month( void );
int     stm32_rtc_get_year( void );
int     stm32_rtc_set_date( const int, const int, const int );
int     stm32_rtc_set_time( const int, const int, const int );
time_t  stm32_rtc_get_timestamp( void );
void    stm32_rtc_bckp_put( size_t, const uint32_t );
uint32_t stm32_rtc_bckp_get( size_t );


/*******************************************************************************
* TIM5
*******************************************************************************/
int     stm32_tim5_init( const uint32_t );
int     stm32_tim5_pwm_start( void );
int     stm32_tim5_pwm_stop( void );

/*******************************************************************************
* USART1
*******************************************************************************/
int         stm32_usart1_init( const size_t, const size_t, const float, const char );
void        stm32_usart1_recv( void );
int         stm32_usart1_recv_dma( const uint8_t *, size_t );
int         stm32_usart1_xmit_dma( const uint8_t *, const size_t );
uint32_t    stm32_usart1_dma_xmit_remainder( void );
uint32_t    stm32_usart1_dma_recv_remainder( void );

void        stm32_usart1_isr( void );
void        stm32_usart1_dma_recv_isr( void );
void        stm32_usart1_dma_xmit_isr( void );

void        stm32_usart1_txe_hook( void );
void        stm32_usart1_tc_hook( void );
void        stm32_usart1_rxne_hook( void );
void        stm32_usart1_idle_hook( void );
void        stm32_usart1_rto_hook( void );
void        stm32_usart1_error_hook( uint32_t sts_reg );
void        stm32_usart1_dma_xmit_full_hook( void );
void        stm32_usart1_dma_xmit_half_hook( void );
void        stm32_usart1_dma_xmit_error_hook( void );
void        stm32_usart1_dma_xmit_dm_error_hook( void );
void        stm32_usart1_dma_xmit_fifo_error_hook( void );
void        stm32_usart1_dma_recv_full_hook( void );
void        stm32_usart1_dma_recv_half_hook( void );
void        stm32_usart1_dma_recv_error_hook( void );
void        stm32_usart1_dma_recv_dm_error_hook( void );
void        stm32_usart1_dma_recv_fifo_error_hook( void );

/*******************************************************************************
* UART4
*******************************************************************************/
int         stm32_uart4_init( const size_t, const size_t, const float, const char );
int         stm32_uart4_recv_dma( const uint8_t *, size_t );
int         stm32_uart4_xmit_dma( const uint8_t *, const size_t );
uint32_t    stm32_uart4_dma_xmit_remainder( void );
uint32_t    stm32_uart4_dma_recv_remainder( void );

void        stm32_uart4_isr( void );
void        stm32_uart4_dma_recv_isr( void );
void        stm32_uart4_dma_xmit_isr( void );

void        stm32_uart4_txe_hook( void );
void        stm32_uart4_tc_hook( void );
void        stm32_uart4_rxne_hook( void );
void        stm32_uart4_idle_hook( void );
void        stm32_uart4_rto_hook( void );
void        stm32_uart4_error_hook( uint32_t sts_reg );
void        stm32_uart4_dma_xmit_full_hook( void );
void        stm32_uart4_dma_xmit_half_hook( void );
void        stm32_uart4_dma_xmit_error_hook( void );
void        stm32_uart4_dma_xmit_dm_error_hook( void );
void        stm32_uart4_dma_xmit_fifo_error_hook( void );
void        stm32_uart4_dma_recv_full_hook( void );
void        stm32_uart4_dma_recv_half_hook( void );
void        stm32_uart4_dma_recv_error_hook( void );
void        stm32_uart4_dma_recv_dm_error_hook( void );
void        stm32_uart4_dma_recv_fifo_error_hook( void );

/*******************************************************************************
* UART7
*******************************************************************************/
void        stm32_uart7_deinit( void );
int         stm32_uart7_init( void );
void        stm32_uart7_config_baudrate( const size_t );
int         stm32_uart7_recv_dma( uint8_t *, size_t );
int         stm32_uart7_xmit_dma( const uint8_t *, size_t );

int         stm32_uart7_isr( void );

uint32_t    stm32_uart7_dma_rx_get_ndtr( void );
void        stm32_uart7_dma_rx_isr( void );

uint32_t    stm32_uart7_dma_tx_get_ndtr( void );
void        stm32_uart7_dma_tx_isr( void );


#endif	//STM32_H
