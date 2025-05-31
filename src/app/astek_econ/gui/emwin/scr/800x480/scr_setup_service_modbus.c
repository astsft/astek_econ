/**
  * @file    scr_setup_service_measure.c
  * @brief   Screen Setup - Service - Sensor
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "main.h"
#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"

/*******************************************************************************
* LOCAL TYPES
*******************************************************************************/
typedef enum    listwheel_idx_e
{
        LISTWHEEL_IDX_0         = 0,
        LISTWHEEL_IDX_1,
        LISTWHEEL_IDX_2,
        LISTWHEEL_IDX_3,
        LISTWHEEL_IDX_4,
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
extern  dev_t           dev;

/*******************************************************************************
* LOCAL VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                        0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,   25,  25, 750, 350, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,            36,  16, 145,  48, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,           181,  16, 145,  48, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,           326,  16, 145,  48, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,           471,  16, 145,  48, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,           616,  16, 145,  48, 0, 0x0, 0 },    
       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,      36, 160, 145,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,     181, 160, 145,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,     326, 160, 145,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,     471, 160, 145,  40, 0, 0x0, 0 },  
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS4,     616, 160, 145,  40, 0, 0x0, 0 },      
    
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,       36,  80, 145, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,      181,  80, 145, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,      326,  80, 145, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,      471, 128, 145, 100, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,      616, 128, 145, 100, 0, 0x0, 0 },    

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,     0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,    400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,     -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  const   char *          addr[]  =
{
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
    "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
    "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60",    
    "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80",        
    "91", "92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110",            
    "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130",    
    "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150",        
    "161", "162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173", "174", "175", "176", "177", "178", "179", "180",            
    "181", "182", "183", "184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200",
    "201", "202", "203", "204", "205", "206", "207", "208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220",                              
    "221", "222", "223", "224", "225", "226", "227", "228", "229", "230", "231", "232", "233", "234", "235", "236", "237", "238", "239", "240",                                  
    "241", "242", "243", "244", "245", "246", "247",    
};

static  const   char *          baudrate[]  =
{
    "9600", "19200", "38400", "57600", "115200",
};

static  const   char *          parity[]  =
{
    "N", "E", "O"
};

static  const   char *          stop_bits[]  =
{
    "1", "2"
};

static  listwheel_idx_t listwheel_idx;
static  WM_HWIN         hWheel;

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
    const   WM_HWIN     hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS0 );
    const   WM_HWIN     hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS1 );
    const   WM_HWIN     hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS2 );
    const   WM_HWIN     hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS3 );            
    const   WM_HWIN     hText4  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS4 );                

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
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );                   
            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );                                   
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
            listwheel_idx   = idx;
            break;
        case LISTWHEEL_IDX_3:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL );                      
            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );                                   
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
            listwheel_idx   = idx;
            break;
        case LISTWHEEL_IDX_4:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );                      
            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL );                                   
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
            listwheel_idx   = idx;
            break;            
         
        default:
            break;
    }
}

static
void
listwheel_swtch_left(                   const   WM_HWIN         hWin )
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
void
listwheel_swtch_rght(                   const   WM_HWIN         hWin )
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

static
void
init_listwheel(                                         WM_HWIN         hWin )
{
    hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
}

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

    scr_init_text( hWin, GUI_ID_TEXT1, "TCP ID" );
    scr_init_text( hWin, GUI_ID_TEXT2, "RTU ID" );    
    scr_init_text( hWin, GUI_ID_TEXT3, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BAUDRATE )   );
    scr_init_text( hWin, GUI_ID_TEXT4, l10n_str_get( dev.cfg.lang, L10N_STR_ID_PARITY )   );
    scr_init_text( hWin, GUI_ID_TEXT5, l10n_str_get( dev.cfg.lang, L10N_STR_ID_STOP_BITS )   );    

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL0, 40, addr, sizeof(addr) / sizeof(char *) );
    LISTWHEEL_SetPos( hItem, dev.cfg.ext_mdbs_cfg.tcp_addr - 1  );
    LISTWHEEL_SetSel( hItem, dev.cfg.ext_mdbs_cfg.tcp_addr - 1  );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL1, 40, addr, sizeof(addr) / sizeof(char *) );
    LISTWHEEL_SetPos( hItem, dev.cfg.ext_mdbs_cfg.rtu_addr - 1  );
    LISTWHEEL_SetSel( hItem, dev.cfg.ext_mdbs_cfg.rtu_addr - 1  );    

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL2, 40, baudrate, sizeof(baudrate) / sizeof(char *) ); 
    LISTWHEEL_SetPos( hItem, dev.cfg.ext_mdbs_cfg.baudrate   );
    LISTWHEEL_SetSel( hItem, dev.cfg.ext_mdbs_cfg.baudrate   );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL3, 40, parity, sizeof(parity) / sizeof(char *) );  
    LISTWHEEL_SetPos( hItem, dev.cfg.ext_mdbs_cfg.parity   );
    LISTWHEEL_SetSel( hItem, dev.cfg.ext_mdbs_cfg.parity   );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );  
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL4, 40, stop_bits, sizeof(stop_bits) / sizeof(char *) );
    LISTWHEEL_SetPos( hItem, dev.cfg.ext_mdbs_cfg.stop_bits );
    LISTWHEEL_SetSel( hItem, dev.cfg.ext_mdbs_cfg.stop_bits );
        
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS0 );
    TEXT_SetBkColor(        hItem, GUI_GRAY                                 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS1 );
    TEXT_SetBkColor(        hItem, GUI_GRAY                                 );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS2 );
    TEXT_SetBkColor(        hItem, GUI_GRAY                                 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS3 );
    TEXT_SetBkColor(        hItem, GUI_GRAY                                 );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS4 );
    TEXT_SetBkColor(        hItem, GUI_GRAY                                 );    

    
    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
    BUTTON_SetFocusable(    hItem, 0 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
    BUTTON_SetFocusable(    hItem, 0 );
}


static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    int             idx;

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
                    beep_play( BEEP_SHRT );
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
                    }
                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    break;
                    
                case GUI_KEY_DOWN:
                    beep_play( BEEP_SHRT );
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
                    break;
                    
                case GUI_KEY_ESCAPE:
                    beep_play( BEEP_SHRT );
                    scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_MODBUS );
                    break;
                    
                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
                {
                    case GUI_ID_LISTWHEEL0:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_LISTWHEEL1:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_1 );
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_LISTWHEEL2:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_2 );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_LISTWHEEL3:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_3 );
                        beep_play( BEEP_SHRT );
                        break;  
                        
                    case GUI_ID_LISTWHEEL4:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_4 );
                        beep_play( BEEP_SHRT );
                        break;                          
                        
                    default:
                        break;
                }
            }
          
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
                {
                    case GUI_ID_BUTTON_CANCEL:
                        scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_MODBUS );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
                      
                        dev.cfg.ext_mdbs_cfg.tcp_addr        = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 )) + 1;
                        dev.cfg.ext_mdbs_cfg.rtu_addr        = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 )) + 1;                        
                        dev.cfg.ext_mdbs_cfg.baudrate    = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ));
                        dev.cfg.ext_mdbs_cfg.parity      = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ));
                        dev.cfg.ext_mdbs_cfg.stop_bits   = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 ));
                        
                        send_cmd_for_nvm_write_param (NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);
                        task_m2m_ext_mdbs_reinit();

                        scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_MODBUS );
                        beep_play( BEEP_TYPE_CONFIRM );
                        break;

                    default:
                        //beep_play( BEEP_TYPE_ERROR );
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );

            listwheel_idx   = LISTWHEEL_IDX_0;
            init_listwheel( pMsg->hWin );
            listwheel_set_focus( pMsg->hWin, listwheel_idx );
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
scr_setup_service_modbus( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
