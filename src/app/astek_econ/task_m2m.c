/**
  * @file    task_m2m.c
  * @brief   Task Machine 2 Machine communications
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <math.h>
#include <stdbool.h>
#include "app_pipe.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32.h"
#include "modbus.h"
#include "os\os_user.h"
#include "external_uart.h"
#include "dev/dev.h"

/*******************************************************************************
*
*******************************************************************************/
typedef union
{
        uint32_t        u32;
        float           f;
        uint16_t        u16[ 2 ];
        int16_t         i16[ 2 ];
        uint8_t         u8[  4 ];
} conc_t;


#pragma pack(4)
static  uint8_t         modbus_adu[ MDBS_RTU_ADU_SIZEOF ];
#pragma pack()


/*******************************************************************************
*
*******************************************************************************/
extern  QueueHandle_t           que_m2m_hndl;
extern  dev_t           dev;

void task_m2m_ext_mdbs_reinit(void)
{
    app_pipe_t queue_data;
    queue_data.tag    = OS_USER_TAG_EXT_MDBS_REINIT;
    
    xQueueSend( que_m2m_hndl, &queue_data, NULL );   
}

/*******************************************************************************
*
*******************************************************************************/
void
task_m2m(                               const   void *          argument )
{
    //TickType_t      polling_cycle_tcks      = 1000 / portTICK_PERIOD_MS;
    bool            received;
    size_t          len;
    os_user_tag_t      sync;
    app_pipe_t queue_data;


    (void) argument;

    osDelay( 1000 );

    external_uart_config_baudrate( 19200 );
    external_uart_init();

    osDelay( 1 );

    external_uart_recv( modbus_adu, MDBS_RTU_ADU_SIZEOF );

    while( true )
    {
        received        =   xQueueReceive( que_m2m_hndl, &queue_data, portMAX_DELAY );
        sync = queue_data.tag;

        if( received )
        {
            switch( sync )
            {
                case OS_USER_TAG_UART7_RECV:
                    len     = mdbs_rtu_slave(   dev.cfg.ext_mdbs_cfg.rtu_addr,
                                                modbus_adu,
                                                MDBS_RTU_ADU_SIZEOF - external_uart_rx_get_ndtr() );

                    external_uart_xmit( modbus_adu, len );
                    //osDelay( 100 );
                    break;
                    
                case OS_USER_TAG_EXT_MDBS_REINIT:
                      external_uart_init();
                      break;

                default:
                    break;
            }

            external_uart_recv( modbus_adu, MDBS_RTU_ADU_SIZEOF );
        }
    }
}
