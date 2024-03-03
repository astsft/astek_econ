/**
  * @file    scr_setup_calibrate_auto_launch.c
  * @brief   Screen Setup Calibrate Auto Launch
  * @author  Igor T. <research.tahoe@gmail.com>
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
        LISTWHEEL_IDX_3,
        LISTWHEEL_IDX_4,
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_YEAR,           150,  20, 100,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MONTH,          250,  20, 100,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DAY,            350,  20, 100,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_HOUR,           450,  20, 100,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MINUTES,        550,  20, 100,  60, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,         150, 160, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         250, 160, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,         350, 160, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,         450, 160, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS4,         550, 160, 100,  40, 0, 0x0, 0 },


    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          150,  80, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          250,  80, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          350,  80, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          450,  80, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          550,  80, 100, 200, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  const   char *                  year[]          =
{
    "2020", "2021", "2022", "2023", "2024", "2025", "2026", "2027", "2028", "2029",
    "2030", "2031", "2032", "2023", "2034", "2035", "2036", "2037", "2038", "2039",
};

static  const   char *                  month[]         =
{
    "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12",
};

static  const   char *                  day[]           =
{
    "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12",
    "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24",
    "25", "26", "27", "28", "29", "30", "31",
};

static  const   char *                  hour[]          =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23",
};

static  const   char *                  minute[]        =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
};

static  listwheel_idx_t         listwheel_idx;
static  WM_HWIN                 hWheel;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
listwheel_set_focus(            const   WM_HWIN                 hWin,
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
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_2:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_3:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_4:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL   );
            listwheel_idx   = idx;
            break;

        default:
            break;
    }
}


static
void
listwheel_next(                                 const   WM_HWIN         hWin,
                                                const   int             dir )
{
    if( dir != 0 )
    {
        if( listwheel_idx < (LISTWHEEL_IDX_LAST - 1) )
        {
            listwheel_idx++;
        }
        else
        {
            listwheel_idx   = LISTWHEEL_IDX_0;
        }
    }
    else
    {
        if( listwheel_idx > 0 )
        {
            listwheel_idx--;
        }
        else
        {
            listwheel_idx   = (listwheel_idx_t) (LISTWHEEL_IDX_LAST - 1);
        }
    }


    switch( listwheel_idx )
    {
        case LISTWHEEL_IDX_0:   listwheel_set_focus( hWin, LISTWHEEL_IDX_0 ); break;
        case LISTWHEEL_IDX_1:   listwheel_set_focus( hWin, LISTWHEEL_IDX_1 ); break;
        case LISTWHEEL_IDX_2:   listwheel_set_focus( hWin, LISTWHEEL_IDX_2 ); break;
        case LISTWHEEL_IDX_3:   listwheel_set_focus( hWin, LISTWHEEL_IDX_3 ); break;
        case LISTWHEEL_IDX_4:   listwheel_set_focus( hWin, LISTWHEEL_IDX_4 ); break;
        case LISTWHEEL_IDX_LAST:
        default:
            break;
    }

    switch( listwheel_idx )
    {
        case LISTWHEEL_IDX_0:   hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );  break;
        case LISTWHEEL_IDX_1:   hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );  break;
        case LISTWHEEL_IDX_2:   hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );  break;
        case LISTWHEEL_IDX_3:   hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );  break;
        case LISTWHEEL_IDX_4:   hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );  break;
        case LISTWHEEL_IDX_LAST:
        default:
            break;
    }
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

        ////////////////////////////////////////////////////////
        // FOOTER AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
        BUTTON_SetFocusable(    hItem, 0                                        );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
        BUTTON_SetFocusable(    hItem, 0                                        );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
}


static
void
dialog_callback(                                        WM_MESSAGE *    pMsg )
{
    int             idx;
    time_t          t;
	struct  tm *    ts;


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    listwheel_next( pMsg->hWin, 0 );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    listwheel_next( pMsg->hWin, 1 );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_UP:
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
                    }
                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_DOWN:
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
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON_LAUNCH );
                    beep_play( BEEP_SHRT );
                    break;
                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if(         pMsg->Data.v == WM_NOTIFICATION_CLICKED )
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
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
                        //ts->tm_year     = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                        //ts->tm_mon      = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) );
                        //ts->tm_mday     = 1 + LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ) );
                        //ts->tm_hour     = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ) );
                        //ts->tm_min      = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 ) );
                        //t               = mktime( ts );
                        //term_cal_auto_start_timestamp_set( &dev, t );
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON_LAUNCH );
                        beep_play( BEEP_TYPE_CONFIRM );
                        break;
                        
                     case GUI_ID_BUTTON_CANCEL:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON_LAUNCH );
                        beep_play( BEEP_SHRT );
                        break;                       
                        
                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );

            t       = 0;
            ts      = gmtime( &t );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, year, sizeof(year) / sizeof(char *) );
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ), ts->tm_year );
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ), ts->tm_year );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, month, sizeof(month) / sizeof(char *) );
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ), ts->tm_mon );
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ), ts->tm_mon );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, day, sizeof(day) / sizeof(char *) );
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ), ts->tm_mday - 1 );
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ), ts->tm_mday - 1 );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, hour, sizeof(hour) / sizeof(char *) );
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ), ts->tm_hour );
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ), ts->tm_hour );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 40, minute, sizeof(minute) / sizeof(char *) );
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 ), ts->tm_min );
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 ), ts->tm_min );

            listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );

            listwheel_idx   = LISTWHEEL_IDX_0;

            hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
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
scr_setup_calibration_auto_start( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
