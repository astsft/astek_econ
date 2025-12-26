/**
  * @file   scr_calibrate_restore.c
  * @brief  Screen Calibrate Restore
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "main.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SPAN_BACK,       50,  40, 375,  64, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_ZERO_BACK,       50, 120, 375,  64, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SPAN,         450,  40, 300,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ZERO,         450, 120, 300,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_FACTORY_ZERO, 450, 200, 300,  64, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
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

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_ZERO_BACK );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_RED                                  );
    cal_ppm = dev.sens->cal_back.zero.ppm.i32;
    //pi      = cal_ppm / 10000;
    //pf      = (cal_ppm % 10000) / 100;
    tstmp   = dev.sens->cal_back.zero.timestamp.u32;
    ts      = (time_t) tstmp;
	cur     = gmtime( &ts );
    y       = cur->tm_year + 1900;
    m       = cur->tm_mon + 1;
    d       = cur->tm_mday;
	//snprintf( str, sizeof(str), "% 4d.%02d%%    %02d.%02d.%02d", pi, pf, y, m, d );
	//snprintf( str, sizeof(str), "% 8d PPM  %02d.%02d.%02d", cal_ppm, y, m, d );
	//snprintf( str, sizeof(str), "% 8d PPM ", cal_ppm);    
        snprintf( str, sizeof(str), "% 4d.%02u%%  %02d.%02d.%02d", cal_ppm / 10000, ( abs(cal_ppm) % 10000) / 100, y, m, d);        
    TEXT_SetText(           hItem, str                                      );
    WM_BringToTop( hItem );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SPAN_BACK );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    //TEXT_SetBkColor(        hItem, GUI_DARKGRAY                          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_RED                                  );
    cal_ppm = dev.sens->cal_back.span.ppm.i32;
    //pi  = cal_ppm / 10000;
    //pf  = (cal_ppm % 10000) / 100;
    tstmp   = dev.sens->cal_back.span.timestamp.u32;
    ts  = (time_t) tstmp;
	cur = gmtime( &ts );
    y   = cur->tm_year + 1900;
    m   = cur->tm_mon + 1;
    d   = cur->tm_mday;
	//snprintf( str, sizeof(str), "% 4d.%02d%%    %02d.%02d.%02d", pi, pf, y, m, d );
	//snprintf( str, sizeof(str), "% 8d PPM  %02d.%02d.%02d", cal_ppm, y, m, d );
	//snprintf( str, sizeof(str), "% 8d PPM ", cal_ppm);        
        snprintf( str, sizeof(str), "% 4d.%02u%%  %02d.%02d.%02d", cal_ppm / 10000, ( abs(cal_ppm) % 10000) / 100, y, m, d );

    TEXT_SetText(           hItem, str                                      );
    WM_BringToTop( hItem );


    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SPAN );
    BUTTON_SetText(         hItem, "SPAN" );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ZERO );
    BUTTON_SetText(         hItem, "ZERO"  );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_FACTORY_ZERO );
    BUTTON_SetText(         hItem, "FACTORY ZERO" );
    BUTTON_SetFocusable(    hItem, 1    );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK )         );
    BUTTON_SetFocusable(    hItem, 0                                        );

}


/**
  * @brief
  * @param  None
  * @retval None
  */
static
void
dialog_callback(                                WM_MESSAGE *    pMsg )
{
    static  int         blink;
            WM_HWIN     hItem;

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
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_RESTORE );
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
                        task_hmi_cal_restore( 1 );
                        scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_ZERO:
                        task_hmi_cal_restore( 0 );
                        scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_FACTORY_ZERO:
                        task_hmi_cal_restore( 2 );
                        scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_CALIBRATE_MANUAL, GUI_ID_BUTTON_RESTORE );
                        break;
                    default:
                        break;
                }
            }
            break;

        case WM_TIMER:
            blink++;
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_ZERO_BACK );
            TEXT_SetTextColor( hItem, (blink & 1) ? GUI_RED : GUI_LIGHTGRAY );
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SPAN_BACK );
            TEXT_SetTextColor( hItem, (blink & 1) ? GUI_RED : GUI_LIGHTGRAY );
            hItem   = pMsg->Data.v;
            WM_RestartTimer( hItem, 0 );
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_CreateTimer( pMsg->hWin, 0, 500, 0 );
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
scr_calibrate_restore( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
