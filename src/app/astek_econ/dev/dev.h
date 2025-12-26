/**
  * @file    dev.h
  * @brief   Device Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef DEV_H
#define DEV_H


/*******************************************************************************
*
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "dev_conf.h"
#include "dev_safe.h"
#include "dev_mcu.h"
#include "dev_info.h"
#include "dev\nvm\nvm.h"
#include "econ.h"
#include "dev\log\log.h"
#include "dev\meas\meas.h"
#include "dev\cl420\cl420.h"

// CLOOP ERRORS CURRENT LEVEL
#define LOW_ERROR_CURRENT_LEVEL         3000
#define HI_ERROR_CURRENT_LEVEL          22000
#define MIN_LOW_CURRENT_LEVEL           4000
#define MAX_HI_CURRENT_LEVEL            20000

// ERRORS RED 
#define SENSON_LINK_ERR	        (1 << 0)
#define PID_LINK_ERR	        (1 << 1)
#define CLOOP_LINK_ERR	        (1 << 2)
#define SENSOR_TEMP_ERR	        (1 << 3)
#define PID_SENSOR_ERR	        (1 << 4)
#define PID_TEMP_TOO_LOW_ERR    (1 << 5)
#define PID_TEMP_TOO_HI_ERR	(1 << 6)
#define STARTUP_ERR	        (1 << 7)
#define EXT_RELAY_LINK_ERR	(1 << 8)
#define VALIDATION_ERR          (1 << 9)
#define CALIBRATION_ERR         (1 << 10)

// WARNINGS YELLOW
#define PID_TEMP_NOT_IN_RANGE_HI_WAR    (1 << 0)
#define PID_TEMP_NOT_IN_RANGE_LOW_WAR   (1 << 1)
#define MEASURE_VALUE_HI_LEVEL_WAR      (1 << 2)
#define MEASURE_VALUE_LOW_LEVEL_WAR     (1 << 3)
#define VALIDATION_START_WAR            (1 << 4)
#define CALIBRATION_ZERO_START_WAR      (1 << 5)
#define CALIBRATION_SPAN_START_WAR      (1 << 6)
#define CALIBRATION_START_WAR           (1 << 7)

//CALLIBRATION TIMESTAMP ADDR IN BACKUP SRAM
#define CAL0_TIMESTAMP_ADDR             BKPSRAM_BASE
#define CAL1_TIMESTAMP_ADDR             (CAL0_TIMESTAMP_ADDR + 4)
#define CAL0_RESTORE_TIMESTAMP_ADDR     (CAL1_TIMESTAMP_ADDR + 4)
#define CAL1_RESTORE_TIMESTAMP_ADDR     (CAL0_RESTORE_TIMESTAMP_ADDR + 4)



extern uint32_t range_r0_value_default;
extern uint32_t range_r1_value_default;
extern uint32_t range_r2_value_default;
extern uint8_t range_idx_default;
extern uint8_t range_units_default;

typedef enum process_status_e
{
  PROCESS_MEASURE = 0,
  PROCESS_ERROR,
  PROCESS_CALIBRATION_ZERO,
  PROCESS_CALIBRATION_SPAN,
  PROCESS_VALIDATION_SPAN,  
  PROCESS_VALIDATION_ZERO,
  PROCESS_CLOOP_CALIBRATION,
  PROCESS_CLOOP_VALIDATION,
} process_status_t;

/*******************************************************************************
* THLD
*******************************************************************************/
typedef enum    dev_thld_compare_e
{
    DEV_THLD_COMPARE_MORE       = 0,
    DEV_THLD_COMPARE_LESS       = 1,
} dev_thld_compare_t;


/******************************************************************************/
typedef struct  dev_thld_s
{
    size_t                  idx;
    //int32_t                 level[ TERM_THLD_IDX_MAX ];

    //uint32_t                    level;
    bool                        state;
    size_t                      delay;
    dev_thld_compare_t          compare;
} dev_thld_t;



typedef enum    dev_thld_idx_s
{
    DEV_THLD_IDX_0              = 0,
    DEV_THLD_IDX_1              = 1,
} dev_thld_idx_t;


/*******************************************************************************
* GUI
*******************************************************************************/
typedef struct  dev_gui_s
{
    size_t                  cal_idx;
    size_t                  scr_idx;    
} dev_gui_t;


