/**
  * @file    scr_setup_calibrate.c
  * @brief   Screen Setup Calibrate
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"


/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                           0,  80, 800, 400, 0, 0x0, 0 },
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SETTINGS,    100,  12, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CYCLE_TIME,  100,  90, 600,  64, 0, 0x0, 0 },    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SPAN,        100, 166, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ZERO,        100, 244, 600,  64, 0, 0x0, 0 },    
    

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                           0, 100, 1024, 500, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SETTINGS,    128,  15,  768,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CYCLE_TIME,  128, 112,  768,  80, 0, 0x0, 0 },    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SPAN,        128, 208,  768,  80, 0, 0x0, 0 },    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ZERO,        128, 305,  768,  80, 0, 0x0, 0 },     

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 400, 1024, 100, 0, 0x0, 0 },
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
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CYCLE_TIME );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CYCLE_TIME ));
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SETTINGS );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SETTINGS ) );
    BUTTON_SetFocusable(    hItem, 1    );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SPAN );
    BUTTON_SetFocusable(    hItem, 1 );
    BUTTON_SetText(         hItem, "SPAN" );
    if (dev.calibration.remote_calibration_enabled)
      WM_EnableWindow(hItem);
    else
      WM_DisableWindow(hItem);
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ZERO );
    BUTTON_SetFocusable(    hItem, 1 );
    BUTTON_SetText(         hItem, "ZERO"  );
    if (dev.calibration.remote_calibration_enabled)
      WM_EnableWindow(hItem);
    else
      WM_DisableWindow(hItem);    

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
                    scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_AUTO );
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
                    case GUI_ID_BUTTON_CYCLE_TIME:
                        scr_switch( SCR_IDX_CALIBRATION_AUTO_CYCLE_TIME, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_BUTTON_SETTINGS:
                        scr_switch( SCR_IDX_CALIBRATION_SETUP, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;        
                        
                    case GUI_ID_BUTTON_ZERO:
                        dev.cloop->cloop_state = CLOOP_FREEZ;
                        dev.calibration.state = CALIBRATION_IN_PROGRESS;
                        dev.state.process_status = PROCESS_CALIBRATION_ZERO;
                        dev.calibration.start = CALIBRATION_MANUAL_START;
                        send_cmd_for_calibration_zero_start();
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_STEP1;
                        scr_switch( SCR_IDX_CALIBRATION_STEP1, GUI_ID_BUTTON_DUMMY );                      
                        beep_play( BEEP_SHRT );
                        break;                        
                        
                    case GUI_ID_BUTTON_SPAN:
                        dev.cloop->cloop_state = CLOOP_FREEZ;
                        dev.calibration.state = CALIBRATION_IN_PROGRESS;
                        dev.state.process_status = PROCESS_CALIBRATION_SPAN;
                        dev.calibration.start = CALIBRATION_MANUAL_START;
                        send_cmd_for_calibration_span_start();
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_STEP1;
                        scr_switch( SCR_IDX_CALIBRATION_STEP1, GUI_ID_BUTTON_DUMMY );                      
                        beep_play( BEEP_SHRT );
                        break;                                                

                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_AUTO );
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
scr_calibrate_auto( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
