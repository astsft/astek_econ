/**
  * @file    mdbs_dev_term.h
  * @brief   MODBUS Device Descriptor
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef MDBS_DEV_TERM_H
#define MDBS_DEV_TERM_H


typedef enum    mdbs_addr_hreg_e
{
        MDBS_TERM_HREG_DEVICE_ID_HI          = 0,
        MDBS_TERM_HREG_DEVICE_ID_LO,
        MDBS_TERM_HREG_HARDWARE_ID_HI,
        MDBS_TERM_HREG_HARDWARE_ID_LO,
        MDBS_TERM_HREG_FIRMWARE_ID_HI,
        MDBS_TERM_HREG_FIRMWARE_ID_MI,
        MDBS_TERM_HREG_FIRMWARE_ID_LO,
        MDBS_TERM_HREG_RESERVED_07,
        MDBS_TERM_HREG_SERIAL_NUM_00,
        MDBS_TERM_HREG_SERIAL_NUM_01,
        MDBS_TERM_HREG_SERIAL_NUM_02,
        MDBS_TERM_HREG_SERIAL_NUM_03,
        MDBS_TERM_HREG_SERIAL_NUM_04,
        MDBS_TERM_HREG_SERIAL_NUM_05,
        MDBS_TERM_HREG_SERIAL_NUM_06,
        MDBS_TERM_HREG_SERIAL_NUM_07,

        MDBS_TERM_HREG_ERR_CODE_LAST,                //= 16,
        MDBS_TERM_HREG_STARTS_COUNTER,
        MDBS_TERM_HREG_RESERVED_12,
        MDBS_TERM_HREG_RESERVED_13,
        MDBS_TERM_HREG_RESERVED_14,
        MDBS_TERM_HREG_RESERVED_15,
        MDBS_TERM_HREG_RESERVED_16,
        MDBS_TERM_HREG_RESERVED_17,
        MDBS_TERM_HREG_MCU_TEMP_DIGC,
        MDBS_TERM_HREG_MCU_VDDA_mV,
        MDBS_TERM_HREG_RAW2PPM_FV_HI,
        MDBS_TERM_HREG_RAW2PPM_FV_LO,
        MDBS_TERM_HREG_RAW2PPM_FT_HI,
        MDBS_TERM_HREG_RAW2PPM_FT_LO,
        MDBS_TERM_HREG_RAW2PPM_FP_HI,
        MDBS_TERM_HREG_RAW2PPM_FP_LO,

        MDBS_TERM_HREG_SENS_OXGN_PPM_HI,             //= 32,
        MDBS_TERM_HREG_SENS_OXGN_PPM_LO,
        MDBS_TERM_HREG_SENS_TEMP_DIGC_HI,
        MDBS_TERM_HREG_SENS_TEMP_DIGC_LO,
        MDBS_TERM_HREG_SENS_PRES_HPA_HI,
        MDBS_TERM_HREG_SENS_PRES_HPA_LO,
        MDBS_TERM_HREG_RESERVED_26,
        MDBS_TERM_HREG_RESERVED_27,
        MDBS_TERM_HREG_SENS_OXGN_RAW_HI,
        MDBS_TERM_HREG_SENS_OXGN_RAW_LO,
        MDBS_TERM_HREG_SENS_TEMP_RAW_HI,
        MDBS_TERM_HREG_SENS_TEMP_RAW_LO,
        MDBS_TERM_HREG_SENS_PRES_RAW_HI,
        MDBS_TERM_HREG_SENS_PRES_RAW_LO,
        MDBS_TERM_HREG_SENS_SLOPE_RAW,
        MDBS_TERM_HREG_SENS_OFST_RAW,

        MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_HI,     //= 48,
        MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_LO,
        MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_HI,
        MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_LO,
        MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_HI,
        MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_LO,
        MDBS_TERM_HREG_RESERVED_36,
        MDBS_TERM_HREG_RESERVED_37,
        MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_HI,
        MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_LO,
        MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_HI,
        MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_LO,
        MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_RAW_HI,
        MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_RAW_LO,
        MDBS_TERM_HREG_RESERVED_3E,
        MDBS_TERM_HREG_RESERVED_3F,


        MDBS_TERM_HREG_MAX,

} mdbs_addr_hreg_t;

/*
typedef struct  modbus_dev_term_s
{
        uint16_t        sens_val_lo;
        uint16_t        sens_val_hi;
        uint16_t        cal_0_value_lo;
        uint16_t        cal_0_value_hi;
        uint16_t        cal_0_timestamp_lo;
        uint16_t        cal_0_timestamp_hi;
        uint16_t        cal_1_value_lo;
        uint16_t        cal_1_value_hi;
        uint16_t        cal_1_timestamp_lo;
        uint16_t        cal_1_timestamp_hi;
} modbus_dev_term_t;
*/

/*******************************************************************************
*
*******************************************************************************/

uint16_t
task_mdbs_get(                          const   mdbs_addr_hreg_t        addr    );

void
task_mdbs_set(                          const   mdbs_addr_hreg_t        addr,
                                        const   uint16_t                value );


#endif  //MDBS_DEV_TERM_H
