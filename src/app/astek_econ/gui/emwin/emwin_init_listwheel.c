/**
  * @file    gui_init_listwheel.c
  * @brief   GUI Init Listwheel
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "DIALOG.h"
#include "gui_user.h"
#include "scr\scr.h"


/*******************************************************************************
* HELPER FUNCTION
*******************************************************************************/
void
gui_init_listwheel(                     WM_HWIN         hWin,
                                        int             id,
                                        size_t          line_height,
                                const   char **         str,
                                        size_t          str_sizeof )
{
    WM_HWIN     hItem   = WM_GetDialogItem( hWin, id );
    int         y_size  = WM_GetWindowSizeY( hItem );


    LISTWHEEL_SetFont(      hItem,  &GUI_FontTahoma30                        );
    LISTWHEEL_SetTextAlign( hItem,  GUI_TA_HCENTER | GUI_TA_VCENTER          );
    LISTWHEEL_SetTextColor( hItem,  LISTWHEEL_CI_UNSEL,     GUI_WHITE         );
    LISTWHEEL_SetTextColor( hItem,  LISTWHEEL_CI_SEL,       GUI_WHITE         );
    LISTWHEEL_SetBkColor(   hItem,  LISTWHEEL_CI_UNSEL,     GUI_TRANSPARENT  );
    LISTWHEEL_SetBkColor(   hItem,  LISTWHEEL_CI_SEL,       GUI_TRANSPARENT  );

    LISTWHEEL_SetLBorder(   hItem,  20                                       );
    LISTWHEEL_SetRBorder(   hItem,  20                                       );
    LISTWHEEL_SetLineHeight( hItem, line_height                              );

    for( size_t i = 0; i < str_sizeof; i++ )
    {
        LISTWHEEL_AddString( hItem, str[ i] );
    }

    LISTWHEEL_SetSnapPosition( hItem, (y_size/2 - line_height/2) );

    WM_SetHasTrans( hItem );
}
