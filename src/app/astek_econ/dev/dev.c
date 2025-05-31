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
#include "hw_relay.h"

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
static uint32_t default_error_count = 3;

uint8_t default_mac[8] = {0x00, 0x08, 0xdc, 0x00, 0xab, 0xcd, 0x00, 0x00};
uint8_t default_ip[] = {192, 168, 1, 100};
uint8_t default_sn[] = {255,255,255,0};
uint8_t default_gw[] = {192, 168, 1, 1};
uint32_t default_port_modbus = 502;
uint32_t default_port_http = 80;
uint8_t  default_network_state = 0;
uint32_t default_validation_value = 1000;
uint32_t default_validation_deviation = 10;
uint32_t default_validation_transition_time = 100;
uint32_t default_validation_measure_time = 12;
uint32_t default_validation_return_time = 100;


/*******************************************************************************
*
*******************************************************************************/
int
dev_init(                       dev_t *         p )
{
    uint32_t config = 0;
    
    int year = dev.mcu->rtc.get_year() + 1970;
    if (year < 2000)
    {
      dev.mcu->rtc.set_date(2000 - 1970, 0, 0);
    }    
    
    log_init(&dev.log);
    log_write_event(&dev.log, LOG_SOURCE_SYSTEM, LOG_SYSTEM_EVENT_START);  
    
    dev_read_cal(p);
    econ_cal_restore(p->sens);

    p->cfg.lang = (l10n_lang_t) ( p->nvm.get( NVM_REG_LANGUAGE ) );
    if( p->cfg.lang >= L10N_LANG_MAX )
    {
      p->cfg.lang = L10N_LANG_RUSSIAN;
      p->nvm.put( NVM_REG_LANGUAGE, L10N_LANG_RUSSIAN );
    }
    dev.cfg.error_filter_count =  dev.nvm.get( NVM_REG_ERROR_COUNT );
    if (dev.cfg.error_filter_count > 10 || dev.cfg.error_filter_count == 0)
    {
      dev.cfg.error_filter_count = default_error_count;
      dev.nvm.put( NVM_REG_ERROR_COUNT, dev.cfg.error_filter_count);
    }    

    p->cfg.display_mode             = DEV_DSPL_MODE_PERCENTS;
    //dev.log.buf         = log_buf;
    //dev.log.buf_size    = CONFIG_LOG_DATA_SIZE;
    //dev.log.head        = 0;

    p->cl420.range_idx              = (dev_range_idx_t) (p->nvm.get( NVM_REG_RANGE_IDX ));
    p->cl420.range[ 0].ppm          = p->nvm.get( NVM_REG_RANGE_R1_PPM );
    p->cl420.range[ 1].ppm          = p->nvm.get( NVM_REG_RANGE_R2_PPM );
    p->cl420.range[ 2].ppm          = p->nvm.get( NVM_REG_RANGE_R3_PPM );
    for (int i = 0; i < 3; i++)
    {
      p->cl420.range[ 0].units      = p->nvm.get( NVM_REG_RANGE_UNITS );
    }
    
    if (dev.cl420.range_idx >= 2)
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
    
   p->ext_relay->relay[0].relay_mode = (dev.nvm.get(NVM_REG_RELAY1_MODE_STATE_TYPE) & 0x00FF0000 ) >> 16;
   p->ext_relay->relay[0].relay_state = (dev.nvm.get(NVM_REG_RELAY1_MODE_STATE_TYPE) & 0x0000FF00 ) >> 8;
   p->ext_relay->relay[0].thld_type = dev.nvm.get(NVM_REG_RELAY1_MODE_STATE_TYPE) & 0x000000FF ;
   p->ext_relay->relay[0].ppm.ppm_f = dev.nvm.get(NVM_REG_RELAY1_THRESHOLD);
   p->ext_relay->relay[0].hyst_ppm.ppm_f = p->ext_relay->relay[0].ppm.ppm_f * 5 / 100;    
   
   p->ext_relay->relay[1].relay_mode = (dev.nvm.get(NVM_REG_RELAY2_MODE_STATE_TYPE) & 0x00FF0000 ) >> 16;
   p->ext_relay->relay[1].relay_state = (dev.nvm.get(NVM_REG_RELAY2_MODE_STATE_TYPE) & 0x0000FF00 ) >> 8;
   p->ext_relay->relay[1].thld_type = dev.nvm.get(NVM_REG_RELAY2_MODE_STATE_TYPE) & 0x000000FF ;
   p->ext_relay->relay[1].ppm.ppm_f = dev.nvm.get(NVM_REG_RELAY2_THRESHOLD);
   p->ext_relay->relay[1].hyst_ppm.ppm_f = p->ext_relay->relay[1].ppm.ppm_f * 5 / 100;
   
   if ((p->ext_relay->relay[0].relay_mode != THRESHOLD_MODE) && (p->ext_relay->relay[0].relay_mode != ERROR_MODE) && (p->ext_relay->relay[0].relay_mode != NOT_ACTIVE_MODE))
   {
     p->ext_relay->relay[0].relay_mode = relay1_default_mode;
     config = dev.nvm.get( NVM_REG_RELAY1_MODE_STATE_TYPE );
     config &= 0xFF00FFFF;
     config |= p->ext_relay->relay[0].relay_mode << 16;
     dev.nvm.put( NVM_REG_RELAY1_MODE_STATE_TYPE, config );        
   }
   if (p->ext_relay->relay[0].relay_state != NORMAL_OPEN_STATE && p->ext_relay->relay[0].relay_state != NORMAL_CLOSE_STATE)
   {
     p->ext_relay->relay[0].relay_state = relay1_default_state;
     config = dev.nvm.get( NVM_REG_RELAY1_MODE_STATE_TYPE );
     config &= 0xFFFF00FF;
     config |= p->ext_relay->relay[0].relay_state << 8;      
     dev.nvm.put(NVM_REG_RELAY1_MODE_STATE_TYPE, config);
   }
   if (p->ext_relay->relay[0].thld_type != LOW_LEVEL_THLD_TYPE && p->ext_relay->relay[0].thld_type != HI_LEVEL_THLD_TYPE)
   {
     p->ext_relay->relay[0].thld_type = relay1_default_type;
     config = dev.nvm.get( NVM_REG_RELAY1_MODE_STATE_TYPE );
     config &= 0xFFFFFF00;
     config |= p->ext_relay->relay[0].thld_type;            
     dev.nvm.put(NVM_REG_RELAY1_MODE_STATE_TYPE, config);
   }
   if (p->ext_relay->relay[0].ppm.ppm_f > 999999 ||  p->ext_relay->relay[0].ppm.ppm_f < 0)
   {
     p->ext_relay->relay[0].ppm.ppm_f = relay1_default_thld;
     dev.nvm.put( NVM_REG_RELAY1_THRESHOLD, (uint32_t)(p->ext_relay->relay[0].ppm.ppm_f) );
   }
   p->ext_relay->relay[0].hyst_ppm.ppm_f = p->ext_relay->relay[0].ppm.ppm_f * 5 / 100;
   
   
   if ((p->ext_relay->relay[1].relay_mode != THRESHOLD_MODE) && (p->ext_relay->relay[1].relay_mode != ERROR_MODE) && (p->ext_relay->relay[1].relay_mode != NOT_ACTIVE_MODE))
   {
     p->ext_relay->relay[1].relay_mode = relay2_default_mode;
     config = dev.nvm.get( NVM_REG_RELAY2_MODE_STATE_TYPE );
     config &= 0xFF00FFFF;
     config |= p->ext_relay->relay[1].relay_mode << 16;
     dev.nvm.put( NVM_REG_RELAY2_MODE_STATE_TYPE, config );        
   }
   if (p->ext_relay->relay[1].relay_state != NORMAL_OPEN_STATE && p->ext_relay->relay[1].relay_state != NORMAL_CLOSE_STATE)
   {
     p->ext_relay->relay[1].relay_state = relay2_default_state;
     config = dev.nvm.get( NVM_REG_RELAY2_MODE_STATE_TYPE );
     config &= 0xFFFF00FF;
     config |= p->ext_relay->relay[1].relay_state << 8;      
     dev.nvm.put(NVM_REG_RELAY2_MODE_STATE_TYPE, config);
   }
   if (p->ext_relay->relay[1].thld_type != LOW_LEVEL_THLD_TYPE && p->ext_relay->relay[1].thld_type != HI_LEVEL_THLD_TYPE)
   {
     p->ext_relay->relay[1].thld_type = relay2_default_type;
     config = dev.nvm.get( NVM_REG_RELAY2_MODE_STATE_TYPE );
     config &= 0xFFFFFF00;
     config |= p->ext_relay->relay[1].thld_type;            
     dev.nvm.put(NVM_REG_RELAY2_MODE_STATE_TYPE, config);
   }
   if (p->ext_relay->relay[1].ppm.ppm_f > 999999 ||  p->ext_relay->relay[1].ppm.ppm_f < 0)
   {
     p->ext_relay->relay[1].ppm.ppm_f = relay2_default_thld;
     dev.nvm.put( NVM_REG_RELAY2_THRESHOLD, (uint32_t)(p->ext_relay->relay[1].ppm.ppm_f) );
   }
   p->ext_relay->relay[1].hyst_ppm.ppm_f = p->ext_relay->relay[1].ppm.ppm_f * 5 / 100;
    
    dev.cloop->error_level = dev.nvm.get( NVM_REG_RANGE_ERR_LEVEL );
    dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
    
    dev.cfg.ext_mdbs_cfg_param = dev.nvm.get( NVM_REG_EXT_MODBUS_CFG );
    if (dev.cfg.ext_mdbs_cfg.data_width != 0)
    {
      dev.cfg.ext_mdbs_cfg.data_width = 0;
      dev.nvm.put(NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);
    }
    
    if (dev.cfg.ext_mdbs_cfg.baudrate > 4)
    {
      dev.cfg.ext_mdbs_cfg.baudrate = 0;
      dev.nvm.put(NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);
    }
    
    if (dev.cfg.ext_mdbs_cfg.rtu_addr > 247)
    {
      dev.cfg.ext_mdbs_cfg.rtu_addr = CFG_RTU_MDBS_DEV_ADDR;
      dev.nvm.put(NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);      
    }
    
    if (dev.cfg.ext_mdbs_cfg.tcp_addr > 247)
    {
      dev.cfg.ext_mdbs_cfg.tcp_addr = CFG_TCP_MDBS_DEV_ADDR;
      dev.nvm.put(NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);      
    }    
    
    if (dev.cfg.ext_mdbs_cfg.parity > 2)
    {
      dev.cfg.ext_mdbs_cfg.parity = 0;
      dev.nvm.put(NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);
    }    
    
    if (dev.cfg.ext_mdbs_cfg.stop_bits > 1)
    {
      dev.cfg.ext_mdbs_cfg.stop_bits = 0;
      dev.nvm.put(NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);
    }
    
    dev.net->net_status = dev.nvm.get( NVM_REG_NET_STATE ); 
    if (dev.net->net_status == 0xFFFFFFFF)
    {
      dev.net->net_status = default_network_state;
      dev.nvm.put( NVM_REG_NET_STATE,  dev.net->net_status); 
    }
      
    dev.net->mac.u32[0] = dev.nvm.get( NVM_REG_NET_MAC_PART1 );
    dev.net->mac.u32[1] = dev.nvm.get( NVM_REG_NET_MAC_PART2 );
    if (dev.net->mac.u8[0]==0xFF && dev.net->mac.u8[1]==0xFF && dev.net->mac.u8[2]==0xFF && dev.net->mac.u8[3]==0xFF && dev.net->mac.u8[4]==0xFF && dev.net->mac.u8[5]==0xFF)
    {
      memcpy(dev.net->mac.u8, default_mac, sizeof(default_mac));             
      dev.nvm.put( NVM_REG_NET_MAC_PART1,  dev.net->mac.u32[0]);
      dev.nvm.put( NVM_REG_NET_MAC_PART2,  dev.net->mac.u32[1]);      
    }
    
    dev.net->ip.u32 = dev.nvm.get( NVM_REG_NET_IP );
    if (dev.net->ip.u32 == 0xFFFFFFFF)
    {
      memcpy(dev.net->ip.u8, default_ip, sizeof(default_ip));
      dev.nvm.put( NVM_REG_NET_IP,  dev.net->ip.u32);      
    }
    
    dev.net->sn.u32 = dev.nvm.get( NVM_REG_NET_SN );
    if (dev.net->sn.u32 == 0xFFFFFFFF)
    {
      memcpy(dev.net->sn.u8, default_sn, sizeof(default_sn));
      dev.nvm.put( NVM_REG_NET_SN,  dev.net->sn.u32);      
    }

    dev.net->gw.u32 = dev.nvm.get( NVM_REG_NET_GATEWAY );
    if (dev.net->gw.u32 == 0xFFFFFFFF)
    {
      memcpy(dev.net->gw.u8, default_gw, sizeof(default_gw));
      dev.nvm.put( NVM_REG_NET_GATEWAY,  dev.net->gw.u32);      
    }    
    
    dev.net->port_http = dev.nvm.get( NVM_REG_NET_HTTP_PORT );
    if (dev.net->port_http > 65535 )
    {
      dev.net->port_http = default_port_http;
      dev.nvm.put( NVM_REG_NET_HTTP_PORT,  dev.net->port_http);      
    }   
    
    dev.net->port_modbus = dev.nvm.get( NVM_REG_NET_MODBUS_PORT );
    if (dev.net->port_modbus > 65535 )
    {
      dev.net->port_modbus = default_port_modbus;
      dev.nvm.put( NVM_REG_NET_MODBUS_PORT,  dev.net->port_modbus);      
    }            
        
    snprintf (dev.info.real_firmware_id, sizeof(dev.info.real_firmware_id), "%s.%s.%s_%s",dev.info.device_str, dev.info.hardware_str, built_date_time, firmware_commit);    
    
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
  int32_t timestamp;
  
  // Read current zero cal value
  ppm = dev.nvm.get(NVM_REG_CAL0_PPM);
  raw = dev.nvm.get(NVM_REG_CAL0_RAW);
#ifdef EXT_FLASH
  timestamp = dev.nvm.get(NVM_REG_CAL0_TIMESTAMP);
#else
  memcpy(&timestamp, (uint32_t*)CAL0_TIMESTAMP_ADDR, sizeof(uint32_t)); 
#endif
  
  // Write current value to backup
  dev.nvm.put(NVM_REG_CAL0_RESTORE_PPM, ppm);
  dev.nvm.put(NVM_REG_CAL0_RESTORE_RAW, raw);
#ifdef EXT_FLASH
  dev.nvm.put(NVM_REG_CAL0_RESTORE_TIMESTAMP, timestamp);  
#else  
  memcpy((uint32_t*)CAL0_RESTORE_TIMESTAMP_ADDR, &timestamp, sizeof (uint32_t));
#endif
  
  p->sens->cal_back.zero.ppm.u32 = ppm;
  p->sens->cal_back.zero.ppm.i32 = (int32_t)ppm;  
  p->sens->cal_back.zero.raw.u32 = raw;   
  p->sens->cal_back.zero.timestamp.i32 = timestamp;
  p->sens->cal_back.zero.timestamp.u32 = (uint32_t)timestamp;
  
    // Write new value
  dev.nvm.put(NVM_REG_CAL0_PPM, p->sens->cal.zero.ppm.u32);
  dev.nvm.put(NVM_REG_CAL0_RAW, p->sens->cal.zero.raw.u32);
#ifdef EXT_FLASH
  dev.nvm.put(NVM_REG_CAL0_TIMESTAMP, p->sens->cal.zero.timestamp.i32);    
#else  
  memcpy((uint32_t*)CAL0_TIMESTAMP_ADDR, &(p->sens->cal.zero.timestamp.i32), sizeof (uint32_t));
#endif
 
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
  int32_t timestamp;
  
    // Read current span cal value
  ppm = dev.nvm.get(NVM_REG_CAL1_PPM);
  raw = dev.nvm.get(NVM_REG_CAL1_RAW);
#ifdef EXT_FLASH
  timestamp = dev.nvm.get(NVM_REG_CAL1_TIMESTAMP);  
#else  
  memcpy(&timestamp, (uint32_t*)CAL1_TIMESTAMP_ADDR, sizeof(uint32_t)); 
#endif  
  
    // Write current value to backup
  dev.nvm.put(NVM_REG_CAL1_RESTORE_PPM, ppm);
  dev.nvm.put(NVM_REG_CAL1_RESTORE_RAW, raw);
#ifdef EXT_FLASH
  dev.nvm.put(NVM_REG_CAL1_RESTORE_TIMESTAMP, timestamp);
#else  
  memcpy((uint32_t*)CAL1_RESTORE_TIMESTAMP_ADDR, &timestamp, sizeof (uint32_t));
#endif  
  
  p->sens->cal_back.span.ppm.u32 = ppm;
  p->sens->cal_back.span.ppm.i32 = (int32_t)ppm;  
  p->sens->cal_back.span.raw.u32 = raw; 
  p->sens->cal_back.span.timestamp.i32 = timestamp;
  p->sens->cal_back.span.timestamp.u32 = (uint32_t)timestamp;  
  
    // Write new value
  dev.nvm.put(NVM_REG_CAL1_PPM, p->sens->cal.span.ppm.u32);
  dev.nvm.put(NVM_REG_CAL1_RAW, p->sens->cal.span.raw.u32);
#ifdef EXT_FLASH
  dev.nvm.put(NVM_REG_CAL1_TIMESTAMP, p->sens->cal.span.timestamp.i32);  
#else  
  memcpy((uint32_t*)CAL1_TIMESTAMP_ADDR, &(p->sens->cal.span.timestamp.i32), sizeof (uint32_t));
#endif  
  
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
   int32_t timestamp;
   
   if( key != DEV_CONF_CAL_RESTORE_KEY )
   {
       return( -1 );
   }   
    
   // Read backup value
   ppm = dev.nvm.get(NVM_REG_CAL0_RESTORE_PPM);
   raw = dev.nvm.get(NVM_REG_CAL0_RESTORE_RAW);
#ifdef EXT_FLASH
   timestamp =  dev.nvm.get(NVM_REG_CAL0_RESTORE_TIMESTAMP);
#else   
   memcpy(&timestamp, (uint32_t*)CAL0_RESTORE_TIMESTAMP_ADDR, sizeof(uint32_t));   
#endif   
      
   // Write backup value   
   dev.nvm.put(NVM_REG_CAL0_PPM, ppm);
   dev.nvm.put(NVM_REG_CAL0_RAW, raw); 
#ifdef EXT_FLASH
   dev.nvm.put(NVM_REG_CAL0_TIMESTAMP, timestamp);    
#else   
   memcpy((uint32_t*)CAL0_TIMESTAMP_ADDR, &timestamp, sizeof (uint32_t));   
#endif   
  
   p->sens->cal.zero.ppm.u32 = ppm;
   p->sens->cal.zero.ppm.i32 = (int32_t)ppm;
   p->sens->cal.zero.raw.u32 = raw;
   p->sens->cal.zero.timestamp.i32 = timestamp;
   p->sens->cal.zero.timestamp.u32 = (uint32_t)timestamp;     
   
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
   int32_t timestamp;
   
   if( key != DEV_CONF_CAL_RESTORE_KEY )
   {
       return( -1 );
   }   
    
   // Read backup value
   ppm = dev.nvm.get(NVM_REG_CAL1_RESTORE_PPM);
   raw = dev.nvm.get(NVM_REG_CAL1_RESTORE_RAW);
#ifdef EXT_FLASH
   timestamp = dev.nvm.get(NVM_REG_CAL1_RESTORE_TIMESTAMP);
#else   
   memcpy(&timestamp, (uint32_t*)CAL1_RESTORE_TIMESTAMP_ADDR, sizeof(uint32_t));    
#endif   
      
   // Write backup value   
   dev.nvm.put(NVM_REG_CAL1_PPM, ppm);
   dev.nvm.put(NVM_REG_CAL1_RAW, raw);
#ifdef EXT_FLASH
   dev.nvm.put(NVM_REG_CAL1_TIMESTAMP, timestamp);
#else   
   memcpy((uint32_t*)CAL1_TIMESTAMP_ADDR, &timestamp, sizeof (uint32_t)); 
#endif   
  
   p->sens->cal.span.ppm.u32 = ppm;
   p->sens->cal.span.ppm.i32 = (int32_t)ppm;
   p->sens->cal.span.raw.u32 = raw;
   p->sens->cal.span.timestamp.i32 = timestamp;
   p->sens->cal.span.timestamp.u32 = (uint32_t)timestamp; 

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
    //TRACE( "\ndev_zero_update: %d\n", p->sens->cal.zero.ppm.i32 );
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
    //TRACE( "\ndev_span_update: %d\n", p->sens->cal.span.ppm.i32 );
}

