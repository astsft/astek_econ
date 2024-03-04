/**
  * @file    task_dspl.c
  * @brief   Display services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


/*******************************************************************************
*
*******************************************************************************/
#include <stdbool.h>
#include "cmsis_os.h"
#include "stm32.h"
#include "config.h"
#include "dev\dev.h"
#include "scr\scr.h"
#include "beep\beep.h"
#include "keyboard\keyboard.h"
#include "touch\touch.h"
#include "os\os_user.h"
#include "app_pipe.h"

/*******************************************************************************
*
*******************************************************************************/
extern  QueueHandle_t   que_hmi_hndl;
extern  QueueHandle_t   que_ibus_hndl;
extern  dev_t           dev;

/*******************************************************************************
*
*******************************************************************************/
void
task_hmi_cal_update(            const   int             idx )
{
    BaseType_t result;
    app_pipe_t      queue_data;
    os_user_tag_t   tag;

    switch( idx )
    {
        case 3:     tag = OS_USER_TAG_CAL1_FACTORY_UPDATE;  break;
        case 2:     tag = OS_USER_TAG_CAL0_FACTORY_UPDATE;  break;
        case 1:     tag = OS_USER_TAG_CAL1_UPDATE;          break;
        case 0:     tag = OS_USER_TAG_CAL0_UPDATE;          break;
        default:    tag = OS_USER_TAG_CAL0_UPDATE;          break;
    }
    
    queue_data.tag = tag;

    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
    
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }
}

void
task_hmi_cal_restore(           const   int             idx )
{
    BaseType_t result;
    app_pipe_t      queue_data;
    os_user_tag_t   tag;

    switch( idx )
    {
        case 3:     tag = OS_USER_TAG_CAL1_FACTORY_RESTORE; break;
        case 2:     tag = OS_USER_TAG_CAL0_FACTORY_RESTORE; break;
        case 1:     tag = OS_USER_TAG_CAL1_RESTORE;         break;
        case 0:     tag = OS_USER_TAG_CAL0_RESTORE;         break;
        default:    tag = OS_USER_TAG_CAL0_RESTORE;         break;
    }
    
    queue_data.tag = tag;

    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
    
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }
}

/*******************************************************************************
* KEYBOARD
*******************************************************************************/
/*
static
uint8_t
get_key( void )
{
    //uint32_t        offset  =   CFG_SER1_RECV_BLCK_SIZE_OCT - stm32_usart1_dma_recv_remainder();
    //return( buf_keyb[ offset-1 ] );


    static  size_t  tile    = 0;
            size_t  head    = CFG_SER1_RECV_BLCK_SIZE_OCT - stm32_usart1_dma_recv_remainder();

    if( tile != head )
    {

    }

    return( buf_keyb[ head-1 ] );

}
*/

void
ui_keyb_read(                           int *           key_gui,
                                        int *           key_pressed )
{
    //uint8_t         key = get_key();
    uint8_t     key = dev.mcu->usart1.rxd;

    //TRACE( "key: %02X\n", key );

    *key_pressed    = (key & 0xC0) == UI_KEY_MODE_RELEASE ? 0 : 1;

    switch( key & 0x3F )
    {
        case UI_KEY_CODE_X:             *key_gui = GUI_KEY_ESCAPE;      break;
        case UI_KEY_CODE_V:             *key_gui = GUI_KEY_ENTER;       break;
        case UI_KEY_CODE_ARRW_UP:       *key_gui = GUI_KEY_UP;          break;
        case UI_KEY_CODE_ARRW_DOWN:     *key_gui = GUI_KEY_DOWN;        break;
        case UI_KEY_CODE_ARRW_LEFT:     *key_gui = GUI_KEY_LEFT;        break;
        case UI_KEY_CODE_ARRW_RGHT:     *key_gui = GUI_KEY_RIGHT;       break;
        default:                        *key_gui = 0;                   break;
    }
}


/*******************************************************************************
*
*******************************************************************************/
void
task_hmi(                               const   void *          argument )
{
    bool            received;
    os_user_tag_t   tag;
    int             key_id, key_pressed;
    app_pipe_t      queue_data;


    #ifdef NDEBUG
    beep_init( 4000 );
    #else
    beep_init( 100 );
    #endif

    beep_play( BEEP_TYPE_CONFIRM );
    scr_init();

    //keyb_init();
    //keyb_start();
    osDelay(1000);
    ui_touch_init();
    stm32_usart1_init( 115200, 8, 1.0, 'N' );
    stm32_usart1_recv();

    while( true )
    {
        received    =   xQueueReceive( que_hmi_hndl, &queue_data, pdMS_TO_TICKS(500) );
        tag = queue_data.tag;

        if( received )
        {
            switch( tag )
            {

                case OS_USER_TAG_USART1_RECV_IDLE:
                    //if( ui_keyb_recv() > 0 )
                    {
                        ui_keyb_read( &key_id, &key_pressed );

                        if( key_pressed )
                        {
                            GUI_SendKeyMsg( key_id, key_pressed );
                        }

                        GUI_Exec();

                        //TRACE( "IDLE key_id: %02X, key_pressed: %d\n", key_id, key_pressed );
                    }
                    break;

                case OS_USER_TAG_USART1_RECV_RXNE:
                    ui_keyb_read( &key_id, &key_pressed );

                    if( key_pressed )
                    {
                        GUI_SendKeyMsg( key_id, key_pressed );
                    }
                    GUI_Exec();
                    //TRACE( "RXNE key_id: %02X, key_pressed: %d\n", key_id, key_pressed );
                    break;


                case OS_USER_TAG_TOUCH_INT:
                    ui_touch_int();
                    ui_touch_scan();
                    GUI_Exec();
                    break;

                case OS_USER_TAG_IBUS_REFRESH:
                    GUI_Exec();
                    break;

                default:
                    GUI_Exec();
                    break;
            }
        }

    } //while( true )
}
