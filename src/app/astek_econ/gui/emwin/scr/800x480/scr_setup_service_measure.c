/**
  * @file    scr_setup_service_measure.c
  * @brief   Screen Setup - Service - Sensor
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "main.h"
#include "scr\scr.h"
#include "dev\dev.h"


/*******************************************************************************
* LOCAL TYPES
*******************************************************************************/
typedef enum    listwheel_idx_e
{
        LISTWHEEL_IDX_0         = 0,
        LISTWHEEL_IDX_1,
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

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FILTER,     100,  16, 200,  48, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_ORDER,      500,  16, 200,  48, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,     100, 160, 200,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,     500, 160, 200,  40, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,      100,  80, 200, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,      500,  80, 200, 200, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,     0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,    400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,     -1,  -1,   1,   1, 0, 0x0, 0 },
};

static const char* cutoff[] = {
    "256", "255", "254", "253", "252", "251", "250", "249", "248", "247", 
    "246", "245", "244", "243", "242", "241", "240", "239", "238", "237", 
    "236", "235", "234", "233", "232", "231", "230", "229", "228", "227", 
    "226", "225", "224", "223", "222", "221", "220", "219", "218", "217", 
    "216", "215", "214", "213", "212", "211", "210", "209", "208", "207", 
    "206", "205", "204", "203", "202", "201", "200", "199", "198", "197", 
    "196", "195", "194", "193", "192", "191", "190", "189", "188", "187", 
    "186", "185", "184", "183", "182", "181", "180", "179", "178", "177", 
    "176", "175", "174", "173", "172", "171", "170", "169", "168", "167", 
    "166", "165", "164", "163", "162", "161", "160", "159", "158", "157", 
    "156", "155", "154", "153", "152", "151", "150", "149", "148", "147", 
    "146", "145", "144", "143", "142", "141", "140", "139", "138", "137", 
    "136", "135", "134", "133", "132", "131", "130", "129", "128", "127", 
    "126", "125", "124", "123", "122", "121", "120", "119", "118", "117", 
    "116", "115", "114", "113", "112", "111", "110", "109", "108", "107", 
    "106", "105", "104", "103", "102", "101", "100", "99", "98", "97", 
    "96", "95", "94", "93", "92", "91", "90", "89", "88", "87", 
    "86", "85", "84", "83", "82", "81", "80", "79", "78", "77", 
    "76", "75", "74", "73", "72", "71", "70", "69", "68", "67", 
    "66", "65", "64", "63", "62", "61", "60", "59", "58", "57", 
    "56", "55", "54", "53", "52", "51", "50", "49", "48", "47", 
    "46", "45", "44", "43", "42", "41", "40", "39", "38", "37", 
    "36", "35", "34", "33", "32", "31", "30", "29", "28", "27", 
    "26", "25", "24", "23", "22", "21", "20", "19", "18", "17", 
    "16", "15", "14", "13", "12", "11", "10", "9", "8", "7", 
    "6", "5", "4", "3", "2", "1"
};

static const char* order[] = {
    "4", "3", "2", "1"
};

static  const   size_t          cutoff_size    = sizeof( cutoff ) / sizeof( char * );
static  const   size_t          order_size    = sizeof( order ) / sizeof( char * );

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

    switch( idx )
    {
        case LISTWHEEL_IDX_0:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
            listwheel_idx   = idx;
            break;
        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
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

    scr_init_text( hWin, GUI_ID_TEXT_FILTER, l10n_str_get( dev.cfg.lang, L10N_STR_ID_FILTER_CUTOFF )  );
    scr_init_text( hWin, GUI_ID_TEXT_ORDER, l10n_str_get( dev.cfg.lang, L10N_STR_ID_FILTER_ORDER )   );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL0, 40, cutoff, sizeof(cutoff) / sizeof(char *) );
    LISTWHEEL_SetPos( hItem, (cutoff_size - 1) - (dev.cfg.lpf_cutoff - 1 ));
    LISTWHEEL_SetSel( hItem, (cutoff_size - 1) - (dev.cfg.lpf_cutoff - 1 ));
    //LISTWHEEL_SetPos( hItem, 0  );
    //LISTWHEEL_SetSel( hItem, 0  );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL1, 40, order, sizeof(order) / sizeof(char *) );
    LISTWHEEL_SetPos( hItem, (order_size - 1) - (dev.cfg.lpf_order - 1) );
    LISTWHEEL_SetSel( hItem, (order_size - 1) - (dev.cfg.lpf_order - 1) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS0 );
    TEXT_SetBkColor(        hItem, GUI_GRAY                                 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS1 );
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
                    scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_MEASURE );
                    break;
                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
                {
                    case GUI_ID_BUTTON_CANCEL:
                        scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_MEASURE );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
                        task_ibus_sens_conf_lpf_update( (cutoff_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0)) + 1,
                                                        (order_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 )) + 1);
                        scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_MEASURE );
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
scr_setup_service_measure( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
