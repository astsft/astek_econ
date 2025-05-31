/**
  * @file    sens.h
  * @brief   Sensor Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef SENS_H
#define SENS_H


/*******************************************************************************
*
*******************************************************************************/
#include <stdint.h>

typedef union   sens_info_u
{
    uint16_t            raw[ 32];

    #pragma pack(2)
    struct  sens_info_s
    {
        uint16_t            device_id;
        uint16_t            hardware_id;
        uint16_t            reserved_0002;
        uint16_t            reserved_0003;
        uint16_t            reserved_0004;
        uint16_t            reserved_0005;
        uint16_t            reserved_0006;
        uint16_t            reserved_0007;
        uint16_t            reserved_0008;
        uint16_t            reserved_0009;
        uint16_t            reserved_000A;
        uint16_t            reserved_000B;
        uint16_t            reserved_000C;
        uint16_t            reserved_000D;
        uint16_t            reserved_000E;
        uint16_t            reserved_000F;
        uint16_t            serial_num_00;
        uint16_t            serial_num_01;
        uint16_t            serial_num_02;
        uint16_t            serial_num_03;
        uint16_t            serial_num_04;
        uint16_t            serial_num_05;
        uint16_t            serial_num_06;
        uint16_t            serial_num_07;
        uint16_t            reserved_0018;
        uint16_t            reserved_0019;
        uint16_t            reserved_001A;
        uint16_t            reserved_001B;
        uint16_t            reserved_001C;
        uint16_t            reserved_001D;
        uint16_t            reserved_001E;
        uint16_t            reserved_001F;
    };
    #pragma pack()
} sens_info_t;

typedef union   sens_conf_u
{
    uint16_t            raw[ 32];

    #pragma pack(2)
    struct  sens_conf_s
    {
        uint16_t            reserved_0200;
        uint16_t            reserved_0201;
        uint16_t            reserved_0202;
        uint16_t            reserved_0203;
        uint16_t            reserved_0204;
        uint16_t            reserved_0205;
        uint16_t            reserved_0206;
        uint16_t            reserved_0207;
        uint16_t            lpf_cutoff;
        uint16_t            lpf_order;
        uint16_t            reserved_020A;
        uint16_t            reserved_020B;
        uint16_t            reserved_020C;
        uint16_t            reserved_020D;
        uint16_t            reserved_020E;
        uint16_t            reserved_020F;
        uint16_t            reserved_0210;
        uint16_t            reserved_0211;
        uint16_t            reserved_0212;
        uint16_t            reserved_0213;
        uint16_t            reserved_0214;
        uint16_t            reserved_0215;
        uint16_t            reserved_0216;
        uint16_t            reserved_0217;
        uint16_t            reserved_0218;
        uint16_t            reserved_0219;
        uint16_t            reserved_021A;
        uint16_t            reserved_021B;
        uint16_t            reserved_021C;
        uint16_t            reserved_021D;
        uint16_t            reserved_021E;
        uint16_t            reserved_021F;
    };
    #pragma pack()
} sens_conf_t;


typedef union  sens_meas_ppm_u
{
    uint16_t            raw[ 32];

    #pragma pack(2)
    struct  sens_meas_ppm_s
    {
        uint16_t            ppm_hi;
        uint16_t            ppm_lo;

        uint16_t            reserved_0802;
        uint16_t            reserved_0803;

        struct
        {
            int16_t             i;
            uint16_t            f;
        } temp_digc;

        struct
        {
            int16_t             i;
            uint16_t            f;
        } pres_hpa;

        struct
        {
            int16_t             i;
            uint16_t            f;
        } hmdt_ph;

        uint16_t            reserved_080A;
        uint16_t            reserved_080B;
        int16_t             slope;
        uint16_t            offset_raw;
        uint16_t            link_err_cnt;
        uint16_t            status;

        int16_t             mcu_temp_digc;
        int16_t             mcu_vdda_mv;
        uint16_t            reserved_0812;
        uint16_t            reserved_0813;
        uint16_t            adc_mV;
        uint16_t            reserved_0815;
        uint16_t            reserved_0816;
        uint16_t            reserved_0817;

        //union
        //{
        //    int32_t             u32;
        //    uint16_t            u16[ 2];
        //} adc_raw;
        uint16_t            adc_raw_hi;
        uint16_t            adc_raw_lo;

        union
        {
            int32_t             u32;
            uint16_t            u16[ 2];
        } temp_raw;

        union
        {
            int32_t             u32;
            uint16_t            u16[ 2];
        } pres_raw;

        uint16_t            reserved_081E;
        uint16_t            reserved_081F;
    };
    #pragma pack()
} sens_meas_ppm_t;


