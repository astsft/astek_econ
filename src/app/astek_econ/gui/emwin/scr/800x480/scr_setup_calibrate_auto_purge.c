/**
  * @file    scr_setup_calibrate_auto_purge.c
  * @brief   Screen Setup Calibrate Auto Purge
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

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MINUTES,        200,  20, 200,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SECONDS,        400,  20, 200,  60, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,         200, 160, 200,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         400, 160, 200,  40, 0, 0x0, 0 },    

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          200,  80, 200, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          400,  80, 200, 200, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  const   char *                  minutes[]       =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
};

static  const   char *                  seconds[]       =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
};

static  const   size_t          list_countof_minutes    = sizeof( minutes ) / sizeof( char * );
static  const   size_t          list_countof_seconds    = sizeof( seconds ) / sizeof( char * );

static  listwheel_idx_t         listwheel_idx;
static  WM_HWIN         hWheel;


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
                                list_countof    = sizeof( minutes ) / sizeof( char * );
                                break;
        case LISTWHEEL_IDX_1:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                                list_countof    = sizeof( seconds ) / sizeof( char * );
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
                                list_countof    = sizeof( minutes ) / sizeof( char * );
                                break;
        case LISTWHEEL_IDX_1:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                                list_countof    = sizeof( seconds ) / sizeof( char * );
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
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN         hItem;


    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );

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
    int             sec, min;


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    beep_play( BEEP_SHRT );
                    listwheel_swtch_left(pMsg->hWin);
                    break;

                case GUI_KEY_RIGHT:
                    beep_play( BEEP_SHRT );
                    listwheel_swtch_rght( pMsg->hWin );
                    break;

                case GUI_KEY_UP:
                    beep_play( BEEP_SHRT );
                    listwheel_increment( pMsg->hWin, listwheel_idx );
                    break;

                case GUI_KEY_DOWN:
                    beep_play( BEEP_SHRT );
                    listwheel_decrement( pMsg->hWin, listwheel_idx );
                    break;

                case GUI_KEY_ESCAPE:
                    beep_play( BEEP_SHRT );
                    scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON4 );
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
                   default:
                        break;
                }
            }          
          
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
              switch( WM_GetId(pMsg->hWinSrc) )
              {
                  case GUI_ID_BUTTON_CANCEL:
                      scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON4 );
                      beep_play( BEEP_SHRT );
                      break;
  
                  case GUI_ID_BUTTON_ENTER:
                  case GUI_ID_BUTTON_DUMMY:
                          //hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                          //min     = LISTWHEEL_GetPos( hWheel );
                          //hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
                          //sec     = LISTWHEEL_GetPos( hWheel );
                          min     = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                          sec     = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) );
                          //term_cal_auto_flow_seconds_set( &dev, (min * 60) + sec );
                          scr_switch( SCR_IDX_SETUP_CALIBRATION_AUTO, GUI_ID_BUTTON4 );
                          beep_play( BEEP_TYPE_CONFIRM );
                      break;
  
                  default:
                      break;
              }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );

            //sec     = term_cal_auto_flow_seconds_get( &dev );
            sec     = 0;
            min     = sec / 60;
            sec     -= min * 60;

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, minutes, list_countof_minutes);
            LISTWHEEL_SetPos( hItem, min );
            LISTWHEEL_SetSel( hItem, min );
            
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
            gui_init_listwheel(  pMsg->hWin, GUI_ID_LISTWHEEL1, 40, seconds, list_countof_seconds);
            LISTWHEEL_SetPos( hItem, sec );
            LISTWHEEL_SetSel( hItem, sec );            

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
scr_setup_calibration_auto_flow( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
