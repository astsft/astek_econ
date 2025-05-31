/**
  * @file    scr.h
  * @brief   Screen Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  SCR_H
#define  SCR_H


#include "DIALOG.h"
#include "font\font.h"
#include "l10n\l10n.h"
#include "beep\beep.h"
#include "gui\emwin\gui_user.h"
#include "gui\emwin\gui_id_user.h"


/*******************************************************************************
*
*******************************************************************************/
typedef enum    scr_idx_e
{
    SCR_IDX_MEASURE                 = 0,
    SCR_IDX_CALIBRATE,
    //SCR_IDX_CALIBRATE_FILTER,
    //SCR_IDX_CALIBRATE_SPAN,
    SCR_IDX_CALIBRATE_FACTORY_SETTINGS,
    SCR_IDX_CALIBRATE_POINT,
    SCR_IDX_CALIBRATE_RESTORE,
    SCR_IDX_CONSOLE,
    SCR_IDX_SETUP,
    SCR_IDX_SETUP_CALIBRATION,
    SCR_IDX_SETUP_CALIBRATION_AUTO,
    //SCR_IDX_SETUP_CALIBRATION_AUTO_CHECK,
    //SCR_IDX_SETUP_CALIBRATION_AUTO_CHECK_ERROR,
    SCR_IDX_SETUP_CALIBRATION_AUTO_CYCLE,
    SCR_IDX_SETUP_CALIBRATION_AUTO_PURGE,
    SCR_IDX_SETUP_CALIBRATION_AUTO_LAUNCH,

    //SCR_IDX_SETUP_CALIBRATION_PASSWORD,
    //SCR_IDX_SETUP_CALIBRATION_PASSWORD_ERROR,
    SCR_IDX_SETUP_MEASURE,
    SCR_IDX_SETUP_MEASURE_SENSOR,
    SCR_IDX_SETUP_MEASURE_CL420,
    SCR_IDX_SETUP_MEASURE_CL420_VALUE,
    SCR_IDX_SETUP_MEASURE_DISCRETE_OUTPUT,
    SCR_IDX_SETUP_SERVICE,
    SCR_IDX_SETUP_SERVICE_ENTRY,
    SCR_IDX_SETUP_SERVICE_ENTRY_ERROR,
    SCR_IDX_SETUP_SERVICE_DIAGNOSTICS,
    SCR_IDX_SETUP_SERVICE_MEASURE,
    SCR_IDX_SETUP_SERVICE_MODBUS, 
    SCR_IDX_SETUP_SERVICE_NETWORK,
    SCR_IDX_SETUP_SERVICE_NETWORK_MAC,
    SCR_IDX_SETUP_SERVICE_NETWORK_IP,
    SCR_IDX_SETUP_SERVICE_NETWORK_SN,
    SCR_IDX_SETUP_SERVICE_NETWORK_GW,
    SCR_IDX_SETUP_SERVICE_NETWORK_MODBUS_PORT,    
    SCR_IDX_SETUP_SERVICE_NETWORK_HTTP_PORT,        
    SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_ENTRY,
    SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_ERROR,
    SCR_IDX_SETUP_SERVICE_RESET_PASSWORD_SUCCESS,
    SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_ENTRY,
    SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_ERROR,
    SCR_IDX_SETUP_SERVICE_RESET_SETTINGS_SUCCESS,
    SCR_IDX_SETUP_SERVICE_CURRENT_LOOP,             
    SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL,
    SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL,
    SCR_IDX_SETUP_SERVICE_CLOOP_ERROR_LEVEL,
    SCR_IDX_SETUP_SERVICE_CLOOP_CAL,
    SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA,
    SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA,
    SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA,
    SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA,    
    SCR_IDX_SETUP_SERVICE_CLOOP_DIAGNOSTIC,  
    SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_DIAGNOSTIC,
    SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_DIAGNOSTIC,    
    SCR_IDX_SETUP_SERVICE_RELAY,  
    SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM,
    SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM,   
    SCR_IDX_SETUP_SEVICE_RELAY_PARAM_MODE,
    SCR_IDX_SETUP_SEVICE_RELAY_PARAM_STATE,
    SCR_IDX_SETUP_SEVICE_RELAY_THRESHOLD,
    SCR_IDX_SETUP_SYSTEM,
    SCR_IDX_SETUP_SYSTEM_DATE,
    SCR_IDX_SETUP_SYSTEM_INFO,
    SCR_IDX_SETUP_SYSTEM_LANGUAGE,
    SCR_IDX_SETUP_SYSTEM_PASSWORD,
    SCR_IDX_SETUP_SYSTEM_PASSWORD_ERROR,
    //SCR_IDX_SETUP_SYSTEM_SERVICE,
    //SCR_IDX_SETUP_CALIBRATION,
    //SCR_IDX_SETUP_SYSTEM_SERVICE_FACTORY_RESET_ERROR,
    //SCR_IDX_SETUP_SYSTEM_SERVICE_FACTORY_RESET_SUCCESS,
    SCR_IDX_SETUP_SYSTEM_TIME,
    SCR_IDX_LAST,
} scr_idx_t;

