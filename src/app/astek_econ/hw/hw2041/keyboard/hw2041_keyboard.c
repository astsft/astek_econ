/**
  * @file    keyboard.c
  * @brief   Keyboard
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include "keyboard.h"
#include "stm32.h"
#include "GUI.h"
#include "dev\dev.h"
#include "app_pipe.h"
#include "cmsis_os.h"

extern  dev_t           dev;
extern  QueueHandle_t           que_hmi_hndl;

void ui_keyb_init(void)
{
   stm32_usart1_init( 115200, 8, 1.0, 'N' );
}

void ui_keyb_start(void)
{
  stm32_usart1_recv();
}

void
ui_keyb_read(                           uint8_t key,
                                        int *           key_gui,
                                        int *           key_pressed )
{
    uint8_t     key_temp = dev.mcu->usart1.rxd;

    *key_pressed    = (key_temp & 0xC0) == UI_KEY_MODE_RELEASE ? 0 : 1;

    switch( key_temp & 0x3F )
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
* USART1 - KEYBOARD
*******************************************************************************/
void
stm32_usart1_idle_hook( void )
{
    //TRACE("stm32_usart1_idle_hook\n");
    app_pipe_t result;
    result.tag    = OS_USER_TAG_KEYBOARD_RECV_IDLE;  

    xQueueSendFromISR( que_hmi_hndl, &result, NULL );
}

void
stm32_usart1_rxne_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_KEYBOARD_RECV_RXNE; 
    result.data = NULL;
    
    dev.mcu->usart1.rxd = LL_USART_ReceiveData8( USART1 );

    xQueueSendFromISR( que_hmi_hndl, &result, NULL );
    //TRACE("stm32_usart1_rxne_hook: %d\n", dev.mcu->usart1.rxd );
}