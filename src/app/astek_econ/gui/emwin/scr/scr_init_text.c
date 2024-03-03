/**
  * @file    scr_init_text.c
  * @brief   Screen Init text
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr.h"


void
scr_init_text(                      WM_HWIN         hWin,
                                        int             id,
                                const   char *          str  )
{
    WM_HWIN         hItem;

    hItem   = WM_GetDialogItem( hWin, id );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_HCENTER | TEXT_CF_VCENTER         );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem,  str                                     );
}
