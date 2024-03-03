/**
  * @file    meas.c
  * @brief   Measure Functions
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "meas.h"


/*******************************************************************************
* MEASURE MODE
*******************************************************************************/

/*******************************************************************************
* MEASURE FILTER
*******************************************************************************/
void
measure_filter_order_set(                   measure_t *             p,
                                            size_t                  order )
{
    if( order < MEASURE_FILTER_ORDER_MAX )
    {
        p->cfg.filter_order = order;
    }
}


size_t
measure_filter_order_get(                   measure_t *             p )
{
    if( p->cfg.filter_order < MEASURE_FILTER_ORDER_MAX )
    {
        return( p->cfg.filter_order );
    }

    return( 0 );
}

/*******************************************************************************
* CFG
*******************************************************************************/
measure_proc_t
meas_cfg_proc_get(                          measure_t *             p )
{
    if( p->cfg.proc < MEASURE_PROC_MAX )
    {
        return( p->cfg.proc );
    }

    return( MEASURE_PROC_MAX );
}


int
meas_cfg_proc_set(                          measure_t *             p,
                                            measure_proc_t          proc )
{
    int     resp    = 0;

    if( p->cfg.proc < MEASURE_PROC_MAX )
    {
        p->cfg.proc = proc;
    }
    else
    {
        resp    = -1;
    }

    return( resp );
}

/*******************************************************************************
* CFG t PURGE
*******************************************************************************/
size_t
meas_cfg_get_t_purge(                       measure_t *             p )
{
    return( p->cfg.t_purge_sec );
}

void
meas_cfg_set_t_purge(                       measure_t *             p,
                                            size_t                  t )
{
    p->cfg.t_purge_sec  = t;
}

/*******************************************************************************
* CFG t CALIBRATE
*******************************************************************************/
size_t
meas_cfg_get_t_calibrate(                   measure_t *             p )
{
    return( p->cfg.t_calibrate_sec );
}

void
meas_cfg_set_t_calibrate(                   measure_t *             p,
                                            size_t                  t )
{
    p->cfg.t_calibrate_sec  = t;
}

/*******************************************************************************
* CFG t MEASURE
*******************************************************************************/
size_t
meas_cfg_get_t_measure(                     measure_t *             p )
{
    return( p->cfg.t_measure_sec );
}

void
meas_cfg_set_t_measure(                     measure_t *             p,
                                            size_t                  t )
{
    p->cfg.t_measure_sec  = t;
}

