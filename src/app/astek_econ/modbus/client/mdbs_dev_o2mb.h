/**
  * @file    mdbs_dev.h
  * @brief   MODBUS Device
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef MDBS_DEV_O2MB_H
#define MDBS_DEV_O2MB_H


typedef enum    mdbs_hreg_e
{
    MDBS_HREG_DEVICE_ID_HI          = 0,
    MDBS_HREG_DEVICE_ID_LO,
    MDBS_HREG_HARDWARE_ID_HI,
    MDBS_HREG_HARDWARE_ID_LO,
    MDBS_HREG_FIRMWARE_ID_HI,
    MDBS_HREG_FIRMWARE_ID_MI,
    MDBS_HREG_FIRMWARE_ID_LO,
    MDBS_HREG_RESERVED_07,
    MDBS_HREG_SERIAL_NUM_00,
    MDBS_HREG_SERIAL_NUM_01,
    MDBS_HREG_SERIAL_NUM_02,
    MDBS_HREG_SERIAL_NUM_03,
    MDBS_HREG_SERIAL_NUM_04,
    MDBS_HREG_SERIAL_NUM_05,
    MDBS_HREG_SERIAL_NUM_06,
    MDBS_HREG_SERIAL_NUM_07,
    
    MDBS_HREG_ERR_CODE_LAST,                //= 16,
    MDBS_HREG_STARTS_COUNTER,
    MDBS_HREG_RESERVED_12,
    MDBS_HREG_RESERVED_13,
    MDBS_HREG_RESERVED_14,
    MDBS_HREG_RESERVED_15,
    MDBS_HREG_RESERVED_16,
    MDBS_HREG_RESERVED_17,
    MDBS_HREG_MCU_TEMP_CELS,
    MDBS_HREG_MCU_VDDA_mV,
    MDBS_HREG_RAW2PPM_FV_HI,
    MDBS_HREG_RAW2PPM_FV_LO,
    MDBS_HREG_RAW2PPM_FT_HI,
    MDBS_HREG_RAW2PPM_FT_LO,
    MDBS_HREG_RAW2PPM_FP_HI,
    MDBS_HREG_RAW2PPM_FP_LO,

    MDBS_HREG_SENS_OXGN_PPM_HI                  = 0x0800,
    MDBS_HREG_SENS_OXGN_PPM_LO,
    MDBS_HREG_SENS_TEMP_DIGC_HI,
    MDBS_HREG_SENS_TEMP_DIGC_LO,
    MDBS_HREG_SENS_PRES_HPA_HI,
    MDBS_HREG_SENS_PRES_HPA_LO,
    MDBS_HREG_RESERVED_26,
    MDBS_HREG_RESERVED_27,
    MDBS_HREG_SENS_RAW_HI,
    MDBS_HREG_SENS_RAW_LO,
    MDBS_HREG_TEMP_RAW_HI,
    MDBS_HREG_TEMP_RAW_LO,
    MDBS_HREG_PRES_RAW_HI,
    MDBS_HREG_PRES_RAW_LO,
    MDBS_HREG_SENS_SLOPE_RAW,
    MDBS_HREG_SENS_OFST_RAW,

    MDBS_HREG_SENS_TRIM_P0_TIMESTMP_HI          = 0x0C00,     //= 48,
    MDBS_HREG_SENS_TRIM_P0_TIMESTMP_LO,
    MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_HI,
    MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_LO,
    MDBS_HREG_SENS_TRIM_P0_OXGN_RAW_HI,
    MDBS_HREG_SENS_TRIM_P0_OXGN_RAW_LO,
    MDBS_HREG_RESERVED_36,
    MDBS_HREG_RESERVED_37,

    MDBS_HREG_SENS_TRIM_P1_TIMESTMP_HI          = 0x0C10,
    MDBS_HREG_SENS_TRIM_P1_TIMESTMP_LO,
    MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_HI,
    MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_LO,
    MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_HI,
    MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_LO,
    MDBS_HREG_RESERVED_3E,
    MDBS_HREG_RESERVED_3F,
    

    MDBS_HREG_SENS_LPF_FCUT                     = 0x0220,
    MDBS_HREG_SENS_LPF_ORDER,
    MDBS_HREG_MAX,
} mdbs_hreg_t;

typedef enum    modbus_coil_mdb_relay_e
{
  MODBUS_COIL_MDB_RELAY_1 = 0,
  MODBUS_COIL_MDB_RELAY_2 
}modbus_coil_mdb_relay_t;

typedef enum    modbus_hreg_cloop_e
{
    MODBUS_HREG_CONF_RNG1_PPM_FRC_HI    = 0x20D,
    MODBUS_HREG_CONF_RNG1_PPM_FRC_LO,
    MODBUS_HREG_CONF_RNG1_PPM_INT_HI,
    MODBUS_HREG_CONF_RNG1_PPM_INT_LO,
    MODBUS_HREG_CONF_RNG2_PPM_FRC_HI,
    MODBUS_HREG_CONF_RNG2_PPM_FRC_LO,
    MODBUS_HREG_CONF_RNG2_PPM_INT_HI,
    MODBUS_HREG_CONF_RNG2_PPM_INT_LO,
    MODBUS_HREG_CONF_RNG3_PPM_FRC_HI,
    MODBUS_HREG_CONF_RNG3_PPM_FRC_LO,
    MODBUS_HREG_CONF_RNG3_PPM_INT_HI,
    MODBUS_HREG_CONF_RNG3_PPM_INT_LO,
    MODBUS_HREG_CONF_RNG_NUM,
    MODBUS_HREG_CONF_RNG_UNITS,
  
    MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_PM_INT_HI     = 0x0800,
    MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_PM_INT_LOW,
    MODBUS_HREG_CLOOP_CHANNEL1_0C02,
    MODBUS_HREG_CLOOP_CHANNEL1_0C03,
    MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_uA_INT_HI,
    MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_uA_INT_LOW,
    MODBUS_HREG_CLOOP_CHANNEL1_0C06,
    MODBUS_HREG_CLOOP_CHANNEL1_0C07,
    MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_RAW_INT_HI,
    MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_RAW_INT_LOW,
    MODBUS_HREG_CLOOP_CHANNEL1_0C0A,
    MODBUS_HREG_CLOOP_CHANNEL1_0C0B,
    MODBUS_HREG_CLOOP_CHANNEL1_0C0C,
    MODBUS_HREG_CLOOP_CHANNEL1_0C0D,
    MODBUS_HREG_CLOOP_CHANNEL1_0C0E,
    MODBUS_HREG_CLOOP_CHANNEL1_0C0F,
    
    MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_PM_INT_HI     = 0x0810,
    MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_PM_INT_LOW,
    MODBUS_HREG_CLOOP_CHANNEL2_0C02,
    MODBUS_HREG_CLOOP_CHANNEL2_0C03,
    MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_uA_INT_HI,
    MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_uA_INT_LOW,
    MODBUS_HREG_CLOOP_CHANNEL2_0C06,
    MODBUS_HREG_CLOOP_CHANNEL2_0C07,
    MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_RAW_INT_HI,
    MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_RAW_INT_LOW,
    MODBUS_HREG_CLOOP_CHANNEL2_0C0A,
    MODBUS_HREG_CLOOP_CHANNEL2_0C0B,
    MODBUS_HREG_CLOOP_CHANNEL2_0C0C,
    MODBUS_HREG_CLOOP_CHANNEL2_0C0D,
    MODBUS_HREG_CLOOP_CHANNEL2_0C0E,
    MODBUS_HREG_CLOOP_CHANNEL2_0C0F,
    
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_TIMESTAMP_HI   = 0x0C00,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_TIMESTAMP_LO,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C02,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C03,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C04,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C05,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C06,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C07,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_4UA_HI,              
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_4UA_LOW,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C0A,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C0B,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_20UA_HI,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_20UA_LO,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C0E,
    MODBUS_HREG_CLOOP_CAL_CHANNEL1_0C0F,

    MODBUS_HREG_CLOOP_CAL_CHANNEL2_TIMESTAMP_HI   = 0x0C10,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_TIMESTAMP_LO,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C02,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C03,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C04,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C05,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C06,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C07,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_4UA_HI,              
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_4UA_LOW,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C0A,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C0B,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_20UA_HI,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_20UA_LO,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C0E,
    MODBUS_HREG_CLOOP_CAL_CHANNEL2_0C0F,
 
}modbus_hreg_cloop_t;


#endif  //MDBS_DEV_O2MB_H