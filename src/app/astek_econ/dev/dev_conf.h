/**
  * @file    dev_cfg.h
  * @brief   Device Configuration Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef DEV_CFG_H
#define DEV_CFG_H


/*******************************************************************************
*
*******************************************************************************/
#include <time.h>
#include "l10n\l10n.h"

#define DEV_CONF_CAL_RESTORE_KEY        0xACDC

/*******************************************************************************
*
*******************************************************************************/
/*******************************************************************************
*
*******************************************************************************/
typedef enum    dev_dspl_mode_e
{
    DEV_DSPL_MODE_PERCENTS      = 0,
    DEV_DSPL_MODE_MAX,
} dev_dspl_mode_t;


typedef struct  dev_cfg_s
{
    l10n_lang_t                 lang;
    uint32_t                    starts_count;
    time_t                      launch_timestamp;
    dev_dspl_mode_t             display_mode;
    time_t                      cal_auto_start_timestamp;
    uint32_t                    cal_auto_cycle_hours;
    uint32_t                    cal_auto_flow_seconds;
    bool                        cal_remotely;
    bool                        cal_autolaunch;
    bool                        sens_t_compensation;
    bool                        sens_p_compensation;
    uint16_t                    error_filter_count;
    //bool                        cal_zero;
    //dev_union_t           password;
    //dev_mode_t      mode;
    //hmi_dspl_t      dspl;

    //time_t          cal_auto_start_timestamp;
    //uint32_t        cal_auto_cycle_hours;
    //uint32_t        cal_auto_flow_seconds;
    union ext_mdbs_cfg_u
    {
      uint32_t ext_mdbs_cfg_param;
      
      struct  ext_mdbs_cfg_s
      {
        uint32_t stop_bits: 2;
        uint32_t parity: 2;
        uint32_t data_width: 2;
        uint32_t baudrate: 4;
        uint32_t rtu_addr: 8;
        uint32_t tcp_addr: 8;        
      }ext_mdbs_cfg;
    };
    
} dev_cfg_t;



#endif //DEV_CFG_H