/*******************************************************************************
* RANGE
*******************************************************************************/
typedef enum    dev_range_idx_e
{
    DEV_RANGE_IDX_R1                = 0,
    DEV_RANGE_IDX_R2,
    DEV_RANGE_IDX_R3,
    DEV_RANGE_IDX_MAX,
} dev_range_idx_t;


typedef struct  dev_cl420_s
{
    cl420_t                 range[ DEV_RANGE_IDX_MAX ];
    dev_range_idx_t         range_idx;
} dev_cl420_t;

/*******************************************************************************
* CLOOP
*******************************************************************************/

typedef enum current_error_level_e
{
  LOW_ERROR_LEVEL = 0,
  HI_ERROR_LEVEL = 1
} current_error_level_t;

typedef struct cloop_info_s
{
  uint16_t device_id;
  uint16_t hardware_id;
  uint16_t serial_num[8]; 
} cloop_info_t;

typedef struct  cloop_cal_s
{
    union
    {
        int32_t         i32;
        uint16_t        u16[ 2];
    } raw;

    union
    {
        int32_t         i32;
        uint16_t        u16[ 2];
    } timestamp;
    
    union
    {
        int32_t         i32;
        uint16_t        u16[ 2];
    } uA;    

    int             update;
} cloop_cal_t;

typedef enum cloop_state_e
{
  CLOOP_NORMAL_WORK = 0,
  CLOOP_FREEZ = 1
} cloop_state_t;

typedef struct  dev_cloop_s
{
    cloop_info_t            info;
    cloop_cal_t             cal_ch1[2];
    cloop_cal_t             cal_ch2[2];    
    uint16_t                value_uA;
    uint16_t                range_uA;
    current_error_level_t   error_level;
    cloop_state_t           cloop_state;
    int                     link_err;
} dev_cloop_t;

typedef struct  dev_net_s
{
    union
    {
        uint32_t       u32[2];
        uint8_t        u8[8];   
    } mac;  ///< MAC Address  
  
    union
    {
        uint32_t       u32;
        uint8_t        u8[4];   
    } ip;  ///< Source IP Address

    union
    {
        uint32_t       u32;
        uint8_t        u8[4];   
    } sn;  ///< Subnet Mask 

    union
    {
        uint32_t       u32;
        uint8_t        u8[4];   
    } gw;  ///< Gateway IP Address 

    uint32_t port_modbus;  ///< Modbus Port     
  
    uint32_t port_http;  ///< HTTP Port         

    int                     net_err;
    uint32_t                 net_status;   
} dev_net_t;

typedef struct  dev_value_s
{
  int32_t ppm;
  int32_t ppb;
}value_t;

typedef struct  dev_timings_s
{
  uint32_t transition_time;
  uint32_t measure_time;
  uint32_t return_time;  
  uint32_t stable_slope_time;
}timings_t;

typedef enum validation_state_e
{
  FINISH = 0,
  IN_PROGRESS = 1,
} validation_state_t;

typedef enum validation_result_e
{
  VALIDATION_SUCCESS = 0,
  VALIDATION_FAIL,
  VALIDATION_CANCELED,   
} validation_result_t;

typedef enum validation_start_e
{
  VALIDATION_STOP = 0,
  VALIDATION_REMOTE_START,  
  VALIDATION_MANUAL_START,   
} validation_start_t;

typedef struct  dev_validation_s
{
  value_t               value;
  value_t               deviation;
  int32_t               measure_buf[600];
  int32_t               measure_value;
  int32_t               measure_deviation;
  uint32_t              measure_num;
  timings_t             timings;
  validation_state_t    state;
  validation_result_t   result;
  validation_start_t    start;
  uint16_t              ppb_hi;
  uint16_t              ppb_lo;
}
dev_validation_t;

typedef enum calibration_state_e
{
  CALIBRATION_FINISH = 0,
  CALIBRATION_IN_PROGRESS = 1,
} calibration_state_t;

typedef enum calibration_result_e
{
  CALIBRATION_SUCCESS = 0,
  CALIBRATION_FAIL,
  CALIBRATION_CANCELED,   
} calibration_result_t;

typedef enum calibration_start_e
{
  CALIBRATION_STOP = 0,
  CALIBRATION_REMOTE_START,  
  CALIBRATION_MANUAL_START,   
} calibration_start_t;

