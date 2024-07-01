/**
  * @file    scr.c
  * @brief   Screen
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "LCDConf.h"
#include "scr.h"
#include "dev\dev.h"
#include "hw_ltdc.h"


static  WM_HWIN         hSbar;
static  WM_HWIN         hWin;
static  WM_MESSAGE      msg_sbar;


extern  dev_t           dev;

/*******************************************************************************
*
*******************************************************************************/
static
void
scr_header_label_update(        const   scr_idx_t           idx )
{
    switch( idx )
    {
        case SCR_IDX_MEASURE:
            msg_sbar.Data.v = L10N_STR_ID_MEASURE;
            break;

        case SCR_IDX_CALIBRATE:
            msg_sbar.Data.v = L10N_STR_ID_CALIBRATION;
            break;

        case SCR_IDX_CALIBRATE_POINT:
            switch( dev.gui.cal_idx )
            {
                case 2:     msg_sbar.Data.v =  L10N_STR_ID_ZERO; break;
                case 1:     msg_sbar.Data.v =  L10N_STR_ID_SPAN; break;
                case 0:
                default:    msg_sbar.Data.v =  L10N_STR_ID_ZERO; break;
            }
            break;

        case SCR_IDX_CALIBRATE_RESTORE:
            msg_sbar.Data.v = L10N_STR_ID_RESTORE;
            break;

        case SCR_IDX_CONSOLE:
            msg_sbar.Data.v = L10N_STR_ID_CONSOLE;
            break;

        case SCR_IDX_SETUP:
            msg_sbar.Data.v = L10N_STR_ID_SETUP;
            break;

        case SCR_IDX_SETUP_CALIBRATION:
            msg_sbar.Data.v = L10N_STR_ID_CALIBRATION;
            break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO:
            msg_sbar.Data.v = L10N_STR_ID_AUTO;
            break;

        //case SCR_IDX_SETUP_CALIBRATION_AUTO_CHECK:
        //    msg_sbar.Data.v = L10N_STR_ID_PASSWORD;
        //    break;

        //case SCR_IDX_SETUP_CALIBRATION_AUTO_CHECK_ERROR:
        //    msg_sbar.Data.v = L10N_STR_ID_PASSWORD;
        //    break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO_PURGE:
            msg_sbar.Data.v = L10N_STR_ID_PURGE;
            break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO_CYCLE:
            msg_sbar.Data.v = L10N_STR_ID_CYCLE;
            break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO_LAUNCH:
            msg_sbar.Data.v = L10N_STR_ID_LAUNCH;
            break;

        //case SCR_IDX_SETUP_CALIBRATION_REMOTELY:
        //    msg_sbar.Data.v = L10N_STR_ID_CALIBRATION;
        //    break;

        //case SCR_IDX_SETUP_HELP:
        //    msg_sbar.Data.v = L10N_STR_ID_HELP;
        //    break;

        case SCR_IDX_SETUP_MEASURE:
            msg_sbar.Data.v = L10N_STR_ID_MEASURE;
            break;

        case SCR_IDX_SETUP_MEASURE_CL420:
            msg_sbar.Data.v = L10N_STR_ID_RANGE;
            break;

        case SCR_IDX_SETUP_MEASURE_CL420_VALUE:
            msg_sbar.Data.v = L10N_STR_ID_RANGE;
            break;

        //case SCR_IDX_SETUP_MEASURE_DISCRETE_OUTPUT:
        //    msg_sbar.Data.v = L10N_STR_ID_DISCRETE_OUTPUT;
        //    break;

        case SCR_IDX_SETUP_MEASURE_SENSOR:
            msg_sbar.Data.v = L10N_STR_ID_SENSOR;
            break;

        case SCR_IDX_SETUP_SERVICE:
            msg_sbar.Data.v = L10N_STR_ID_SERVICE;
            break;

        case SCR_IDX_SETUP_SERVICE_DIAGNOSTICS:
            msg_sbar.Data.v = L10N_STR_ID_DIAGNOSTICS;
            break;

        case SCR_IDX_SETUP_SERVICE_MEASURE:
            msg_sbar.Data.v = L10N_STR_ID_FILTER;
            break;

        case SCR_IDX_SETUP_SERVICE_ENTRY:
            msg_sbar.Data.v = L10N_STR_ID_PASSWORD;
            break;
            
        case SCR_IDX_SETUP_SERVICE_ENTRY_ERROR:
            msg_sbar.Data.v = L10N_STR_ID_PASSWORD;
            break;

        //case SCR_IDX_SETUP_CALIBRATION_PASSWORD:
        //    msg_sbar.Data.v = L10N_STR_ID_PASSWORD;
        //    break;

        //case SCR_IDX_SETUP_CALIBRATION_PASSWORD_ERROR:
        //    msg_sbar.Data.v = L10N_STR_ID_PASSWORD;
        //    break;

        //case SCR_IDX_SETUP_MEASURE:
        //    msg_sbar.Data.v = SCR_IDX_SETUP_MEASURE;
        //    break;

        //case SCR_IDX_SETUP_RANGE:
        //    msg_sbar.Data.v = L10N_STR_ID_RANGE;
        //    break;

        //case SCR_IDX_SETUP_RANGE_VALUE:
        //    msg_sbar.Data.v = L10N_STR_ID_RANGE;
        //    break;

        case SCR_IDX_SETUP_SYSTEM:
            msg_sbar.Data.v = L10N_STR_ID_SYSTEM;
            break;

        case SCR_IDX_SETUP_SYSTEM_DATE:
            msg_sbar.Data.v = L10N_STR_ID_DATE;
            break;

        //case SCR_IDX_SETUP_SYSTEM_INFO:
        //    msg_sbar.Data.v = L10N_STR_ID_INFO;
        //    break;

        case SCR_IDX_SETUP_SYSTEM_LANGUAGE:
            msg_sbar.Data.v = L10N_STR_ID_LANGUAGE;
            break;

        case SCR_IDX_SETUP_SYSTEM_PASSWORD:
            msg_sbar.Data.v = L10N_STR_ID_PASSWORD;
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_ENTRY:
            msg_sbar.Data.v = L10N_STR_ID_PASSWORD_RESET;
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_ERROR:
            msg_sbar.Data.v = L10N_STR_ID_PASSWORD_RESET;
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_SUCCESS:
            msg_sbar.Data.v = L10N_STR_ID_PASSWORD_RESET;
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_ENTRY:
            msg_sbar.Data.v = L10N_STR_ID_FACTORY_SETTINGS;
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_ERROR:
            msg_sbar.Data.v = L10N_STR_ID_FACTORY_SETTINGS;
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_SUCCESS:
            msg_sbar.Data.v = L10N_STR_ID_FACTORY_SETTINGS;
            break;
            
        case SCR_IDX_SETUP_SERVICE_CURRENT_LOOP:
            msg_sbar.Data.v = L10N_STR_ID_CLOOP;
            break;            
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_ERROR_LEVEL:
            msg_sbar.Data.v = L10N_STR_ID_CLOOP_ERROR;
            break;         
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL:
            msg_sbar.Data.v = L10N_STR_ID_CALIBRATION;
            break;     
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL:
            msg_sbar.Data.v = L10N_STR_ID_CALIBRATION_CHANNEL_1;
            break;
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL:
            msg_sbar.Data.v = L10N_STR_ID_CALIBRATION_CHANNEL_2;
            break;
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:          
            msg_sbar.Data.v = L10N_STR_ID_CLOOP_4MA;
            break;

        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:          
            msg_sbar.Data.v = L10N_STR_ID_CLOOP_20MA;
            break;
                        
        case SCR_IDX_SETUP_SERVICE_RELAY:
            msg_sbar.Data.v = L10N_STR_ID_RELAY;
            break;

        case SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM:
            msg_sbar.Data.v = L10N_STR_ID_RELAY_1;
            break;

        case SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM:
            msg_sbar.Data.v = L10N_STR_ID_RELAY_2;
            break;            
            
        case SCR_IDX_SETUP_SEVICE_RELAY_PARAM_MODE:
            msg_sbar.Data.v = L10N_STR_ID_MODE;
            break;
            
        case SCR_IDX_SETUP_SEVICE_RELAY_PARAM_STATE:
            msg_sbar.Data.v = L10N_STR_ID_STATE;
            break;
          
        case SCR_IDX_SETUP_SEVICE_RELAY_THRESHOLD:
            msg_sbar.Data.v = L10N_STR_ID_THRESHOLD;
            break;            

        case SCR_IDX_SETUP_SYSTEM_TIME:
            msg_sbar.Data.v = L10N_STR_ID_TIME;
            break;

        default:
            msg_sbar.Data.v = L10N_STR_ID_LAST;
            break;
    }

    msg_sbar.MsgId       = WM_MESSAGE_HEADER_LABEL_UPDATE;
    WM_SendMessage( hSbar, &msg_sbar );
}


