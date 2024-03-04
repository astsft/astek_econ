/**
  * @file    dev.c
  * @brief   Device
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "dev\log\log.h"
#include "dev.h"
#include "config.h"
#include "os\os_user.h"
#include "version.h"

extern  dev_t                   dev;

static uint32_t relay1_default_mode = NOT_ACTIVE_MODE;
static uint32_t relay2_default_mode = NOT_ACTIVE_MODE;
static uint32_t relay1_default_state = NORMAL_OPEN_STATE;
static uint32_t relay2_default_state = NORMAL_OPEN_STATE;
static uint32_t relay1_default_type = LOW_LEVEL_THLD_TYPE;
static uint32_t relay2_default_type = HI_LEVEL_THLD_TYPE;
static uint32_t relay1_default_thld = 100;
static uint32_t relay2_default_thld = 900;
uint32_t range_r0_value_default = 1000;
uint32_t range_r1_value_default = 500;
uint32_t range_r2_value_default = 100;
uint8_t range_idx_default = 0;
uint8_t range_units_default = 0;

/*******************************************************************************
*
*******************************************************************************/
int
dev_init(                       dev_t *         p )
{
    uint32_t config = 0;
    
    p->cfg.starts_count     = p->nvm.get( NVM_REG_STARTS_COUNTER );
    p->cfg.starts_count++;
    p->nvm.put( NVM_REG_STARTS_COUNTER, p->cfg.starts_count );
    
    log_init(&dev.log);
    log_write_event(&dev.log, LOG_SOURCE_SYSTEM, LOG_SYSTEM_EVENT_START);  

    p->cfg.lang = (l10n_lang_t) ( p->nvm.get( NVM_REG_LANGUAGE ) );

    p->cfg.launch_timestamp = p->mcu->rtc.get_timestamp();
    //p->nvm.put( NVM_REG_LAUNCH_TIMESTAMP, p->cfg.launch_timestamp );

    p->cfg.cal_auto_start_timestamp = p->nvm.get( NVM_REG_CAL_AUTO_START_TIMESTAMP );
    p->cfg.cal_auto_cycle_hours     = p->nvm.get( NVM_REG_CAL_AUTO_CYCLE_HOURS );
    p->cfg.cal_auto_flow_seconds    = p->nvm.get( NVM_REG_CAL_AUTO_FLOW_SECONDS );

    p->cfg.display_mode             = DEV_DSPL_MODE_PERCENTS;
    //dev.log.buf         = log_buf;
    //dev.log.buf_size    = CONFIG_LOG_DATA_SIZE;
    //dev.log.head        = 0;

    //p->cl420.range_idx              = (dev_range_idx_t) (p->nvm.get( NVM_REG_RANGE_IDX ));
    //p->cl420.range[ 0].ppm          = p->nvm.get( NVM_REG_RANGE_R1_PPM );
    //p->cl420.range[ 1].ppm          = p->nvm.get( NVM_REG_RANGE_R2_PPM );
    //p->cl420.range[ 2].ppm          = p->nvm.get( NVM_REG_RANGE_R3_PPM );
    //for (int i = 0; i < 3; i++)
    //{
    //  p->cl420.range[ 0].units      = p->nvm.get( NVM_REG_RANGE_UNITS );
    //}
    
    if (dev.cl420.range_idx > 2)
    {
      dev_cl420_set_range_idx(&p->cl420, range_idx_default);
    }
    
    for (int i = 0; i < 3; i++)
    {
      if(dev.cl420.range[i].units >= CL420_UNITS_MAX)
      {
        dev_cl420_set_units(&p->cl420, range_units_default);        
      }
    }    
    
   p->mdb_relay->relay[0].relay_mode = (dev.nvm.get(NVM_REG_RELAY1_MODE_STATE_TYPE) & 0x00FF0000 ) >> 16;
   p->mdb_relay->relay[0].relay_state = (dev.nvm.get(NVM_REG_RELAY1_MODE_STATE_TYPE) & 0x0000FF00 ) >> 8;
   p->mdb_relay->relay[0].thld_type = dev.nvm.get(NVM_REG_RELAY1_MODE_STATE_TYPE) & 0x000000FF ;
   p->mdb_relay->relay[0].ppm.ppm_f = dev.nvm.get(NVM_REG_RELAY1_THRESHOLD);
   p->mdb_relay->relay[0].hyst_ppm.ppm_f = p->mdb_relay->relay[0].ppm.ppm_f * 5 / 100;    
   
   p->mdb_relay->relay[1].relay_mode = (dev.nvm.get(NVM_REG_RELAY2_MODE_STATE_TYPE) & 0x00FF0000 ) >> 16;
   p->mdb_relay->relay[1].relay_state = (dev.nvm.get(NVM_REG_RELAY2_MODE_STATE_TYPE) & 0x0000FF00 ) >> 8;
   p->mdb_relay->relay[1].thld_type = dev.nvm.get(NVM_REG_RELAY2_MODE_STATE_TYPE) & 0x000000FF ;
   p->mdb_relay->relay[1].ppm.ppm_f = dev.nvm.get(NVM_REG_RELAY2_THRESHOLD);
   p->mdb_relay->relay[1].hyst_ppm.ppm_f = p->mdb_relay->relay[1].ppm.ppm_f * 5 / 100;
   
   if ((p->mdb_relay->relay[0].relay_mode != THRESHOLD_MODE) && (p->mdb_relay->relay[0].relay_mode != ERROR_MODE) && (p->mdb_relay->relay[0].relay_mode != NOT_ACTIVE_MODE))
   {
     p->mdb_relay->relay[0].relay_mode = relay1_default_mode;
     config = dev.nvm.get( NVM_REG_RELAY1_MODE_STATE_TYPE );
     config &= 0xFF00FFFF;
     config |= p->mdb_relay->relay[0].relay_mode << 16;
     dev.nvm.put( NVM_REG_RELAY1_MODE_STATE_TYPE, config );        
   }
   if (p->mdb_relay->relay[0].relay_state != NORMAL_OPEN_STATE && p->mdb_relay->relay[0].relay_state != NORMAL_CLOSE_STATE)
   {
     p->mdb_relay->relay[0].relay_state = relay1_default_state;
     config = dev.nvm.get( NVM_REG_RELAY1_MODE_STATE_TYPE );
     config &= 0xFFFF00FF;
     config |= p->mdb_relay->relay[0].relay_state << 8;      
     dev.nvm.put(NVM_REG_RELAY1_MODE_STATE_TYPE, config);
   }
   if (p->mdb_relay->relay[0].thld_type != LOW_LEVEL_THLD_TYPE && p->mdb_relay->relay[0].thld_type != HI_LEVEL_THLD_TYPE)
   {
     p->mdb_relay->relay[0].thld_type = relay1_default_type;
     config = dev.nvm.get( NVM_REG_RELAY1_MODE_STATE_TYPE );
     config &= 0xFFFFFF00;
     config |= p->mdb_relay->relay[0].thld_type;            
     dev.nvm.put(NVM_REG_RELAY1_MODE_STATE_TYPE, config);
   }
   if (p->mdb_relay->relay[0].ppm.ppm_f > 999999 ||  p->mdb_relay->relay[0].ppm.ppm_f < 0)
   {
     p->mdb_relay->relay[0].ppm.ppm_f = relay1_default_thld;
     dev.nvm.put( NVM_REG_RELAY1_THRESHOLD, p->mdb_relay->relay[0].ppm.ppm_f );
   }
   p->mdb_relay->relay[0].hyst_ppm.ppm_f = p->mdb_relay->relay[0].ppm.ppm_f * 5 / 100;
   
   
   if ((p->mdb_relay->relay[1].relay_mode != THRESHOLD_MODE) && (p->mdb_relay->relay[1].relay_mode != ERROR_MODE) && (p->mdb_relay->relay[1].relay_mode != NOT_ACTIVE_MODE))
   {
     p->mdb_relay->relay[1].relay_mode = relay2_default_mode;
     config = dev.nvm.get( NVM_REG_RELAY2_MODE_STATE_TYPE );
     config &= 0xFF00FFFF;
     config |= p->mdb_relay->relay[1].relay_mode << 16;
     dev.nvm.put( NVM_REG_RELAY2_MODE_STATE_TYPE, config );        
   }
   if (p->mdb_relay->relay[1].relay_state != NORMAL_OPEN_STATE && p->mdb_relay->relay[1].relay_state != NORMAL_CLOSE_STATE)
   {
     p->mdb_relay->relay[1].relay_state = relay2_default_state;
     config = dev.nvm.get( NVM_REG_RELAY2_MODE_STATE_TYPE );
     config &= 0xFFFF00FF;
     config |= p->mdb_relay->relay[1].relay_state << 8;      
     dev.nvm.put(NVM_REG_RELAY2_MODE_STATE_TYPE, config);
   }
   if (p->mdb_relay->relay[1].thld_type != LOW_LEVEL_THLD_TYPE && p->mdb_relay->relay[1].thld_type != HI_LEVEL_THLD_TYPE)
   {
     p->mdb_relay->relay[1].thld_type = relay2_default_type;
     config = dev.nvm.get( NVM_REG_RELAY2_MODE_STATE_TYPE );
     config &= 0xFFFFFF00;
     config |= p->mdb_relay->relay[1].thld_type;            
     dev.nvm.put(NVM_REG_RELAY2_MODE_STATE_TYPE, config);
   }
   if (p->mdb_relay->relay[1].ppm.ppm_f > 999999 ||  p->mdb_relay->relay[1].ppm.ppm_f < 0)
   {
     p->mdb_relay->relay[1].ppm.ppm_f = relay2_default_thld;
     dev.nvm.put( NVM_REG_RELAY1_THRESHOLD, p->mdb_relay->relay[1].ppm.ppm_f );
   }
   p->mdb_relay->relay[1].hyst_ppm.ppm_f = p->mdb_relay->relay[1].ppm.ppm_f * 5 / 100;
    
    dev.cloop->error_level = dev.nvm.get( NVM_REG_RANGE_IDX_UNITS_ERR_LEVEL ) & 0x000000FF;
    dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
    
    snprintf (dev.info.real_firmware_id, sizeof(dev.info.real_firmware_id), "%s %s %s %s %s",dev.info.device_str, dev.info.hardware_str, version, firmware_commit, built_date_time);
        
    return( 0 );
}


