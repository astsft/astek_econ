/**
  * @file    measure.h
  * @brief   Measure Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef MEASURE_H
#define MEASURE_H


#include <stdint.h>
#include <stdio.h>


#define MEASURE_FILTER_ORDER_MAX        8


typedef enum    measure_proc_e
{
    MEASURE_PROC_CONTINUOUS     = 0,
    MEASURE_PROC_CZM_CYCLE,
    MEASURE_PROC_CSM_CYCLE,
    MEASURE_PROC_PCZM_CYCLE,
    MEASURE_PROC_PCSM_CYCLE,
    MEASURE_PROC_MAX,
} measure_proc_t;

typedef struct  meas_cfg_s
{
    measure_proc_t      proc;
    size_t              filter_order_max;
    size_t              filter_order;
    size_t              t_purge_sec;
    size_t              t_calibrate_sec;
    size_t              t_measure_sec;
} meas_cfg_t;

typedef struct  measure_s
{
    meas_cfg_t          cfg;
    //size_t              t_purge_sec;
    //size_t              t_calibrate_sec;
    //size_t              t_measure_sec;
} measure_t;

/*******************************************************************************
* FUNCTION PROTOTYPE
*******************************************************************************/
measure_proc_t meas_cfg_proc_get( measure_t * );
int     meas_cfg_proc_set( measure_t *, measure_proc_t );

size_t  meas_cfg_get_t_purge( measure_t * );
size_t  meas_cfg_get_t_calibrate( measure_t * );
size_t  meas_cfg_get_t_measure( measure_t * );

void    meas_cfg_set_t_purge( measure_t *, size_t );
void    meas_cfg_set_t_calibrate( measure_t *, size_t );
void    meas_cfg_set_t_measure( measure_t *, size_t );

size_t  measure_filter_order_get( measure_t * );
void    measure_filter_order_set( measure_t *, size_t );


#endif //MEASURE_H
