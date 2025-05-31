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

                case MDBS_TERM_HREG_SERIAL_NUM_00:
                  *data   = *dev.info.uid_u16[5];
                  break;
                case MDBS_TERM_HREG_SERIAL_NUM_01:
                  *data   = *dev.info.uid_u16[4];
                  break;                  
                case MDBS_TERM_HREG_SERIAL_NUM_02:
                  *data   = *dev.info.uid_u16[3];
                  break;                  
                case MDBS_TERM_HREG_SERIAL_NUM_03:
                  *data   = *dev.info.uid_u16[2];
                  break;                  
                case MDBS_TERM_HREG_SERIAL_NUM_04:
                  *data   = *dev.info.uid_u16[1];
                  break;                  
                case MDBS_TERM_HREG_SERIAL_NUM_05:
                 *data   = *dev.info.uid_u16[0];
                  break;
                case MDBS_TERM_HREG_SERIAL_NUM_06:
                  *data  = 0;
                  break;                                    
                case MDBS_TERM_HREG_SERIAL_NUM_07:
                  *data  = 0;
                  break;                                    
                case MDBS_TERM_HREG_SERIAL_NUM_08:
                  *data   = 0;
                  break;           
                  
                case MDBS_TERM_HREG_INFO_RESERVED_25:        
                case MDBS_TERM_HREG_INFO_RESERVED_26:        
                case MDBS_TERM_HREG_INFO_RESERVED_27:
                case MDBS_TERM_HREG_INFO_RESERVED_28:        
                case MDBS_TERM_HREG_INFO_RESERVED_29:        
                case MDBS_TERM_HREG_INFO_RESERVED_30:
                case MDBS_TERM_HREG_INFO_RESERVED_31:                                    
                  *data   = 0;
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
                case MDBS_HREG_RANGE1_PPM_MSB:
                  *data   = dev.cl420.range[0].ppm >> 16;
                  break;
                case MDBS_HREG_RANGE1_PPM_LSB:
                  *data   = dev.cl420.range[0].ppm & 0x0000FFFF;
                  break;
                case MDBS_HREG_RANGE2_PPM_MSB:
                  *data   = dev.cl420.range[1].ppm >> 16;
                  break;
                case MDBS_HREG_RANGE2_PPM_LSB:
                  *data   = dev.cl420.range[2].ppm & 0x0000FFFF;
                  break;
                case MDBS_HREG_RANGE3_PPM_MSB:
                  *data   = dev.cl420.range[2].ppm >> 16;
                  break;
                case MDBS_HREG_RANGE3_PPM_LSB:
                  *data   = dev.cl420.range[2].ppm & 0x0000FFFF;
                  break;
                case MDBS_HREG_CURRENT_RANGE:
                  *data   = dev.cl420.range_idx;
                  break; 
                  
                case MDBS_TERM_HREG_STS_RESERVED_207:
                case MDBS_HREG_LPF_CUTOFF:
                case MDBS_HREG_LPF_ORDER:
                case MDBS_HREG_GAIN:             
                case MDBS_TERM_HREG_STS_RESERVED_20B:
                case MDBS_HREG_T_COMP:
                case MDBS_HREG_P_COMP:    
                case MDBS_TERM_HREG_STS_RESERVED_20E:
                case MDBS_TERM_HREG_STS_RESERVED_20F: 
                case MDBS_HREG_POLY_A_MSB:
                case MDBS_HREG_POLY_A_LSB:
                case MDBS_HREG_POLY_B_MSB:
                case MDBS_HREG_POLY_B_LSB:
                case MDBS_HREG_POLY_C_MSB:
                case MDBS_HREG_POLY_C_LSB:
                case MDBS_HREG_POLY_D_MSB:
                case MDBS_HREG_POLY_D_LSB:
                case MDBS_HREG_POLY_E_MSB:
                case MDBS_HREG_POLY_E_LSB:
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
                  *data   = dev.state.error_status & 0x00FF;
                  break;
                  
                case MDBS_TERM_HREG_WARNING_REG_MSB:
                  *data   = dev.state.warnings_status >> 16;
                  break;
                  
                case MDBS_TERM_HREG_WARNING_REG_LSB:
                  *data   = dev.state.warnings_status & 0x00FF;
                  break;
                  
                case MODBUS_HREG_STS_ERROR_FILTER_COUNT:  *data = (dev.cfg.error_filter_count); break;
                
                case MDBS_HREG_TIMESTAMP_MSB:
                  temp = dev.mcu->rtc.get_timestamp();
                  *data = temp >> 16;
                  break;
                  
                case MDBS_HREG_TIMESTAMP_LSB:
                  temp = dev.mcu->rtc.get_timestamp();
                  *data = temp&0x00FF;
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
                        
                case MDBS_TERM_HREG_SENS_OXGN_PPM_HI:
                        ppm = dev.sens->meas.ppm.integral;
                        if (ppm < 0)
                          *data = 0;
                        else
                          *data   = dev.sens->meas.ppm.integral >> 16;
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_PPM_LO:
                        ppm = dev.sens->meas.ppm.integral;
                        if (ppm < 0)
                          *data = 0;
                        else
                          *data   = dev.sens->meas.ppm.integral & 0xFFFF;
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
        time_t time_temp;
        struct tm *     ts;

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
                case MDBS_TERM_HREG_SERIAL_NUM_00:
                case MDBS_TERM_HREG_SERIAL_NUM_01:
                case MDBS_TERM_HREG_SERIAL_NUM_02:
                case MDBS_TERM_HREG_SERIAL_NUM_03:
                case MDBS_TERM_HREG_SERIAL_NUM_04:
                case MDBS_TERM_HREG_SERIAL_NUM_05:
                case MDBS_TERM_HREG_SERIAL_NUM_06:
                case MDBS_TERM_HREG_SERIAL_NUM_07:
                case MDBS_TERM_HREG_INFO_RESERVED_25:        
                case MDBS_TERM_HREG_INFO_RESERVED_26:        
                case MDBS_TERM_HREG_INFO_RESERVED_27:
                case MDBS_TERM_HREG_INFO_RESERVED_28:        
                case MDBS_TERM_HREG_INFO_RESERVED_29:        
                case MDBS_TERM_HREG_INFO_RESERVED_30:
                case MDBS_TERM_HREG_INFO_RESERVED_31:                  
                        //do nothing, read-only register
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
                        
                case MDBS_HREG_RANGE1_PPM_MSB:
                    dev.cl420.range[0].ppm = *data << 16 ;
                    break;
                case MDBS_HREG_RANGE1_PPM_LSB:
                    dev.cl420.range[0].ppm |= *data;
                    send_cmd_for_cloop_write_range(); 
                    break;                 
                case MDBS_HREG_RANGE2_PPM_MSB:
                    dev.cl420.range[1].ppm = *data << 16;
                    break;           
                case MDBS_HREG_RANGE2_PPM_LSB:
                    dev.cl420.range[1].ppm |= *data;
                    send_cmd_for_cloop_write_range();
                    break;                 
                case MDBS_HREG_RANGE3_PPM_MSB:
                    dev.cl420.range[2].ppm = *data << 16;
                    break;               
                case MDBS_HREG_RANGE3_PPM_LSB:
                    dev.cl420.range[2].ppm |= *data;
                    send_cmd_for_cloop_write_range();
                    break;                 
                case MDBS_HREG_CURRENT_RANGE:
                    dev.cl420.range_idx = *data;
                    send_cmd_for_cloop_write_range();
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

                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}