/*******************************************************************************
*
*******************************************************************************/
WM_HWIN
scr_switch(                     const   scr_idx_t       idx,
                                const   int             gui_id )
{
    scr_header_label_update( idx );
    GUI_EndDialog( hWin, 0 );

    switch( idx )
    {
        case SCR_IDX_MEASURE:
            hWin    = scr_measure();
            break;

        case SCR_IDX_CALIBRATE:
            hWin    = scr_calibrate();
            break;

        //case SCR_IDX_CALIBRATE_FACTORY_SETTINGS:
        //    hWin    = scr_calibrate_factory_settings();
        //    break;

        case SCR_IDX_CALIBRATE_POINT:
            hWin    = scr_calibrate_point();
            break;

        case SCR_IDX_CALIBRATE_RESTORE:
            hWin    = scr_calibrate_restore();
            break;

        case SCR_IDX_CONSOLE:
            hWin    = scr_console();
            break;

        case SCR_IDX_SETUP:
            hWin    = scr_setup();
            break;

        case SCR_IDX_SETUP_CALIBRATION:
            hWin    = scr_setup_calibration();
            break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO_CYCLE:
            hWin    = scr_setup_calibration_auto_cycle();
            break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO_PURGE:
            hWin    = scr_setup_calibration_auto_flow();
            break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO:
            hWin    = scr_setup_calibration_auto();
            break;

        case SCR_IDX_SETUP_CALIBRATION_AUTO_LAUNCH:
            hWin    = scr_setup_calibration_auto_start();
            break;

        //case SCR_IDX_SETUP_HELP:
        //    hWin    = scr_setup_help();
        //    break;

        case SCR_IDX_SETUP_MEASURE:
            hWin    = scr_setup_measure();
            break;

        case SCR_IDX_SETUP_MEASURE_CL420:
            hWin    = scr_setup_measure_cl420();
            break;

        case SCR_IDX_SETUP_MEASURE_CL420_VALUE:
            hWin    = scr_setup_measure_cl420_value();
            break;

        //case SCR_IDX_SETUP_MEASURE_DISCRETE_OUTPUT:
        //    hWin    = scr_setup_measure_discrete_output();
        //    break;

        case SCR_IDX_SETUP_MEASURE_SENSOR:
            hWin    = scr_setup_measure_sensor();
            break;

        case SCR_IDX_SETUP_SERVICE:
            hWin    = scr_setup_service();
            break;

        case SCR_IDX_SETUP_SERVICE_DIAGNOSTICS:
            hWin    = scr_setup_service_diagnostics();
            break;

        case SCR_IDX_SETUP_SERVICE_MEASURE:
            hWin    = scr_setup_service_measure();
            break;

        case SCR_IDX_SETUP_SERVICE_ENTRY:
            hWin    = scr_setup_service_entry();
            break;

        case SCR_IDX_SETUP_SERVICE_ENTRY_ERROR:
            hWin    = scr_setup_service_entry_error();
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_ENTRY:
            hWin    = scr_setup_service_reset_password_entry();
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_ERROR:
            hWin    = scr_setup_service_reset_password_error();
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_SUCCESS:
            hWin    = scr_setup_service_reset_password_success();
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_ENTRY:
            hWin    = scr_setup_service_reset_settings_entry();
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_ERROR:
            hWin    = scr_setup_service_reset_settings_error();
            break;

        case SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_SUCCESS:
            hWin    = scr_setup_service_reset_settings_success();
            break;
            
        case SCR_IDX_SETUP_SERVICE_RELAY:
            hWin    = scr_setup_service_relay();
            break;    
            
        case SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM:
            hWin    = scr_setup_service_relay_param();
            break;  

        case SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM:
            hWin    = scr_setup_service_relay_param();
            break;              
            
        case SCR_IDX_SETUP_SEVICE_RELAY_PARAM_MODE:
            hWin    = scr_setup_service_relay_param_mode();
            break;
            
        case SCR_IDX_SETUP_SEVICE_RELAY_PARAM_STATE:
            hWin    = scr_setup_service_relay_param_state();
            break;
          
        case SCR_IDX_SETUP_SEVICE_RELAY_THRESHOLD:
            hWin    = scr_setup_service_relay_param_threshold();
            break;
            
        case SCR_IDX_SETUP_SERVICE_CURRENT_LOOP:
            hWin    = scr_setup_service_cloop();
            break;                  
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_ERROR_LEVEL:
            hWin    = scr_setup_service_cloop_error_level();
            break;                  
          
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL:
            hWin = scr_setup_service_cloop_channel();
            break;          
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL:
        case SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL:
            hWin    = scr_setup_service_cloop_cal();
            break;
            
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
            hWin    = scr_setup_service_cloop_cal_4_20mA();
            break;

        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
        case SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:          
            hWin    = scr_setup_service_cloop_cal_4_20mA();
            break;
            
        case SCR_IDX_SETUP_SYSTEM:
            hWin    = scr_setup_system();
            break;

        //case SCR_IDX_SETUP_SYSTEM_INFO:
        //    hWin    = scr_setup_system_info();
        //    break;

        case SCR_IDX_SETUP_SYSTEM_LANGUAGE:
            hWin    = scr_setup_system_language();
            break;

        case SCR_IDX_SETUP_SYSTEM_PASSWORD:
            hWin    = scr_setup_system_password();
            break;

        case SCR_IDX_SETUP_SYSTEM_DATE:
            hWin    = scr_setup_system_date();
            break;

        case SCR_IDX_SETUP_SYSTEM_TIME:
            hWin    = scr_setup_system_time();
            break;

        case SCR_IDX_LAST:
        default:
            hWin    = 0;
            break;
    }

    WM_SetFocus( WM_GetDialogItem( hWin, gui_id ) );
    return( hWin );
}

