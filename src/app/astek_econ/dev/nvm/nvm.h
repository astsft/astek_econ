/**
  * @file    nvm.h
  * @brief   Non-Volatile Memory header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef NVM_H
#define NVM_H


/*******************************************************************************
*
*******************************************************************************/
typedef enum    nvm_reg_e
{
    NVM_REG_PASSWORD    = 0,
    NVM_REG_LANGUAGE,
    NVM_REG_RELAY1_MODE_STATE_TYPE, 
    NVM_REG_RELAY2_MODE_STATE_TYPE,
    NVM_REG_RELAY1_THRESHOLD,
    NVM_REG_RELAY2_THRESHOLD,
    NVM_REG_RANGE_R1_PPM,
    NVM_REG_RANGE_R2_PPM,
    NVM_REG_RANGE_R3_PPM,
    NVM_REG_RANGE_IDX,
    NVM_REG_RANGE_UNITS,
    NVM_REG_RANGE_ERR_LEVEL,
    NVM_REG_CAL0_PPM,
    NVM_REG_CAL0_RAW,
    NVM_REG_CAL1_PPM,
    NVM_REG_CAL1_RAW,
    NVM_REG_CAL0_RESTORE_PPM,
    NVM_REG_CAL0_RESTORE_RAW,
    NVM_REG_CAL1_RESTORE_PPM,
    NVM_REG_CAL1_RESTORE_RAW,
    NVM_REG_CL420_CH1_CAL0_TIMESTAMP,
    NVM_REG_CL420_CH1_CAL0_UA,
    NVM_REG_CL420_CH1_CAL0_RAW,
    NVM_REG_CL420_CH1_CAL1_TIMESTAMP,
    NVM_REG_CL420_CH1_CAL1_UA,
    NVM_REG_CL420_CH1_CAL1_RAW,    
    NVM_REG_CL420_CH2_CAL0_TIMESTAMP,
    NVM_REG_CL420_CH2_CAL0_UA,
    NVM_REG_CL420_CH2_CAL0_RAW,
    NVM_REG_CL420_CH2_CAL1_TIMESTAMP,
    NVM_REG_CL420_CH2_CAL1_UA,
    NVM_REG_CL420_CH2_CAL1_RAW,
    
    // ONLY FOR EXTERNAL FLASH  
    NVM_REG_CAL0_TIMESTAMP,
    NVM_REG_CAL0_RESTORE_TIMESTAMP,
    NVM_REG_CAL1_TIMESTAMP,
    NVM_REG_CAL1_RESTORE_TIMESTAMP,    
    NVM_REG_EXT_MODBUS_CFG,
    NVM_REG_NET_MAC_PART1,
    NVM_REG_NET_MAC_PART2,
    NVM_REG_NET_IP,       
    NVM_REG_NET_SN,
    NVM_REG_NET_GATEWAY,
    NVM_REG_NET_MODBUS_PORT,    
    NVM_REG_NET_HTTP_PORT,
    NVM_REG_NET_STATE,   
    NVM_REG_ERROR_COUNT,
    NVM_REG_SERNUM,
    NVM_REG_TAG_PART1,
    NVM_REG_TAG_PART2,
    NVM_REG_TAG_PART3,
    NVM_SMA_FILTER_CUTOFF,
    NVM_SMA_FILTER_ORDER,
    NVM_REG_VALIDATION_TRANSITION_TIME,
    NVM_REG_VALIDATION_MEASURE_TIME,
    NVM_REG_VALIDATION_RETURN_TIME,
    NVM_REG_VALIDATION_VALUE,
    NVM_REG_VALIDATION_DEVIATION,
    NVM_REG_CALIBRATION_ZERO,
    NVM_REG_CALIBRATION_SPAN,
    NVM_REG_CALIBRATION_DEVIATION,
    NVM_REG_CALIBRATION_TRANSITION_TIME,
    NVM_REG_CALIBRATION_MEASURE_TIME,
    NVM_REG_CALIBRATION_RETURN_TIME,
    NVM_REG_CALIBRATION_REMOTE_ENABLE,
    NVM_REG_CALIBRATION_SLOPE,
    NVM_REG_CALIBRATION_SLOPE_TIME,
    
} nvm_reg_t;



/*******************************************************************************
*
*******************************************************************************/
typedef	struct	nvm_s
{
        void            (* put)         (size_t, const uint32_t);
        uint32_t        (* get)         (size_t);
} nvm_t;


#endif //NVM_H
