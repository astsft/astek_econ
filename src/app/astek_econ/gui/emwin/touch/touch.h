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
int
ui_touch_init( void );

void
ui_touch_int( void );

void
ui_touch_scan( void );


#endif	//TOUCH_H
