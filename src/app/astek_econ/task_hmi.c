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
#include "touch\touch.h"
#include "os\os_user.h"
#include "app_pipe.h"
#include "keyboard.h"

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
/*
void
task_hmi_cal_factory_settings_update( const int         idx )
{
    os_user_tag_t   tag;

    switch( idx )
    {
        case 3:     tag = OS_USER_TAG_CAL1_FACTORY_UPDATE;  break;
        case 2:     tag = OS_USER_TAG_CAL0_FACTORY_UPDATE;  break;
        case 1:     tag = OS_USER_TAG_CAL1_UPDATE;          break;
        case 0:     tag = OS_USER_TAG_CAL0_UPDATE;          break;
        default:    tag = OS_USER_TAG_CAL0_UPDATE;          break;
    }

    xQueueSend( que_ibus_hndl, &tag, NULL );
}
*/



/*******************************************************************************
*
*******************************************************************************/
void
task_hmi(                               const   void *          argument )
{
    bool            received;
    os_user_tag_t   tag;
    uint8_t *key;
    int             key_id, key_pressed;
    app_pipe_t      queue_data = 
    {
      .cnt = 0, 
      .data = NULL,
      .tag = 0
    };


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
    //ui_touch_init();       
    ui_keyb_init();
    ui_keyb_start();
    
    while( true )
    {
        received    =   xQueueReceive( que_hmi_hndl, &queue_data, pdMS_TO_TICKS(500) );
        tag = queue_data.tag;
        key = queue_data.data;

        if( received )
        {
            switch( tag )
            {

                case OS_USER_TAG_KEYBOARD_RECV_IDLE:
                    //if( ui_keyb_recv() > 0 )
                    {
                        ui_keyb_read(*key, &key_id, &key_pressed );

                        if( key_pressed )
                        {
                            GUI_SendKeyMsg( key_id, key_pressed );
                        }

                        GUI_Exec();

                        //TRACE( "IDLE key_id: %02X, key_pressed: %d\n", key_id, key_pressed );
                    }
                    break;

                case OS_USER_TAG_KEYBOARD_RECV_RXNE:
                    ui_keyb_read(*key, &key_id, &key_pressed );

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
