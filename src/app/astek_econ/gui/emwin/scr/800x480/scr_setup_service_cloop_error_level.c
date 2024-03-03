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

        WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        //
        // MIDDLE AREA
        //
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
        TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
        WM_BringToBottom( hItem );

        hRadio_0   = RADIO_CreateEx(  350, 80, 125, 240, hWin, WM_CF_SHOW, 0, GUI_ID_RADIO0, 2, 120 );
        RADIO_SetBkColor(       hRadio_0,  GUI_TRANSPARENT                         );
        RADIO_SetFont(          hRadio_0,  &GUI_FontTahoma30                       );
        RADIO_SetTextColor( hRadio_0, GUI_BLUE                                );
        RADIO_SetFocusColor( hRadio_0, GUI_BLACK                                     ); 
        RADIO_SetText( hRadio_0, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP_3MA ) , 0 );
        RADIO_SetText( hRadio_0, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP_22MA ) , 1 );
        RADIO_SetValue( hRadio_0,  dev.cloop->error_level );
        WM_SetFocus( WM_GetDialogItem(hWin, GUI_ID_RADIO0 ) );
        
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
        uint32_t cloop_config = 0; 
        int order;

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
                                        scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_CLOOP_ERROR_LEVEL );
                                        break;

                                case GUI_KEY_LEFT:
                                        beep_play( BEEP_SHRT );
                                        break;
                                case GUI_KEY_RIGHT:
                                        beep_play( BEEP_SHRT );
                                        break;    
                                        
                                case GUI_KEY_ENTER:
                                        order = RADIO_GetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0));
                                        switch (order)
                                        {
                                          case 0:  dev.cloop->error_level = LOW_ERROR_LEVEL;                                                       
                                            break;
                                          case 1:   dev.cloop->error_level = HI_ERROR_LEVEL;
                                            break;                                                
                                        }                                                  
                                        cloop_config = dev.nvm.get( NVM_REG_RANGE_IDX_UNITS_ERR_LEVEL);
                                        cloop_config &= 0xFFFFFF00;
                                        cloop_config |= dev.cloop->error_level;
                                        dev.nvm.put( NVM_REG_RANGE_IDX_UNITS_ERR_LEVEL, cloop_config);
                                        scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_CURRENT_LOOP);                                 
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
                                            scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_CURRENT_LOOP );
                                            beep_play( BEEP_SHRT );
                                        }
                                        break;

                                case GUI_ID_BUTTON_ENTER:
                                case GUI_ID_BUTTON_DUMMY:
                                        if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                                        {                      
                                            int order = RADIO_GetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0));
                                            switch (order)
                                            {
                                              case 0:  dev.cloop->error_level = LOW_ERROR_LEVEL;                                                       
                                                break;
                                              case 1:   dev.cloop->error_level = HI_ERROR_LEVEL;
                                                break;                                                
                                            }                                                  
                                            cloop_config = dev.nvm.get( NVM_REG_RANGE_IDX_UNITS_ERR_LEVEL);
                                            cloop_config &= 0xFFFFFF00;
                                            cloop_config |= dev.cloop->error_level;
                                            dev.nvm.put( NVM_REG_RANGE_IDX_UNITS_ERR_LEVEL, cloop_config);
                                            scr_switch( SCR_IDX_SETUP_SERVICE_CURRENT_LOOP, GUI_ID_BUTTON_CURRENT_LOOP);
                                            
                                            beep_play( BEEP_TYPE_CONFIRM );
                                        }
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
scr_setup_service_cloop_error_level( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
