/**
  * @file    scr_calibrate.c
  * @brief   Screen Calibrate
  * @author  Nikonov Konstantin
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  const   GUI_WIDGET_CREATE_INFO  dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,    80, 800, 400, 0, 0x0, 0 },
   
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CLOOP_4MA,      100,  64, 600,  64, 0, 0x0, 0 }, 
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CLOOP_20MA,     100, 192, 600,  64, 0, 0x0, 0 }, 
        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,             0, 320, 800,  80, 0, 0x0, 0 },
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
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CLOOP_4MA );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP_4MA ));
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CLOOP_20MA );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP_20MA ));
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
                                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL)
                                          scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL, GUI_ID_BUTTON_CLOOP_CHANNEL_1 );
                                        else
                                          scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL, GUI_ID_BUTTON_CLOOP_CHANNEL_2 );
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
                                        case GUI_ID_BUTTON_CLOOP_4MA:
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL)
                                                  dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA;
                                                else 
                                                  dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA;
                                                
                                                if (dev.safe.user.password.u32 != 0)  scr_switch( SCR_IDX_SETUP_SYSTEM_PASSWORD, GUI_ID_BUTTON_DUMMY );                                                
                                                else if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA)
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA, GUI_ID_BUTTON_DUMMY );
                                                else 
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA, GUI_ID_BUTTON_DUMMY );
                                                beep_play( BEEP_SHRT );
                                                break;

                                        case GUI_ID_BUTTON_CLOOP_20MA:
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL)
                                                  dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA;
                                                else 
                                                  dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA;

                                                if (dev.safe.user.password.u32 != 0)  scr_switch( SCR_IDX_SETUP_SYSTEM_PASSWORD, GUI_ID_BUTTON_DUMMY );
                                                else if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA)
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA, GUI_ID_BUTTON_DUMMY );
                                                else 
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA, GUI_ID_BUTTON_DUMMY );
                                                beep_play( BEEP_SHRT );
                                                break;

                                        case GUI_ID_BUTTON_BACK:
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL)
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL, GUI_ID_BUTTON_CLOOP_CHANNEL_1 );
                                                else
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CAL, GUI_ID_BUTTON_CLOOP_CHANNEL_2 );
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
scr_setup_service_cloop_cal( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}