typedef struct  dev_remote_calibr_s
{
  value_t                zero;
  value_t                span;  
  value_t                deviation;
  timings_t              timings;
  calibration_state_t    state;
  calibration_result_t   result;
  calibration_start_t    start; 
  bool                   remote_calibration_enabled;
  int32_t                max_slope;
  uint16_t              ppb_hi;
  uint16_t              ppb_lo;
}
dev_calibr_t;


/*******************************************************************************
* RELAY MODBUS
*******************************************************************************/
typedef union
{
    int32_t     i32;
    uint32_t    u32;
    uint16_t    u16[ 2];
} ant3n_type_t;

typedef struct ppm_s
{
        ant3n_type_t   integral;
        ant3n_type_t   fractional;  
        float          ppm_f;
}ppm_t; 

typedef enum relay_mode_s
{
  THRESHOLD_MODE,
  ERROR_MODE,
  NOT_ACTIVE_MODE,
#if defined(USE_VALIDATION)    
  VALIDATION_MODE,  
#endif
} relay_mode_e;

typedef enum relay_state_s
{
  NORMAL_OPEN_STATE,
  NORMAL_CLOSE_STATE,
  
  UNKNOWN_STATE,
} relay_state_e;

typedef enum relay_position_s
{
  SWITCH_OPEN,
  SWITCH_CLOSE,
  
  UNKNOWN_POSITION,
} relay_position_e;

typedef enum thld_type_s
{
  LOW_LEVEL_THLD_TYPE,
  HI_LEVEL_THLD_TYPE,
} thld_type_e;

typedef struct  dev_relay_s
{
    relay_mode_e         relay_mode;  
    relay_state_e        relay_state;
    thld_type_e          thld_type;
        
    ppm_t                ppm;    
    ppm_t                hyst_ppm;
  
} dev_relay_t;

typedef struct mdb_relay_info_s
{
  uint16_t device_id;
  uint16_t hardware_id;
  uint16_t serial_num[8]; 
} ext_relay_info_t;

typedef struct  dev_mdb_relay_s
{
    ext_relay_info_t        info;
    int                     link_err;
    dev_relay_t             relay[8];
} dev_ext_relay_t;

/*******************************************************************************
* STATE
*******************************************************************************/
typedef struct  dev_state_s
{
  uint32_t error_status;
  uint32_t warnings_status;
  uint32_t process_status;
}dev_state_t;

/*******************************************************************************
* DEV
*******************************************************************************/

typedef struct  dev_s
{
    dev_mcu_t *                 mcu;
    dev_gui_t                   gui;
    dev_cfg_t                   cfg;
    nvm_t                       nvm;
    dev_safe_t                  safe;
    dev_info_t                  info;
    econ_t *                    sens;
    dev_cl420_t                 cl420;
    dev_thld_t                  thld[ 2];
    dev_thld_idx_t              thld_idx;
    log_t                       log;
    void    (* factory_reset)       (dev_cfg_t *);
    dev_ext_relay_t *           ext_relay;
    dev_state_t                 state;
    dev_cloop_t *               cloop;
    dev_net_t *                 net;
    dev_validation_t            validation;
    dev_calibr_t                calibration;    
} dev_t;


/*******************************************************************************
*
*******************************************************************************/
int     dev_init( dev_t * );
void    dev_factory_reset( dev_cfg_t * );
void    dev_zero_update( dev_t *     p );
void    dev_span_update( dev_t *     p );
int     dev_zero_restore( dev_t *     p, const   uint16_t    key );
int     dev_span_restore( dev_t *     p, const   uint16_t    key );
void    dev_read_cal (dev_t *     p);


dev_range_idx_t     dev_cl420_get_range_idx( dev_cl420_t * );
void                dev_cl420_set_range_idx( dev_cl420_t *, const dev_range_idx_t );

void                dev_cl420_set_range( dev_cl420_t *, uint32_t );
uint32_t            dev_cl420_get_range( dev_cl420_t * );

cl420_units_t       dev_cl420_get_units( dev_cl420_t * );
void                dev_cl420_set_units( dev_cl420_t *,  cl420_units_t);

void convert_flt_to_int_fract (float flt, int32_t *integ, int32_t *fract);
void convert_int_fract_to_flt (float *flt, int32_t integ, int32_t fract);
void    dev_find_and_write_error_warnings (log_t * log, uint32_t previous_error_status, uint32_t error_status, uint32_t previous_warning_status, uint32_t warning_status);


#endif //DEV_H
