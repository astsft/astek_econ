/**
  * @file   scr_setup_help.c
  * @brief  Screen Setup Help
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
    { WINDOW_CreateIndirect,    "", 0,                                0,  80, 800, 400, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SENSOR,           100,  12, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CL420,            100,  90, 600,  64, 0, 0x0, 0 },
    //{ BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DISCRETE_OUTPUT,  100, 166, 600,  64, 0, 0x0, 0 },
    //{ BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SERVICE,      100, 244, 600,  64, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,               0, 320, 800,  80, 0, 0x0, 0 },
    //{ BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,             -1,  -1,   1,   1, 0, 0x0, 0 },
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
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SENSOR );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ) );
    BUTTON_SetFocusable(    hItem, 1    );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CL420 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CL420 ) );
    BUTTON_SetFocusable(    hItem, 1    );

    //hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DISCRETE_OUTPUT );
    //BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_DISCRETE_OUTPUT ) );
    //BUTTON_SetFocusable(    hItem, 1    );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK ) );
    BUTTON_SetFocusable(    hItem, 1 );
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
                case GUI_KEY_RIGHT:
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_MEASURE );
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
                    case GUI_ID_BUTTON_SENSOR:
                        scr_switch( SCR_IDX_SETUP_MEASURE_SENSOR, GUI_ID_BUTTON_SENSOR );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_CL420:
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON_SENSOR );
                        beep_play( BEEP_SHRT );
                        break;
                    //case GUI_ID_BUTTON_DISCRETE_OUTPUT:
                    //    scr_switch( SCR_IDX_SETUP_MEASURE_DISCRETE_OUTPUT, GUI_ID_BUTTON_SENSOR );
                    //    beep_play( BEEP_SHRT );
                    //    break;
                    case GUI_ID_BUTTON_BACK:
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_MEASURE );
                        beep_play( BEEP_SHRT );
                        break;
                    default:
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
scr_setup_measure( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
