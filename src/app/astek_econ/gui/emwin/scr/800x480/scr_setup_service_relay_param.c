/**
  * @file   scr_setup_system.c
  * @brief  Screen Setup System
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "hw_relay.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/                                                                
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                 0,  80, 800, 400, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RELAY_MODE,        100,  32, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RELAY_STATE,       100, 128, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RELAY_THRESHOLD,   100, 224, 600,  64, 0, 0x0, 0 },
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,                0, 320, 800,  80, 0, 0x0, 0 },
};


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    scr_cfg_sys_init(                               WM_HWIN         hWin )
{
        WM_HWIN         hItem;


        //WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RELAY_MODE );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MODE )     );
        BUTTON_SetFocusable(    hItem, 1                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RELAY_STATE );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_STATE )     );
        BUTTON_SetFocusable(    hItem, 1                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RELAY_THRESHOLD );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_THRESHOLD )         );
        BUTTON_SetFocusable(    hItem, 1                                        );
        
        switch (dev.gui.scr_idx) {
          case SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM:
            if (dev.ext_relay->relay[EXTERNAL_1].relay_mode != THRESHOLD_MODE) WM_DisableWindow(hItem);
            break;
          
          case SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM:
            if (dev.ext_relay->relay[EXTERNAL_2].relay_mode != THRESHOLD_MODE) WM_DisableWindow(hItem);
            break;
        }

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
                                        scr_switch( SCR_IDX_SETUP_SERVICE_RELAY, (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) ? GUI_ID_BUTTON_RELAY_1 : GUI_ID_BUTTON_RELAY_2  );
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
                                        case GUI_ID_BUTTON_RELAY_MODE:
                                                scr_switch( SCR_IDX_SETUP_SEVICE_RELAY_PARAM_MODE, GUI_ID_BUTTON_DUMMY );
                                                beep_play( BEEP_SHRT );
                                                break;
                                      
                                        case GUI_ID_BUTTON_RELAY_STATE:
                                                scr_switch( SCR_IDX_SETUP_SEVICE_RELAY_PARAM_STATE, GUI_ID_BUTTON_RESET );
                                                beep_play( BEEP_SHRT );
                                                break;
                                      
                                        case GUI_ID_BUTTON_RELAY_THRESHOLD:                                                
                                                scr_switch( SCR_IDX_SETUP_SEVICE_RELAY_THRESHOLD, GUI_ID_BUTTON_DUMMY );
                                                beep_play( BEEP_SHRT );
                                                break;
                                                
                                        case GUI_ID_BUTTON_BACK:
                                                scr_switch( SCR_IDX_SETUP_SERVICE_RELAY, (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) ? GUI_ID_BUTTON_RELAY_1 : GUI_ID_BUTTON_RELAY_2  );
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
scr_setup_service_relay_param( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