void
dev_factory_reset(                      dev_cfg_t *         p )
{

}

/*******************************************************************************
* CALIBRATION
*******************************************************************************/
/**
 * @brief
 */
static int
dev_conf_zero_save(                     dev_t *         p )
{
  uint32_t ppm;
  uint32_t raw;
  
    // Read current zero cal value
  ppm = dev.nvm.get(NVM_REG_CAL0_PPM);
  raw = dev.nvm.get(NVM_REG_CAL0_RAW);
  
    // Write current value to backup
  dev.nvm.put(NVM_REG_CAL0_RESTORE_PPM, ppm);
  dev.nvm.put(NVM_REG_CAL0_RESTORE_RAW, raw);
  
    // Write new value
  dev.nvm.put(NVM_REG_CAL0_PPM, p->sens->cal.zero.ppm.u32);
  dev.nvm.put(NVM_REG_CAL0_RAW, p->sens->cal.zero.raw.u32);
 
  return( 0 );
}


/**
 * @brief
 */
static int
dev_conf_span_save(                     dev_t *         p )
{
  uint32_t ppm;
  uint32_t raw;
  
    // Read current span cal value
  ppm = dev.nvm.get(NVM_REG_CAL1_PPM);
  raw = dev.nvm.get(NVM_REG_CAL1_RAW);
  
    // Write current value to backup
  dev.nvm.put(NVM_REG_CAL1_RESTORE_PPM, ppm);
  dev.nvm.put(NVM_REG_CAL1_RESTORE_RAW, raw);
  
    // Write new value
  dev.nvm.put(NVM_REG_CAL1_PPM, p->sens->cal.span.ppm.u32);
  dev.nvm.put(NVM_REG_CAL1_RAW, p->sens->cal.span.raw.u32);

  return( 0 );
}

