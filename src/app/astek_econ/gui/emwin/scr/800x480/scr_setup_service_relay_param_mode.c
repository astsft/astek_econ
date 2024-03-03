/**
  * @file    ui_dspl_scr_setup_system_language.c
  * @brief   User Interface - Screen Setup System Language
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "DIALOG.h"
#include "scr\scr.h"
#include "dev\dev.h"


/*******************************************************************************
* GLOBAL VARIBLES
*******************************************************************************/
extern  dev_t           dev;

/*******************************************************************************
* PRIVATE VARIBLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,               250, 140, 300, 40, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          250,  100, 300, 120, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    init_dialog(                            WM_HWIN                 hWin )
{
        WM_HWIN         hItem;
        int             mode;
        char const **   list    = l10n_relay_mode_list_get(dev.cfg.lang);
        size_t          countof = l10n_mode_countof_get();


        WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
        
        list    = l10n_relay_mode_list_get(dev.cfg.lang);              
        gui_init_listwheel( hWin, GUI_ID_LISTWHEEL0, 50, list, countof );

        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) mode = dev.mdb_relay->relay[0].relay_mode;
        else mode = dev.mdb_relay->relay[1].relay_mode;
        
        LISTWHEEL_SetPos(       hItem, mode );
        LISTWHEEL_SetSel(       hItem, mode );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
        TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
        TEXT_SetTextAlign(      hItem, GUI_TA_HCENTER | TEXT_CF_VCENTER         );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_WHITE                                );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
        TEXT_SetBkColor(        hItem, GUI_DARKGRAY                             );


        ////////////////////////////////////////////////////////
        // FOOTER AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
        BUTTON_SetFocusable(    hItem, 1                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
        BUTTON_SetFocusable(    hItem, 0                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
        BUTTON_SetFocusable(    hItem, 0                                        );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
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
        WM_HWIN         hItem;
        int             mode;
        size_t          list_countof    = l10n_mode_countof_get();
        uint32_t config = 0;
        //int             id              = WM_GetId( pMsg->hWinSrc );


        switch( pMsg->MsgId )
        {
                case WM_KEY:
                        switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
                        {
                                case GUI_KEY_UP:
                                        beep_play( BEEP_SHRT );
                                        hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                                        mode    = LISTWHEEL_GetPos( hItem );

                                        if( ++mode >= list_countof )
                                        {
                                                mode    = 0;
                                        }

                                        LISTWHEEL_SetPos(       hItem, mode );
                                        LISTWHEEL_SetSel(       hItem, mode );

                                        break;

                                case GUI_KEY_DOWN:
                                        beep_play( BEEP_SHRT );
                                        hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                                        mode    = LISTWHEEL_GetPos( hItem );
                                        if( --mode < 0 )
                                        {
                                                mode    = list_countof-1;
                                        }

                                        LISTWHEEL_SetPos(       hItem, mode );
                                        LISTWHEEL_SetSel(       hItem, mode );

                                        break;

                                case GUI_KEY_ESCAPE:
                                        beep_play( BEEP_SHRT );
                                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                        else scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                        break;

                                case GUI_KEY_LEFT:
                                case GUI_KEY_RIGHT:
                                default:
                                        beep_play( BEEP_TYPE_ERROR );
                                        break;
                        }

                        break;

                case WM_NOTIFY_PARENT:
                        switch( WM_GetId(pMsg->hWinSrc) )
                        {
                                case GUI_ID_LISTWHEEL0:
                                        if(         pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                                        {
                                                beep_play( BEEP_SHRT );
                                        }
                                        else if(    pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                                        {
                                        }
                                        else if(    pMsg->Data.v == WM_NOTIFICATION_SEL_CHANGED )
                                        {
                                        }
                                        else if(    pMsg->Data.v == WM_NOTIFICATION_MOVED_OUT )
                                        {
                                        }
                                        break;


                                case GUI_ID_BUTTON_CANCEL:
                                        if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                                        {
                                                beep_play( BEEP_SHRT );
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                                else scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                        }
                                        break;

                                case GUI_ID_BUTTON_ENTER:
                                case GUI_ID_BUTTON_DUMMY:
                                        if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                                        {
                                                hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                                                mode    = LISTWHEEL_GetPos( hItem );
                                                if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM)
                                                {
                                                  dev.mdb_relay->relay[0].relay_mode  = (relay_mode_e) mode;
                                                  config = dev.nvm.get( NVM_REG_RELAY1_MODE_STATE_TYPE );
                                                  config &= 0xFF00FFFF;
                                                  config |= dev.mdb_relay->relay[0].relay_mode << 16;
                                                  dev.nvm.put( NVM_REG_RELAY1_MODE_STATE_TYPE, config ); 
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                                }
                                                else
                                                {
                                                  dev.mdb_relay->relay[1].relay_mode  = (relay_mode_e) mode;
                                                  config = dev.nvm.get( NVM_REG_RELAY2_MODE_STATE_TYPE );
                                                  config &= 0xFF00FFFF;
                                                  config |= dev.mdb_relay->relay[1].relay_mode << 16;
                                                  dev.nvm.put( NVM_REG_RELAY2_MODE_STATE_TYPE, config ); 
                                                  scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_MODE );
                                                }
                                                beep_play( BEEP_TYPE_CONFIRM );
                                        }
                                        break;

                                case GUI_ID_TEXT_BACKPLANE:
                                        break;

                                case GUI_ID_TEXT0:
                                        break;

                                default:
                                        //beep_play( BEEP_TYPE_ERROR );
                                        break;
                        }
                        break;

                case WM_INIT_DIALOG:
                        init_dialog( pMsg->hWin );
                        WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                        TEXT_SetBkColor( WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT0 ), CONFIG_UI_COLOR_WHEEL_SEL   );
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
scr_setup_service_relay_param_mode( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
