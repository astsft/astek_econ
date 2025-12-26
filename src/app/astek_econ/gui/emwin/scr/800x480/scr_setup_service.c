/**
  * @file   scr_setup_service.c
  * @brief  Screen Setup Service
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
    { WINDOW_CreateIndirect,    "", 0,                                  0,  80, 800, 400, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MEASURE,         64, 12,   304,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DIAGNOSTICS,     64, 88,   304,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RESET_PASSWORD,  64, 164,  304,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NETWORK,         64, 240,  304,  64, 0, 0x0, 0 },    
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RESET_SETTINGS, 432, 12,  304,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RELAY,          432, 88,  304,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CURRENT_LOOP,   432, 164, 304,  64, 0, 0x0, 0 },    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MODBUS,         432, 240, 304,  64, 0, 0x0, 0 }, 
    
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

    ////////////////////////////////////////////////////////////////////////////
    // MIDDLE AREA
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_MEASURE );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE      ));
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DIAGNOSTICS );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_DIAGNOSTICS   ));
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RESET_PASSWORD );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_PASSWORD_RESET ));
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RESET_SETTINGS );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_FACTORY_SETTINGS ));
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RELAY );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_RELAY ));
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CURRENT_LOOP );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP ));
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NETWORK );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_NETWORK ));
    BUTTON_SetFocusable(    hItem, 1                                        );        
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_MODBUS );
    BUTTON_SetText(         hItem,  l10n_str_get( dev.cfg.lang, L10N_STR_ID_MODBUS ));
    BUTTON_SetFocusable(    hItem, 1                                        );     
        
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
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_SERVICE );
                    break;
                    
                case GUI_KEY_LEFT:
                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        beep_play( BEEP_SHRT );
                        break;

                case GUI_KEY_RIGHT:
                        GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
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
                    case GUI_ID_BUTTON_MEASURE:
                        scr_switch( SCR_IDX_SETUP_SERVICE_MEASURE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_DIAGNOSTICS:
                        scr_switch( SCR_IDX_SETUP_SERVICE_DIAGNOSTICS, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_RESET_PASSWORD:
                        scr_switch( SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_ENTRY, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_RESET_SETTINGS:
                        scr_switch( SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_ENTRY, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_BUTTON_RELAY:
                        scr_switch( SCR_IDX_SETUP_SERVICE_RELAY, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_CURRENT_LOOP :
                        scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;                 
                        
                    case GUI_ID_BUTTON_MODBUS :
                        scr_switch( SCR_IDX_SETUP_SERVICE_MODBUS, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;      
                        
                    case GUI_ID_BUTTON_NETWORK:
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK, GUI_ID_BUTTON_NETWORK_STATE );
                        beep_play( BEEP_SHRT );
                        break;                             

                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_SERVICE );
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
scr_setup_service( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
