/**
  * @file    keyboard.c
  * @brief   Keyboard
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "keyboard.h"

#include "stm32.h"
#include "gui.h"
#include "config.h"


//static  uint8_t                 ui_keyb[ CFG_SER1_RECV_BLCK_SIZE_OCT ];

/*
void
ui_keyb_init( void )
{
        stm32_usart1_config_baudrate( CONFIG_UI_KEYBOARD_USART_BAUDRATE );
        stm32_usart1_init();
}
*/
/*
void
ui_keyb_start( void )
{
        stm32_usart1_recv_dma( ui_keyb, CFG_SER1_RECV_BLCK_SIZE_OCT );
}
*/
/*
size_t
ui_keyb_recv( void )
{
        static  size_t          ndtr[2] = { CFG_SER1_RECV_BLCK_SIZE_OCT, CFG_SER1_RECV_BLCK_SIZE_OCT };
                size_t          cnt;

        ndtr[0] =   ndtr[1];
        ndtr[1] =   stm32_usart1_dma_recv_remainder();


        if(             ndtr[0] > ndtr[1] )
        {
                cnt     =   ndtr[0] - ndtr[1];
        }
        else if(        ndtr[0] < ndtr[1] )
        {
                cnt     =   ndtr[1] - ndtr[0];
        }
        else
        {
                cnt     =   0;
        }

        return( cnt );
}
*/
/*
void    ui_keyb_error( void )
{
        //ui_keyb_start();
}
*/
/*
static
uint8_t
ui_keyb_get( void )
{
        uint32_t        offset  =   CFG_SER1_RECV_BLCK_SIZE_OCT - stm32_usart1_dma_recv_remainder();

        return( ui_keyb[ offset-1 ] );
}


void    ui_keyb_read(                           int *           key_gui,
                                                int *           key_pressed )
{
        uint8_t         key;


        key             = ui_keyb_get();
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
*/