/*******************************************************************************
* BACKGROUD WINDOW CALLBACK
*******************************************************************************/
static
void
bgwin_cbk(                              WM_MESSAGE *            pMsg )
{
    switch( pMsg->MsgId )
    {
        default:
            WM_DefaultProc( pMsg );
            break;
    }
}


#ifdef NDEBUG
#include "gui/emwin/res/astek_logo_800x480_png.c"

static
void
scr_show_info( void )
{
/*
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_VCENTER );
    GUI_DispStringAt( "МОДЕЛЬ: ",           512, 400 );

    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_VCENTER );
    GUI_DispStringAt( "ASTEK 1013",         512, 400 );

    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_VCENTER );
    GUI_DispStringAt( "СЕРИЙНЫЙ НОМЕР: ",   512, 450 );

    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_VCENTER );
    GUI_DispStringAt( "0000-0001",          512, 450 );

    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_VCENTER );
    GUI_DispStringAt( "ВЕРСИЯ ПО: ",        512, 500 );

    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_VCENTER );
    GUI_DispStringAt( "12.34.56.78",        512, 500 );
*/
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_VCENTER );
    GUI_DispStringAt( "МОДЕЛЬ: ",           400, 320 );

    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_VCENTER );
    GUI_DispStringAt( "АСТЕК ЭКОН",         400, 320 );

    //GUI_SetColor(GUI_LIGHTGRAY);
    //GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_VCENTER );
    //GUI_DispStringAt( "СЕРИЙНЫЙ НОМЕР: ",   400, 360 );
    //
    //GUI_SetColor(GUI_WHITE);
    //GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_VCENTER );
    //GUI_DispStringAt( "0000-XXXX",          400, 360 );

    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetTextAlign( GUI_TA_RIGHT | GUI_TA_VCENTER );
    GUI_DispStringAt( "ВЕРСИЯ ПО: ",        400, 400 );

    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign( GUI_TA_LEFT | GUI_TA_VCENTER );
    GUI_DispStringAt( "astekecon.1.0",        400, 400 );
}