typedef union  sens_meas_ppb_u
{
    uint16_t            raw[ 32];

    #pragma pack(2)
    struct  sens_meas_ppb_s
    {
        uint16_t            ppb_hi;
        uint16_t            ppb_lo;

        uint16_t            reserved_0802;
        uint16_t            reserved_0803;

        struct
        {
            int16_t             i;
            uint16_t            f;
        } temp_digc;

        //struct
        //{
        //    int16_t             i;
        //    uint16_t            f;
        //} pres_pa;

        int16_t             pres_pa_hi;
        uint16_t            pres_pa_lo;

        struct
        {
            int16_t             i;
            uint16_t            f;
        } hmdt_ph;

        uint16_t            reserved_080A;
        uint16_t            reserved_080B;
        uint16_t            slope;
        uint16_t            offset_raw;
        uint16_t            link_err_cnt;
        uint16_t            status;

        int16_t             mcu_temp_digc;
        int16_t             mcu_vdda_mv;
        uint16_t            reserved_0812;
        uint16_t            reserved_0813;
        uint16_t            adc_mV;
        uint16_t            reserved_0815;
        uint16_t            reserved_0816;
        uint16_t            reserved_0817;

        uint16_t            adc_raw_hi;
        uint16_t            adc_raw_lo;

        union
        {
            int32_t             u32;
            uint16_t            u16[ 2];
        } temp_raw;

        union
        {
            int32_t             u32;
            uint16_t            u16[ 2];
        } pres_raw;

        uint16_t            reserved_081E;
        uint16_t            reserved_081F;
    };
    #pragma pack()
} sens_meas_ppb_t;


typedef union  sens_cal_point_u
{
    uint16_t            raw[ 16];

    #pragma pack(2)
    struct  sens_cal_s
    {
        uint16_t            timestamp_hi;
        uint16_t            timestamp_lo;
        uint16_t            reserved_0Cx2;
        uint16_t            reserved_0Cx3;
        uint16_t            reserved_0Cx4;
        uint16_t            reserved_0Cx5;
        uint16_t            reserved_0Cx6;
        uint16_t            reserved_0Cx7;
        uint16_t            reserved_0Cx8;
        uint16_t            reserved_0Cx9;
        uint16_t            reserved_0CxA;
        uint16_t            reserved_0CxB;
        uint16_t            reserved_0CxC;
        uint16_t            reserved_0CxD;
        uint16_t            ppm_int_hi;
        uint16_t            ppm_int_lo;
    };
    #pragma pack()

} sens_cal_point_t;

typedef struct  sens_cal_u
{
    sens_cal_point_t    zero;
    sens_cal_point_t    span;
} sens_cal_t;


typedef struct  sens_sts_s
{
    uint16_t        error_code;
    uint16_t        starts_counter;
    //int16_t         mcu_temp_digc;
    //int16_t         mcu_vdda_mv;
} sens_sts_t;


typedef struct  sens_s
{
    sens_info_t     info;
    sens_conf_t     conf;
    //sens_meas_ppm_t     meas;
    sens_meas_ppb_t meas;
    sens_cal_t      cal;
    sens_cal_t      cal_back;
    sens_cal_t      cal_factory;
    //sens_cal_t      cal_back[ 2];
    sens_sts_t      sts;

    int             link_err;
    int             cal_update;
} sens_t;


#endif //SENS_H
