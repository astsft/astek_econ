/**
  * @file    gui_user.h
  * @brief   GUI User Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  GUI_USER_H
#define  GUI_USER_H


#include "WM.h"


#if (GUI_USE_ARGB)
  #define GUI_GRAY_0F       0xFF0F0F0F
  #define GUI_GRAY_17       0xFF171717
  #define GUI_GRAY_1F       0xFF1F1F1F
  #define GUI_GRAY_27       0xFF272727
  #define GUI_GRAY_2F       0xFF2F2F2F
  #define GUI_GRAY_3F       0xFF3F3F3F
  #define GUI_GRAY_10       0xFF101010
  #define GUI_GRAY_20       0xFF202020
  #define GUI_GRAY_28       0xFF282828
  #define GUI_GRAY_30       0xFF303030
#else
  #define GUI_GRAY_0F       0x000F0F0F
  #define GUI_GRAY_17       0x00171717
  #define GUI_GRAY_1F       0x001F1F1F
  #define GUI_GRAY_27       0x00272727
  #define GUI_GRAY_2F       0x002F2F2F
  #define GUI_GRAY_3F       0x003F3F3F
  #define GUI_GRAY_10       0x00101010
  #define GUI_GRAY_20       0x00202020
  #define GUI_GRAY_28       0x00282828
  #define GUI_GRAY_30       0x00303030
#endif


void    gui_init_listwheel( WM_HWIN, int, size_t, const char **, size_t );
void    gui_set_defaults( void );


#endif  //GUI_USER_H
