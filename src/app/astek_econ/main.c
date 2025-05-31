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

#ifdef EXT_FLASH
  #include "hw2331_ext_flash.h"
#endif

/*******************************************************************************
*
*******************************************************************************/
extern  QueueHandle_t           que_hmi_hndl;
        uint32_t                task_stck_start[ configMINIMAL_STACK_SIZE ];
        osStaticThreadDef_t     tcb_start;
        osThreadId              task_hndl_start;

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

dev_ext_relay_t ext_relay;
dev_cloop_t cloop;
dev_net_t   net;

#pragma data_alignment = 8
dev_t           dev =
{
    .sens                       = &sens,
    .factory_reset              = dev_factory_reset,
#ifdef EXT_FLASH    
    .nvm.put                    = flash_write_param,
    .nvm.get                    = flash_read_param,       
#else 
    .nvm.put                    = stm32_rtc_bckp_put,
    .nvm.get                    = stm32_rtc_bckp_get,
#endif
    .safe.master.password.u08   = {0x00,0x00,0x00,0x00,},
    .mcu                        = &dev_mcu,

    .info.device_id             = CONFIG_DEVICE_ID,
    .info.hardware_id           = CONFIG_HARDWARE_ID,
    .info.hardware_str          = CONFIG_HARDWARE_STR,
    .info.device_str            = CONFIG_DEVICE_STR,    

    .info.uid_u32[ 0] = (uint32_t *) (UID_BASE_ADDRESS +  0),
    .info.uid_u32[ 1] = (uint32_t *) (UID_BASE_ADDRESS +  4),
    .info.uid_u32[ 2] = (uint32_t *) (UID_BASE_ADDRESS +  8),
    .info.uid_u32[ 3] = (uint32_t *) (UID_BASE_ADDRESS + 12),

    .info.uid_u16[ 0] = (uint16_t *) (UID_BASE_ADDRESS +  0),
    .info.uid_u16[ 1] = (uint16_t *) (UID_BASE_ADDRESS +  2),
    .info.uid_u16[ 2] = (uint16_t *) (UID_BASE_ADDRESS +  4),
    .info.uid_u16[ 3] = (uint16_t *) (UID_BASE_ADDRESS +  6),
    .info.uid_u16[ 4] = (uint16_t *) (UID_BASE_ADDRESS +  8),
    .info.uid_u16[ 5] = (uint16_t *) (UID_BASE_ADDRESS + 10),
    .info.uid_u16[ 6] = (uint16_t *) (UID_BASE_ADDRESS + 12),
    .info.uid_u16[ 7] = (uint16_t *) (UID_BASE_ADDRESS + 14),

    .cfg.display_mode       = DEV_DSPL_MODE_PERCENTS,
    .ext_relay              = &ext_relay,
    .cloop                  = &cloop,
    .net                    = &net,
};

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

    stm32_clock_init();
    stm32_rtc_init();
    stm32_fmc_init(); 
    
#ifdef EXT_FLASH
    stm32_spi2_init();
#endif
    
    osThreadStaticDef( START,    task_start,    osPriorityNormal,    0, configMINIMAL_STACK_SIZE,  task_stck_start,   &tcb_start   );
    task_hndl_start =   osThreadCreate( osThread( START ), NULL );

    osKernelStart();

    #ifdef  NDEBUG
    NVIC_SystemReset();
    #else
    __asm volatile ("bkpt #0\n");
    #endif //NDEBUG
}
