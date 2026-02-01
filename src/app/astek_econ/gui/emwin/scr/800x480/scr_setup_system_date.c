/**
  * @file   scr_setup_system_date.c
  * @brief  Screen Setup System Date
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
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_YEAR,           100,  20, 200,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MONTH,          300,  20, 200,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DAY,            500,  20, 200,  60, 0, 0x0, 0 },

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

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_YEAR,       128,  25, 256,  75, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MONTH,      384,  25, 256,  75, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DAY,       640,  25, 256,  75, 0, 0x0, 0 },

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

static const char *year[] = {
    "2039", "2038", "2037", "2036", "2035", "2034", "2033", "2032", "2031", "2030",
    "2029", "2028", "2027", "2026", "2025", "2024", "2023", "2022", "2021", "2020",
    "2019", "2018", "2017", "2016", "2015", "2014", "2013", "2012", "2011", "2010",
    "2009", "2008", "2007", "2006", "2005", "2004", "2003", "2002", "2001", "2000"
};

static const char *month[] = {
    "12", "11", "10", "09", "08", "07", "06", "05", "04", "03", "02", "01"
};

static const char *day[] = {
    "31", "30", "29", "28", "27", "26", "25",
    "24", "23", "22", "21", "20", "19", "18",
    "17", "16", "15", "14", "13", "12", "11",
    "10", "09", "08", "07", "06", "05", "04",
    "03", "02", "01"
};

static  listwheel_idx_t         listwheel_idx;
static  WM_HWIN                 hWheel;


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

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_YEAR );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_YEAR )         );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MONTH );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MONTH )        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_DAY );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_DAY )   );

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


static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    WM_HWIN         hItem;
    int             idx;
    int             idx_year;
    int             idx_month;
    int             idx_day;
    time_t          timestamp;
    struct tm *     ts;

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
                        idx = LISTWHEEL_GetNumItems( hWheel ) - 1;
                    }

                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );

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

                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_DATE );
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
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_DATE );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_ENTER:
                case GUI_ID_BUTTON_DUMMY:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        idx_year    = 30 + (((sizeof(year) / sizeof( char * )) - 1) - LISTWHEEL_GetPos(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTWHEEL0)));
                        idx_month   = 1 + (((sizeof(month) / sizeof( char * )) - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 )));
                        idx_day     = 1 + (((sizeof(day) / sizeof( char * )) - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 )));
                        dev.mcu->rtc.set_date( idx_year, idx_month, idx_day );
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_DATE );
                        beep_play( BEEP_TYPE_CONFIRM );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            timestamp = dev.mcu->rtc.get_timestamp();
            ts = localtime(&timestamp);            

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, day, sizeof( day ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 50, day, sizeof( day ) / sizeof( char * ) );            
#endif            
            idx     = ts->tm_mday;
            LISTWHEEL_SetPos( hItem, (sizeof( day ) / sizeof( char * )) - idx );
            LISTWHEEL_SetSel( hItem, (sizeof( day ) / sizeof( char * )) - idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, month, sizeof( month ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 50, month, sizeof( month ) / sizeof( char * ) );            
#endif            
            idx     = ts->tm_mon + 1;
            LISTWHEEL_SetPos( hItem, (sizeof( month ) / sizeof( char * )) - idx );
            LISTWHEEL_SetSel( hItem, (sizeof( month ) / sizeof( char * )) - idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, year, sizeof( year ) / sizeof( char * ) );            
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 50, year, sizeof( year ) / sizeof( char * ) );            
#endif                        
            idx     = ts->tm_year + 1900 - 2000;
            LISTWHEEL_SetPos( hItem, (sizeof( year ) / sizeof( char * )) - 1 - idx );
            LISTWHEEL_SetSel( hItem, (sizeof( year ) / sizeof( char * )) - 1 - idx );

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
scr_setup_system_date( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