typedef enum    ui_dspl_trim_idx_e
{
    UI_DSPL_TRIM_IDX_0                      = 0,
    UI_DSPL_TRIM_IDX_1,
    UI_DSPL_TRIM_IDX_LAST,
} ui_dspl_trim_idx_t;


/*******************************************************************************
* FUNCTION PROTOTYPES
*******************************************************************************/
void    scr_init( void );

WM_HWIN scr_switch( const scr_idx_t, const int );

void    scr_init_text( WM_HWIN, int, const char *);
void    scr_init_text_label( WM_HWIN hItem );
void    scr_init_text_value( WM_HWIN hItem );

WM_HWIN scr_measure( void );
WM_HWIN scr_calibrate( void );
WM_HWIN scr_calibrate_factory_settings( void );
WM_HWIN scr_calibrate_point( void );
WM_HWIN scr_calibrate_restore( void );
WM_HWIN scr_console( void );
WM_HWIN scr_setup( void );
//WM_HWIN scr_setup_help( void );
WM_HWIN scr_setup_calibration( void );
WM_HWIN scr_setup_calibration_auto_cycle( void );
WM_HWIN scr_setup_calibration_auto_flow( void );
WM_HWIN scr_setup_calibration_auto( void );
//WM_HWIN scr_setup_calibrate_auto_check( void );
//WM_HWIN scr_setup_calibrate_auto_check_error( void );
WM_HWIN scr_setup_calibration_auto_start( void );
//WM_HWIN scr_setup_calibrate_point_low( void );
//WM_HWIN scr_setup_calibrate_point_low_check( void );
//WM_HWIN scr_setup_calibrate_point_low_check_error( void );
//WM_HWIN scr_setup_calibrate_remotely( void );
//WM_HWIN scr_setup_calibrate_remotely_check( void );
//WM_HWIN scr_setup_calibrate_remotely_check_error( void );
WM_HWIN scr_setup_calibration_password( void );
WM_HWIN scr_setup_calibration_password_error( void );
//WM_HWIN scr_setup_calibrate_restore( void );
//WM_HWIN scr_setup_calibrate_restore_check( void );
//WM_HWIN scr_setup_calibrate_restore_check_error( void );
WM_HWIN scr_setup_measure( void );
WM_HWIN scr_setup_measure_cl420( void );
WM_HWIN scr_setup_measure_cl420_value( void );
WM_HWIN scr_setup_measure_discrete_output( void );
WM_HWIN scr_setup_measure_sensor( void );
WM_HWIN scr_setup_output( void );
WM_HWIN scr_setup_range( void );
WM_HWIN scr_setup_range_value( void );
WM_HWIN scr_setup_service( void );
WM_HWIN scr_setup_service_diagnostics( void );
WM_HWIN scr_setup_service_measure( void );
WM_HWIN scr_setup_service_modbus( void );
WM_HWIN scr_setup_service_network( void );
WM_HWIN scr_setup_service_network_mac( void );
WM_HWIN scr_setup_service_network_ip( void );
WM_HWIN scr_setup_service_network_port( void );
WM_HWIN scr_setup_service_entry( void );
WM_HWIN scr_setup_service_entry_error( void );
WM_HWIN scr_setup_service_reset_password_entry( void );
WM_HWIN scr_setup_service_reset_password_error( void );
WM_HWIN scr_setup_service_reset_password_success( void );
WM_HWIN scr_setup_service_reset_settings_entry( void );
WM_HWIN scr_setup_service_reset_settings_error( void );
WM_HWIN scr_setup_service_reset_settings_success( void );
WM_HWIN scr_setup_service_relay( void );
WM_HWIN scr_setup_service_relay_param( void );
WM_HWIN scr_setup_service_relay_param_mode( void );
WM_HWIN scr_setup_service_relay_param_state( void );
WM_HWIN scr_setup_service_relay_param_threshold( void );
WM_HWIN scr_setup_service_cloop( void );
WM_HWIN scr_setup_service_cloop_error_level(void);
WM_HWIN scr_setup_service_cloop_cal( void );
WM_HWIN scr_setup_service_cloop_channel (void);
WM_HWIN scr_setup_service_cloop_cal_4_20mA( void );
WM_HWIN scr_setup_service_cloop_diagnostic(void);
WM_HWIN scr_setup_system( void );
WM_HWIN scr_setup_system_date( void );
WM_HWIN scr_setup_system_info( void );
WM_HWIN scr_setup_system_language( void );
WM_HWIN scr_setup_system_password( void );
WM_HWIN scr_setup_system_service( void );
WM_HWIN scr_setup_system_service_correction( void );
WM_HWIN scr_setup_system_service_factory_reset_error( void );
WM_HWIN scr_setup_system_service_factory_reset_success( void );
WM_HWIN scr_setup_system_time( void );
//WM_HWIN scr_setup_threshold( void );
//WM_HWIN scr_setup_threshold_value( void );
WM_HWIN scr_status_bar( void );


#endif  //SCR_H
