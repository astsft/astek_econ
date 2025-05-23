/**
  * @file    scr_setup_service_entry.c
  * @brief   Screen Setup Calibrate
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
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,         200, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         300, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,         400, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,         500, 140, 100,  40, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          200,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          300,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          400,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          500,  60, 100, 200, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

//static  const   char *  symb[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };
static  const   char *  list[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };
static  const   size_t  list_countof    = sizeof( list ) / sizeof( char * );

static  listwheel_idx_t listwheel_idx;
static  WM_HWIN         hWheel;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
/*
static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
    const   WM_HWIN     hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS0 );
    const   WM_HWIN     hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS1 );
    const   WM_HWIN     hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS2 );
    const   WM_HWIN     hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS3 );


    switch( idx )
    {
        case LISTWHEEL_IDX_0:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_2:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_3:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
            listwheel_idx   = idx;
            break;

        default:
            break;
    }
}
*/

static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
    const   WM_HWIN     hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS0 );
    const   WM_HWIN     hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS1 );
    const   WM_HWIN     hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS2 );
    const   WM_HWIN     hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS3 );


    switch( idx )
    {
        case LISTWHEEL_IDX_0:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_2:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_3:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
            listwheel_idx   = idx;
            break;

        default:
            break;
    }
}




/*
static
void
listwheel_move(                                 const   WM_HWIN         hWin,
                                                const   int             dir )
{
        LISTWHEEL_SetBkColor(   hWheel, LISTWHEEL_CI_UNSEL,     GUI_BLACK        );
        LISTWHEEL_SetBkColor(   hWheel, LISTWHEEL_CI_SEL,       GUI_BLACK        );

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
                case LISTWHEEL_IDX_0:
                        hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
                        break;

                case LISTWHEEL_IDX_1:
                        hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                        break;

                case LISTWHEEL_IDX_2:
                        hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
                        break;

                case LISTWHEEL_IDX_3:
                        hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
                        break;

                case LISTWHEEL_IDX_LAST:
                default:
                        break;
        }

        LISTWHEEL_SetBkColor(   hWheel, LISTWHEEL_CI_UNSEL,     GUI_BLACK        );
        LISTWHEEL_SetBkColor(   hWheel, LISTWHEEL_CI_SEL,       GUI_GRAY_2F      );
}
*/

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





/*
static
void
setup_listwheel(                                        WM_HWIN         hWin,
                                                const   char **         list,
                                                const   int             list_countof )
{
        const   int     y_size          = WM_GetWindowSizeY( hWin );
        const   size_t  line_height     = 40;


        LISTWHEEL_SetFont(      hWin,  &GUI_FontTahoma40                        );
        LISTWHEEL_SetBkColor(   hWin,  LISTWHEEL_CI_UNSEL,     GUI_BLACK        );
        LISTWHEEL_SetBkColor(   hWin,  LISTWHEEL_CI_SEL,       GUI_BLACK        );
        LISTWHEEL_SetLBorder(   hWin,  20                                       );
        LISTWHEEL_SetLineHeight( hWin, line_height                              );
        LISTWHEEL_SetRBorder(   hWin,  20                                       );
        LISTWHEEL_SetTextAlign( hWin,  GUI_TA_HCENTER | GUI_TA_VCENTER          );
        LISTWHEEL_SetTextColor( hWin,  LISTWHEEL_CI_UNSEL,     GUI_GRAY         );
        LISTWHEEL_SetTextColor( hWin,  LISTWHEEL_CI_SEL,       GUI_BLUE         );

        for( int i = 0; i < list_countof; i++ )
        {
                LISTWHEEL_AddString( hWin, list[i] );
        }

        LISTWHEEL_SetSnapPosition( hWin, (y_size/2 - line_height/2) );
}
*/

static
void
init_listwheel(                                         WM_HWIN         hWin )
{
        //WM_HWIN         hItem;
        //int             idx;


        //hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
        //setup_listwheel( hItem, symb, sizeof( symb ) / sizeof( char * ) );
        //idx     = 0;
        //LISTWHEEL_SetPos( hItem, idx );
        //LISTWHEEL_SetSel( hItem, idx );

        //hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
        //setup_listwheel( hItem, symb, sizeof( symb ) / sizeof( char * ) );
        //idx     = 0;
        //LISTWHEEL_SetPos( hItem, idx );
        //LISTWHEEL_SetSel( hItem, idx );

        //hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
        //setup_listwheel( hItem, symb, sizeof( symb ) / sizeof( char * ) );
        //idx     = 0;
        //LISTWHEEL_SetPos( hItem, idx );
        //LISTWHEEL_SetSel( hItem, idx );

        //hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
        //setup_listwheel( hItem, symb, sizeof( symb ) / sizeof( char * ) );
        //idx     = 0;
        //LISTWHEEL_SetPos( hItem, idx );
        //LISTWHEEL_SetSel( hItem, idx );

        //LISTWHEEL_SetBkColor(   hItem,  LISTWHEEL_CI_UNSEL, GUI_BLACK        );
        //LISTWHEEL_SetBkColor(   hItem,  LISTWHEEL_CI_SEL,   GUI_GRAY_2F      );

        //listwheel_idx   = LISTWHEEL_IDX_0;

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
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );

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
dialog_callback(                                        WM_MESSAGE *    pMsg )
{
    union
    {
        uint8_t     u08[ 4];
        uint32_t    u32;
    } password;

    int             idx;


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    //listwheel_move( pMsg->hWin, 0 );
                    listwheel_swtch_left( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_RIGHT:
                    //listwheel_move( pMsg->hWin, 1 );
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
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_SERVICE );
                    beep_play( BEEP_SHRT );
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
                    default:
                        break;
                }
            }

            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON_CANCEL:
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_SERVICE );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
                        password.u08[ 3] = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                        password.u08[ 2] = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) );
                        password.u08[ 1] = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ) );
                        password.u08[ 0] = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ) );
                        if( password.u32 == dev.safe.master.password.u32 )
                        {
                            scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_SERVICE );
                        }
                        else
                        {
                            scr_switch( SCR_IDX_SETUP_SERVICE_ENTRY_ERROR, GUI_ID_BUTTON_MEASURE );
                        }
                        beep_play( BEEP_SHRT );
                        break;

                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, list, list_countof );
            //setup_listwheel( pMsg->hWin, LISTWHEEL_IDX_3, 0 );

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
scr_setup_service_entry( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
