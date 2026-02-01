/**
  * @file   scr_status_bar.c
  * @brief  Screen Status Bar
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", GUI_ID_WIN_SBAR,              0,   0, 800, 480, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SBAR_HEADER,      0,   0, 800,  80, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SBAR_TIME,        0,   0, 150,  80, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SBAR_STS0,      650,   0, 150,  80, 0, 0x0, 0 },
};
/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN                 hItem;

    ////////////////////////////////////////////////////////
    // HEADER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SBAR_TIME );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, ""                                       );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SBAR_HEADER );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_WHITE                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE )      );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SBAR_STS0 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_GRAY                                 );
    TEXT_SetText(           hItem, "⬤"                                      );
}


static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    WM_HWIN         hItem;
    char            str[6];
    time_t          timestamp;
    struct tm *     ts;


    switch( pMsg->MsgId )
    {
        case WM_MESSAGE_HEADER_LABEL_UPDATE:
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SBAR_HEADER );
            TEXT_SetText( hItem, l10n_str_get( dev.cfg.lang, (l10n_str_id_t) pMsg->Data.v ) );
            break;

        case WM_TIMER:
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SBAR_TIME );
            timestamp = dev.mcu->rtc.get_timestamp();
            ts = localtime(&timestamp);
            snprintf( str, sizeof(str), "%02d:%02d", ts->tm_hour, ts->tm_min);
            TEXT_SetText( hItem, str );

            // Set Green
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SBAR_STS0 );
            TEXT_SetTextColor( hItem, GUI_GREEN );            
            
            // Check for warnings
            if (dev.state.warnings_status != 0)
            {
              hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SBAR_STS0 );
              TEXT_SetTextColor( hItem, GUI_YELLOW );
            }
            
            // Check for errors
            if (dev.state.error_status != 0)
            {
              hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SBAR_STS0 );
              TEXT_SetTextColor( hItem, GUI_RED );
            }   

            WM_RestartTimer( pMsg->Data.v, 0 );
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_CreateTimer( pMsg->hWin, 0, 100, 0 );
            break;

        default:
            WM_DefaultProc(pMsg);
            break;
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
scr_status_bar( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
