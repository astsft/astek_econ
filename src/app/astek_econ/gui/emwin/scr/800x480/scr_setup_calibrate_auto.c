/**
  * @file    scr_setup_calibrate_auto.c
  * @brief   Screen Setup Calibrate Auto
  * @author  Igor T. <research.tahoe@gmail.com>
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
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                50,  40, 350,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_LAUNCH,          50, 100, 350,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_CYCLE,           50, 160, 350,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,                50, 220, 350,  60, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_AUTOLAUNCH,   450,  40, 300,  60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LAUNCH,       450, 100, 300,  60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CYCLE,        450, 160, 300,  60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON4,             450, 220, 300,  60, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
};


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
setup_buttons(                                          WM_HWIN         hWin )
{
    if( dev.cfg.cal_autolaunch )
    {
        BUTTON_SetText(  WM_GetDialogItem( hWin, GUI_ID_BUTTON_AUTOLAUNCH ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_ALLOWED ) );
        WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_LAUNCH ) );
        WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_CYCLE ) );
        WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON4 ) );
    }
    else
    {
        BUTTON_SetText(   WM_GetDialogItem( hWin, GUI_ID_BUTTON_AUTOLAUNCH ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_PROHIBITED ) );
        WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_LAUNCH ) );
        WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_CYCLE ) );
        WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON4 ) );
    }
}


static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN         hItem;
    //char            str[32];


    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_GRAY                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_AUTOLAUNCH )  );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_LAUNCH );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_GRAY                                 );
    //term_cal_auto_start_timestamp_str( &dev, str, sizeof( str ) );
    //TEXT_SetText(           hItem, str                                      );
    TEXT_SetText(           hItem, "1970/01/01 00:00" );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_CYCLE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_GRAY                                 );
    //term_cal_auto_cycle_hours_str( &dev, str, sizeof( str ) );
    //TEXT_SetText(           hItem, str                                      );
    TEXT_SetText(           hItem, "0" );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_GRAY                                 );
    //term_cal_auto_flow_seconds_str( &dev, str, sizeof( str ) );
    //TEXT_SetText(           hItem, str                                      );
    TEXT_SetText(           hItem, "0" );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_AUTOLAUNCH );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LAUNCH );
    BUTTON_SetFocusable(    hItem, 1                                        );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_LAUNCH )        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CYCLE );
    BUTTON_SetFocusable(    hItem, 1                                        );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CYCLE )        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON4 );
    BUTTON_SetFocusable(    hItem, 1                                        );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_PURGE )        );

    setup_buttons( hWin );

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
                    scr_switch( SCR_IDX_SETUP_CALIBRATION, GUI_ID_BUTTON_AUTO );
                    beep_play( BEEP_SHRT );
                    break;
                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON_AUTOLAUNCH:
                        dev.cfg.cal_autolaunch  = dev.cfg.cal_autolaunch ? false : true;
                        //if( dev.cfg.cal_autolaunch )
                        //{
                        //    dev.cfg.cal_autolaunch  = false;
                        //}
                        //else
                        //{
                        //    dev.cfg.cal_autolaunch  = true;
                        //}
                        setup_buttons( pMsg->hWin );
                        beep_play( BEEP_TYPE_CONFIRM );
                        break;

                    case GUI_ID_BUTTON_LAUNCH:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO_LAUNCH, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_CYCLE:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO_CYCLE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON4:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO_PURGE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_BACK:
                    case GUI_ID_BUTTON_CANCEL:
                    case GUI_ID_CANCEL:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION, GUI_ID_BUTTON_AUTO );
                        beep_play( BEEP_SHRT );
                        break;

                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_DUMMY ) );
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
scr_setup_calibration_auto( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
