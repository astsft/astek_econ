/**
  * @file    mdbs_dev.c
  * @brief   MODBUS Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "modbus.h"
#include "modbus\server\modbus_server_dev.h"
#include "main.h"
#include "dev\dev.h"
#include "version.h"
#include "os\os_user.h"
#include "hw_relay.h"

extern  dev_t          dev;



/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_coil_read(                         const   size_t                  idx,
                                                uint8_t *              data )
{
  mdbs_err_t err = MDBS_ERR_NONE;
  bool relay_state = false;
  
  switch (idx) {
    case MDBS_COIL_FAILURE:
      if (dev.state.error_status)
        *data = 1;
      else
        *data = 0;        
      break;
      
    case MDBS_COIL_OUT_OF_MEASURE_RANGE:
      *data = 0;        
      break;
      
    case MDBS_COIL_MAINTENANCE_MODE:
      *data = 0;        
      break;
      
    case MDBS_COIL_CALIBRATION_ZERO: 
      *data = 0;
      break;
      
    case MDBS_COIL_CALIBRATION_SPAN:
      *data = 0;
      break; 
      
#if defined(USE_VALIDATION)      
    case MDBS_COIL_VALIDATION_SPAN:
      if(dev.validation.state == IN_PROGRESS)
        *data = 1;
      else
        *data = 0;
      break;
            
    case MDBS_COIL_VALIDATION_ZERO:
      if(dev.validation.state == IN_PROGRESS)
        *data = 1;
      else
        *data = 0;      
      break;
#endif          
    
    case MDBS_COIL_RESERVED_0007:
    case MDBS_COIL_RESERVED_0008:
    case MDBS_COIL_RESERVED_0009:
    case MDBS_COIL_RESERVED_000A:
    case MDBS_COIL_RESERVED_000B:
    case MDBS_COIL_RESERVED_000C:
    case MDBS_COIL_RESERVED_000D:
    case MDBS_COIL_RESERVED_000E:
    case MDBS_COIL_RESERVED_000F:  
      *data           = 0;
      break;
      
    case MDBS_COIL_RELAY_1:
      relay_get_state(EXTERNAL_1, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;
      
    case MDBS_COIL_RELAY_2:
      relay_get_state(EXTERNAL_2, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;
      
    case MDBS_COIL_RELAY_3:
      relay_get_state(EXTERNAL_3, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;
      
    case MDBS_COIL_RELAY_4:
      relay_get_state(EXTERNAL_4, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;
      
    case MDBS_COIL_RELAY_5:
      relay_get_state(EXTERNAL_5, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;
      
    case MDBS_COIL_RELAY_6:
      relay_get_state(EXTERNAL_6, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;
      
    case MDBS_COIL_RELAY_7:
      relay_get_state(EXTERNAL_7, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;
      
    case MDBS_COIL_RELAY_8:
      relay_get_state(EXTERNAL_8, &relay_state);
      if (relay_state)
        *data = 1;
      else
        *data = 0;
      break;      
    
    case MDBS_COIL_RESERVED_0018:
    case MDBS_COIL_RESERVED_0019:
    case MDBS_COIL_RESERVED_001A:
    case MDBS_COIL_RESERVED_001B:
    case MDBS_COIL_RESERVED_001C:
    case MDBS_COIL_RESERVED_001D:  
    case MDBS_COIL_RESERVED_001E: 
    case MDBS_COIL_RESERVED_001F:
      *data           = 0;
      break;
      
    default:
      *data           = 0;
      err = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
      break;
  }
  
      return( err );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_coil_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
  mdbs_err_t err = MDBS_ERR_NONE;
  bool relay_state = false;
  
  switch (idx) {
    case MDBS_COIL_FAILURE:
    case MDBS_COIL_OUT_OF_MEASURE_RANGE:
      break;
      
    case MDBS_COIL_MAINTENANCE_MODE:
      switch( *data )
      {
              case 0x0000:    break;
              case 0xFF00:    break;
              default:
                      err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                      break;
      }
      break;
      
    case MDBS_COIL_CALIBRATION_ZERO: 
      switch( *data )
      {
              case 0x0000:    break;
              case 0xFF00:    break;
              default:
                      err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                      break;
      }
      break;
      
    case MDBS_COIL_CALIBRATION_SPAN:
      switch( *data )
      {
              case 0x0000:    break;
              case 0xFF00:    break;
              default:
                      err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                      break;
      }
      break;
      
#if defined(USE_VALIDATION)      
    case MDBS_COIL_VALIDATION_SPAN:
      switch( *data )
      {
              case 0x0000:    break;
              case 0xFF00:    break;
              default:
                      err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                      break;
      }
      break;
      
    case MDBS_COIL_VALIDATION_ZERO:
      switch( *data )
      {
              case 0x0000:    break;
              case 0xFF00:    break;
              default:
                      err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                      break;
      }
      break;
#endif    
      
    case MDBS_COIL_RESERVED_0007:
    case MDBS_COIL_RESERVED_0008:
    case MDBS_COIL_RESERVED_0009:
    case MDBS_COIL_RESERVED_000A:
    case MDBS_COIL_RESERVED_000B:
    case MDBS_COIL_RESERVED_000C:
    case MDBS_COIL_RESERVED_000D:
    case MDBS_COIL_RESERVED_000E:
    case MDBS_COIL_RESERVED_000F:  
      break;
      
    case MDBS_COIL_RELAY_1:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_1, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_1, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
      
    case MDBS_COIL_RELAY_2:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_2, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_2, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
      
    case MDBS_COIL_RELAY_3:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_3, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_3, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
      
    case MDBS_COIL_RELAY_4:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_4, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_4, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
      
    case MDBS_COIL_RELAY_5:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_5, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_5, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
      
    case MDBS_COIL_RELAY_6:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_6, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_6, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
      
    case MDBS_COIL_RELAY_7:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_7, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_7, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
      
    case MDBS_COIL_RELAY_8:
    switch( *data )
    {
            case 0x0000:    relay_set_state(  EXTERNAL_8, false ); break;
            case 0xFF00:    relay_set_state(  EXTERNAL_8, true );  break;
            default:
                    err     = MDBS_ERR_ILLEGAL_DATA_VALUE;
                    break;
    }
    break;
    
    case MDBS_COIL_RESERVED_0018:
    case MDBS_COIL_RESERVED_0019:
    case MDBS_COIL_RESERVED_001A:
    case MDBS_COIL_RESERVED_001B:
    case MDBS_COIL_RESERVED_001C:
    case MDBS_COIL_RESERVED_001D:  
    case MDBS_COIL_RESERVED_001E: 
    case MDBS_COIL_RESERVED_001F:
      break;
      
    default:
      err = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
      break;
  }
  
      return( err );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_dinp_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_dinp_write(                        const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_ireg_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_ireg_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


mdbs_err_t
mdbs_hreg_read(                         const   size_t                  addr,
                                                uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;
        int32_t     ppm;
        int32_t     ppb;
        uint32_t    temp = 0;

        switch( addr )
        {
                // INFO------------------------------------------------------
                case MDBS_TERM_HREG_DEVICE_ID:
                  *data = dev.info.device_id;
                  break;
                  
                case MDBS_TERM_HREG_HARDWARE_ID:
                  *data = dev.info.hardware_id;
                  break;
                  
                case MDBS_TERM_HREG_FW_COMMIT_HI:
                  *data = strtoul(firmware_commit, NULL, 16) >> 16;
                  break;
                  
                case MDBS_TERM_HREG_FW_COMMIT_LO:
                  *data = strtoul(firmware_commit, NULL, 16) & 0xFFFF; 
                  break;
                  
                case MDBS_TERM_HREG_FW_DATE_HI:
                  *data = strtoul(built_date_time, NULL, 10) >> 16;
                  break;                  
                  
                case MDBS_TERM_HREG_FW_DATE_LO:
                  *data = strtoul(built_date_time, NULL, 10)  & 0xFFFF;
                  break;                  
                  
                case MDBS_TERM_HREG_FW_TIME_HI:
                  *data = strtoul(&built_date_time[7], NULL, 10) >> 16;
                  break;       
                  
                case MDBS_TERM_HREG_FW_TIME_LO:                  
                  *data = strtoul(&built_date_time[7], NULL, 10) & 0xFFFF;
                  break;                    
                                                      
                case MDBS_HREG_INFO_0008:
                case MDBS_HREG_INFO_0009:
                case MDBS_HREG_INFO_000A:
                case MDBS_HREG_INFO_000B:
                case MDBS_HREG_INFO_000C:
                case MDBS_HREG_INFO_000D:
                case MDBS_HREG_INFO_000E:
                case MDBS_HREG_INFO_000F: 
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_UID_00:
                  *data   = *dev.info.uid_u16[5];
                  break;
                case MDBS_TERM_HREG_UID_01:
                  *data   = *dev.info.uid_u16[4];
                  break;                  
                case MDBS_TERM_HREG_UID_02:
                  *data   = *dev.info.uid_u16[3];
                  break;                  
                case MDBS_TERM_HREG_UID_03:
                  *data   = *dev.info.uid_u16[2];
                  break;                  
                case MDBS_TERM_HREG_UID_04:
                  *data   = *dev.info.uid_u16[1];
                  break;                  
                case MDBS_TERM_HREG_UID_05:
                 *data   = *dev.info.uid_u16[0];
                  break;
                case MDBS_TERM_HREG_UID_06:
                  *data  = 0;
                  break;                                    
                case MDBS_TERM_HREG_UID_07:
                  *data  = 0;
                  break;                                    
                case MDBS_TERM_HREG_UID_08:
                  *data   = 0;
                  break;           
                  
                case MDBS_TERM_HREG_INFO_RESERVED_25:        
                case MDBS_TERM_HREG_INFO_RESERVED_26:        
                  *data   = 0;
                  break;
                case MDBS_TERM_HREG_INFO_TAG_00:
                  temp = dev.info.tag->c_tag[0];
                  temp <<= 8;
                  temp |= dev.info.tag->c_tag[1];
                  *data = temp;
                  break;
                case MDBS_TERM_HREG_INFO_TAG_01:   
                  temp = dev.info.tag->c_tag[2];
                  temp <<= 8;
                  temp |= dev.info.tag->c_tag[3];
                  *data = temp;
                  break;                  
                case MDBS_TERM_HREG_INFO_TAG_02:        
                  temp = dev.info.tag->c_tag[4];
                  temp <<= 8;
                  temp |= dev.info.tag->c_tag[5];
                  *data = temp;
                  break;                  
                case MDBS_TERM_HREG_INFO_TAG_03:
                  temp = dev.info.tag->c_tag[6];
                  temp <<= 8;
                  temp |= dev.info.tag->c_tag[7];
                  *data = temp;
                  break;                  
                case MDBS_TERM_HREG_INFO_TAG_04:                                    
                  temp = dev.info.tag->c_tag[8];
                  temp <<= 8;
                  temp |= dev.info.tag->c_tag[9];
                  *data = temp;
                  break;
                  
                //--------------------------------------------------------------                  
                  
                case MDBS_TERM_HREG_SERIAL_NUM_HI:
                  *data = dev.info.sn >> 16;
                  break;
                case MDBS_TERM_HREG_SERIAL_NUM_LO:                  
                  *data = dev.info.sn & 0x0000FFFF;
                  break;
                  
                case MDBS_TERM_HREG_DEVICE_RESERVED_258:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_259:        
                  *data = 0;
                  break;
                  
                case MDBS_TERM_HREG_FW_ID_HI:
                  *data = CONFIG_DEVICE_ID;
                  break;
                case MDBS_TERM_HREG_FW_ID_LO:
                  *data = 0x10;
                  break;
                  
                case MDBS_TERM_HREG_DEVICE_RESERVED_262:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_263: 
                  *data   = 0;
                  break;
                  
                case MDBS_TERM_HREG_RTU_MDBS_ADDR:   
                  *data = dev.cfg.ext_mdbs_cfg.rtu_addr;
                  break;
                  
                case MDBS_TERM_HREG_TCP_MDBS_ADDR: 
                  *data = dev.cfg.ext_mdbs_cfg.tcp_addr;
                  break;
                case MDBS_TERM_HREG_DEVICE_RESERVED_266:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_267:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_268:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_269:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_270:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_271:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_272:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_273:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_274:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_275:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_276:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_277:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_278:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_279:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_280:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_281:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_282:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_283:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_284:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_285:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_286:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_287:                                      
                        *data   = 0;
                        break;
                        
                // CONFIG ------------------------------------------------------        
                case MDBS_HREG_RANGE1_PPB_MSB:
                  *data   = dev.cl420.range[0].ppb >> 16;
                  break;
                case MDBS_HREG_RANGE1_PPB_LSB:
                  *data   = dev.cl420.range[0].ppb & 0x0000FFFF;
                  break;
                case MDBS_HREG_RANGE2_PPB_MSB:
                  *data   = dev.cl420.range[1].ppb >> 16;
                  break;
                case MDBS_HREG_RANGE2_PPB_LSB:
                  *data   = dev.cl420.range[2].ppb & 0x0000FFFF;
                  break;
                case MDBS_HREG_RANGE3_PPB_MSB:
                  *data   = dev.cl420.range[2].ppb >> 16;
                  break;
                case MDBS_HREG_RANGE3_PPB_LSB:
                  *data   = dev.cl420.range[2].ppb & 0x0000FFFF;
                  break;
                case MDBS_HREG_CURRENT_RANGE:
                  *data   = dev.cl420.range_idx;
                  break; 
                  
                case MDBS_TERM_HREG_STS_RESERVED_207:
                  *data   = 0;
                  break;
                
                case MDBS_HREG_LPF_CUTOFF:
                  *data = dev.cfg.lpf_cutoff;
                  break;
                  
                case MDBS_HREG_LPF_ORDER:
                  *data = dev.cfg.lpf_order;
                  break;
                  
                case MDBS_HREG_GAIN:             
                case MDBS_TERM_HREG_STS_RESERVED_20B:
                case MDBS_HREG_T_COMP:
                case MDBS_HREG_P_COMP:    
                case MDBS_TERM_HREG_STS_RESERVED_20E:
                case MDBS_TERM_HREG_STS_RESERVED_20F: 
                case MDBS_TERM_HREG_STS_RESERVED_210:
                case MDBS_TERM_HREG_STS_RESERVED_211:
                case MDBS_TERM_HREG_STS_RESERVED_212:
                case MDBS_TERM_HREG_STS_RESERVED_213:
                case MDBS_TERM_HREG_STS_RESERVED_214:
                case MDBS_TERM_HREG_STS_RESERVED_215:
                case MDBS_TERM_HREG_STS_RESERVED_216:
                case MDBS_TERM_HREG_STS_RESERVED_217:
                case MDBS_TERM_HREG_STS_RESERVED_218:
                case MDBS_TERM_HREG_STS_RESERVED_219:
                case MDBS_TERM_HREG_STS_RESERVED_21A:
                case MDBS_TERM_HREG_STS_RESERVED_21B:
                case MDBS_TERM_HREG_STS_RESERVED_21C:
                case MDBS_TERM_HREG_STS_RESERVED_21D:
                case MDBS_TERM_HREG_STS_RESERVED_21E:
                case MDBS_TERM_HREG_STS_RESERVED_21F:
                        *data   = 0;
                        break;

                //STATUS------------------------------------------------------
                case MDBS_TERM_HREG_STARTS_COUNTER:
                  *data   = dev.cfg.starts_count;
                  break;
                  
                case MDBS_TERM_HREG_STATE:
                  *data = dev.state.process_status;
                  break;
                  
                case MDBS_TERM_HREG_ERROR_REG_MSB:
                  *data   = dev.state.error_status >> 16;
                  break;
                  
                case MDBS_TERM_HREG_ERROR_REG_LSB:
                  *data   = dev.state.error_status & 0x0000FFFF;
                  break;
                  
                case MDBS_TERM_HREG_WARNING_REG_MSB:
                  *data   = dev.state.warnings_status >> 16;
                  break;
                  
                case MDBS_TERM_HREG_WARNING_REG_LSB:
                  *data   = dev.state.warnings_status & 0x0000FFFF;
                  break;
                  
                case MODBUS_HREG_STS_ERROR_FILTER_COUNT:  *data = (dev.cfg.error_filter_count); break;
                
                case MDBS_HREG_TIMESTAMP_MSB:
                  temp = dev.mcu->rtc.get_timestamp();
                  *data = temp >> 16;
                  break;
                  
                case MDBS_HREG_TIMESTAMP_LSB:
                  temp = dev.mcu->rtc.get_timestamp();
                  *data = temp&0x0000FFFF;
                  break;
                  
                case MDBS_TERM_HREG_STS_RESERVED_407:
                case MDBS_TERM_HREG_STS_RESERVED_40A:
                case MDBS_TERM_HREG_STS_RESERVED_40B:
                case MDBS_TERM_HREG_STS_RESERVED_40C:
                case MDBS_TERM_HREG_STS_RESERVED_40D:
                case MDBS_TERM_HREG_STS_RESERVED_40E:
                case MDBS_TERM_HREG_STS_RESERVED_40F:
                case MDBS_TERM_HREG_STS_RESERVED_410:
                case MDBS_TERM_HREG_STS_RESERVED_411:
                case MDBS_TERM_HREG_STS_RESERVED_412:
                case MDBS_TERM_HREG_STS_RESERVED_413:
                case MDBS_TERM_HREG_STS_RESERVED_414:
                case MDBS_TERM_HREG_STS_RESERVED_415:
                case MDBS_TERM_HREG_STS_RESERVED_416:
                case MDBS_TERM_HREG_STS_RESERVED_417:
                case MDBS_TERM_HREG_STS_RESERVED_418:
                case MDBS_TERM_HREG_STS_RESERVED_419:
                case MDBS_TERM_HREG_STS_RESERVED_41A:
                case MDBS_TERM_HREG_STS_RESERVED_41B:
                case MDBS_TERM_HREG_STS_RESERVED_41C:
                case MDBS_TERM_HREG_STS_RESERVED_41D:
                case MDBS_TERM_HREG_STS_RESERVED_41E:
                case MDBS_TERM_HREG_STS_RESERVED_41F:
                case MDBS_TERM_HREG_STS_RESERVED_420:
                case MDBS_TERM_HREG_STS_RESERVED_421:
                case MDBS_TERM_HREG_STS_RESERVED_422:
                case MDBS_TERM_HREG_STS_RESERVED_423:
                case MDBS_TERM_HREG_STS_RESERVED_424:
                case MDBS_TERM_HREG_STS_RESERVED_425:
                case MDBS_TERM_HREG_STS_RESERVED_426:
                case MDBS_TERM_HREG_STS_RESERVED_427:
                case MDBS_TERM_HREG_STS_RESERVED_428:
                case MDBS_TERM_HREG_STS_RESERVED_429:
                case MDBS_TERM_HREG_STS_RESERVED_42A:
                case MDBS_TERM_HREG_STS_RESERVED_42B:
                case MDBS_TERM_HREG_STS_RESERVED_42C:
                case MDBS_TERM_HREG_STS_RESERVED_42D:
                case MDBS_TERM_HREG_STS_RESERVED_42E:
                case MDBS_TERM_HREG_STS_RESERVED_42F:
                case MDBS_TERM_HREG_STS_RESERVED_430:
                case MDBS_TERM_HREG_STS_RESERVED_431:
                case MDBS_TERM_HREG_STS_RESERVED_432:
                case MDBS_TERM_HREG_STS_RESERVED_433:
                case MDBS_TERM_HREG_STS_RESERVED_434:
                case MDBS_TERM_HREG_STS_RESERVED_435:
                case MDBS_TERM_HREG_STS_RESERVED_436:
                case MDBS_TERM_HREG_STS_RESERVED_437:
                case MDBS_TERM_HREG_STS_RESERVED_438:
                case MDBS_TERM_HREG_STS_RESERVED_439:
                case MDBS_TERM_HREG_STS_RESERVED_43A:
                case MDBS_TERM_HREG_STS_RESERVED_43B:
                case MDBS_TERM_HREG_STS_RESERVED_43C:
                case MDBS_TERM_HREG_STS_RESERVED_43D:
                case MDBS_TERM_HREG_STS_RESERVED_43E:
                case MDBS_TERM_HREG_STS_RESERVED_43F:
                        *data   = 0;
                        break;
                //--------------------------------------------------------------
#if defined(USE_VALIDATION)                        
                case MDBS_VALIDATION_VALUE_PPB_MSB:
                  ppb         = dev.validation.value.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;
                  
                case MDBS_VALIDATION_VALUE_PPB_LSB:
                  ppb         = dev.validation.value.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb & 0xFFFF;
                  break;                  
                  
                case MDBS_VALIDATION_DEVIATION_PPB_MSB:
                  ppb         = dev.validation.deviation.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;
                  
                case MDBS_VALIDATION_DEVIATION_PPB_LSB:
                  ppb         = dev.validation.deviation.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb & 0xFFFF;
                  break;  
                  
                case MDBS_VALIDATION_TRANSITION_TIME:                 
                  *data   = dev.validation.timings.transition_time; 
                  break;                   
                  
                case MDBS_VALIDATION_MEASURE_TIME:                 
                  *data   = dev.validation.timings.measure_time; 
                  break;                                     
                  
                case MDBS_VALIDATION_RETURN_TIME:                 
                  *data   = dev.validation.timings.return_time; 
                  break;    
                  
                case MDBS_VALIDATION_RESERVED_507:
                case MDBS_VALIDATION_RESERVED_508:  
                  *data   = 0;
                  break;
                  
                  
                case MDBS_VALIDATION_START:                 
                  *data = dev.validation.start;
                  break;    
                  
                case MDBS_VALIDATION_RESERVED_50A:
                case MDBS_VALIDATION_RESERVED_50B:  
                  *data   = 0;
                  break;           
                  
                case MDBS_VALIDATION_PROCESS_VALUE_PPB_MSB:
                  ppb         = (dev.validation.ppb_hi << 16) | (dev.validation.ppb_lo & 0xFFFF);                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;                   
                  
                case MDBS_VALIDATION_PROCESS_VALUE_PPB_LSB:
                  ppb         = (dev.validation.ppb_hi << 16) | (dev.validation.ppb_lo & 0xFFFF);
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = dev.validation.ppb_lo;
                  break;                                    
                  
                case MDBS_VALIDATION_STATE:
                  *data   = dev.validation.state; 
                  break;         
                  
                case MDBS_VALIDATION_RESERVED_50F:
                case MDBS_VALIDATION_RESERVED_510:  
                  *data   = 0;
                  break;                   
                                                      
                case MDBS_VALIDATION_MEAS_VALUE_PPB_MSB:
                  ppb         = dev.validation.measure_value;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;
                  
                case MDBS_VALIDATION_MEAS_VALUE_PPB_LSB:
                  ppb         = dev.validation.measure_value;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb & 0xFFFF;
                  break;  
                  
                case MDBS_VALIDATION_MEAS_DEVIATION_PPB_MSB:
                  ppb         = dev.validation.measure_deviation;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;
                  
                case MDBS_VALIDATION_MEAS_DEVIATION_PPB_LSB:
                  ppb         = dev.validation.measure_deviation;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb & 0xFFFF;
                  break;             
                  
                case MDBS_VALIDATION_RESULT:                 
                  *data   = dev.validation.result; 
                  break;                
#endif             
                  
#if defined(USE_REMOTE_CALIBRATION) 
                case MDBS_REMOTE_CALIBRATION_SPAN_VALUE_PPB_MSB:
                  ppb         = dev.calibration.span.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;
                  
                case MDBS_REMOTE_CALIBRATION_SPAN_VALUE_PPB_LSB:
                  ppb         = dev.calibration.span.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb & 0xFFFF;
                  break;                  
                  
                case MDBS_REMOTE_CALIBRATION_ZERO_VALUE_PPB_MSB:
                  ppb         = dev.calibration.zero.ppb;                         
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;
                  
                case MDBS_REMOTE_CALIBRATION_ZERO_VALUE_PPB_LSB:
                  ppb         = dev.calibration.zero.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb & 0xFFFF;
                  break;  
                  
                case MDBS_REMOTE_CALIBRATION_DEVIATION_VALUE_PPB_MSB:
                  ppb         = dev.calibration.deviation.ppb;                         
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;
                  
                case MDBS_REMOTE_CALIBRATION_DEVIATION_VALUE_PPB_LSB:
                  ppb         =  dev.calibration.deviation.ppb;                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb & 0xFFFF;
                  break;                    
                  
                case MDBS_REMOTE_CALIBRATION_TRANSITION_TIME:                 
                  *data   = dev.calibration.timings.transition_time; 
                  break;                   
                  
                case MDBS_REMOTE_CALIBRATION_MEASURE_TIME:                 
                  *data   = dev.calibration.timings.measure_time; 
                  break;                                     
                  
                case MDBS_REMOTE_CALIBRATION_RETURN_TIME:                 
                  *data   = dev.calibration.timings.return_time; 
                  break;    
                  
                case MDBS_REMOTE_CALIBRATION_SLOPE_TIME:                 
                  *data   = dev.calibration.timings.stable_slope_time; 
                  break;          
                  
                case MDBS_REMOTE_CALIBRATION_SLOPE_VALUE:                 
                  *data   = dev.calibration.max_slope; 
                  break;   
                  
                case MDBS_REMOTE_CALIBRATION_ENABLE:                 
                  *data = dev.calibration.remote_calibration_enabled;
                  break;                      
                  
                case MDBS_REMOTE_CALIBRATION_RESERVED_070C:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_070D:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_070E:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_070F: 
                  *data = 0;
                  break;    
                  
                                                                                       
                case MDBS_REMOTE_CALIBRATION_SPAN_START:
                  if (dev.state.process_status == PROCESS_CALIBRATION_SPAN)
                    *data = dev.calibration.start;
                  else 
                    *data = 0;
                  break;    
                  
                case MDBS_REMOTE_CALIBRATION_ZERO_START:                 
                  if (dev.state.process_status == PROCESS_CALIBRATION_ZERO)
                    *data = dev.calibration.start;
                  else 
                    *data = 0;                  
                  break;             
                  
                case MDBS_REMOTE_CALIBRATION_RESERVED_0712:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_0713:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_0714:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_0715: 
                  *data = 0;
                  break;  
                  
                case MDBS_REMOTE_CALIBRATION_PROCESS_SLOPE:
                    *data   = dev.sens->meas.slope;
                  break;                      
                                                                                                                            
                case MDBS_REMOTE_CALIBRATION_PROCESS_VALUE_PPB_MSB:
                  ppb         = (dev.calibration.ppb_hi << 16) | (dev.calibration.ppb_lo & 0xFFFF);                        
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data   = ppb >> 16;
                  break;                   
                  
                case MDBS_REMOTE_CALIBRATION_PROCESS_VALUE_PPB_LSB:
                  ppb         = (dev.calibration.ppb_hi << 16) | (dev.calibration.ppb_lo & 0xFFFF);  
                  if (ppb < 0) 
                    *data = 0;
                  else
                    *data = dev.calibration.ppb_lo;
                  break;                                    
                  
                case MDBS_REMOTE_CALIBRATION_STATE:
                  *data   = dev.calibration.state; 
                  break;         
                  
                case MDBS_REMOTE_CALIBRATION_RESERVED_071A:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_071B:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_071C:            
                case MDBS_REMOTE_CALIBRATION_RESERVED_071D: 
                  *data = 0;
                  break;                    
                                    
                case MDBS_REMOTE_CALIBRATION_RESULT:                 
                  *data   = dev.calibration.result; 
                  break;                  
#endif                  
                        
                //--------------------------------------------------------------
                        
                case MDBS_TERM_HREG_SENS_OXGN_PPM_HI:
                        ppm = dev.sens->meas.ppm.i32;
                        if (ppm < 0)
                          *data = 0;
                        else
                          *data   = dev.sens->meas.ppm.i32 >> 16;
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_PPM_LO:
                        ppm = dev.sens->meas.ppm.i32;
                        if (ppm < 0)
                          *data = 0;
                        else
                          *data   = dev.sens->meas.ppm.i32 & 0xFFFF;
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_DIGC_PRIM:
                        *data   = dev.sens->meas.digc.integral;
                        break;
                        
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_SEC:
                        *data   = 0;
                        break;                        
                        
                case MDBS_TERM_HREG_SENS_PRES_PA_HI:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_PRES_PA_LO:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RESERVED_802:
                case MDBS_TERM_HREG_RESERVED_803:
                case MDBS_TERM_HREG_RESERVED_808:
                case MDBS_TERM_HREG_RESERVED_809:
                case MDBS_TERM_HREG_RESERVED_80A:
                case MDBS_TERM_HREG_RESERVED_80B:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_SLOPE:
                        *data   = dev.sens->meas.slope;
                        break;

                case MDBS_TERM_HREG_RESERVED_80D:
                case MDBS_TERM_HREG_RESERVED_80E:
                case MDBS_TERM_HREG_RESERVED_80F:
                        *data   = 0;
                        break;
                        
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_cDGR:
                        *data   = 0;
                        break;
                                
                case MDBS_TERM_HREG_SENS_VDDA_mV:
                        *data   = 0;
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_812:
                case MDBS_TERM_HREG_RESERVED_813:
                        *data   = 0;
                        break;
                        
                case MDBS_TERM_HREG_SENS_ADC_mV:
                        *data   = 0;
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_815:
                case MDBS_TERM_HREG_RESERVED_816:
                case MDBS_TERM_HREG_RESERVED_817:                  
                        *data   = 0;
                        break;
                              
                case MDBS_TERM_HREG_SENS_ADC_RAW_HI:
                        *data   = dev.sens->meas.raw >> 16;
                        break;

                case MDBS_TERM_HREG_SENS_ADC_RAW_LO:
                        *data   = dev.sens->meas.raw & 0x0000FFFF;
                        break;

                case MDBS_TERM_HREG_RESERVED_81A:
                case MDBS_TERM_HREG_RESERVED_81B:
                case MDBS_TERM_HREG_RESERVED_81C:
                case MDBS_TERM_HREG_RESERVED_81D:
                case MDBS_TERM_HREG_RESERVED_81E:          
                case MDBS_TERM_HREG_RESERVED_81F:                            
                        *data = 0;  
                        break;                        
                        
          //--------------------------------------------------------------                         
                        

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_TIMESTMP_HI:
                        *data   = dev.sens->cal.zero.timestamp.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_TIMESTMP_LO:
                        *data   = dev.sens->cal.zero.timestamp.u16[0];
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_C02:
                case MDBS_TERM_HREG_RESERVED_C03:        
                case MDBS_TERM_HREG_RESERVED_C04:
                case MDBS_TERM_HREG_RESERVED_C05:        
                case MDBS_TERM_HREG_RESERVED_C06:
                case MDBS_TERM_HREG_RESERVED_C07:        
                case MDBS_TERM_HREG_RESERVED_C08:
                case MDBS_TERM_HREG_RESERVED_C09:        
                case MDBS_TERM_HREG_RESERVED_C0A:
                case MDBS_TERM_HREG_RESERVED_C0B:        
                case MDBS_TERM_HREG_RESERVED_C0C:
                case MDBS_TERM_HREG_RESERVED_C0D:               
                  *data = 0;
                  break;                        

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_HI:
                        *data   = dev.sens->cal.zero.ppm.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_LO:
                        *data   = dev.sens->cal.zero.ppm.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_TIMESTMP_HI:
                        *data   = dev.sens->cal.span.timestamp.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_TIMESTMP_LO:
                        *data   = dev.sens->cal.span.timestamp.u16[ 0];
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_C12:
                case MDBS_TERM_HREG_RESERVED_C13:        
                case MDBS_TERM_HREG_RESERVED_C14:
                case MDBS_TERM_HREG_RESERVED_C15:        
                case MDBS_TERM_HREG_RESERVED_C16:
                case MDBS_TERM_HREG_RESERVED_C17:        
                case MDBS_TERM_HREG_RESERVED_C18:
                case MDBS_TERM_HREG_RESERVED_C19:        
                case MDBS_TERM_HREG_RESERVED_C1A:
                case MDBS_TERM_HREG_RESERVED_C1B:        
                case MDBS_TERM_HREG_RESERVED_C1C:
                case MDBS_TERM_HREG_RESERVED_C1D:                         
                  *data = 0;
                  break;                        

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_HI:
                        *data   = dev.sens->cal.span.ppm.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_LO:
                        *data   = dev.sens->cal.span.ppm.u16[ 0];
                        break;
                        
                case MDBS_TERM_HREG_SENS_OXGN_PPB_HI:
                        ppb         = (dev.sens->modbus_ppb_hi << 16) | (dev.sens->modbus_ppb_lo & 0xFFFF);                        
                        if (ppb < 0) 
                          *data = 0;
                        else
                          *data   = ppb >> 16;
                        break;                  
                  
                case MDBS_TERM_HREG_SENS_OXGN_PPB_LO:
                       ppb         = (dev.sens->modbus_ppb_hi << 16) | (dev.sens->modbus_ppb_lo & 0xFFFF);                       
                       if (ppb < 0) 
                         *data = 0;
                       else
                         *data   = ppb & 0x0000FFFF;
                       break;
                       
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_PRIM_2:
                      temp = dev.sens->meas.digc.integral * 100;
                      temp += dev.sens->meas.digc.fractional; 
                      *data = temp;
                      break;
                      
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_SEC_2:                      
                case MDBS_TERM_HREG_SENS_PRES_PA_HI_2:
                case MDBS_TERM_HREG_SENS_PRES_PA_LO_2:  
                     *data = 0;
                     break;
                        
                case MDBS_TERM_HREG_SENS_SLOPE_2:
                        *data = dev.sens->meas.slope;
                        break;     
                        
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_cDGR_2:                           
                case MDBS_TERM_HREG_SENS_VDDA_mV_2:        
                case MDBS_TERM_HREG_SENS_ADC_mV_2:
                case MDBS_TERM_HREG_SENS_ADC_RAW_HI_2:
                case MDBS_TERM_HREG_SENS_ADC_RAW_LO_2:        
                        *data   = 0;
                        break;                        
        
                case MDBS_TERM_HREG_RESERVED_1802:
                case MDBS_TERM_HREG_RESERVED_1803:        
                case MDBS_TERM_HREG_RESERVED_1808:
                case MDBS_TERM_HREG_RESERVED_1809:
                case MDBS_TERM_HREG_RESERVED_180A:
                case MDBS_TERM_HREG_RESERVED_180B:
                case MDBS_TERM_HREG_RESERVED_180D:
                case MDBS_TERM_HREG_RESERVED_180E:
                case MDBS_TERM_HREG_RESERVED_180F:
                case MDBS_TERM_HREG_RESERVED_1812:
                case MDBS_TERM_HREG_RESERVED_1813:
                case MDBS_TERM_HREG_RESERVED_1815:
                case MDBS_TERM_HREG_RESERVED_1816:
                case MDBS_TERM_HREG_RESERVED_1817:        
                case MDBS_TERM_HREG_RESERVED_181A:
                case MDBS_TERM_HREG_RESERVED_181B:
                case MDBS_TERM_HREG_RESERVED_181C:  
                case MDBS_TERM_HREG_RESERVED_181D:
                case MDBS_TERM_HREG_RESERVED_181E:
                case MDBS_TERM_HREG_RESERVED_181F:                         
                        *data = 0;
                        break;
                //--------------------------------------------------------------        
                        
                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_TIMESTMP_HI:
                        *data = dev.sens->cal.zero.timestamp.u16[1];                  
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_TIMESTMP_LO:                       
                        *data = dev.sens->cal.zero.timestamp.u16[0];                        
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1C03:
                case MDBS_TERM_HREG_RESERVED_1C04:    
                case MDBS_TERM_HREG_RESERVED_1C05:
                case MDBS_TERM_HREG_RESERVED_1C06:        
                case MDBS_TERM_HREG_RESERVED_1C07:
                case MDBS_TERM_HREG_RESERVED_1C08:
                case MDBS_TERM_HREG_RESERVED_1C09:        
                case MDBS_TERM_HREG_RESERVED_1C0A:
                case MDBS_TERM_HREG_RESERVED_1C0B: 
                        *data = 0;
                        break;
                case MDBS_TERM_HREG_SENS_CAL_ZERO_ADC_RAW_MSB:
                        *data = dev.sens->cal.zero.raw.u16[1];
                        break;
                
                case MDBS_TERM_HREG_SENS_CAL_ZERO_ADC_RAW_LSB:
                        *data = dev.sens->cal.zero.raw.u16[0];
                        break;
                  
                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_HI:
                        *data = dev.sens->cal.zero.ppb.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_LO:
                        *data = dev.sens->cal.zero.ppb.u16[0];
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_TIMESTMP_HI:
                        *data = dev.sens->cal.span.timestamp.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_TIMESTMP_LO:
                        *data = dev.sens->cal.span.timestamp.u16[0];                  
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1C12:
                case MDBS_TERM_HREG_RESERVED_1C13:    
                case MDBS_TERM_HREG_RESERVED_1C14:
                case MDBS_TERM_HREG_RESERVED_1C15:     
                case MDBS_TERM_HREG_RESERVED_1C16:
                case MDBS_TERM_HREG_RESERVED_1C17:        
                case MDBS_TERM_HREG_RESERVED_1C18:
                case MDBS_TERM_HREG_RESERVED_1C19:        
                case MDBS_TERM_HREG_RESERVED_1C1A:
                case MDBS_TERM_HREG_RESERVED_1C1B:  
                        *data = 0;
                        break;
                case MDBS_TERM_HREG_SENS_CAL_SPAN_ADC_RAW_MSB:
                        *data = dev.sens->cal.span.raw.u16[1];
                        break;
                case MDBS_TERM_HREG_SENS_CAL_SPAN_ADC_RAW_LSB:  
                        *data = dev.sens->cal.span.raw.u16[0];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_HI:
                        *data = dev.sens->cal.span.ppb.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_LO:
                        *data = dev.sens->cal.span.ppb.u16[0];
                        break;   
                        
 //--------------------------------------------------------------                                
                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_TIMESTMP_HI:
                        *data = dev.sens->cal_back.zero.timestamp.u16[1];                  
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_TIMESTMP_LO:                       
                        *data = dev.sens->cal_back.zero.timestamp.u16[0];                        
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1D02:
                case MDBS_TERM_HREG_RESERVED_1D03:    
                case MDBS_TERM_HREG_RESERVED_1D04:
                case MDBS_TERM_HREG_RESERVED_1D05:        
                case MDBS_TERM_HREG_RESERVED_1D06:
                case MDBS_TERM_HREG_RESERVED_1D07:       
                case MDBS_TERM_HREG_RESERVED_1D08:
                case MDBS_TERM_HREG_RESERVED_1D09:        
                case MDBS_TERM_HREG_RESERVED_1D0A:
                case MDBS_TERM_HREG_RESERVED_1D0B:        
                        *data = 0;
                        break;
                  
                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_ADC_RAW_MSB:
                        *data = dev.sens->cal_back.zero.raw.u16[1];
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_ADC_RAW_LSB:   
                        *data =  dev.sens->cal_back.zero.raw.u16[0];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_HI:
                        *data = dev.sens->cal_back.zero.ppb.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_LO:
                        *data = dev.sens->cal_back.zero.ppb.u16[0];
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_TIMESTMP_HI:
                        *data = dev.sens->cal_back.span.timestamp.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_TIMESTMP_LO:
                        *data = dev.sens->cal_back.span.timestamp.u16[0];                  
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1D12:                        
                case MDBS_TERM_HREG_RESERVED_1D13:    
                case MDBS_TERM_HREG_RESERVED_1D14:
                case MDBS_TERM_HREG_RESERVED_1D15:     
                case MDBS_TERM_HREG_RESERVED_1D16:
                case MDBS_TERM_HREG_RESERVED_1D17:        
                case MDBS_TERM_HREG_RESERVED_1D18:
                case MDBS_TERM_HREG_RESERVED_1D19:        
                case MDBS_TERM_HREG_RESERVED_1D1A:
                case MDBS_TERM_HREG_RESERVED_1D1B:   
                        *data = 0;
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_ADC_RAW_MSB:
                        *data = dev.sens->cal_back.span.raw.u16[1];                  
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_ADC_RAW_LSB:    
                        *data = dev.sens->cal_back.span.raw.u16[0];                  
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_HI:
                        *data = dev.sens->cal_back.span.ppb.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_LO:
                        *data = dev.sens->cal_back.span.ppb.u16[0];
                        break;                            

                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}


mdbs_err_t
mdbs_hreg_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;
        static uint32_t temp = 0;
        struct tm *     ts;
        int32_t ppb;
        static time_t time_temp;

        switch( idx )
        {
                case MDBS_TERM_HREG_DEVICE_ID:
                case MDBS_TERM_HREG_HARDWARE_ID:
                case MDBS_TERM_HREG_FW_COMMIT_HI:
                case MDBS_TERM_HREG_FW_COMMIT_LO:
                case MDBS_TERM_HREG_FW_DATE_HI:
                case MDBS_TERM_HREG_FW_DATE_LO:
                case MDBS_TERM_HREG_FW_TIME_HI:
                case MDBS_TERM_HREG_FW_TIME_LO:      
                case MDBS_HREG_INFO_0008:
                case MDBS_HREG_INFO_0009:
                case MDBS_HREG_INFO_000A:
                case MDBS_HREG_INFO_000B:
                case MDBS_HREG_INFO_000C:
                case MDBS_HREG_INFO_000D:
                case MDBS_HREG_INFO_000E:
                case MDBS_HREG_INFO_000F:                  
                case MDBS_TERM_HREG_UID_00:
                case MDBS_TERM_HREG_UID_01:
                case MDBS_TERM_HREG_UID_02:
                case MDBS_TERM_HREG_UID_03:
                case MDBS_TERM_HREG_UID_04:
                case MDBS_TERM_HREG_UID_05:
                case MDBS_TERM_HREG_UID_06:
                case MDBS_TERM_HREG_UID_07:
                case MDBS_TERM_HREG_INFO_RESERVED_25:        
                case MDBS_TERM_HREG_INFO_RESERVED_26:   
                    break;
                    
                case MDBS_TERM_HREG_INFO_TAG_00:
                  temp = *data;
                  dev.info.tag->c_tag[0] = temp >> 8;
                  dev.info.tag->c_tag[1] = temp & 0x00FF; 
                  break;                  
                case MDBS_TERM_HREG_INFO_TAG_01: 
                  temp = *data;
                  dev.info.tag->c_tag[2] = temp >> 8;
                  dev.info.tag->c_tag[3] = temp & 0x00FF; 
                  break;                   
                case MDBS_TERM_HREG_INFO_TAG_02:        
                  temp = *data;
                  dev.info.tag->c_tag[4] = temp >> 8;
                  dev.info.tag->c_tag[5] = temp & 0x00FF; 
                  break;                   
                case MDBS_TERM_HREG_INFO_TAG_03:
                  temp = *data;
                  dev.info.tag->c_tag[6] = temp >> 8;
                  dev.info.tag->c_tag[7] = temp & 0x00FF; 
                  break;                   
                case MDBS_TERM_HREG_INFO_TAG_04:                  
                  temp = *data;
                  dev.info.tag->c_tag[8] = temp >> 8;
                  dev.info.tag->c_tag[9] = temp & 0x00FF; 
                  break;
                        
                case MDBS_TERM_HREG_DEVICE_RESERVED_255:                 
                  if (*data == 0x19D8 && temp!= 0)
                  {                    
                    dev.info.sn = temp;
                    send_cmd_for_nvm_write_param(NVM_REG_SERNUM, dev.info.sn);                     
                  }
                  temp = 0;
                  break;
                  
                case MDBS_TERM_HREG_SERIAL_NUM_HI:
                  temp = *data;
                  temp <<= 16;
                  break;
                case MDBS_TERM_HREG_SERIAL_NUM_LO:                  
                  temp |= *data;
                  break;     
                  
                case MDBS_TERM_HREG_DEVICE_RESERVED_258:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_259:        
                case MDBS_TERM_HREG_FW_ID_HI:
                case MDBS_TERM_HREG_FW_ID_LO:
                case MDBS_TERM_HREG_DEVICE_RESERVED_262:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_263:  
                  break;
                  
                case MDBS_TERM_HREG_RTU_MDBS_ADDR:     
                  dev.cfg.ext_mdbs_cfg.rtu_addr = *data;
                  send_cmd_for_nvm_write_param (NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);
                  break;
                  
                case MDBS_TERM_HREG_TCP_MDBS_ADDR:     
                  dev.cfg.ext_mdbs_cfg.tcp_addr = *data;
                  send_cmd_for_nvm_write_param (NVM_REG_EXT_MODBUS_CFG, dev.cfg.ext_mdbs_cfg_param);
                  break;
                  
                case MDBS_TERM_HREG_DEVICE_RESERVED_266:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_267:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_268:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_269:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_270:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_271:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_272:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_273:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_274:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_275:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_276:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_277:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_278:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_279:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_280:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_281:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_282:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_283:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_284:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_285:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_286:        
                case MDBS_TERM_HREG_DEVICE_RESERVED_287:  
                  //do nothing, read-only registers
                  break;                        
                        
                case MDBS_TERM_HREG_STATE:
                case MDBS_TERM_HREG_STARTS_COUNTER:
                        //do nothing, read-only registers
                        break;
                        
                case MDBS_HREG_RANGE1_PPB_MSB:
                    dev.cl420.range[0].ppb = *data << 16 ;                    
                    break;
                case MDBS_HREG_RANGE1_PPB_LSB:
                    dev.cl420.range[0].ppb |= *data;
                    dev.cl420.range[0].ppm = dev.cl420.range[0].ppb / 1000;
                    send_cmd_for_cloop_write_range(); 
                    break;                 
                case MDBS_HREG_RANGE2_PPB_MSB:
                    dev.cl420.range[1].ppb = *data << 16;
                    break;           
                case MDBS_HREG_RANGE2_PPB_LSB:
                    dev.cl420.range[1].ppb |= *data;
                    dev.cl420.range[1].ppm = dev.cl420.range[1].ppb / 1000;
                    send_cmd_for_cloop_write_range();
                    break;                 
                case MDBS_HREG_RANGE3_PPB_MSB:
                    dev.cl420.range[2].ppb = *data << 16;
                    break;               
                case MDBS_HREG_RANGE3_PPB_LSB:
                    dev.cl420.range[2].ppb |= *data;
                    dev.cl420.range[2].ppm = dev.cl420.range[2].ppb / 1000;                    
                    send_cmd_for_cloop_write_range();
                    break;                 
                case MDBS_HREG_CURRENT_RANGE:
                    dev.cl420.range_idx = *data;
                    send_cmd_for_cloop_write_range();
                    break;        
                    
                case MDBS_HREG_LPF_CUTOFF:
                  dev.cfg.lpf_cutoff = *data;
                  task_ibus_sens_conf_lpf_update(dev.cfg.lpf_cutoff,
                                                 dev.cfg.lpf_order);
                  break;
                  
                case MDBS_HREG_LPF_ORDER:
                  dev.cfg.lpf_order = *data;
                  task_ibus_sens_conf_lpf_update(dev.cfg.lpf_cutoff,
                                                 dev.cfg.lpf_order);                  
                   break;                    
                    
                case MODBUS_HREG_STS_ERROR_FILTER_COUNT:    if (*data <= 10) dev.cfg.error_filter_count = *data;  break;         
                
                case MDBS_HREG_TIMESTAMP_MSB:
                  time_temp = *data;
                  time_temp <<= 16;
                  break;
                  
                case MDBS_HREG_TIMESTAMP_LSB:
                  time_temp |= *data;
                  ts = localtime(&time_temp); 
                  time_temp = 0;
                  dev.mcu->rtc.set_date( ts->tm_year, ts->tm_mon, ts->tm_mday);
                  dev.mcu->rtc.set_time(ts->tm_hour, ts->tm_min, ts->tm_sec);
                  break;                    
//------------------------------------------------------------------------------

#if defined(USE_VALIDATION)                  
                case MDBS_VALIDATION_VALUE_PPB_MSB:
                        ppb = *data;
                        ppb <<= 16;
                        dev.validation.value.ppb = ppb;
                        break;

                case MDBS_VALIDATION_VALUE_PPB_LSB:
                        ppb = *data;
                        dev.validation.value.ppb |=ppb ;
                        break;                   
                  
                case MDBS_VALIDATION_DEVIATION_PPB_MSB:
                        ppb = *data;
                        ppb <<= 16;
                        dev.validation.deviation.ppb = ppb;
                        break;
                        
                case MDBS_VALIDATION_DEVIATION_PPB_LSB:                        
                        ppb = *data;
                        dev.validation.deviation.ppb |=ppb ;
                        break;    
                        
                case MDBS_VALIDATION_TRANSITION_TIME:
                        dev.validation.timings.transition_time = *data;
                        break;
                        
                case MDBS_VALIDATION_MEASURE_TIME:
                        dev.validation.timings.measure_time = *data;                  
                        break;
                        
                case MDBS_VALIDATION_RETURN_TIME:                       
                        dev.validation.timings.return_time = *data;                  
                        break;
                        
                case MDBS_VALIDATION_START:
                        switch (*data) {
                        case VALIDATION_STOP: 
                           dev.validation.start = VALIDATION_STOP;
                          send_cmd_for_remote_validation_break();
                          break;                        
                        case VALIDATION_REMOTE_START:
                           dev.validation.start = VALIDATION_REMOTE_START;
                           send_cmd_for_remote_validation_start();
                           break;                                                  
                        }
                        break;
                        
                case MDBS_VALIDATION_RESERVED_507:
                case MDBS_VALIDATION_RESERVED_508: 
                case MDBS_VALIDATION_RESERVED_50A:
                case MDBS_VALIDATION_RESERVED_50B:  
                case MDBS_VALIDATION_PROCESS_VALUE_PPB_MSB:
                case MDBS_VALIDATION_PROCESS_VALUE_PPB_LSB:                      
                case MDBS_VALIDATION_STATE:     
                case MDBS_VALIDATION_RESERVED_50F:
                case MDBS_VALIDATION_RESERVED_510:                   
                case MDBS_VALIDATION_MEAS_VALUE_PPB_MSB:
                case MDBS_VALIDATION_MEAS_VALUE_PPB_LSB:
                case MDBS_VALIDATION_MEAS_DEVIATION_PPB_MSB:
                case MDBS_VALIDATION_MEAS_DEVIATION_PPB_LSB: 
                case MDBS_VALIDATION_RESULT:                  
                        break;
#endif            
                        
#if defined(USE_REMOTE_CALIBRATION)                  
                case MDBS_REMOTE_CALIBRATION_SPAN_VALUE_PPB_MSB:
                        ppb = *data;
                        ppb <<= 16;
                        dev.calibration.span.ppb = ppb;
                        break;

                case MDBS_REMOTE_CALIBRATION_SPAN_VALUE_PPB_LSB:
                        ppb = *data;
                        dev.calibration.span.ppb |=ppb ;
                        break;                   
                  
                case MDBS_REMOTE_CALIBRATION_ZERO_VALUE_PPB_MSB:
                        ppb = *data;
                        ppb <<= 16;
                        dev.calibration.zero.ppb = ppb;
                        break;
                        
                case MDBS_REMOTE_CALIBRATION_ZERO_VALUE_PPB_LSB:                        
                        ppb = *data;
                        dev.calibration.zero.ppb |=ppb ;
                        break;    
                        
                case MDBS_REMOTE_CALIBRATION_DEVIATION_VALUE_PPB_MSB:
                        ppb = *data;
                        ppb <<= 16;
                        dev.calibration.deviation.ppb = ppb;
                        break;
                        
                case MDBS_REMOTE_CALIBRATION_DEVIATION_VALUE_PPB_LSB:                        
                        ppb = *data;
                        dev.calibration.deviation.ppb |=ppb ;
                        break;                           
                        
                case MDBS_REMOTE_CALIBRATION_TRANSITION_TIME:
                        dev.calibration.timings.transition_time = *data;
                        break;
                        
                case MDBS_REMOTE_CALIBRATION_MEASURE_TIME:
                        dev.calibration.timings.measure_time = *data;                  
                        break;
                        
                case MDBS_REMOTE_CALIBRATION_RETURN_TIME:                       
                        dev.calibration.timings.return_time = *data;                  
                        break;
                        
                case MDBS_REMOTE_CALIBRATION_SLOPE_TIME:                       
                        dev.calibration.timings.stable_slope_time = *data;
                        break;    
                        
                case MDBS_REMOTE_CALIBRATION_SLOPE_VALUE:                       
                        dev.calibration.max_slope = *data;                  
                        break;        
                        
                case MDBS_REMOTE_CALIBRATION_ENABLE:
                        dev.calibration.remote_calibration_enabled = *data;
                        break;

                case MDBS_REMOTE_CALIBRATION_RESERVED_070C:                        
                case MDBS_REMOTE_CALIBRATION_RESERVED_070D:
                case MDBS_REMOTE_CALIBRATION_RESERVED_070E:                        
                case MDBS_REMOTE_CALIBRATION_RESERVED_070F:  
                        break;
                        
                case MDBS_REMOTE_CALIBRATION_SPAN_START:
                        switch (*data) {
                        case CALIBRATION_STOP: 
                           dev.calibration.start = CALIBRATION_STOP;
                          send_cmd_for_remote_calibration_break();
                          break;                        
                        case CALIBRATION_REMOTE_START:
                           dev.state.process_status = PROCESS_CALIBRATION_SPAN;
                           dev.calibration.start = CALIBRATION_REMOTE_START;
                           send_cmd_for_remote_calibration_span_start();
                           break;                                                  
                        }
                        break;
                        
                case MDBS_REMOTE_CALIBRATION_ZERO_START:
                        switch (*data) {
                        case CALIBRATION_STOP: 
                          dev.calibration.start = CALIBRATION_STOP;
                          send_cmd_for_remote_calibration_break();
                          break;                        
                        case CALIBRATION_REMOTE_START:
                           dev.state.process_status = PROCESS_CALIBRATION_ZERO;
                           dev.calibration.start = CALIBRATION_REMOTE_START;
                           send_cmd_for_remote_calibration_zero_start();
                           break;                                                  
                        }
                        break;   
                        
                case MDBS_REMOTE_CALIBRATION_RESERVED_0712:                        
                case MDBS_REMOTE_CALIBRATION_RESERVED_0713:
                case MDBS_REMOTE_CALIBRATION_RESERVED_0714:                        
                case MDBS_REMOTE_CALIBRATION_RESERVED_0715:  
                case MDBS_REMOTE_CALIBRATION_PROCESS_SLOPE:   
                case MDBS_REMOTE_CALIBRATION_PROCESS_VALUE_PPB_MSB:
                case MDBS_REMOTE_CALIBRATION_PROCESS_VALUE_PPB_LSB:                      
                case MDBS_REMOTE_CALIBRATION_STATE:                      
                case MDBS_REMOTE_CALIBRATION_RESULT:                  
                        break;
#endif                        
                  
                case MDBS_TERM_HREG_SENS_OXGN_PPM_HI:
                case MDBS_TERM_HREG_SENS_OXGN_PPM_LO:
                case MDBS_TERM_HREG_RESERVED_802:
                case MDBS_TERM_HREG_RESERVED_803:        
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_PRIM:
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_SEC:
                case MDBS_TERM_HREG_SENS_PRES_PA_HI:
                case MDBS_TERM_HREG_SENS_PRES_PA_LO:
                case MDBS_TERM_HREG_RESERVED_808:
                case MDBS_TERM_HREG_RESERVED_809:
                case MDBS_TERM_HREG_RESERVED_80A:
                case MDBS_TERM_HREG_RESERVED_80B:
                case MDBS_TERM_HREG_SENS_SLOPE:
                case MDBS_TERM_HREG_RESERVED_80D:
                case MDBS_TERM_HREG_RESERVED_80E:
                case MDBS_TERM_HREG_RESERVED_80F:
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_cDGR:
                case MDBS_TERM_HREG_SENS_VDDA_mV:
                case MDBS_TERM_HREG_RESERVED_812:
                case MDBS_TERM_HREG_RESERVED_813:
                case MDBS_TERM_HREG_SENS_ADC_mV: 
                case MDBS_TERM_HREG_RESERVED_815:
                case MDBS_TERM_HREG_RESERVED_816:
                case MDBS_TERM_HREG_RESERVED_817:  
                case MDBS_TERM_HREG_SENS_ADC_RAW_HI: 
                case MDBS_TERM_HREG_SENS_ADC_RAW_LO:         
                case MDBS_TERM_HREG_RESERVED_81A:
                case MDBS_TERM_HREG_RESERVED_81B:
                case MDBS_TERM_HREG_RESERVED_81C:  
                case MDBS_TERM_HREG_RESERVED_81D:
                case MDBS_TERM_HREG_RESERVED_81E:
                case MDBS_TERM_HREG_RESERVED_81F: 
                        //do nothing, read-only registers
                    break;

                //--------------------------------------------------------------
                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_TIMESTMP_HI:
                        dev.sens->cal.zero.timestamp.u16[1]  = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_TIMESTMP_LO:
                        dev.sens->cal.zero.timestamp.u16[ 0]  = *data;
                        task_hmi_cal_update( 0 );
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_C02:
                case MDBS_TERM_HREG_RESERVED_C03:        
                case MDBS_TERM_HREG_RESERVED_C04:
                case MDBS_TERM_HREG_RESERVED_C05:        
                case MDBS_TERM_HREG_RESERVED_C06:
                case MDBS_TERM_HREG_RESERVED_C07:        
                case MDBS_TERM_HREG_RESERVED_C08:
                case MDBS_TERM_HREG_RESERVED_C09:        
                case MDBS_TERM_HREG_RESERVED_C0A:
                case MDBS_TERM_HREG_RESERVED_C0B:        
                case MDBS_TERM_HREG_RESERVED_C0C:
                case MDBS_TERM_HREG_RESERVED_C0D:                         
                  break;                        

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_HI:
                        dev.sens->cal.zero.ppm.u16[ 1]    = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPM_LO:
                        dev.sens->cal.zero.ppm.u16[ 0]    = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_TIMESTMP_HI:
                        dev.sens->cal.span.timestamp.u16[ 1] = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_TIMESTMP_LO:
                        dev.sens->cal.span.timestamp.u16[ 0] = *data;
                        task_hmi_cal_update( 1 );
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_C12:
                case MDBS_TERM_HREG_RESERVED_C13:        
                case MDBS_TERM_HREG_RESERVED_C14:
                case MDBS_TERM_HREG_RESERVED_C15:        
                case MDBS_TERM_HREG_RESERVED_C16:
                case MDBS_TERM_HREG_RESERVED_C17:        
                case MDBS_TERM_HREG_RESERVED_C18:
                case MDBS_TERM_HREG_RESERVED_C19:        
                case MDBS_TERM_HREG_RESERVED_C1A:
                case MDBS_TERM_HREG_RESERVED_C1B:        
                case MDBS_TERM_HREG_RESERVED_C1C:
                case MDBS_TERM_HREG_RESERVED_C1D:                         
                  break;                        

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_HI:
                        dev.sens->cal.span.ppm.u16[ 1]  = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPM_LO:
                        dev.sens->cal.span.ppm.u16[ 0] = *data;
                        break;
                        
                //--------------------------------------------------------------        
                        
                case MDBS_TERM_HREG_SENS_OXGN_PPB_HI:
                case MDBS_TERM_HREG_SENS_OXGN_PPB_LO:
                case MDBS_TERM_HREG_RESERVED_1802:
                case MDBS_TERM_HREG_RESERVED_1803:        
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_PRIM_2:
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_SEC_2:
                case MDBS_TERM_HREG_SENS_PRES_PA_HI_2:
                case MDBS_TERM_HREG_SENS_PRES_PA_LO_2:
                case MDBS_TERM_HREG_RESERVED_1808:
                case MDBS_TERM_HREG_RESERVED_1809:
                case MDBS_TERM_HREG_RESERVED_180A:
                case MDBS_TERM_HREG_RESERVED_180B:
                case MDBS_TERM_HREG_SENS_SLOPE_2:
                case MDBS_TERM_HREG_RESERVED_180D:
                case MDBS_TERM_HREG_RESERVED_180E:
                case MDBS_TERM_HREG_RESERVED_180F:
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_cDGR_2:
                case MDBS_TERM_HREG_SENS_VDDA_mV_2:
                case MDBS_TERM_HREG_RESERVED_1812:
                case MDBS_TERM_HREG_RESERVED_1813:
                case MDBS_TERM_HREG_SENS_ADC_mV_2: 
                case MDBS_TERM_HREG_RESERVED_1815:
                case MDBS_TERM_HREG_RESERVED_1816:
                case MDBS_TERM_HREG_RESERVED_1817:  
                case MDBS_TERM_HREG_SENS_ADC_RAW_HI_2: 
                case MDBS_TERM_HREG_SENS_ADC_RAW_LO_2:         
                case MDBS_TERM_HREG_RESERVED_181A:
                case MDBS_TERM_HREG_RESERVED_181B:
                case MDBS_TERM_HREG_RESERVED_181C:  
                case MDBS_TERM_HREG_RESERVED_181D:
                case MDBS_TERM_HREG_RESERVED_181E:
                case MDBS_TERM_HREG_RESERVED_181F: 
                        //do nothing, read-only registers
                    break;                         
                        
                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_TIMESTMP_HI:
                        dev.sens->cal.zero.timestamp.u16[1]  = *data;                        
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_TIMESTMP_LO:
                        dev.sens->cal.zero.timestamp.u16[0]  = *data;                        
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1C02:            
                case MDBS_TERM_HREG_RESERVED_1C03:                       
                case MDBS_TERM_HREG_RESERVED_1C04:                
                case MDBS_TERM_HREG_RESERVED_1C05:               
                case MDBS_TERM_HREG_RESERVED_1C06:          
                case MDBS_TERM_HREG_RESERVED_1C07:             
                case MDBS_TERM_HREG_RESERVED_1C08:
                case MDBS_TERM_HREG_RESERVED_1C09:        
                case MDBS_TERM_HREG_RESERVED_1C0A:
                case MDBS_TERM_HREG_RESERVED_1C0B:   
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_ZERO_ADC_RAW_MSB:
                        dev.sens->cal.zero.raw.u16[1] = *data;                        
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_ZERO_ADC_RAW_LSB:     
                        dev.sens->cal.zero.raw.u16[0] = *data;                        
                        break;                  
                  
                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_HI:
                        dev.sens->cal.zero.ppb.u16[1]    = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_PPB_LO:
                        dev.sens->cal.zero.ppb.u16[0]    = *data;
                        dev.sens->cal.zero.ppm.i32 = dev.sens->cal.zero.ppb.i32 / 1000;
                        task_hmi_cal_update( 0 );
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_TIMESTMP_HI:
                        dev.sens->cal.span.timestamp.u16[1]  = *data;                        
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_TIMESTMP_LO:
                        dev.sens->cal.span.timestamp.u16[0]  = *data;                                          
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1C12:
                case MDBS_TERM_HREG_RESERVED_1C13:     
                case MDBS_TERM_HREG_RESERVED_1C14:
                case MDBS_TERM_HREG_RESERVED_1C15:                       
                case MDBS_TERM_HREG_RESERVED_1C16:
                case MDBS_TERM_HREG_RESERVED_1C17:       
                case MDBS_TERM_HREG_RESERVED_1C18:
                case MDBS_TERM_HREG_RESERVED_1C19:        
                case MDBS_TERM_HREG_RESERVED_1C1A:
                case MDBS_TERM_HREG_RESERVED_1C1B:  
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_ADC_RAW_MSB:
                        dev.sens->cal.span.raw.u16[1] = *data;                        
                        break;                  
                  
                case MDBS_TERM_HREG_SENS_CAL_SPAN_ADC_RAW_LSB:  
                        dev.sens->cal.span.raw.u16[0] = *data;                        
                        break;  

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_HI:
                        dev.sens->cal.span.ppb.u16[1]    = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_PPB_LO:
                        dev.sens->cal.span.ppb.u16[0]    = *data;
                        dev.sens->cal.span.ppm.i32 = dev.sens->cal.span.ppb.i32 / 1000;
                        task_hmi_cal_update( 1 );                        
                        break;    
                        
//------------------------------------------------------------------------------                         

                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_TIMESTMP_HI:
                        dev.sens->cal_back.zero.timestamp.u16[1]  = *data;                        
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_TIMESTMP_LO:
                        dev.sens->cal_back.zero.timestamp.u16[0]  = *data;                        
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1D02:
                case MDBS_TERM_HREG_RESERVED_1D03:   
                case MDBS_TERM_HREG_RESERVED_1D04:
                case MDBS_TERM_HREG_RESERVED_1D05:     
                case MDBS_TERM_HREG_RESERVED_1D06:
                case MDBS_TERM_HREG_RESERVED_1D07:        
                case MDBS_TERM_HREG_RESERVED_1D08:
                case MDBS_TERM_HREG_RESERVED_1D09:        
                case MDBS_TERM_HREG_RESERVED_1D0A:
                case MDBS_TERM_HREG_RESERVED_1D0B:   
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_ADC_RAW_MSB:
                        dev.sens->cal_back.zero.raw.u16[1] = *data;                        
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_ADC_RAW_LSB:     
                        dev.sens->cal_back.zero.raw.u16[0] = *data;                                                
                        break;                  
                  
                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_HI:
                        dev.sens->cal_back.zero.ppb.u16[1] = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_ZERO_BACK_PPB_LO:
                        dev.sens->cal_back.zero.ppb.u16[0] = *data;
                        dev.sens->cal_back.zero.ppm.i32 = dev.sens->cal_back.zero.ppb.i32 / 1000;
                        task_hmi_cal_restore( 0 );
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_TIMESTMP_HI:
                        dev.sens->cal_back.span.timestamp.u16[1]  = *data;                        
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_TIMESTMP_LO:
                        dev.sens->cal_back.span.timestamp.u16[0]  = *data;                                          
                        break;
                        
                case MDBS_TERM_HREG_RESERVED_1D12: 
                case MDBS_TERM_HREG_RESERVED_1D13:    
                case MDBS_TERM_HREG_RESERVED_1D14:    
                case MDBS_TERM_HREG_RESERVED_1D15:    
                case MDBS_TERM_HREG_RESERVED_1D16:    
                case MDBS_TERM_HREG_RESERVED_1D17:                        
                case MDBS_TERM_HREG_RESERVED_1D18:
                case MDBS_TERM_HREG_RESERVED_1D19:        
                case MDBS_TERM_HREG_RESERVED_1D1A:
                case MDBS_TERM_HREG_RESERVED_1D1B:  
                        break;
                        
                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_ADC_RAW_MSB:
                        dev.sens->cal_back.span.raw.u16[1] = *data;                        
                        break;                  
                  
                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_ADC_RAW_LSB:  
                        dev.sens->cal_back.span.raw.u16[0] = *data;                        
                        break;  

                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_HI:
                        dev.sens->cal_back.span.ppb.u16[1] = *data;
                        break;

                case MDBS_TERM_HREG_SENS_CAL_SPAN_BACK_PPB_LO:
                        dev.sens->cal_back.span.ppb.u16[0] = *data;
                        task_hmi_cal_restore( 1 );                        
                        break;                        
                                                                                              
                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}
