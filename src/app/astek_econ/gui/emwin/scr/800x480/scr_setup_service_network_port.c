/**
  * @file    scr_setup_service_network_ip.c
  * @brief   setup ip address, subnet mask and gateway ip
  * @author  nkp144
  */


#include "DIALOG.h"
#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "main.h"



/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
extern  dev_t           dev;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  100, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,               150, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,               250, 140, 100,  40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               350, 140, 100,  40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,               450, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,               550, 140, 100,  40, 0, 0x0, 0 },
            
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          150,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          250,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          350,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          450,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          550,  60, 100, 200, 0, 0x0, 0 },
                                                                
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  100, 1024, 500, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       32,   25,  960, 350, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,               192, 175, 128,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,               320, 175, 128,  50, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               448, 175, 128,  50, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,               576, 175, 128,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,               704, 175, 128,  50, 0, 0x0, 0 },
            
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          192,  75, 128, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          320,  75, 128, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          448,  75, 128, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          576,  75, 128, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          704,  75, 128, 250, 0, 0x0, 0 },
                                                                

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        512, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#endif

typedef enum    listwheel_idx_e
{
        LISTWHEEL_IDX_0         = 0,
        LISTWHEEL_IDX_1,
        LISTWHEEL_IDX_2,
        LISTWHEEL_IDX_3,
        LISTWHEEL_IDX_4,
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

static  listwheel_idx_t         listwheel_idx;
static  WM_HWIN                 hWheel;
static const char* symb[] = {
    "9", "8", "7", "6", "5", "4", "3", "2", "1", "0"
};
static const char* symb2[] = {
    "6", "5", "4", "3", "2", "1", "0"
};
static  const   size_t  symb_size = sizeof( symb ) / sizeof( char * );
static  const   size_t  symb2_size = sizeof( symb2 ) / sizeof( char * );

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static void check_port (WM_HWIN         hWin)
{
  uint32_t        port = 0;  
  uint8_t idx;

  port  += 10000 * ((symb2_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 )));
  port  += 1000  * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 )));
  port  += 100   * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 )));
  port  += 10    * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 )));
  port  += 1     * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 )));
  
  if (port > 65535)
    port = 65535;
        
  idx     = (symb2_size - 1) - (port / 10000) % 10;                              
  LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), idx );
  LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), idx );
  
  idx     = (symb_size - 1) - (port / 1000) % 10;                              
  LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), idx );
  LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), idx );
  
  idx     = (symb_size - 1) - (port / 100) % 10;                              
  LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), idx );
  LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), idx );
      
  idx     = (symb_size - 1) - (port / 10) % 10;                              
  LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), idx );
  LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), idx );
    
  idx     = (symb_size - 1) - (port / 1) % 10;                              
  LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4), idx );
  LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4), idx );
 
}


static
void    init_dialog(                                    WM_HWIN         hWin )
{
        WM_HWIN         hItem;


        WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        

        ////////////////////////////////////////////////////////
        // FOOTER AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
        BUTTON_SetFocusable(    hItem, 1                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
        BUTTON_SetFocusable(    hItem, 0                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
        BUTTON_SetFocusable(    hItem, 0                                        );
}


static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
        const   WM_HWIN         hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
        const   WM_HWIN         hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
        const   WM_HWIN         hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
        const   WM_HWIN         hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT3 );
        const   WM_HWIN         hText4  = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );

        switch( idx )
        {
                case LISTWHEEL_IDX_0:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_1:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_2:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_3:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_4:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL   );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
                        listwheel_idx   = idx;
                        break;

                default:
                        break;
        }
}


static
void    listwheel_swtch_left(                   const   WM_HWIN         hWin )
{
        if( listwheel_idx > 0 )
        {
                listwheel_idx--;
        }
        else
        {
                listwheel_idx   = (listwheel_idx_t) (LISTWHEEL_IDX_LAST - 1);
        }

        listwheel_set_focus( hWin, listwheel_idx );
}


