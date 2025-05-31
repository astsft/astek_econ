/**
  * @file   scr_setup_service_network.c
  * @brief  Screen Setup Service Network
  * @author nkp144
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "cmsis_os.h"
#include "app_pipe.h"

extern  dev_t           dev;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                   0,  80, 800, 400, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_NETWORK_MAC,             20,   88, 100,  64, 0, 0x0, 0 },   
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_NETWORK_IP,              20,  164, 100,  64, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_NETWORK_SN,              20,  240, 100,  64, 0, 0x0, 0 },   
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK_STATE,         140,  12, 240,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK_MAC,           140,  88, 240,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK_IP,            140, 164, 240,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK_SN,            140, 240, 240,  64, 0, 0x0, 0 },    

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_NETWORK_GW,              400,   88, 120,  64, 0, 0x0, 0 },   
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_NETWORK_PORT,            400,  164, 120,  32, 0, 0x0, 0 },        
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_NETWORK_MODBUS,          400,  196, 120,  32, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                        400,  240, 120,  32, 0, 0x0, 0 },   
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_NETWORK_HTTP,            400,  272, 120,  32, 0, 0x0, 0 },      
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK_GW,            540, 88,  240,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK_PORT_MODBUS,   540, 164, 240,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK_PORT_HTTP,     540, 240, 240,  64, 0, 0x0, 0 },    
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,                  0, 320, 800,  80, 0, 0x0, 0 },
};

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
setup_buttons(                                          WM_HWIN         hWin )
{
    if (dev.net->net_status)
    {
      BUTTON_SetText(  WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_STATE ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENABLED ) );
      WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_MAC ) );
      WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_IP ) );
      WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_SN ) );  
      WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_GW ) );
      WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_PORT_MODBUS ) );
      WM_EnableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_PORT_HTTP ) );      
    }
    else
    {
      BUTTON_SetText(  WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_STATE ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_DISABLED ) );
      WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_MAC ) );
      WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_IP ) );
      WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_SN ) );  
      WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_GW ) );
      WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_PORT_MODBUS ) );
      WM_DisableWindow( WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_PORT_HTTP ) );    
    }
}


static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN         hItem;
    char            str[64];

    ////////////////////////////////////////////////////////////////////////////
    // MIDDLE AREA
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_STATE );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_NETWORK_MAC );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MAC )  );      
    
    
    snprintf(str, sizeof(str),"%02X-%02X-%02X-%02X-%02X-%02X",
            dev.net->mac.u8[0], dev.net->mac.u8[1], dev.net->mac.u8[2], dev.net->mac.u8[3], dev.net->mac.u8[4], dev.net->mac.u8[5]);
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_MAC );
    BUTTON_SetText(         hItem, str);
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_NETWORK_IP );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_IP )  );      

    snprintf(str, sizeof(str),"%03d.%03d.%03d.%03d",
        dev.net->ip.u8[0], dev.net->ip.u8[1], dev.net->ip.u8[2], dev.net->ip.u8[3]);
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_IP );
    BUTTON_SetText(         hItem, str);
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_NETWORK_SN );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SN )  );    
    
    snprintf(str, sizeof(str),"%03d.%03d.%03d.%03d",
        dev.net->sn.u8[0], dev.net->sn.u8[1], dev.net->sn.u8[2], dev.net->sn.u8[3]);    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_SN );
    BUTTON_SetText(         hItem, str);
    BUTTON_SetFocusable(    hItem, 1                                        );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_NETWORK_GW );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_GW )  );      

    snprintf(str, sizeof(str),"%03d.%03d.%03d.%03d",
        dev.net->gw.u8[0], dev.net->gw.u8[1], dev.net->gw.u8[2], dev.net->gw.u8[3]);        
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_GW );
    BUTTON_SetText(         hItem, str);
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_NETWORK_MODBUS );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MODBUS )  );      
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_NETWORK_PORT );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_PORT )  );    
    
    snprintf(str, sizeof(str),"%u", dev.net->port_modbus);            
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_PORT_MODBUS );
    BUTTON_SetText(         hItem, str);
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_NETWORK_HTTP );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_HTTP )  );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_PORT )  );    
  
    snprintf(str, sizeof(str),"%u", dev.net->port_http);                
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK_PORT_HTTP );
    BUTTON_SetText(         hItem, str);
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    setup_buttons(hWin);
        
    ////////////////////////////////////////////////////////////////////////////
    // FOOTER AREA
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
                    scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_NETWORK);
                    break;
                    
                case GUI_KEY_LEFT:
                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        beep_play( BEEP_SHRT );
                        break;

                case GUI_KEY_RIGHT:
                        GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                        GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                        GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                        beep_play( BEEP_SHRT );
                        break;                    

                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
                {
                    case GUI_ID_BUTTON_NETWORK_STATE:
                        dev.net->net_status = dev.net->net_status ? false : true;
                        setup_buttons( pMsg->hWin );
                        send_cmd_for_nvm_write_param(NVM_REG_NET_STATE, dev.net->net_status);
                        send_cmd_to_net_task(OS_USER_TAG_ETH_CHANGE_PARAM);
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_NETWORK_MAC:
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK_MAC, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_NETWORK_IP:
                        dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_NETWORK_IP;
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK_IP, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_BUTTON_NETWORK_SN:
                        dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_NETWORK_SN;
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK_SN, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;                        

                    case GUI_ID_BUTTON_NETWORK_GW:
                        dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_NETWORK_GW;
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK_GW, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_BUTTON_NETWORK_PORT_MODBUS:
                        dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_NETWORK_MODBUS_PORT;
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK_MODBUS_PORT, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_NETWORK_PORT_HTTP :
                        dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_NETWORK_HTTP_PORT;
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK_HTTP_PORT, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;  
                        
                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_NETWORK);
                        break;

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
scr_setup_service_network( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
