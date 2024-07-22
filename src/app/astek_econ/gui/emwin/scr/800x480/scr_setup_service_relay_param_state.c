/**
  * @file    ui_dspl_scr_setup_system_language.c
  * @brief   User Interface - Screen Setup System Language
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "DIALOG.h"
#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"

/*******************************************************************************
* GLOBAL VARIBLES
*******************************************************************************/
extern  dev_t           dev;

/*******************************************************************************
* PRIVATE VARIBLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },


    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_dialog(                                            WM_HWIN         hWin )
{
        WM_HWIN         hItem;
        RADIO_Handle    hRadio_0;
        int value;


        WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        //
        // MIDDLE AREA
        //
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
        TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
        WM_BringToBottom( hItem );

        hRadio_0   = RADIO_CreateEx(  250, 80, 375, 240, hWin, WM_CF_SHOW, 0, GUI_ID_RADIO0, 2, 120 );
        RADIO_SetBkColor(       hRadio_0,  GUI_TRANSPARENT                         );
        RADIO_SetFont(          hRadio_0,  &GUI_FontTahoma30                       );
        RADIO_SetTextColor( hRadio_0, GUI_BLUE                                );
        RADIO_SetFocusColor( hRadio_0, GUI_BLACK                                     ); 
        RADIO_SetText( hRadio_0, l10n_str_get( dev.cfg.lang, L10N_STR_ID_RELAY_STATE_NO ), 0 );
        RADIO_SetText( hRadio_0, l10n_str_get( dev.cfg.lang, L10N_STR_ID_RELAY_STATE_NC ), 1 );
        WM_SetFocus( WM_GetDialogItem(hWin, GUI_ID_RADIO0 ) );
        
        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) value = dev.ext_relay->relay[0].relay_state;
        else value = dev.ext_relay->relay[1].relay_state;
        RADIO_SetValue( hRadio_0,  value );

        //
        // FOOTER AREA
        //
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
        BUTTON_SetFocusable(    hItem, 1                                        );
        
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
        BUTTON_SetFocusable(    hItem, 0                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
        BUTTON_SetFocusable(    hItem, 0                                        );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
        uint32_t config = 0;
        relay_state_e relay_state = 0;

        switch( pMsg->MsgId )
        {
                case WM_KEY:
                        switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
                        {
                                case GUI_KEY_UP:
                                        beep_play( BEEP_SHRT );
                                        break;

                                case GUI_KEY_DOWN:
                                        beep_play( BEEP_SHRT );
                                        break;

                                case GUI_KEY_ESCAPE:
                                        beep_play( BEEP_SHRT );
                                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                        else scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                        break;

                                case GUI_KEY_LEFT:
                                        beep_play( BEEP_SHRT );
                                        break;
                                case GUI_KEY_RIGHT:
                                        beep_play( BEEP_SHRT );
                                        break;    
                                        
                                case GUI_KEY_ENTER:
                                            relay_state = (relay_state_e) RADIO_GetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0));                                                                                    
                                            if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM)
                                            {
                                              dev.ext_relay->relay[0].relay_state = relay_state;
                                              config =  dev.ext_relay->relay[0].relay_mode << 16;
                                              config |= dev.ext_relay->relay[0].relay_state << 8;
                                              config |= dev.ext_relay->relay[0].thld_type;  
                                              send_cmd_for_nvm_write_param(NVM_REG_RELAY1_MODE_STATE_TYPE, config);
                                              scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_STATE );
                                            }
                                            else
                                            {
                                              dev.ext_relay->relay[1].relay_state = relay_state;                                               
                                              config =  dev.ext_relay->relay[1].relay_mode << 16;
                                              config |= dev.ext_relay->relay[1].relay_state << 8;
                                              config |= dev.ext_relay->relay[1].thld_type;  
                                              send_cmd_for_nvm_write_param(NVM_REG_RELAY2_MODE_STATE_TYPE, config);                                              
                                              scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_STATE );
                                            }                                  
                                        beep_play( BEEP_TYPE_CONFIRM );                                 
                                        break;
                                
                                default:
                                        beep_play( BEEP_TYPE_ERROR );
                                        break;
                        }

                        break;

                case WM_NOTIFY_PARENT:
                        switch( WM_GetId(pMsg->hWinSrc) )
                        {
                                case GUI_ID_BUTTON_CANCEL:
                                       if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                                        {
                                            if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_STATE );
                                            else scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_STATE );
                                            beep_play( BEEP_SHRT );
                                        }
                                        break;

                                case GUI_ID_BUTTON_ENTER:
                                case GUI_ID_BUTTON_DUMMY:
                                        if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                                        {                      
                                            relay_state = (relay_state_e) RADIO_GetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0));                                                                                    
                                            if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM)
                                            {
                                              dev.ext_relay->relay[0].relay_state = relay_state;                                                  
                                              config =  dev.ext_relay->relay[0].relay_mode << 16;
                                              config |= dev.ext_relay->relay[0].relay_state << 8;
                                              config |= dev.ext_relay->relay[0].thld_type;
                                              send_cmd_for_nvm_write_param(NVM_REG_RELAY1_MODE_STATE_TYPE, config);                                              
                                              scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_STATE );
                                            }
                                            else
                                            {
                                              dev.ext_relay->relay[1].relay_state = relay_state;
                                              config =  dev.ext_relay->relay[1].relay_mode << 16;
                                              config |= dev.ext_relay->relay[1].relay_state << 8;
                                              config |= dev.ext_relay->relay[1].thld_type;
                                              send_cmd_for_nvm_write_param(NVM_REG_RELAY2_MODE_STATE_TYPE, config);                                               
                                              scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_STATE );
                                            }
                                            
                                            beep_play( BEEP_TYPE_CONFIRM );
                                        }
                                        break;

                                case GUI_ID_TEXT_BACKPLANE:
                                        break;

                                case GUI_ID_TEXT0:
                                        break;
                                        
                                case GUI_ID_RADIO0:
                                        if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                                        {
                                          beep_play(BEEP_SHRT);
                                        }
                                        break;

                                default:
                                        //beep_play( BEEP_TYPE_ERROR );
                                        break;
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
scr_setup_service_relay_param_state( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
