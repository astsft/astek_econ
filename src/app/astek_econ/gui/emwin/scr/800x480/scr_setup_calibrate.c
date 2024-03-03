/**
  * @file    scr_setup_calibrate.c
  * @brief   Screen Setup Calibrate
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_REMOTELY,        50,  40, 250,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_AUTO,            50, 100, 250,  60, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_REMOTELY,     350,  40, 400,  60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_AUTO,         350, 100, 400,  60, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
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
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_REMOTELY );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, dev.cfg.cal_remotely ? GUI_GREEN : GUI_RED );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, dev.cfg.cal_remotely ? L10N_STR_ID_ALLOWED : L10N_STR_ID_PROHIBITED ) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_AUTO );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, dev.cfg.cal_autolaunch ? GUI_GREEN : GUI_RED );
    TEXT_SetText( hItem, l10n_str_get( dev.cfg.lang, dev.cfg.cal_autolaunch ? L10N_STR_ID_ALLOWED : L10N_STR_ID_PROHIBITED ) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_AUTO );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_AUTO ));
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_REMOTELY );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_REMOTELY ));
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
dialog_callback(                                WM_MESSAGE *    pMsg )
{
    WM_HWIN         hItem;


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    beep_play( BEEP_SHRT );
                    GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                    break;

                case GUI_KEY_DOWN:
                    beep_play( BEEP_SHRT );
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                    break;

                case GUI_KEY_ESCAPE:
                    beep_play( BEEP_SHRT );
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_CALIBRATION );
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
                    case GUI_ID_BUTTON_REMOTELY:
                        dev.cfg.cal_remotely = dev.cfg.cal_remotely ? false : true;
                        hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_REMOTELY );
                        TEXT_SetText( hItem, l10n_str_get( dev.cfg.lang, dev.cfg.cal_remotely ? L10N_STR_ID_ALLOWED : L10N_STR_ID_PROHIBITED ) );
                        TEXT_SetTextColor( hItem, dev.cfg.cal_remotely ? GUI_GREEN : GUI_RED );
                        beep_play( BEEP_SHRT );

                        //if( dev.safe.user.password.u32 != 0 )
                        //{
                        //    scr_switch( SCR_IDX_SETUP_CALIBRATION_REMOTELY_CHECK, GUI_ID_LISTWHEEL0 );
                        //}
                        //else
                        //{
                        //    scr_switch( SCR_IDX_SETUP_CALIBRATION_REMOTELY, GUI_ID_BUTTON1 );
                        //}
                        break;

                    case GUI_ID_BUTTON_AUTO:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON1 );
                        //if( dev.safe.user.password.u32 != 0 )
                        //{
                        //    scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO_CHECK, GUI_ID_LISTWHEEL0 );
                        //}
                        //else
                        //{
                        //    scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON1 );
                        //}
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_CALIBRATION );
                        break;

                    default:
                        beep_play( BEEP_TYPE_ERROR );
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_REMOTELY ) );
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
scr_setup_calibration( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
