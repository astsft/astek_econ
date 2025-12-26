/**
  * @file   scr_setup_system.c
  * @brief  Screen Setup System
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
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LANGUAGE,     100,  10, 600,  52, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DATE,         100,  72, 600,  52, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_TIME,         100, 134, 600,  52, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_PASSWORD,     100, 196, 600,  52, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_INFO,         100, 258, 600,  52, 0, 0x0, 0 },    

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
};


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    scr_cfg_sys_init(                               WM_HWIN         hWin )
{
    WM_HWIN         hItem;


    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LANGUAGE );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_LANGUAGE )     );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_PASSWORD );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_PASSWORD )     );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DATE );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_DATE )         );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_TIME );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_TIME )         );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_INFO );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_INFO )         );
    BUTTON_SetFocusable(    hItem, 1                                        );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK )         );
    BUTTON_SetFocusable(    hItem, 0                                        );
}


static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_DOWN:
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_SYSTEM );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_LEFT:
                    GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_RIGHT:
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;

                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                int Id = WM_GetId(pMsg->hWinSrc);
                switch( Id )
                {
                    case GUI_ID_BUTTON_INFO:
                        scr_switch( SCR_IDX_SETUP_SYSTEM_INFO, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_PASSWORD:
                        scr_switch( SCR_IDX_SETUP_SYSTEM_PASSWORD, GUI_ID_LISTWHEEL0 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_DATE:
                        scr_switch( SCR_IDX_SETUP_SYSTEM_DATE, GUI_ID_LISTWHEEL0 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_TIME:
                        scr_switch( SCR_IDX_SETUP_SYSTEM_TIME, GUI_ID_LISTWHEEL0 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_LANGUAGE:
                        scr_switch( SCR_IDX_SETUP_SYSTEM_LANGUAGE, GUI_ID_LISTWHEEL0 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_BACK:
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_SYSTEM );
                        beep_play( BEEP_SHRT );
                        break;
                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            scr_cfg_sys_init( pMsg->hWin );
            WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_SYSTEM ) );
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
scr_setup_system( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
