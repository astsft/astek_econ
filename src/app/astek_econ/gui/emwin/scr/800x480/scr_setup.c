/**
  * @file   scr_setup.c
  * @brief  Screen Setup
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

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RANGE,      100,  12, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CALIBRATION,  100,  90, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SYSTEM,       100, 166, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SERVICE,      100, 244, 600,  64, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LEFT,           0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RIGHT,        400, 320, 400,  80, 0, 0x0, 0 },
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
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RANGE );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_RANGE ) );
    BUTTON_SetFocusable(    hItem, 1    );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CALIBRATION );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CALIBRATION ) );
    BUTTON_SetFocusable(    hItem, 1    );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SYSTEM );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ) );
    BUTTON_SetFocusable(    hItem, 1    );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SERVICE );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SERVICE ) );
    BUTTON_SetFocusable(    hItem, 1    );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LEFT );
    BUTTON_SetFocusable(    hItem, 0 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CONSOLE_AL ) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RIGHT );
    BUTTON_SetFocusable(    hItem, 0 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE_AR )   );
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
                case GUI_KEY_LEFT:
                    //scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_SPAN );
                    scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_DUMMY );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    //scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_DUMMY );
                    scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
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
                    case GUI_ID_BUTTON_RANGE:
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_SYSTEM:
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_INFO );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_CALIBRATION:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION, GUI_ID_BUTTON_REMOTELY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_SERVICE:
                        scr_switch( SCR_IDX_SETUP_SERVICE_ENTRY, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_LEFT:
                        scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_DUMMY );
                        //scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_RIGHT:
                        scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
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
scr_setup( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
