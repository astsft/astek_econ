/**
  * @file    scr_init_text_value.c
  * @brief   Screen Init text value
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr.h"


void
scr_init_text_value(                    WM_HWIN         hItem )
{
        TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
        TEXT_SetTextAlign(      hItem, GUI_TA_RIGHT | TEXT_CF_VCENTER           );
        TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
}
