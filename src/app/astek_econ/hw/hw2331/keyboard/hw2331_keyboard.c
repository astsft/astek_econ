/**
  * @file    keyboard.c
  * @brief   keyboard 
  * @author  Konstantin.Nikonov@pmi-ai.ru
  */

#include "keyboard.h"
#include "GUI.h"
#include "dev\dev.h"
#include "app_pipe.h"
#include "cmsis_os.h"

extern  dev_t           dev;
extern  QueueHandle_t           que_hmi_hndl;
uint8_t keyboard_buf[1];

void ui_keyb_init(void)
{
   stm32_usart2_init( 115200, 8, 1.0, 'N' );
}

void ui_keyb_start(void)
{
  stm32_usart2_recv_dma(keyboard_buf, sizeof(keyboard_buf));
}

void
ui_keyb_read(                           int *           key_gui,
                                        int *           key_pressed )
{
    uint8_t     key = dev.mcu->usart2.rxd;

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
* USART2 - KEYBOARD
*******************************************************************************/
void
stm32_usart2_idle_hook( void )
{
    TRACE("stm32_usart2_idle_hook\n");
    dev.mcu->usart2.rxd = keyboard_buf[0];
    app_pipe_t result;
    result.tag    = OS_USER_TAG_KEYBOARD_RECV_IDLE;  

    xQueueSendFromISR( que_hmi_hndl, &result, NULL );    
    
    stm32_usart2_recv_dma(keyboard_buf, sizeof(keyboard_buf));
}

void
stm32_usart2_rxne_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_KEYBOARD_RECV_RXNE; 
    
    dev.mcu->usart2.rxd = LL_USART_ReceiveData8( USART2 );

    xQueueSendFromISR( que_hmi_hndl, &result, NULL );
    
    //stm32_usart2_recv_dma(keyboard_buf, sizeof(keyboard_buf));
}

void
stm32_usart2_error_hook(        uint32_t sts_reg )
{
    TRACE( "USART2->ISR: %08X ORE=%c, NE=%c, FE=%c, PE=%c\n",
            sts_reg,
            sts_reg & USART_ISR_ORE ? '1' : '0',
            sts_reg & USART_ISR_NE ? '1' : '0',
            sts_reg & USART_ISR_FE ? '1' : '0',
            sts_reg & USART_ISR_PE ? '1' : '0'  );
}