static
void
scr_show_welcome( void )
{
    GUI_MEMDEV_Handle   hMem    = GUI_MEMDEV_Create( 0, 0, XSIZE_PHYS, YSIZE_PHYS );

    scr_show_info();
    GUI_MEMDEV_Select( hMem );
    GUI_PNG_Draw( astek_logo_800x480_png, sizeof(astek_logo_800x480_png), 0, 0 );
    scr_show_info();

    GUI_MEMDEV_CopyToLCD( hMem );
    GUI_Delay( 4000 );
    GUI_MEMDEV_Delete( hMem );
}
#endif


/*******************************************************************************
* SCREEN INIT
*******************************************************************************/
void
scr_init( void )
{
    GUI_Init();
    
    ltdc_reset_swap(0);
    GUI_Delay(500);
    ltdc_reset_swap(1); 
    GUI_Delay(500);

    ltdc_swap_x( 1 );
    ltdc_swap_y( 1 );


//GUI_EnableAlpha(1);
    GUI_Clear();
    GUI_SetColor( GUI_BLACK );
    GUI_FillRect( 0, 0, XSIZE_PHYS-1, YSIZE_PHYS-1 );
    GUI_UC_SetEncodeUTF8();
    GUI_SetFont( &GUI_FontTahoma40 );
    GUI_SetTextMode(GUI_TM_NORMAL);
    #ifdef NDEBUG
    scr_show_welcome();
    #endif
    WM_EnableMemdev( WM_HBKWIN );
    WM_SetCreateFlags( WM_CF_MEMDEV );
    WM_SetCallback( WM_HBKWIN, &bgwin_cbk );
    WM_MULTIBUF_Enable( 2 );
    gui_set_defaults();

    hSbar   = scr_status_bar();
    WM_BringToBottom( hSbar );

    scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
}