static
void    listwheel_swtch_rght(                   const   WM_HWIN         hWin )
{
        if( listwheel_idx < (LISTWHEEL_IDX_LAST - 1) )
        {
                listwheel_idx++;
        }
        else
        {
                listwheel_idx   = LISTWHEEL_IDX_0;
        }

        listwheel_set_focus( hWin, listwheel_idx );
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
    int             idx;
    uint32_t        port = 0x00;  
    gui_id_button_t button_id;

    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    listwheel_swtch_left( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_RIGHT:
                    listwheel_swtch_rght( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_UP:
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( idx > 0 )
                    {
                        idx--;
                    }
                    else
                    {
                        idx     = LISTWHEEL_GetNumItems( hWheel ) - 1;
                    }
                    
                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    
                    check_port(pMsg->hWin);
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_DOWN:
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
                    }

                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    check_port(pMsg->hWin);
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_ESCAPE:
                    if(dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_MODBUS_PORT)
                    {
                      button_id = GUI_ID_BUTTON_NETWORK_PORT_MODBUS;
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_HTTP_PORT)
                    {
                      button_id = GUI_ID_BUTTON_NETWORK_PORT_HTTP;
                    }                       
                    
                    scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK, button_id);
                    beep_play( BEEP_SHRT );
                    break;

                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            switch( WM_GetId( pMsg->hWinSrc ) )
            {
                case GUI_ID_LISTWHEEL0:
                if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                {
                    listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );
                    beep_play( BEEP_SHRT );
                }
                else
                  check_port(pMsg->hWin);
                break;

                case GUI_ID_LISTWHEEL1:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_1 );
                        beep_play( BEEP_SHRT );
                    }
                    else                    
                      check_port(pMsg->hWin);
                    break;

                case GUI_ID_LISTWHEEL2:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_2 );
                        beep_play( BEEP_SHRT );
                    }
                    else                    
                      check_port(pMsg->hWin);
                    break;

                case GUI_ID_LISTWHEEL3:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_3 );
                        beep_play( BEEP_SHRT );
                    }
                    else                    
                      check_port(pMsg->hWin);                    
                    break;

                case GUI_ID_LISTWHEEL4:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_4 );
                        beep_play( BEEP_SHRT );
                    }
                    else                    
                      check_port(pMsg->hWin);                    
                    break;           
                    
                case GUI_ID_BUTTON_CANCEL:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        if(dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_MODBUS_PORT)
                        {
                          button_id = GUI_ID_BUTTON_NETWORK_PORT_MODBUS;
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_HTTP_PORT)
                        {
                          button_id = GUI_ID_BUTTON_NETWORK_PORT_HTTP;
                        }                     
                        
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK, button_id);
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_ENTER:
                case GUI_ID_BUTTON_DUMMY:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {                                            
                        port  += 10000 * ((symb2_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 )));
                        port  += 1000  * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 )));
                        port  += 100   * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 )));
                        port  += 10    * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 )));
                        port  += 1     * ((symb_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 )));                      
                        
                        if(dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_MODBUS_PORT)
                        {
                          dev.net->port_modbus = port;
                          send_cmd_for_nvm_write_param(NVM_REG_NET_MODBUS_PORT, dev.net->port_modbus);
                          button_id = GUI_ID_BUTTON_NETWORK_PORT_MODBUS;
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_HTTP_PORT)
                        {
                          dev.net->port_http = port;
                          send_cmd_for_nvm_write_param(NVM_REG_NET_HTTP_PORT, dev.net->port_http);      
                          button_id = GUI_ID_BUTTON_NETWORK_PORT_HTTP;
                        }

                        send_cmd_to_net_task(OS_USER_TAG_ETH_CHANGE_PARAM);
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK, button_id);
                        beep_play( BEEP_TYPE_CONFIRM );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            
            if(dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_MODBUS_PORT)
            {
              port = dev.net->port_modbus;   
            }  
            else if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_NETWORK_HTTP_PORT)
            {
              port = dev.net->port_http;   
            }       
            
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0);
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 60, symb2, symb2_size );              
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
              gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 75, symb2, symb2_size );            
#endif                        
            idx     = (symb2_size - 1) - (port / 10000) % 10;                              
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );
            
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1);
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 60, symb, sizeof( symb ) / sizeof( char * ) );              
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
              gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 75, symb, sizeof( symb ) / sizeof( char * ) );            
#endif                                                
            idx     = (symb_size - 1) - (port / 1000) % 10;                              
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );
            
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2);
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 60, symb, sizeof( symb ) / sizeof( char * ) );              
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
              gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 75, symb, sizeof( symb ) / sizeof( char * ) );            
#endif                                                            
            idx     = (symb_size - 1) - (port / 100) % 10;                              
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );
                
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3);
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 60, symb, sizeof( symb ) / sizeof( char * ) );              
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
              gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 75, symb, sizeof( symb ) / sizeof( char * ) );            
#endif                                                            
            idx     = (symb_size - 1) - (port / 10) % 10;                              
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );
                        
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4);
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 60, symb, sizeof( symb ) / sizeof( char * ) );              
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
              gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 75, symb, sizeof( symb ) / sizeof( char * ) );            
#endif                                                            
            idx     = (symb_size - 1) - (port / 1) % 10;                              
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );
                                                                         
            listwheel_idx   = LISTWHEEL_IDX_0;
            listwheel_set_focus( pMsg->hWin, listwheel_idx );

            hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
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
scr_setup_service_network_port( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
