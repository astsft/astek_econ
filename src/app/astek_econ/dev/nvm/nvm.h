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
    NVM_REG_STARTS_COUNTER              = 0,
    NVM_REG_LAUNCH_TIMESTAMP,
    NVM_REG_PASSWORD,
    NVM_REG_LANGUAGE,
    NVM_REG_SERNUM,
    NVM_REG_RELAY1_MODE_STATE_TYPE, 
    NVM_REG_RELAY2_MODE_STATE_TYPE,
    NVM_REG_RELAY1_THRESHOLD,
    NVM_REG_RELAY2_THRESHOLD,
    NVM_REG_STATUS,
    NVM_REG_CAL_AUTO_START_TIMESTAMP,
    NVM_REG_CAL_AUTO_CYCLE_HOURS,
    NVM_REG_CAL_AUTO_FLOW_SECONDS,
    NVM_REG_RANGE_R1_PPM,
    NVM_REG_RANGE_R2_PPM,
    NVM_REG_RANGE_R3_PPM,
    NVM_REG_RANGE_IDX,
    NVM_REG_RANGE_UNITS,
    NVM_REG_RANGE_IDX_UNITS_ERR_LEVEL,
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
    NVM_REG_CL420_CH2_CAL1_RAW          
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
