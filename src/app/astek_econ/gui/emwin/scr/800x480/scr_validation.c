/**
  * @file   scr_setup.c
  * @brief  Screen Setup
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"

extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480 
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SETUP,        100,  64, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_START,        100, 192, 600,  64, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LEFT,           0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RIGHT,        400, 320, 400,  80, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  100, 1024, 500, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SETUP,       128, 112, 768,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_START,       128, 208, 768,  80, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LEFT,           0, 400, 512,  100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RIGHT,        512, 400, 512,  100, 0, 0x0, 0 },
};
#endif


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
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SETUP );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SETUP ) );
    BUTTON_SetFocusable(    hItem, 1    );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_START );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BEGIN ) );
    BUTTON_SetFocusable(    hItem, 1    );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LEFT );
    BUTTON_SetFocusable(    hItem, 0 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CALIBRATE_AL ) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RIGHT );
    BUTTON_SetFocusable(    hItem, 0 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CONSOLE_AR )   );
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
                    scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_DUMMY );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_DUMMY );
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
                    case GUI_ID_BUTTON_SETUP:
                        scr_switch( SCR_IDX_VALIDATION_SETUP, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_START:
                        dev.cloop->cloop_state = CLOOP_FREEZ;
                        dev.validation.state = IN_PROGRESS;
                        dev.state.process_status = PROCESS_VALIDATION_SPAN;
                        dev.validation.start = VALIDATION_MANUAL_START;
                        send_cmd_for_validation_start();
                        scr_switch( SCR_IDX_VALIDATION_STEP1, GUI_ID_BUTTON_DUMMY );                      
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_LEFT:
                        scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_RIGHT:
                        dev.gui.scr_idx = SCR_IDX_VALIDATION;
                        scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_DUMMY );
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
scr_validation( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
