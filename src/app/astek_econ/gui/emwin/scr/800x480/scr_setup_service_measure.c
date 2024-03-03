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

static  const   char *          filter[]  =
{
    "1", "2", "3", "4", "5", "6", "7", "8",
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
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL0, 40, filter, sizeof(filter) / sizeof(char *) );
    //LISTWHEEL_SetPos( hItem, dev.sens->conf.lpf_cutoff  );
    //LISTWHEEL_SetSel( hItem, dev.sens->conf.lpf_cutoff  );
    //LISTWHEEL_SetPos( hItem, 0  );
    //LISTWHEEL_SetSel( hItem, 0  );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL1, 40, filter, sizeof(filter) / sizeof(char *) );
    //LISTWHEEL_SetPos( hItem, dev.sens->conf.lpf_order   );
    //LISTWHEEL_SetSel( hItem, dev.sens->conf.lpf_order   );
    //LISTWHEEL_SetPos( hItem, 0   );
    //LISTWHEEL_SetSel( hItem, 0   );


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
                //case GUI_KEY_UP:
                //    beep_play( BEEP_TYPE_EMPTY );
                //    //term_thld_idx_set( dev.ibus, 0 );
                //    //WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON1 ) );
                //    break;
                //case GUI_KEY_DOWN:
                //    beep_play( BEEP_TYPE_EMPTY );
                //    //term_thld_idx_set( dev.ibus, 1 );
                //    //WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON2 ) );
                //    break;
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
                        task_ibus_sens_conf_lpf_update( LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) ),
                                                        LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) )   );
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