int
dev_zero_restore(                       dev_t *     p,
                                const   uint16_t    key )
{
    int     err = dev_conf_zero_restore( p, key );

    //TRACE( "\ndev_zero_restore: %d\terr: %d\n", p->sens->cal.zero.ppm.i32, err );

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

    //TRACE( "\ndev_span_restore: %d\terr: %d\n", p->sens->cal.span.ppm.i32, err );

    if( err )
    {
        return( err );
    }

    econ_cal_restore( p->sens );

    return( 0 );
}

void 
dev_read_cal (dev_t *     p)
{
  uint32_t ppm;
  uint32_t raw;
  int32_t timestamp;
  
  // Read current zero cal value
  ppm = dev.nvm.get(NVM_REG_CAL0_PPM);
  raw = dev.nvm.get(NVM_REG_CAL0_RAW);
#ifdef EXT_FLASH
  timestamp = dev.nvm.get(NVM_REG_CAL0_TIMESTAMP);
#else  
  memcpy(&timestamp, (uint32_t*)CAL0_TIMESTAMP_ADDR, sizeof(uint32_t));
#endif  
  
  p->sens->cal.zero.ppm.u32 = ppm;
  p->sens->cal.zero.ppm.i32 = (int32_t)ppm;
  p->sens->cal.zero.raw.u32 = raw;
  p->sens->cal.zero.timestamp.i32 = timestamp;
  p->sens->cal.zero.timestamp.u32 = (uint32_t)timestamp;   
    
  // Read current span cal value
  ppm = dev.nvm.get(NVM_REG_CAL1_PPM);
  raw = dev.nvm.get(NVM_REG_CAL1_RAW); 
#ifdef EXT_FLASH
  timestamp = dev.nvm.get(NVM_REG_CAL1_TIMESTAMP);
#else  
  memcpy(&timestamp, (uint32_t*)CAL1_TIMESTAMP_ADDR, sizeof(uint32_t));  
#endif  
  
  p->sens->cal.span.ppm.u32 = ppm;
  p->sens->cal.span.ppm.i32 = (int32_t)ppm;
  p->sens->cal.span.raw.u32 = raw;
  p->sens->cal.span.timestamp.i32 = timestamp;
  p->sens->cal.span.timestamp.u32 = (uint32_t)timestamp;   
    
  // Read current backup_zero cal value
  ppm = dev.nvm.get(NVM_REG_CAL0_RESTORE_PPM);
  raw = dev.nvm.get(NVM_REG_CAL0_RESTORE_RAW); 
#ifdef EXT_FLASH
  timestamp = dev.nvm.get(NVM_REG_CAL0_RESTORE_TIMESTAMP); 
#else  
  memcpy(&timestamp, (uint32_t*)CAL0_RESTORE_TIMESTAMP_ADDR, sizeof(uint32_t));
#endif  
  
  p->sens->cal_back.zero.ppm.u32 = ppm;
  p->sens->cal_back.zero.ppm.i32 = (int32_t)ppm;  
  p->sens->cal_back.zero.raw.u32 = raw;  
  p->sens->cal_back.zero.timestamp.i32 = timestamp;
  p->sens->cal_back.zero.timestamp.u32 = (uint32_t)timestamp; 
  
  // Read current backup_span cal value
  ppm = dev.nvm.get(NVM_REG_CAL1_RESTORE_PPM);
  raw = dev.nvm.get(NVM_REG_CAL1_RESTORE_RAW);  
#ifdef EXT_FLASH
  timestamp = dev.nvm.get(NVM_REG_CAL1_RESTORE_TIMESTAMP);  
#else  
  memcpy(&timestamp, (uint32_t*)CAL1_RESTORE_TIMESTAMP_ADDR, sizeof(uint32_t));
#endif  
  
  p->sens->cal_back.span.ppm.u32 = ppm;
  p->sens->cal_back.span.ppm.i32 = (int32_t)ppm;  
  p->sens->cal_back.span.raw.u32 = raw;    
  p->sens->cal_back.span.timestamp.i32 = timestamp;
  p->sens->cal_back.span.timestamp.u32 = (uint32_t)timestamp; 
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
            break;
        case DEV_RANGE_IDX_R2:
            p->range[ 1].ppm    = ppm;
            break;
        case DEV_RANGE_IDX_R3:
            p->range[ 2].ppm    = ppm;
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

   send_cmd_for_nvm_write_param(NVM_REG_RANGE_UNITS, units);
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
  
 //----------------- EXT_RELAY ERRORS ------------------------------------------------  
#ifndef NO_RELAY  
  if ((previous_error_status & EXT_RELAY_LINK_ERR) != (error_status & EXT_RELAY_LINK_ERR))
  {
    if (error_status & EXT_RELAY_LINK_ERR)
      log_write_event(log, LOG_SOURCE_EXT_RELAY, LOG_EXT_RELAY_EVENT_LINK_ERR);
    else
      log_write_event(log, LOG_SOURCE_EXT_RELAY, LOG_EXT_RELAY_EVENT_LINK_OK);    
  }  
#endif
  
}