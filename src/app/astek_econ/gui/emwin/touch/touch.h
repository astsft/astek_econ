/**
  * @file    touch.h
  * @brief   Touchpad Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  TOUCH_H
#define  TOUCH_H


#include <stdint.h>
#include <string.h>


/*******************************************************************************
*
*******************************************************************************/
/*
typedef enum    ui_touch_error_e
{
    TOUCH_ERROR_NONE        = 0,
    TOUCH_ERROR_INIT,
    TOUCH_ERROR_CONFIG,
    TOUCH_ERROR_LAST,
} ui_touch_error_t;
*/


/*******************************************************************************
*
*******************************************************************************/
int
ui_touch_init( void );

void
ui_touch_int( void );

void
ui_touch_scan( void );


#endif	//TOUCH_H
