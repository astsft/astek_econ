/**
  * @file    scr_setup_calibrate_auto_cycle.c
  * @brief   Screen Setup Calibrate Auto Cycle
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
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DAY,            250,  20, 100,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_HOUR,           450,  20, 100,  60, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,         250, 160, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         450, 160, 100,  40, 0, 0x0, 0 },
    
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          250,  80, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          450,  80, 100, 200, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  const   char *                  days[]          =
{
        "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11",
        "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23",
        "24", "25", "26", "27", "28", "29", "30", "31",
};

static  const   char *                  hours[]         =
{
        "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "20", "21", "22", "23",
};

static  const   size_t          list_countof_days    = sizeof( days ) / sizeof( char * );
static  const   size_t          list_countof_hours    = sizeof( hours ) / sizeof( char * );

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

    switch( idx )
    {
        case LISTWHEEL_IDX_0:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            listwheel_idx   = idx;
            break;

        default:
            break;
    }
}


static
void
listwheel_swtch_left(   const   WM_HWIN                 hWin )
{
    if( listwheel_idx > 0 )
    {
        listwheel_idx--;
    }
    else
    {
        listwheel_idx  = (listwheel_idx_t) (LISTWHEEL_IDX_LAST - 1);
    }

    listwheel_set_focus( hWin, listwheel_idx );
}

static
void
listwheel_swtch_rght(   const   WM_HWIN                 hWin )
{
    if( listwheel_idx < (LISTWHEEL_IDX_LAST - 1) )
    {
        listwheel_idx++;
    }
    else
    {
        listwheel_idx  = LISTWHEEL_IDX_0;
    }

    listwheel_set_focus( hWin, listwheel_idx );
}

static
int
listwheel_increment(            const   WM_HWIN                 hWin,
                                        listwheel_idx_t         idx )
{
    WM_HWIN     hItem;
    size_t      list_countof;
    int         val;


    switch( idx )
    {
        case LISTWHEEL_IDX_0:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 ); 
                                list_countof    = sizeof( days ) / sizeof( char * );
                                break;
        case LISTWHEEL_IDX_1:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                                list_countof    = sizeof( hours ) / sizeof( char * );
                                break;
        default:
            return( 0 );
    }

    val     = LISTWHEEL_GetPos( hItem );

    if( ++val >= list_countof )
    {
        val       = 0;
    }

    LISTWHEEL_SetPos( hItem, val );
    LISTWHEEL_SetSel( hItem, val );
    return( val );
}

static
int
listwheel_decrement(            const   WM_HWIN                 hWin,
                                        listwheel_idx_t         idx )
{
    WM_HWIN     hItem;
    int         val;
    size_t      list_countof;


    switch( idx )
    {
        case LISTWHEEL_IDX_0:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
                                list_countof    = sizeof( days ) / sizeof( char * );
                                break;
        case LISTWHEEL_IDX_1:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                                list_countof    = sizeof( hours ) / sizeof( char * );
                                break;
        default:
                return( 0 );
    }

    val     = LISTWHEEL_GetPos( hItem );

    if( val > 0 )
    {
        val--;
    }
    else
    {
        val       = list_countof-1;
    }

    LISTWHEEL_SetPos( hItem, val );
    LISTWHEEL_SetSel( hItem, val );
    return( val );
}


static
void
init_dialog(                                    WM_HWIN         hWin )
{
    WM_HWIN         hItem;


    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_DAY );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_DAY )         );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_HOUR );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_HOUR )        );

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
dialog_callback(                                WM_MESSAGE *    pMsg )
{
    WM_HWIN         hItem;
    int             idx;
    int             day, hour;


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    listwheel_swtch_left(pMsg->hWin);
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    listwheel_swtch_rght( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_UP:
                    listwheel_increment( pMsg->hWin, listwheel_idx );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_DOWN:
                    listwheel_decrement( pMsg->hWin, listwheel_idx );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON_CYCLE );
                    beep_play( BEEP_SHRT );
                    break;
                default:
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
                        hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                        day     = LISTWHEEL_GetPos( hWheel );
                        hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
                        hour    = LISTWHEEL_GetPos( hWheel );
                        //term_cal_auto_cycle_hours_set( &dev, day * 24 + hour );
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON_CYCLE );
                        beep_play( BEEP_TYPE_CONFIRM );
                        break;
                        
                    case GUI_ID_BUTTON_CANCEL:
                        scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON_CYCLE );
                        beep_play( BEEP_SHRT );
                        break;
                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );

            //hour    = term_cal_auto_cycle_hours_get( &dev );
            hour    = 0;
            day     = hour / 24;
            hour    -= (day * 24);

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, days, list_countof_days );
            LISTWHEEL_SetPos( hItem, day );
            LISTWHEEL_SetSel( hItem, day );  


            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, hours, list_countof_hours);
            LISTWHEEL_SetPos( hItem, hour );
            LISTWHEEL_SetSel( hItem, hour );
            
            listwheel_idx   = LISTWHEEL_IDX_0;
            listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );            
            
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
scr_setup_calibration_auto_cycle( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
