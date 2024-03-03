/**
  * @file    cl420.h
  * @brief   Current Loop 4-20mA Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef CL420_H
#define CL420_H


#include <stdint.h>


/*******************************************************************************
* CL420
*******************************************************************************/
typedef union   cl420_output_u
{
    uint16_t            raw[ 4];

    #pragma pack(2)
    struct  cl420_output_s
    {
        uint32_t            uA;
        uint16_t            reserved_0806;
        uint16_t            reserved_0007;
    };
    #pragma pack()
} cl420_output_t;


typedef enum    cl420_units_e
{
    CL420_UNITS_PERCENT = 0,
    CL420_UNITS_MAX,
} cl420_units_t;


typedef struct  cl420_s
{
    //uint16_t                value_uA;
    //uint16_t                range_uA;
    uint32_t                ppm;
    cl420_units_t           units;
    cl420_output_t          mb_output;
} cl420_t;


/*******************************************************************************
*
*******************************************************************************/
uint16_t    cl420_ppb2uA( cl420_t *, const uint32_t );
void        cl420_set_range( cl420_t *, uint32_t );


#endif  //CL420_H
