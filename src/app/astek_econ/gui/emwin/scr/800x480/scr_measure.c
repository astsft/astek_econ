/**
  * @file   scr_measure.c
  * @brief  Screen Measure
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

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 160, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_VALUE,      25,  20, 500, 160, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_UNITS,     550,  20, 225, 160, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,      25, 200, 350,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_VALUE,      25, 240, 350,  60, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_RANGE_LABEL,    425, 200, 350,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_RANGE_VALUE,    425, 240, 350,  60, 0, 0x0, 0 },

    //{ TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,      25, 200, 750, 40, 0, 0x0, 0 },
    //{ TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_VALUE,      25, 240, 750, 60, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LEFT,           0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RIGHT,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
scr_update(                             WM_HWIN         hWin,
                                        dev_t *         p )
{
    char        str[32];
    int32_t     ppm         = p->sens->meas.ppm.integral;
    int16_t     temp_digc_i = p->sens->meas.digc.integral;
    uint16_t    temp_digc_f = p->sens->meas.digc.fractional;

    switch( dev.cfg.display_mode )
    {
        //case DEV_DSPL_MODE_PPM:
        //    snprintf( str, sizeof(str), "%d", abs(ppm) < 1000000 ? ppm : 999999 );
        //    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_VALUE ), str );
        //    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_UNITS ), "PPM" );
        //    break;
        case DEV_DSPL_MODE_PERCENTS:
        default:
            //snprintf( str, sizeof(str), "% 4d.%03u", ppm / 10000, ( abs(ppm) % 10000) / 10 );
            snprintf( str, sizeof(str), "% 4d.%02u", ppm / 10000, ( abs(ppm) % 10000) / 100 );
            TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_VALUE ), str );
            TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_UNITS ), "%" );
            break;
    }

    snprintf(   str, sizeof(str), "% 3i.%01u °C", temp_digc_i, (temp_digc_f*10 + 5) / 100 );
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_VALUE ), str );
    TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_TEMP_VALUE), dev.sens->link_err ? GUI_GRAY : GUI_LIGHTGREEN );

    //snprintf(   str, sizeof(str), "% 4i.%02u hPa", );
    //snprintf(   str, sizeof(str), "% 8i Pa", pres_pa );
    //snprintf( str, sizeof(str), "% 8i Pa\0", pres_pa );
    //snprintf(   str, sizeof(str), "% 4i.%02u hPa", pres_pa / 100, abs(pres_pa) % 100 );
    //snprintf(   str, sizeof(str), "% 4i hPa", pres_pa / 100 );
    //TEXT_SetText(   WM_GetDialogItem( hWin, GUI_ID_TEXT_PRES_VALUE ),  str );
    
    //if (dev_cl420_get_units(&dev.cl420) == CL420_UNITS_PPM)
    //{
    //    snprintf( str, sizeof(str), "0 - %d PPM", dev.cl420.range[dev.cl420.range_idx].ppm);            
    //}
    //else
    //{
    //  snprintf(str, sizeof(str), "0 - %2d.%04u%", dev.cl420.range[dev.cl420.range_idx].ppm / 10000, dev.cl420.range[dev.cl420.range_idx].ppm % 10000);
    //}
    snprintf(str, sizeof(str), "0 - %2d.%02u%", dev.cl420.range[dev.cl420.range_idx].ppm / 10000, dev.cl420.range[dev.cl420.range_idx].ppm % 10000);
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_RANGE_VALUE ), str                                      );    

    if( dev.sens->link_err )
    {
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_MEAS_VALUE), GUI_GRAY );
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_TEMP_VALUE), GUI_GRAY );
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_PRES_VALUE), GUI_GRAY );
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_RANGE_VALUE), GUI_GRAY );
    }
    else
    {
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_MEAS_VALUE), GUI_LIGHTGREEN );
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_TEMP_VALUE), GUI_LIGHTGREEN );
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_PRES_VALUE), GUI_LIGHTGREEN );
        TEXT_SetTextColor( WM_GetDialogItem(hWin, GUI_ID_TEXT_RANGE_VALUE), GUI_LIGHTGREEN );
    }
}


static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN     hItem;


    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma150                       );
    TEXT_SetTextAlign(      hItem, GUI_TA_CENTER | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_TRANSPARENT                          );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_VALUE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma150                       );
    TEXT_SetTextAlign(      hItem, GUI_TA_RIGHT | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_UNITS );
    TEXT_SetFont(           hItem, &GUI_FontTahoma150                       );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_CYAN                                 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_TEMPERATURE )  );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_VALUE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );


    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_PRES_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_PRESSURE )     );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_PRES_VALUE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );


    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_RANGE_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_RANGE )        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_RANGE_VALUE );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_GRAY                                 );
    //term_range_str_get( &dev, dev.ibus->range.idx, str, sizeof( str ) );
    //TEXT_SetText(           hItem, str                                      );
    TEXT_SetText(           hItem, "0 - 100%"                               );

    //scr_update( hWin, &dev );

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LEFT );
    BUTTON_SetFocusable(    hItem, 0                                        );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SETUP_AL ) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RIGHT );
    BUTTON_SetFocusable(    hItem, 0                                        );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CALIBRATE_AR ) );
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


    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    if( dev.cfg.display_mode > 0 )
                    {
                        dev.cfg.display_mode--;
                        beep_play( BEEP_SHRT );
                    }
                    else
                    {
                        beep_play( BEEP_TYPE_ERROR );
                    }
                    scr_update( pMsg->hWin, &dev );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_DOWN:
                    if( dev.cfg.display_mode < DEV_DSPL_MODE_MAX-1 )
                    {
                        dev.cfg.display_mode++;
                        beep_play( BEEP_SHRT );
                    }
                    else
                    {
                        beep_play( BEEP_TYPE_ERROR );
                    }
                    scr_update( pMsg->hWin, &dev );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_LEFT:
                    //scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_DUMMY );
                    scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_DUMMY );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_SPAN );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            switch( WM_GetId( pMsg->hWinSrc ) )
            {

                case GUI_ID_TEXT_MEAS_VALUE:
                case GUI_ID_TEXT_MEAS_UNITS:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        if( dev.cfg.display_mode < DEV_DSPL_MODE_MAX-1 )
                        {
                            dev.cfg.display_mode++;
                        }
                        else
                        {
                            dev.cfg.display_mode    = DEV_DSPL_MODE_PERCENTS;
                        }
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_LEFT:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_RIGHT:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_SPAN );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_TIMER:
            scr_update( pMsg->hWin, &dev );
            hItem   = pMsg->Data.v;
            WM_RestartTimer( hItem, 0 );
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_CreateTimer( pMsg->hWin, 0, 500, 0 );
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
scr_measure( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
