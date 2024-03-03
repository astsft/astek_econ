/**
  * @file   scr_setup_system_language.c
  * @brief  Screen Setup System Language
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIBLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,               250, 140, 300,  40, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          250, 100, 300, 120, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    init_dialog(                            WM_HWIN                 hWin )
{
    WM_HWIN         hItem;
    int             lang;
    char const **   list    = l10n_lang_list_get();
    size_t          countof = l10n_lang_countof_get();


    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
    gui_init_listwheel( hWin, GUI_ID_LISTWHEEL0, 50, list, countof );

    lang    = dev.cfg.lang;
    LISTWHEEL_SetPos(       hItem, lang );
    LISTWHEEL_SetSel(       hItem, lang );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_HCENTER | TEXT_CF_VCENTER         );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
    TEXT_SetBkColor(        hItem, GUI_DARKGRAY                             );


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


/**
  * @brief
  * @param  None
  * @retval None
  */
static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    WM_HWIN         hItem;
    int             lang;
    size_t          list_countof    = l10n_lang_countof_get();
    //int             id              = WM_GetId( pMsg->hWinSrc );


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    beep_play( BEEP_SHRT );
                    hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                    lang    = LISTWHEEL_GetPos( hItem );
                    if( ++lang >= list_countof )
                    {
                        lang    = 0;
                    }
                    LISTWHEEL_SetPos(       hItem, lang );
                    LISTWHEEL_SetSel(       hItem, lang );
                    break;

                case GUI_KEY_DOWN:
                    beep_play( BEEP_SHRT );
                    hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                    lang    = LISTWHEEL_GetPos( hItem );
                    if( --lang < 0 )
                    {
                        lang    = list_countof-1;
                    }
                    LISTWHEEL_SetPos(       hItem, lang );
                    LISTWHEEL_SetSel(       hItem, lang );
                    break;

                case GUI_KEY_ESCAPE:
                    beep_play( BEEP_SHRT );
                    scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_LANGUAGE );
                    break;

                case GUI_KEY_LEFT:
                case GUI_KEY_RIGHT:
                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            switch( WM_GetId(pMsg->hWinSrc) )
            {
                case GUI_ID_LISTWHEEL0:
                    if(         pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        beep_play( BEEP_SHRT );
                    }
                    else if(    pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                    }
                    else if(    pMsg->Data.v == WM_NOTIFICATION_SEL_CHANGED )
                    {
                    }
                    else if(    pMsg->Data.v == WM_NOTIFICATION_MOVED_OUT )
                    {
                    }
                    break;


                case GUI_ID_BUTTON_CANCEL:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_LANGUAGE );
                    }
                    break;

                case GUI_ID_BUTTON_ENTER:
                case GUI_ID_BUTTON_DUMMY:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
                        lang    = LISTWHEEL_GetPos( hItem );
                        //sys_cfg_language_set( sys.cfg, lang );
                        if( lang < L10N_LANG_MAX )
                        {
                            dev.cfg.lang    = (l10n_lang_t) lang;
                        }
                        dev.nvm.put( NVM_REG_LANGUAGE, lang );
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_LANGUAGE );
                        beep_play( BEEP_TYPE_CONFIRM );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
            TEXT_SetBkColor( WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT0 ), CONFIG_UI_COLOR_WHEEL_SEL   );
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
scr_setup_system_language( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
