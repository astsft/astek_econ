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
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480
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
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                        0, 100,1024, 500, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,   32,  25, 960, 350, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,           46,   20, 185,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,           232,  20, 185,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,           417,  20, 185,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,           602,  20, 185,  60, 0, 0x0, 0 },   
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,           788,  20, 185,  60, 0, 0x0, 0 },    
       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,     46,  200, 185,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,     232, 200, 185,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,     417, 200, 185,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,     602, 200, 185,  50, 0, 0x0, 0 },  
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS4,     788, 200, 185,  50, 0, 0x0, 0 },     
    
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,      46,  100, 185, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,      232, 100, 185, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,      417, 100, 185, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,      602, 160, 185, 125, 0, 0x0, 0 },   
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,      788, 160, 185, 125, 0, 0x0, 0 },     
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        512, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#endif

static const char* addr[] = {
    "247", "246", "245", "244", "243", "242", "241",
    "240", "239", "238", "237", "236", "235", "234", "233", "232", "231",
    "230", "229", "228", "227", "226", "225", "224", "223", "222", "221",
    "220", "219", "218", "217", "216", "215", "214", "213", "212", "211",
    "210", "209", "208", "207", "206", "205", "204", "203", "202", "201",
    "200", "199", "198", "197", "196", "195", "194", "193", "192", "191",
    "190", "189", "188", "187", "186", "185", "184", "183", "182", "181",
    "180", "179", "178", "177", "176", "175", "174", "173", "172", "171",
    "170", "169", "168", "167", "166", "165", "164", "163", "162", "161",
    "150", "149", "148", "147", "146", "145", "144", "143", "142", "141",
    "140", "139", "138", "137", "136", "135", "134", "133", "132", "131",
    "130", "129", "128", "127", "126", "125", "124", "123", "122", "121",
    "120", "119", "118", "117", "116", "115", "114", "113", "112", "111",
    "110", "109", "108", "107", "106", "105", "104", "103", "102", "101",
    "100", "99", "98", "97", "96", "95", "94", "93", "92", "91",
    "80", "79", "78", "77", "76", "75", "74", "73", "72", "71",
    "70", "69", "68", "67", "66", "65", "64", "63", "62", "61",
    "60", "59", "58", "57", "56", "55", "54", "53", "52", "51",
    "50", "49", "48", "47", "46", "45", "44", "43", "42", "41",
    "40", "39", "38", "37", "36", "35", "34", "33", "32", "31",
    "30", "29", "28", "27", "26", "25", "24", "23", "22", "21",
    "20", "19", "18", "17", "16", "15", "14", "13", "12", "11",
    "10", "9", "8", "7", "6", "5", "4", "3", "2", "1"
};

static const char* baudrate[] = {
    "115200", "57600", "38400", "19200", "9600"
};

static  const   char *          parity[]  =
{
    "N", "E", "O"
};

static  const   char *          stop_bits[]  =
{
    "1", "2"
};

static  const   size_t          addr_countof    = sizeof( addr ) / sizeof( char * );
static  const   size_t          baudrate_countof    = sizeof( baudrate ) / sizeof( char * );

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
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL0, 40, addr, sizeof(addr) / sizeof(char *) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL0, 50, addr, sizeof(addr) / sizeof(char *) );    
#endif    
    LISTWHEEL_SetPos( hItem, (addr_countof - 1) - (dev.cfg.ext_mdbs_cfg.tcp_addr - 1)  );
    LISTWHEEL_SetSel( hItem, (addr_countof - 1) - (dev.cfg.ext_mdbs_cfg.tcp_addr - 1)  );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL1, 40, addr, sizeof(addr) / sizeof(char *) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL1, 50, addr, sizeof(addr) / sizeof(char *) );    
#endif    
    LISTWHEEL_SetPos( hItem, (addr_countof - 1) - (dev.cfg.ext_mdbs_cfg.rtu_addr - 1));
    LISTWHEEL_SetSel( hItem, (addr_countof - 1) - (dev.cfg.ext_mdbs_cfg.rtu_addr - 1));    

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL2, 40, baudrate, sizeof(baudrate) / sizeof(char *) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL2, 50, baudrate, sizeof(baudrate) / sizeof(char *) );
#endif    
    LISTWHEEL_SetPos( hItem, (baudrate_countof - 1) - dev.cfg.ext_mdbs_cfg.baudrate );
    LISTWHEEL_SetSel( hItem, (baudrate_countof - 1) - dev.cfg.ext_mdbs_cfg.baudrate );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL3, 40, parity, sizeof(parity) / sizeof(char *) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL3, 50, parity, sizeof(parity) / sizeof(char *) );
#endif    
    LISTWHEEL_SetPos( hItem, dev.cfg.ext_mdbs_cfg.parity   );
    LISTWHEEL_SetSel( hItem, dev.cfg.ext_mdbs_cfg.parity   );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480    
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL4, 40, stop_bits, sizeof(stop_bits) / sizeof(char *) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL4, 50, stop_bits, sizeof(stop_bits) / sizeof(char *) );
#endif    
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
                    
                case GUI_KEY_DOWN:
                    beep_play( BEEP_SHRT );
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
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
                      
                        dev.cfg.ext_mdbs_cfg.tcp_addr    = (addr_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 )) + 1;
                        dev.cfg.ext_mdbs_cfg.rtu_addr    = (addr_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 )) + 1;                        
                        dev.cfg.ext_mdbs_cfg.baudrate    = (baudrate_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ));
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
