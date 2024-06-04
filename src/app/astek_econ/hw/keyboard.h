/**
  * @file    ui_keyb.h
  * @brief   User Interface - Keyboard
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  UI_KEYB_H
#define  UI_KEYB_H


#include <stdint.h>
#include <string.h>


#define UI_KEY_MODE_RELEASE             0x00
#define UI_KEY_MODE_PUSH                0x40
#define UI_KEY_MODE_HOLD                0xC0

#define UI_KEY_CODE_V                   0x02
#define UI_KEY_CODE_X                   0x03
#define UI_KEY_CODE_ARRW_UP             0x04
#define UI_KEY_CODE_ARRW_DOWN           0x05
#define UI_KEY_CODE_ARRW_LEFT           0x06
#define UI_KEY_CODE_ARRW_RGHT           0x07


void    ui_keyb_init( void );
void    ui_keyb_start( void );
void    ui_keyb_read( uint8_t key, int * key_id, int * key_pressed );
uint32_t keyb_rx_get_ndtr( void );

#endif	//UI_KEYB_H
