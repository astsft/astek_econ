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

        MDBS_TERM_HREG_STS_RESERVED_0x200               = 0x0200,
        MDBS_TERM_HREG_STS_RESERVED_0x201,
        MDBS_TERM_HREG_STS_RESERVED_0x202,
        MDBS_TERM_HREG_STS_RESERVED_0x203,
        MDBS_TERM_HREG_STS_RESERVED_0x204,
        MDBS_TERM_HREG_STS_RESERVED_0x205,
        MDBS_TERM_HREG_STS_RESERVED_0x206,
        MDBS_TERM_HREG_STS_RESERVED_0x207,
        MDBS_TERM_HREG_STS_RESERVED_0x208,
        MDBS_TERM_HREG_STS_RESERVED_0x209,
        MDBS_TERM_HREG_STS_RESERVED_0x20A,
        MDBS_TERM_HREG_STS_RESERVED_0x20B,
        MDBS_TERM_HREG_STS_RESERVED_0x20C,
        MDBS_TERM_HREG_STS_RESERVED_0x20D,
        MDBS_TERM_HREG_STS_RESERVED_0x20E,
        MDBS_TERM_HREG_STS_RESERVED_0x20F,
        MDBS_TERM_HREG_STS_RESERVED_0x210,
        MDBS_TERM_HREG_STS_RESERVED_0x211,
        MDBS_TERM_HREG_STS_RESERVED_0x212,
        MDBS_TERM_HREG_STS_RESERVED_0x213,
        MDBS_TERM_HREG_STS_RESERVED_0x214,
        MDBS_TERM_HREG_STS_RESERVED_0x215,
        MDBS_TERM_HREG_STS_RESERVED_0x216,
        MDBS_TERM_HREG_STS_RESERVED_0x217,
        MDBS_TERM_HREG_STS_RESERVED_0x218,
        MDBS_TERM_HREG_STS_RESERVED_0x219,
        MDBS_TERM_HREG_STS_RESERVED_0x21A,
        MDBS_TERM_HREG_STS_RESERVED_0x21B,
        MDBS_TERM_HREG_STS_RESERVED_0x21C,
        MDBS_TERM_HREG_STS_RESERVED_0x21D,
        MDBS_TERM_HREG_STS_RESERVED_0x21E,
        MDBS_TERM_HREG_STS_RESERVED_0x21F,


        MDBS_TERM_HREG_STS_RESERVED_0x400               = 0x0400,
        MDBS_TERM_HREG_STS_RESERVED_0x401,
        MDBS_TERM_HREG_STS_RESERVED_0x402,
        MDBS_TERM_HREG_STS_RESERVED_0x403,
        MDBS_TERM_HREG_STS_RESERVED_0x404,
        MDBS_TERM_HREG_STS_RESERVED_0x405,
        MDBS_TERM_HREG_STS_RESERVED_0x406,
        MDBS_TERM_HREG_STS_RESERVED_0x407,
        MDBS_TERM_HREG_STS_RESERVED_0x408,
        MDBS_TERM_HREG_STS_RESERVED_0x409,
        MDBS_TERM_HREG_STS_RESERVED_0x40A,
        MDBS_TERM_HREG_STS_RESERVED_0x40B,
        MDBS_TERM_HREG_STS_RESERVED_0x40C,
        MDBS_TERM_HREG_STS_RESERVED_0x40D,
        MDBS_TERM_HREG_STS_RESERVED_0x40E,
        MDBS_TERM_HREG_STS_RESERVED_0x40F,
        MDBS_TERM_HREG_STS_RESERVED_0x410,
        MDBS_TERM_HREG_STS_RESERVED_0x411,
        MDBS_TERM_HREG_STS_RESERVED_0x412,
        MDBS_TERM_HREG_STS_RESERVED_0x413,
        MDBS_TERM_HREG_STS_RESERVED_0x414,
        MDBS_TERM_HREG_STS_RESERVED_0x415,
        MDBS_TERM_HREG_STS_RESERVED_0x416,
        MDBS_TERM_HREG_STS_RESERVED_0x417,
        MDBS_TERM_HREG_STS_RESERVED_0x418,
        MDBS_TERM_HREG_STS_RESERVED_0x419,
        MDBS_TERM_HREG_STS_RESERVED_0x41A,
        MDBS_TERM_HREG_STS_RESERVED_0x41B,
        MDBS_TERM_HREG_STS_RESERVED_0x41C,
        MDBS_TERM_HREG_STS_RESERVED_0x41D,
        MDBS_TERM_HREG_STS_RESERVED_0x41E,
        MDBS_TERM_HREG_STS_RESERVED_0x41F,
        MDBS_TERM_HREG_STS_RESERVED_0x420,
        MDBS_TERM_HREG_STS_RESERVED_0x421,
        MDBS_TERM_HREG_STS_RESERVED_0x422,
        MDBS_TERM_HREG_STS_RESERVED_0x423,
        MDBS_TERM_HREG_STS_RESERVED_0x424,
        MDBS_TERM_HREG_STS_RESERVED_0x425,
        MDBS_TERM_HREG_STS_RESERVED_0x426,
        MDBS_TERM_HREG_STS_RESERVED_0x427,
        MDBS_TERM_HREG_STS_RESERVED_0x428,
        MDBS_TERM_HREG_STS_RESERVED_0x429,
        MDBS_TERM_HREG_STS_RESERVED_0x42A,
        MDBS_TERM_HREG_STS_RESERVED_0x42B,
        MDBS_TERM_HREG_STS_RESERVED_0x42C,
        MDBS_TERM_HREG_STS_RESERVED_0x42D,
        MDBS_TERM_HREG_STS_RESERVED_0x42E,
        MDBS_TERM_HREG_STS_RESERVED_0x42F,
        MDBS_TERM_HREG_STS_RESERVED_0x430,
        MDBS_TERM_HREG_STS_RESERVED_0x431,
        MDBS_TERM_HREG_STS_RESERVED_0x432,
        MDBS_TERM_HREG_STS_RESERVED_0x433,
        MDBS_TERM_HREG_STS_RESERVED_0x434,
        MDBS_TERM_HREG_STS_RESERVED_0x435,
        MDBS_TERM_HREG_STS_RESERVED_0x436,
        MDBS_TERM_HREG_STS_RESERVED_0x437,
        MDBS_TERM_HREG_STS_RESERVED_0x438,
        MDBS_TERM_HREG_STS_RESERVED_0x439,
        MDBS_TERM_HREG_STS_RESERVED_0x43A,
        MDBS_TERM_HREG_STS_RESERVED_0x43B,
        MDBS_TERM_HREG_STS_RESERVED_0x43C,
        MDBS_TERM_HREG_STS_RESERVED_0x43D,
        MDBS_TERM_HREG_STS_RESERVED_0x43E,
        MDBS_TERM_HREG_STS_RESERVED_0x43F,

        MDBS_TERM_HREG_SENS_OXGN_PPM_HI                 = 0x0800,
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

        MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_HI         = 0x0C00,
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