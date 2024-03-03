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


/*******************************************************************************
*
*******************************************************************************/
//#define CONF_SER_POLLING_CYCLE_mSEC     500
//#define MDBS_DEV_BAUDRATE               19200
#define MDBS_DEV_ADDR                   13



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

    stm32_uart7_config_baudrate( 19200 );
    stm32_uart7_init();

    osDelay( 1 );

    stm32_uart7_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );

    while( true )
    {
        received        =   xQueueReceive( que_m2m_hndl, &queue_data, portMAX_DELAY );
        sync = queue_data.tag;

        if( received )
        {
            switch( sync )
            {
                case OS_USER_TAG_UART7_RECV:
                    len     = mdbs_rtu_slave(   MDBS_DEV_ADDR,
                                                modbus_adu,
                                                MDBS_RTU_ADU_SIZEOF - stm32_uart7_dma_rx_get_ndtr() );

                    stm32_uart7_xmit_dma( modbus_adu, len );
                    //osDelay( 100 );
                    break;

                default:
                    break;
            }

            stm32_uart7_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );
        }
    }
}
