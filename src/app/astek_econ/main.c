/**
  * @file    main.c
  * @brief   Main
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>
#include <stdbool.h>
#include "stm32.h"
#include "main.h"
#include "app_pipe.h"
#include "cmsis_os.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "config.h"
#include "modbus.h"


/*******************************************************************************
*
*******************************************************************************/
extern  QueueHandle_t           que_mdbs_clnt_hndl;
extern  QueueHandle_t           que_hmi_hndl;
        uint32_t                task_stck_start[ configMINIMAL_STACK_SIZE ];
        osStaticThreadDef_t     tcb_start;
        osThreadId              task_hndl_start;
static  log_data_t              log_data[ CONFIG_LOG_DATA_SIZE ];

#pragma data_alignment = 8
econ_t          sens;

#pragma data_alignment = 8
dev_mcu_t       dev_mcu =
{
    .rtc.set_date               = stm32_rtc_set_date,
    .rtc.set_time               = stm32_rtc_set_time,
    .rtc.get_timestamp          = stm32_rtc_get_timestamp,
    .rtc.get_date               = stm32_rtc_get_date,
    .rtc.get_time               = stm32_rtc_get_time,
    .rtc.get_year               = stm32_rtc_get_year,
    .rtc.get_month              = stm32_rtc_get_month,
    .rtc.get_day                = stm32_rtc_get_day,
    .rtc.get_hour               = stm32_rtc_get_hour,
    .rtc.get_minute             = stm32_rtc_get_minute,
    .rtc.get_second             = stm32_rtc_get_second,
};

dev_mdb_relay_t mdb_relay;
dev_cloop_t cloop;

#pragma data_alignment = 8
dev_t           dev =
{
    .sens                       = &sens,
    .factory_reset              = dev_factory_reset,
    .nvm.put                    = stm32_rtc_bckp_put,
    .nvm.get                    = stm32_rtc_bckp_get,
    .safe.master.password.u08   = {0x00,0x00,0x00,0x00,},
    .mcu                        = &dev_mcu,

    .info.device_id             = CONFIG_DEVICE_ID,
    .info.hardware_id           = CONFIG_HARDWARE_ID,
    .info.hardware_str          = CONFIG_HARDWARE_STR,
    .info.device_str            = CONFIG_DEVICE_STR,    

    .info.serial_num_u32[ 0] = (uint32_t *) (UID_BASE_ADDRESS +  0),
    .info.serial_num_u32[ 1] = (uint32_t *) (UID_BASE_ADDRESS +  4),
    .info.serial_num_u32[ 2] = (uint32_t *) (UID_BASE_ADDRESS +  8),
    .info.serial_num_u32[ 3] = (uint32_t *) (UID_BASE_ADDRESS + 12),

    .info.serial_num_u16[ 0] = (uint16_t *) (UID_BASE_ADDRESS +  0),
    .info.serial_num_u16[ 1] = (uint16_t *) (UID_BASE_ADDRESS +  2),
    .info.serial_num_u16[ 2] = (uint16_t *) (UID_BASE_ADDRESS +  4),
    .info.serial_num_u16[ 3] = (uint16_t *) (UID_BASE_ADDRESS +  6),
    .info.serial_num_u16[ 4] = (uint16_t *) (UID_BASE_ADDRESS +  8),
    .info.serial_num_u16[ 5] = (uint16_t *) (UID_BASE_ADDRESS + 10),
    .info.serial_num_u16[ 6] = (uint16_t *) (UID_BASE_ADDRESS + 12),
    .info.serial_num_u16[ 7] = (uint16_t *) (UID_BASE_ADDRESS + 14),

    .cfg.display_mode       = DEV_DSPL_MODE_PERCENTS,
    .mdb_relay              = &mdb_relay,
    .cloop                  = &cloop,
};


/*******************************************************************************
* USART1 - KEYBOARD
*******************************************************************************/
void
stm32_usart1_idle_hook( void )
{
    //TRACE("stm32_usart1_idle_hook\n");
    app_pipe_t result;
    result.tag    = OS_USER_TAG_USART1_RECV_IDLE;  

    xQueueSendFromISR( que_hmi_hndl, &result, NULL );
}

void
stm32_usart1_rxne_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_USART1_RECV_RXNE; 
    
    dev.mcu->usart1.rxd = LL_USART_ReceiveData8( USART1 );

    xQueueSendFromISR( que_hmi_hndl, &result, NULL );
    //TRACE("stm32_usart1_rxne_hook: %d\n", dev.mcu->usart1.rxd );
}

/*******************************************************************************
* UART4
*******************************************************************************/
void
stm32_uart4_idle_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_UART4_RECV_IDLE; 
    
    xQueueSendFromISR( que_mdbs_clnt_hndl, &result, NULL );
}

void
stm32_uart4_rto_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_UART4_RECV_TOUT; 
  
    xQueueSendFromISR( que_mdbs_clnt_hndl, &result, NULL );
}

void
stm32_uart4_error_hook(         uint32_t sts_reg )
{
    TRACE( "UART4->ISR: %08X ORE=%c, NE=%c, FE=%c, PE=%c\n",
            sts_reg,
            sts_reg & USART_ISR_ORE ? '1' : '0',
            sts_reg & USART_ISR_NE ? '1' : '0',
            sts_reg & USART_ISR_FE ? '1' : '0',
            sts_reg & USART_ISR_PE ? '1' : '0'  );
}

void
stm32_uart4_dma_xmit_error_hook( void )
{
    TRACE("stm32_uart4_dma_xmit_error_hook\n");
}

void
stm32_uart4_dma_xmit_dm_error_hook( void )
{
    TRACE("stm32_uart4_dma_xmit_dm_error_hook\n");
}

void
stm32_uart4_dma_xmit_fifo_error_hook( void )
{
    TRACE("stm32_uart4_dma_xmit_fifo_error_hook\n");
}

void
stm32_uart4_dma_recv_error_hook( void )
{
    TRACE("stm32_uart4_dma_recv_error_hook\n");
}

void
stm32_uart4_dma_recv_dm_error_hook( void )
{
    TRACE("stm32_uart4_dma_recv_dm_error_hook\n");
}

void
stm32_uart4_dma_recv_fifo_error_hook( void )
{
    TRACE("stm32_uart4_dma_recv_fifo_error_hook\n");
}


/*******************************************************************************
* MAIN
*******************************************************************************/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int
main( void )
{
    stm32_mpu_init();

    SCB_InvalidateICache(); //Invalidate I-Cache : ICIALLU register
    SCB->CCR |= (1 <<18);   //Enable branch prediction
    __DSB();
    SCB_InvalidateICache(); //Invalidate I-Cache : ICIALLU register
    SCB_EnableICache();     //Enable I-Cache
    SCB_InvalidateDCache();
    SCB_EnableDCache();

    HAL_Init();

    stm32_clock_init();
    stm32_rtc_init();
    stm32_fmc_init();
    dev_init( &dev );

    osThreadStaticDef( START,    task_start,    osPriorityNormal,    0, configMINIMAL_STACK_SIZE,  task_stck_start,   &tcb_start   );
    task_hndl_start =   osThreadCreate( osThread( START ), NULL );

    osKernelStart();

    #ifdef  NDEBUG
    NVIC_SystemReset();
    #else
    __asm volatile ("bkpt #0\n");
    #endif //NDEBUG
}