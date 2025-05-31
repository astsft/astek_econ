/**
  * @file    scr_setup_service_entry.c
  * @brief   Screen Setup Calibrate
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"

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
    
    { TEXT_CreateIndirect,      ".", GUI_ID_TEXT1,              394, 140, 18,  40, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               600, 140, 100,  40, 0, 0x0, 0 },     
  

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  const   char *  list[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };
static  const   size_t  list_countof    = sizeof( list ) / sizeof( char * );

static  listwheel_idx_t listwheel_idx;
static  WM_HWIN         hWheel;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static void check_current (WM_HWIN         hWin)
{
   uint32_t temp_uA = 0;
   uint8_t idx;
   
   temp_uA   += 10000 * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 ) );
   temp_uA   += 1000  * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 ) );
   temp_uA   += 100   * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 ) );
   temp_uA   += 10    * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 ) );  
   
   if (temp_uA < 3900)
     temp_uA = 3900;
   else if (temp_uA > 21000)
     temp_uA = 21000;
   
   
   idx     = (temp_uA / 10000) % 10;
   LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), idx );
   LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), idx );   

   idx     = (temp_uA / 1000) % 10;
   LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), idx );
   LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), idx );   
   
   idx     = (temp_uA / 100) % 10;
   LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), idx );
   LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), idx );      
   
   idx     = (temp_uA / 10) % 10;
   LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), idx );
   LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), idx );         
   
}

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
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
    if (dev.cfg.lang == L10N_LANG_ENGLISH)
      TEXT_SetText(           hItem, "mA"  );
    else 
      TEXT_SetText(           hItem, "мА"  );

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

    int             idx;
    uint32_t        temp_uA = 4000;

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
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
                    }
                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    
                    check_current(pMsg->hWin);
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
                    
                    check_current(pMsg->hWin);                    
                    break;

                case GUI_KEY_ESCAPE:
                        dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
                        dev.state.process_status = PROCESS_MEASURE;
                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_DIAGNOSTIC)                                                
                          scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_DIAGNOSTIC, GUI_ID_BUTTON_CLOOP_CHANNEL_1 );                                                
                        else
                          scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_DIAGNOSTIC, GUI_ID_BUTTON_CLOOP_CHANNEL_2  );                                                                                                  
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
                check_current(pMsg->hWin);
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON_CANCEL:
                        dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
                        dev.state.process_status = PROCESS_MEASURE;
                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_DIAGNOSTIC)                                                
                          scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_DIAGNOSTIC, GUI_ID_BUTTON_CLOOP_CHANNEL_1 );                                                
                        else
                          scr_switch( SCR_IDX_SETUP_SERVICE_CLOOP_DIAGNOSTIC, GUI_ID_BUTTON_CLOOP_CHANNEL_2  );                                                                                                  
                        beep_play( BEEP_SHRT );                        
                        break;

                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
                        temp_uA = 0;
                        temp_uA   += 10000 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                        temp_uA   += 1000  * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) );
                        temp_uA   += 100   * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ) );
                        temp_uA   += 10    * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ) );
                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_DIAGNOSTIC)
                          send_cmd_for_cloop_set_current(1, temp_uA);
                        else
                          send_cmd_for_cloop_set_current(2, temp_uA);                          
                        beep_play( BEEP_TYPE_CONFIRM );
                        break;

                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            dev.cloop->cloop_state = CLOOP_FREEZ;
            dev.state.process_status = PROCESS_CLOOP_CALIBRATION;            
                     
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, list, list_countof );         

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
scr_setup_service_cloop_diagnostic( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