/**
  * @brief
  */
static int
dev_conf_zero_restore(                  dev_t *     p,
                                const   uint16_t    key )
{
   uint32_t ppm;
   uint32_t raw;
   
   if( key != DEV_CONF_CAL_RESTORE_KEY )
   {
       return( -1 );
   }   
    
   // Read backup value
   ppm = dev.nvm.get(NVM_REG_CAL0_RESTORE_PPM);
   raw = dev.nvm.get(NVM_REG_CAL0_RESTORE_RAW);
      
   // Write backup value   
   dev.nvm.put(NVM_REG_CAL0_PPM, ppm);
   dev.nvm.put(NVM_REG_CAL0_RAW, raw);   
  
   p->sens->cal.zero.ppm.u32 = ppm;
   p->sens->cal.zero.ppm.i32 = (int32_t)ppm;
   p->sens->cal.zero.raw.u32 = raw;
   
   return( 0 );
}

/**
  * @brief
  */
static int
dev_conf_span_restore(                  dev_t *     p,
                                const   uint16_t    key )
{
   uint32_t ppm;
   uint32_t raw;
   
   if( key != DEV_CONF_CAL_RESTORE_KEY )
   {
       return( -1 );
   }   
    
   // Read backup value
   ppm = dev.nvm.get(NVM_REG_CAL1_RESTORE_PPM);
   raw = dev.nvm.get(NVM_REG_CAL1_RESTORE_RAW);
      
   // Write backup value   
   dev.nvm.put(NVM_REG_CAL1_PPM, ppm);
   dev.nvm.put(NVM_REG_CAL1_RAW, raw);   
  
   p->sens->cal.span.ppm.u32 = ppm;
   p->sens->cal.span.ppm.i32 = (int32_t)ppm;
   p->sens->cal.span.raw.u32 = raw;

    return( 0 );
}

