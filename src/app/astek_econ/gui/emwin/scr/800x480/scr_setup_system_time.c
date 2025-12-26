/**
  * @file   scr_setup_system_time.c
  * @brief  Screen Setup System Time
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE TYPES
*******************************************************************************/
typedef enum    listwheel_idx_e
{
    LISTWHEEL_IDX_0         = 0,
    LISTWHEEL_IDX_1,
    LISTWHEEL_IDX_2,
    LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

/*******************************************************************************
* PRIVATE VARIBLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_HOUR,           100,  20, 200,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MINUTES,        300,  20, 200,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SECONDS,        500,  20, 200,  60, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,               100, 160, 200,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,               300, 160, 200,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               500, 160, 200,  40, 0, 0x0, 0 },

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          100,  80, 200, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          300,  80, 200, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          500,  80, 200, 200, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                        0, 100,1024, 500, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,   32,  25, 960, 350, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_HOUR,       128,  25, 256,  75, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MINUTES,    384,  25, 256,  75, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SECONDS,    640,  25, 256,  75, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,           128, 200, 256,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,           384, 200, 256,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,           640, 200, 256,  50, 0, 0x0, 0 },

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,      128, 100, 256, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,      384, 100, 256, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,      640, 100, 256, 250, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,     0, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,    512, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,     -1,  -1,   1,   1, 0, 0x0, 0 },
};
#endif

//static  const   char *                  hours[]         =
//{
//    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
//    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
//    "20", "21", "22", "23",
//};
//
//static  const   char *                  minutes[]       =
//{
//    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
//    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
//    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
//    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
//    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
//    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
//};
//
//static  const   char *                  seconds[]       =
//{
//    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
//    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
//    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
//    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
//    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
//    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
//};


static const char* hours[] = {
    "23", "22", "21", "20", "19", "18", "17", "16", "15", "14",
    "13", "12", "11", "10", "09", "08", "07", "06", "05", "04",
    "03", "02", "01", "00"
};

static const char* minutes[] = {
    "59", "58", "57", "56", "55", "54", "53", "52", "51", "50",
    "49", "48", "47", "46", "45", "44", "43", "42", "41", "40",
    "39", "38", "37", "36", "35", "34", "33", "32", "31", "30",
    "29", "28", "27", "26", "25", "24", "23", "22", "21", "20",
    "19", "18", "17", "16", "15", "14", "13", "12", "11", "10",
    "09", "08", "07", "06", "05", "04", "03", "02", "01", "00"
};

static const char* seconds[] = {
    "59", "58", "57", "56", "55", "54", "53", "52", "51", "50",
    "49", "48", "47", "46", "45", "44", "43", "42", "41", "40",
    "39", "38", "37", "36", "35", "34", "33", "32", "31", "30",
    "29", "28", "27", "26", "25", "24", "23", "22", "21", "20",
    "19", "18", "17", "16", "15", "14", "13", "12", "11", "10",
    "09", "08", "07", "06", "05", "04", "03", "02", "01", "00"
};

uint32_t hours_size =  sizeof( hours ) / sizeof( char * );
uint32_t minutes_size =  sizeof( minutes ) / sizeof( char * );
uint32_t seconds_size =  sizeof( seconds ) / sizeof( char * );

static  listwheel_idx_t     listwheel_idx;
static  WM_HWIN             hWheel;

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN         hItem;


    //WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_HOUR );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_HOUR )        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MINUTES );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MINUTES )      );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SECONDS );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SECONDS )      );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL ) );
    BUTTON_SetFocusable(    hItem, 0 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
    BUTTON_SetFocusable(    hItem, 0 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER ) );
}


static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
    const   WM_HWIN     hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
    const   WM_HWIN     hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    const   WM_HWIN     hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );


    switch( idx )
    {
        case LISTWHEEL_IDX_0:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_2:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
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
dialog_callback(                        WM_MESSAGE *            pMsg )
{
    WM_HWIN     hItem;
    int         idx;
    int         idx_hour;
    int         idx_minute;
    int         idx_second;


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    beep_play( BEEP_SHRT );
                    listwheel_swtch_left( pMsg->hWin );
                    break;

                case GUI_KEY_RIGHT:
                    beep_play( BEEP_SHRT );
                    listwheel_swtch_rght( pMsg->hWin );
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
                        idx = LISTWHEEL_GetNumItems( hWheel ) - 1;
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
                    scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_TIME );
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
                    break;

                case GUI_ID_LISTWHEEL1:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_1 );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_LISTWHEEL2:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_2 );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_CANCEL:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_TIME );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_ENTER:
                case GUI_ID_BUTTON_DUMMY:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        idx_hour    = (hours_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                        idx_minute  = (minutes_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) );
                        idx_second  = (seconds_size - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ) );
                        dev.mcu->rtc.set_time( idx_hour, idx_minute, idx_second );
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_TIME );
                        beep_play( BEEP_TYPE_CONFIRM );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, seconds, seconds_size );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 50, seconds, seconds_size  );
#endif            
            idx     = dev.mcu->rtc.get_second();
            LISTWHEEL_SetPos( hItem, (seconds_size - 1) - idx );
            LISTWHEEL_SetSel( hItem, (seconds_size - 1) - idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, minutes, minutes_size );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 50, minutes, minutes_size );            
#endif            
            idx     = dev.mcu->rtc.get_minute();
            LISTWHEEL_SetPos( hItem, (minutes_size - 1)  - idx );
            LISTWHEEL_SetSel( hItem, (minutes_size - 1)  - idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, hours, hours_size );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 50, hours, hours_size );            
#endif                        
            idx     = dev.mcu->rtc.get_hour();
            LISTWHEEL_SetPos( hItem, (hours_size - 1) - idx );
            LISTWHEEL_SetSel( hItem, (hours_size - 1) - idx );

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
scr_setup_system_time( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
