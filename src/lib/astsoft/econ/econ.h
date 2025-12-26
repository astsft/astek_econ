 /**
  * @file    econ.h
  * @brief   Econ header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef ECON_H
#define ECON_H

#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
*
*******************************************************************************/
typedef union
{
    int32_t     i32;
    int32_t     u32;
    uint16_t    u16[ 2];
} econ_type_t;

typedef struct  econ_cal_point_s
{
    econ_type_t         raw;
    econ_type_t         ppm;
    econ_type_t         ppb;
    econ_type_t         timestamp;
} econ_cal_point_t;

typedef struct  econ_cal_s
{
    econ_cal_point_t        zero;
    econ_cal_point_t        span;
    //econ_cal_point_t        point[ 2];
    float                   ofst;
    float                   tg;
} econ_cal_t;

typedef struct  econ_meas_s
{
    uint32_t            raw;
    uint32_t            raw_t;     
    uint16_t            mV;

    struct
    {
        int32_t         i32;
        uint16_t        u16[2];
    } ppm;

    struct
    {
        int32_t             integral;
        uint32_t            fractional;
    } digc;
    
    union
    {
      int32_t  i32;
      uint16_t u16[2];
    } ppb;

    int16_t             slope;
} econ_meas_t;

typedef struct  econ_s
{
    econ_cal_t          cal;
    econ_cal_t          cal_back;    
    econ_meas_t         meas;
    uint16_t            error;
    uint16_t            link_err_cnt;
    int16_t             link_err;   
    
    uint16_t        modbus_ppm_hi;
    uint16_t        modbus_ppm_lo;
    uint16_t        modbus_ppb_hi;
    uint16_t        modbus_ppb_lo;
} econ_t;


/*******************************************************************************
*
*******************************************************************************/
void        econ_cal_restore( econ_t * );
int32_t     econ_raw2ppm( econ_t *, const uint32_t );
int32_t     econ_calc_slope (const int32_t raw);


#endif	//ECON_H
