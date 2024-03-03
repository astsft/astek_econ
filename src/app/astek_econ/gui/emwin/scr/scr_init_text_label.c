/**
  * @file    scr_init_text_label.c
  * @brief   Screen Init text label
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr.h"


void
scr_init_text_label(                                WM_HWIN         hItem )
{
    TEXT_SetFont(       hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(  hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(    hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(  hItem, GUI_LIGHTGRAY                            );
}