/**
  * @brief
  */
void
dev_zero_update(                        dev_t *     p )
{
    p->sens->cal.zero.raw.u32   = p->sens->meas.raw;
    dev_conf_zero_save( p );
    econ_cal_restore( p->sens );
    TRACE( "\ndev_zero_update: %d\n", p->sens->cal.zero.ppm.i32 );
}

/**
  * @brief
  */
void
dev_span_update(                        dev_t *     p )
{
    p->sens->cal.span.raw.u32   = p->sens->meas.raw;
    dev_conf_span_save( p );
    econ_cal_restore( p->sens );
    TRACE( "\ndev_span_update: %d\n", p->sens->cal.span.ppm.i32 );
}

int
dev_zero_restore(                       dev_t *     p,
                                const   uint16_t    key )
{
    int     err = dev_conf_zero_restore( p, key );

    TRACE( "\ndev_zero_restore: %d\terr: %d\n", p->sens->cal.zero.ppm.i32, err );

    if( err )
    {
        return( err );
    }

    econ_cal_restore( p->sens );

    return( 0 );
}

int
dev_span_restore(                       dev_t *     p,
                                const   uint16_t    key )
{
    int     err = dev_conf_span_restore( p, key );

    TRACE( "\ndev_span_restore: %d\terr: %d\n", p->sens->cal.span.ppm.i32, err );

    if( err )
    {
        return( err );
    }

    econ_cal_restore( p->sens );

    return( 0 );
}

/*******************************************************************************
* DEV CL420
*******************************************************************************/
/**
  * @brief
  * @param  None
  * @retval None
  */
