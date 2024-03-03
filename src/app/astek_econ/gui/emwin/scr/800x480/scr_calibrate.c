/**
  * @file   scr_calibrate.c
  * @brief  Screen Calibrate
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SPAN,            50,  40, 375,  64, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_ZERO,            50, 120, 375,  64, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SPAN,         450,  40, 300,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ZERO,         450, 120, 300,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RESTORE,      450, 200, 300,  64, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LEFT,           0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RIGHT,        400, 320, 400,  80, 0, 0x0, 0 },
};


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_dialog(                            const   WM_HWIN         hWin )
{
    WM_HWIN         hItem;
    char            str[ 64];
    time_t          ts;
    //int             pi, pf, y, m, d;
    int             y, m, d;
	struct tm *     cur;
    int32_t         cal_ppm, tstmp;

    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_BLACK                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SPAN );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    //TEXT_SetBkColor(        hItem, GUI_DARKGRAY                          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    //cal_ppm = ( (dev.sens->cal.span.ppm_int_hi << 16) | (dev.sens->cal.span.ppm_int_lo & 0xFFFF) );
    cal_ppm = dev.sens->cal.span.ppm.i32;
    //pi  = cal_ppm / 10000;
    //pf  = (cal_ppm % 10000) / 100;
    //tstmp   = (dev.sens->cal.span.timestamp_hi << 16) | (dev.sens->cal.span.timestamp_lo & 0xFFFF);
    tstmp   = dev.sens->cal.span.timestamp.i32;
    ts  = (time_t) tstmp;
	cur = gmtime( &ts );
    y   = cur->tm_year + 1900;
    m   = cur->tm_mon + 1;
    d   = cur->tm_mday;
	//snprintf( str, sizeof(str), "% 4d.%02d%%    %02d.%02d.%02d", pi, pf, y, m, d );
	snprintf( str, sizeof(str), "% 8d PPM  %02d.%02d.%02d", cal_ppm, y, m, d );
    //snprintf( str, sizeof(str), "% 8d PPM", ppb / 1000 );
    TEXT_SetText(           hItem, str                                      );
    WM_BringToTop( hItem );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_ZERO );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    //TEXT_SetBkColor(        hItem, GUI_DARKGRAY                          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    //cal_ppm = ( (dev.sens->cal.zero.ppm_int_hi << 16) | (dev.sens->cal.zero.ppm_int_lo & 0xFFFF) );
    cal_ppm = dev.sens->cal.zero.ppm.i32;
    //pi      = cal_ppm / 10000;
    //pf      = (cal_ppm % 10000) / 100;
    //tstmp   = (dev.sens->cal.zero.timestamp_hi << 16) | (dev.sens->cal.zero.timestamp_lo & 0xFFFF);
    tstmp   = dev.sens->cal.zero.timestamp.i32;
    ts      = (time_t) tstmp;
	cur     = gmtime( &ts );
    y       = cur->tm_year + 1900;
    m       = cur->tm_mon + 1;
    d       = cur->tm_mday;
	//snprintf( str, sizeof(str), "% 4d.%02d%%    %02d.%02d.%02d", pi, pf, y, m, d );
	snprintf( str, sizeof(str), "% 8d PPM  %02d.%02d.%02d", cal_ppm, y, m, d );
    TEXT_SetText(           hItem, str                                      );
    WM_BringToTop( hItem );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SPAN );
    BUTTON_SetFocusable(    hItem, 1 );
    BUTTON_SetText(         hItem, "SPAN" );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ZERO );
    BUTTON_SetFocusable(    hItem, 1 );
    BUTTON_SetText(         hItem, "ZERO"  );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RESTORE );
    BUTTON_SetFocusable(    hItem, 1 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_RESTORE )  );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LEFT );
    BUTTON_SetFocusable(    hItem, 0 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE_AL )   );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RIGHT );
    BUTTON_SetFocusable(    hItem, 0 );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SETUP_AR )   );
}


/**
  * @brief
  * @param  None
  * @retval None */
static
void
dialog_callback(                                WM_MESSAGE *    pMsg )
{
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_DOWN:
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_LEFT:
                    scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_MEASURE );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                    beep_play( BEEP_SHRT );
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
                    case GUI_ID_BUTTON_SPAN:
                        dev.gui.cal_idx = 1;
                        scr_switch( SCR_IDX_CALIBRATE_POINT, GUI_ID_LISTWHEEL0 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_ZERO:
                        dev.gui.cal_idx = 0;
                        scr_switch( SCR_IDX_CALIBRATE_POINT, GUI_ID_LISTWHEEL0 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_RESTORE:
                        scr_switch( SCR_IDX_CALIBRATE_RESTORE, GUI_ID_BUTTON_SPAN );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_LEFT:
                        scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );                  
                        break;
                    case GUI_ID_BUTTON_RIGHT:
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_MEASURE );
                        beep_play( BEEP_SHRT );                                          
                        break;
                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            break;

        default:
            WM_DefaultProc( pMsg );
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
scr_calibrate( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
