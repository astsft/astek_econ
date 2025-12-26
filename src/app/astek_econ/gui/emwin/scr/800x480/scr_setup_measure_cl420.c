/**
  * @file    scr_setup_measure_cl420.c
  * @brief   Screen Setup Measure CL 4-20
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "main.h"


extern  dev_t                   dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON1,             200,  44, 400,  60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON2,             200, 130, 400,  60, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON3,             200, 214, 400,  60, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  cl420_units_t           range_units;
static  dev_range_idx_t         range_idx;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
button_text_update(                             WM_HWIN                 hItem,
                                                dev_range_idx_t         idx,
                                                cl420_units_t           units )
{
    int32_t         ppm      = dev.cl420.range[ idx ].ppm;
    char            str[ 32];
    size_t          nmax    = sizeof( str );

    switch( idx )
    {
        case DEV_RANGE_IDX_R1:
            snprintf( str, nmax, "%3.2f% %", (float) ppm / 10000 );
            break;
        case DEV_RANGE_IDX_R2:
            snprintf( str, nmax, "%3.2f% %", (float) ppm / 10000 );
            break;
        case DEV_RANGE_IDX_R3:
            snprintf( str, nmax, "%3.2f% %", (float) ppm / 10000 );
            break;
        default:
            snprintf( str, nmax, l10n_str_get( dev.cfg.lang, L10N_STR_ID_AUTO ) );
            break;
    }

    BUTTON_SetText(         hItem, str );
}


static
void
range_get_str(                          char *          str,
                                        size_t          nmax,
                                        cl420_t *       p )
{
            snprintf( str, nmax, "%d %%", p->ppm );
}

static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN         hItem;
    char            str[ 32];


    WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

    //
    // MIDDLE AREA
    //
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    WM_BringToBottom( hItem );

    hItem   = RADIO_CreateEx(  75, 60, 98, 450, hWin, WM_CF_SHOW, 0, GUI_ID_RADIO0, DEV_RANGE_IDX_MAX, 85 );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_RADIO0 );
    RADIO_SetBkColor(       hItem,  GUI_TRANSPARENT                         );
    RADIO_SetFont(          hItem,  &GUI_FontTahoma30                       );
    RADIO_SetTextColor( hItem, GUI_WHITE                                );
    RADIO_SetText( hItem, " R1:  ", 0 );
    RADIO_SetText( hItem, " R2:  ", 1 );
    RADIO_SetText( hItem, " R3:  ", 2 );
    RADIO_SetValue( hItem, range_idx );
    WM_SetFocus( WM_GetDialogItem( hWin, GUI_ID_RADIO0 ) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON1 );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    button_text_update(     hItem, DEV_RANGE_IDX_R1, range_units );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON2 );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    button_text_update(     hItem, DEV_RANGE_IDX_R2, range_units );    

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON3 );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    button_text_update(     hItem, DEV_RANGE_IDX_R3, range_units );       
    
    //
    // FOOTER AREA
    //
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
dialog_callback(                        WM_MESSAGE *            pMsg )
{
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {              
                case GUI_KEY_LEFT:
                        beep_play( BEEP_SHRT );
                        if(             1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) ) )
                        {
                                RADIO_SetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ), range_idx );
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON1 ) );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON1 ) ) )
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON2 ) ) )
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON3 ) ) )
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        else
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        break;

                case GUI_KEY_RIGHT:
                        beep_play( BEEP_SHRT );
                        if(             1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) ) )
                        {
                                RADIO_SetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ), range_idx );
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON1 ) );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON1 ) ) )
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON2 ) ) )
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON3 ) ) )
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        else
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }
                        break;

                case GUI_KEY_UP:
                        beep_play( BEEP_SHRT );
                        if(             1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON1 ) ) )
                        {
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON2 ) ) )
                        {
                                dev_cl420_set_range_idx( &dev.cl420, DEV_RANGE_IDX_R1 );
                                GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON3 ) ) )
                        {
                                dev_cl420_set_range_idx( &dev.cl420, DEV_RANGE_IDX_R2 );
                                GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) ) )
                        {
                                if( range_idx > 0 )
                                {
                                        range_idx--;
                                        RADIO_SetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ), range_idx );
                                }
                        }
                        else
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }                        
                        break;

                case GUI_KEY_DOWN:
                        beep_play( BEEP_SHRT );
                        if(             1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON1 ) ) )
                        {
                                
                                dev_cl420_set_range_idx( &dev.cl420, DEV_RANGE_IDX_R2 );
                                GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON2 ) ) )
                        {
                                dev_cl420_set_range_idx( &dev.cl420, DEV_RANGE_IDX_R3 );
                                GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON3 ) ) )
                        {
                        }
                        else if(        1 == WM_HasFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) ) )
                        {
                                if( range_idx < DEV_RANGE_IDX_MAX-1 )
                                {
                                        range_idx++;
                                        RADIO_SetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ), range_idx );
                                }
                        }
                        else
                        {
                                WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0 ) );
                        }                         
                        break;              
                    
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_RANGE );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ENTER:
                    dev_cl420_set_range_idx( &dev.cl420, range_idx );
                    dev_cl420_set_units(&dev.cl420, range_units);
                    task_ibus_cl420_ch2_update();
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_RANGE );
                    beep_play( BEEP_TYPE_CONFIRM );
                    break;
                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON1:
                        dev_cl420_set_range_idx( &dev.cl420, DEV_RANGE_IDX_R1 );
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420_VALUE, GUI_ID_BUTTON_CL420 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON2:
                        dev_cl420_set_range_idx( &dev.cl420, DEV_RANGE_IDX_R2 );
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420_VALUE, GUI_ID_BUTTON_CL420 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON3:
                        dev_cl420_set_range_idx( &dev.cl420, DEV_RANGE_IDX_R3 );
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420_VALUE, GUI_ID_BUTTON_CL420 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_RADIO0:
                        range_idx = (dev_range_idx_t)RADIO_GetValue( WM_GetDialogItem( pMsg->hWin, GUI_ID_RADIO0));         
                        beep_play( BEEP_SHRT );
                        break;                         
                    case GUI_ID_BUTTON_DUMMY:
                    case GUI_ID_BUTTON_ENTER:
                        dev_cl420_set_range_idx( &dev.cl420, range_idx );
                        task_ibus_cl420_ch2_update();
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_RANGE );
                        beep_play( BEEP_TYPE_CONFIRM );
                        break;
                    case GUI_ID_BUTTON_CANCEL:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_RANGE );
                        break;                        
                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            range_idx   = dev_cl420_get_range_idx( &dev.cl420 );
            range_units = dev_cl420_get_units(&dev.cl420);
             
            init_dialog( pMsg->hWin );
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
scr_setup_measure_cl420( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