dev_range_idx_t
dev_cl420_get_range_idx(                dev_cl420_t *       p )
{
    return p->range_idx;
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void
dev_cl420_set_range_idx(                dev_cl420_t *       p,
                                const   dev_range_idx_t     idx )
{
    if( idx < DEV_RANGE_IDX_MAX )
    {
        p->range_idx  = idx;
        //dev.nvm.put( NVM_REG_RANGE_IDX, idx );        
    }
    
     send_cmd_for_cloop_write_range(); 
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void
dev_cl420_set_range(                    dev_cl420_t *       p,
                                        uint32_t            ppm )
{
    switch( p->range_idx )
    {
        case DEV_RANGE_IDX_R1:
            p->range[ 0].ppm    = ppm;
            //dev.nvm.put( NVM_REG_RANGE_R1_PPM, ppm );
            break;
        case DEV_RANGE_IDX_R2:
            p->range[ 1].ppm    = ppm;
            //dev.nvm.put( NVM_REG_RANGE_R2_PPM, ppm );
            break;
        case DEV_RANGE_IDX_R3:
            p->range[ 2].ppm    = ppm;
            //dev.nvm.put( NVM_REG_RANGE_R3_PPM, ppm );
            break;
        default:
            break;
    }
    
    send_cmd_for_cloop_write_range();    
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uint32_t
dev_cl420_get_range(                    dev_cl420_t *       p )
{
    uint32_t    ppm = 0;

    if( p->range_idx < DEV_RANGE_IDX_MAX )
    {
        ppm = p->range[ p->range_idx ].ppm;
    }
    return( ppm );
}

/**
  * @brief
  * @param  None
  * @retval None
  */
cl420_units_t
dev_cl420_get_units(                    dev_cl420_t *       p )
{
    cl420_units_t   units   = CL420_UNITS_PERCENT;

    if( p->range_idx < DEV_RANGE_IDX_MAX )
    {
        units   = p->range[ p->range_idx ].units;
    }
    return( units );
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void
dev_cl420_set_units(                    dev_cl420_t *       p,  cl420_units_t   units)
{
   for (int i = 0; i < 3; i++)
   {                          
     p->range[i].units = units;
   }
  
   //dev.nvm.put( NVM_REG_RANGE_UNITS, units );
}

void convert_flt_to_int_fract (float flt, int32_t *integ, int32_t *fract)
{
  int32_t sign = 1;
  int64_t fractional = 0;
  
  if (flt < 0)
  {
    sign = -1;
    flt *= -1;
  }
  
  *integ = (int32_t)flt * sign;
  fractional = (int64_t)(flt * 10000); 
  *fract = (fractional % 10000) * sign;
}

void convert_int_fract_to_flt (float *flt, int32_t integ, int32_t fract)
{
   int32_t sign = 1;
  
   if (fract < 0 || integ < 0)
   {
     sign = -1; 
     
     fract *= -1;
     integ *= -1;
   }
    
   *flt = (float)(integ) + ((float)(fract) / 10000);
   *flt *= sign;
}

void    dev_find_and_write_error_warnings (log_t * log, uint32_t previous_error_status, uint32_t error_status, uint32_t previous_warning_status, uint32_t warning_status)
{
 //----------------- SENSOR ERRORS ------------------------------------------------
  
  if ((previous_error_status & SENSON_LINK_ERR) != (error_status & SENSON_LINK_ERR)) 
  {
    if (error_status & SENSON_LINK_ERR)
      log_write_event(log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_LINK_ERR);
    else
      log_write_event(log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_LINK_OK);      
  }
  
  if ((previous_error_status & SENSOR_TEMP_ERR) != (error_status & SENSOR_TEMP_ERR))
  {
    if (error_status & SENSOR_TEMP_ERR){}
      //log_write_event(dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_LINK_ERR);
    else{}
      //log_write_event(dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_LINK_OK);    
  }    
  
  //----------------- SENSOR WARNINGS ------------------------------------------------
  if (((previous_warning_status & MEASURE_VALUE_HI_LEVEL_WAR) != (warning_status & MEASURE_VALUE_HI_LEVEL_WAR)) ||
      ((previous_warning_status & MEASURE_VALUE_LOW_LEVEL_WAR) != (warning_status & MEASURE_VALUE_LOW_LEVEL_WAR))) 
  {
    if (warning_status & MEASURE_VALUE_HI_LEVEL_WAR)
    {
      log_write_event(log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_VALUE_TOO_HIGH_WAR);
    }    
    else if (warning_status & MEASURE_VALUE_LOW_LEVEL_WAR)
    {
      log_write_event(log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_VALUE_TOO_LOW_WAR);
    }    
    else
    {
      log_write_event(log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_VALUE_OK);      
    }
  }
    
 //----------------- CLOOP ERRORS ------------------------------------------------  
  
  if ((previous_error_status & CLOOP_LINK_ERR) != (error_status & CLOOP_LINK_ERR))
  {
    if (error_status & CLOOP_LINK_ERR)
      log_write_event(log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_LINK_ERR);
    else
      log_write_event(log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_LINK_OK);    
  }
  
 //----------------- MDB_RELAY ERRORS ------------------------------------------------  
#ifndef NO_RELAY  
  if ((previous_error_status & MDB_RELAY_LINK_ERR) != (error_status & MDB_RELAY_LINK_ERR))
  {
    if (error_status & MDB_RELAY_LINK_ERR)
      log_write_event(log, LOG_SOURCE_MDB_RELAY, LOG_MDB_RELAY_EVENT_LINK_ERR);
    else
      log_write_event(log, LOG_SOURCE_MDB_RELAY, LOG_MDB_RELAY_EVENT_LINK_OK);    
  }  
#endif
  
}