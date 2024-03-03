/**
  * @file    scr_setup_service_reset_password_success.c
  * @brief   Screen Setup Service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>
#include "main.h"
#include "DIALOG.h"
#include "scr\scr.h"

#include "dev\dev.h"


extern  dev_t           dev;



/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                50, 120, 700,  60, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_dialog(                                            WM_HWIN         hWin )
{
        WM_HWIN         hItem;


        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
        TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
        TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
        TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
        TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
        TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_COMPLETED )    );

        ////////////////////////////////////////////////////////
        // FOOTER AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
        BUTTON_SetFocusable(    hItem, 1                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK )         );
        BUTTON_SetFocusable(    hItem, 0                                        );

}


static
void
dialog_callback(                                WM_MESSAGE *    pMsg )
{
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_RESET_PASSWORD );
                    beep_play( BEEP_SHRT );
                    break;

                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON_BACK:
                        scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_RESET_PASSWORD );
                        beep_play( BEEP_SHRT );
                        break;

                    default:
                        beep_play( BEEP_TYPE_ERROR );
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            break;

        default:
            WM_DefaultProc(pMsg);
    }
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief
  * @param  None
  * @retval None
  */
WM_HWIN
scr_setup_service_reset_password_success( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
