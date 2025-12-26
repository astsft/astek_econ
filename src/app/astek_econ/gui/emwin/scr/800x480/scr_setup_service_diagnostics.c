/**
  * @file    scr_setup_service_diagnostics.c
  * @brief   User Interface - Screen Setup System Service Diagnostics
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;


//#define ADC_LSB_mV              ( (float) 3300 / (float) 16777216 )
#define ADC_LSB_mV              ( (float) 3300 / (float) 4096 )
//#define ADC_LSB_mV              ( (float) 2500 / (float) 16777216 )


/*******************************************************************************
* PRIVATE VARIBLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                    0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,               25,  20, 750, 280, 0, 0x0, 0 },

    { PROGBAR_CreateIndirect,   "", GUI_ID_PROGBAR0,                     50,  32,  38,  204, PROGBAR_CF_VERTICAL, 0, 0 },

    { TEXT_CreateIndirect,      "",         GUI_ID_TEXT_MEAS_LABEL,     100,  32, 200,  32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      " %",       GUI_ID_TEXT_MEAS_VALUE,     300,  32, 100,  32, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "",         GUI_ID_TEXT1,               100,  64, 200,  32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      " °C",      GUI_ID_TEXT_DIGC,           300,  64, 100,  32, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "",         GUI_ID_TEXT_RAW_VALUE,      400,  32, 200,  32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      " RAW",     GUI_ID_TEXT_RAW_LABEL,      600,  32, 100,  32, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "",         GUI_ID_TEXT2,               400,  64, 200,  32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      " T RAW",   GUI_ID_TEXT3,               600,  64, 100,  32, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "",         GUI_ID_TEXT_SLOPE_VALUE,    400, 128, 200,  32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      " SLOPE",   GUI_ID_TEXT_SLOPE_LABEL,    600, 128, 100,  32, 0, 0x0, 0 },    
        
    { TEXT_CreateIndirect,      "",         GUI_ID_TEXT_REAL_FW_VER,    50,  270, 896,  32, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "",         GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "",         GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    init_dialog(                                    WM_HWIN         hWin )
{
    WM_HWIN         hItem;


    // MIDDLE AREA
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_PROGBAR0 );
    PROGBAR_SetFont(     hItem, &GUI_FontTahoma30                           );
    //PROGBAR_SetTextAlign( hItem, GUI_TA_RIGHT );

    PROGBAR_SetTextColor( hItem, 0, GUI_LIGHTGREEN  );
    PROGBAR_SetTextColor( hItem, 1, GUI_LIGHTGREEN  );

    PROGBAR_SetBarColor( hItem, 0, GUI_DARKGRAY     );
    PROGBAR_SetBarColor( hItem, 1, GUI_GRAY_17      );

    PROGBAR_SetMinMax( hItem, 0, 100 );

    WM_SetHasTrans( hItem );
    PROGBAR_SetValue( hItem, 0 );

    scr_init_text_value( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_VALUE ) );
    scr_init_text_value( WM_GetDialogItem( hWin, GUI_ID_TEXT1           ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT_DIGC       ) );

    scr_init_text_value( WM_GetDialogItem( hWin, GUI_ID_TEXT_RAW_VALUE  ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT_RAW_LABEL  ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT_REAL_FW_VER ) );
    
    scr_init_text_value( WM_GetDialogItem( hWin, GUI_ID_TEXT2  ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT3  ) );    

    scr_init_text_value( WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_VALUE ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_LABEL ) );    

    
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_REAL_FW_VER ), dev.info.real_firmware_id );    
    
   

    ////////////////////////////////////////////////////////////////////////////
    // FOOTER AREA
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1                                        );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK )         );
    BUTTON_SetFocusable(    hItem, 0                                        );
}


static
void
update( WM_HWIN         hWin )
{
    WM_HWIN     hItem;
    char        str[64];

    int32_t     ppm     = dev.sens->meas.ppm.i32;
    uint32_t    adc_raw = dev.sens->meas.raw;

    hItem   = WM_GetDialogItem( hWin, GUI_ID_PROGBAR0 );
    PROGBAR_SetValue( hItem, ppm / 10000 );    

    snprintf( str, sizeof(str), "%4d.%02u", ppm / 10000, ( abs(ppm) % 10000) / 100 );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL );
    TEXT_SetText( hItem, str );

    snprintf( str, sizeof(str), "% 3d.%01u", dev.sens->meas.digc.integral, dev.sens->meas.digc.fractional);
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    TEXT_SetText( hItem, str );

    //snprintf( str, sizeof(str), "% 3d.%01d °C", dev.sens->meas.temp_digc.i, dev.sens->meas.temp_digc.f );
    //snprintf( str, sizeof(str), "% 3d.%01d °C", dev.sens->meas.digc.integral, dev.sens->meas.digc.fractional );
    //snprintf( str, sizeof(str), "% 3i.%02u\0", dev.sens->meas.pres_hpa.i, dev.sens->meas.pres_hpa.f );
    //snprintf( str, sizeof(str), "% 8i\0", (dev.sens->meas.pres_pa_hi << 16) | (dev.sens->meas.pres_pa_lo & 0xFFFF) );
    //hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
    //TEXT_SetText( hItem, str );

    //snprintf( str, sizeof(str), "%d\0", dev.sens->sts.mcu_temp_digc );
    //snprintf( str, sizeof(str), "%d\0", dev.mcu->temp_digc );
    //hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT3 );
    //TEXT_SetText( hItem, str );
    //TEXT_SetText( hItem, "---" );

    //snprintf( str, sizeof(str), "%d\0", dev.sens->sts.mcu_vdda_mv );
    //snprintf( str, sizeof(str), "%d\0", dev.mcu->vref_mv );
    //hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );
    //TEXT_SetText( hItem, str );
    //TEXT_SetText( hItem, "---" );

    //snprintf( str, sizeof(str), "%08X\0", dev.sens->meas.adc_raw.u32 );
    //snprintf( str, sizeof(str), "%08X\0", adc_raw );
    //snprintf( str, sizeof(str), "%06Xh\0", (dev.sens->meas.adc_raw_hi << 16) | (dev.sens->meas.adc_raw_lo & 0xFFFF) );
    snprintf( str, sizeof(str), "%d\0", dev.sens->meas.raw );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_RAW_VALUE );
    TEXT_SetText( hItem, str );
    
    snprintf( str, sizeof(str), "%d\0", dev.sens->meas.raw_t );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
    TEXT_SetText( hItem, str );  

    //snprintf( str, sizeof(str), "%d\0", dev.sens->meas.adc_mV );
    //snprintf( str, sizeof(str), "0\0" );
    //hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_mV_VALUE );
    //TEXT_SetText( hItem, str );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_VALUE );
    snprintf( str, sizeof(str), "%d\0", dev.sens->meas.slope );
    TEXT_SetText( hItem, str );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
static
void
dialog_callback(                                        WM_MESSAGE *    pMsg )
{
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_ESCAPE:
                    beep_play( BEEP_SHRT );
                    scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_DIAGNOSTICS );
                    break;

                case GUI_KEY_UP:
                case GUI_KEY_DOWN:
                case GUI_KEY_LEFT:
                case GUI_KEY_RIGHT:
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
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
                    case GUI_ID_BUTTON_FACTORY_ZERO:
                        dev.gui.cal_idx = 2;
                        scr_switch( SCR_IDX_CALIBRATE_POINT, GUI_ID_LISTWHEEL0 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_BACK:
                        scr_switch( SCR_IDX_SETUP_SERVICE, GUI_ID_BUTTON_DIAGNOSTICS );
                        beep_play( BEEP_SHRT );
                        break;
                    default:
                        beep_play( BEEP_TYPE_ERROR );
                        break;
                }
            }
            break;

        case WM_TIMER:
            //pMsg->Data.v contains a handle the expired timer only if the message WM_TIMER is currently processed
            WM_RestartTimer( pMsg->Data.v, 0 );
            update( pMsg->hWin );
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            update( pMsg->hWin );
            WM_CreateTimer( pMsg->hWin, 0, 500, 0 );
            break;

        default:
            WM_DefaultProc(pMsg);
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
scr_setup_service_diagnostics( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
