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
    { WINDOW_CreateIndirect,    "", 0,                            0, 80, 800, 400, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CLOOP_CHANNEL_1,    100,  64, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CLOOP_CHANNEL_2,    100, 192, 600,  64, 0, 0x0, 0 },
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 320, 800, 80, 0, 0x0, 0 },
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
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CLOOP_CHANNEL_1 );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CALIBRATION_CHANNEL_1));
        BUTTON_SetFocusable(    hItem, 1  );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CLOOP_CHANNEL_2 );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CALIBRATION_CHANNEL_2));
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
                                        beep_play( BEEP_SHRT );
                                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );                                        
                                        break;

                                case GUI_KEY_DOWN:
                                        beep_play( BEEP_SHRT );
                                        GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                                        break;

                                case GUI_KEY_ESCAPE:
                                        beep_play( BEEP_SHRT );
                                        scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_CLOOP_CAL );
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
                                        case GUI_ID_BUTTON_CLOOP_CHANNEL_1:
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CAL)
                                                {
                                                  dev.gui.scr_idx    = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL, GUI_ID_BUTTON_DUMMY );
                                                }
                                                else
                                                {
                                                  dev.gui.scr_idx    = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_DIAGNOSTIC;
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_DIAGNOSTIC, GUI_ID_BUTTON_DUMMY );                                                  
                                                }
                                                beep_play( BEEP_SHRT );
                                                break;

                                        case GUI_ID_BUTTON_CLOOP_CHANNEL_2:
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CAL)
                                                {
                                                  dev.gui.scr_idx    = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL, GUI_ID_BUTTON_DUMMY );                                                  
                                                }
                                                else
                                                {
                                                  dev.gui.scr_idx    = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_DIAGNOSTIC;
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_DIAGNOSTIC, GUI_ID_BUTTON_DUMMY );                                                                                                    
                                                }
                                                beep_play( BEEP_SHRT );
                                                break;                                                
                                                                                                
                                        case GUI_ID_BUTTON_BACK:
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CAL)                                                
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_CLOOP_CAL );                                                
                                                else
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_CLOOP_VERIFY );                                                                                                  
                                                beep_play( BEEP_SHRT );
                                                break;

                                        default:
                                                beep_play( BEEP_TYPE_ERROR );
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
scr_setup_service_cloop_channel( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
