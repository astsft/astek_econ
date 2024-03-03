 /**
  * @file    econ.c
  * @brief   Econ
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdlib.h>
#include "econ.h"


/**
  * @brief
  * @param  None
  * @retval None
  */
void
econ_cal_restore(                       econ_t *        p )
{
    econ_cal_t *    cal         = &( p->cal );
    float           zero_ppm    = cal->zero.ppm.i32;
    float           zero_raw    = cal->zero.raw.u32;
    float           span_ppm    = cal->span.ppm.i32;
    float           span_raw    = cal->span.raw.u32;

    cal->tg     = (span_ppm - zero_ppm) / (span_raw - zero_raw);
    cal->ofst   = span_ppm - (span_raw * cal->tg);
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int32_t
econ_raw2ppm(                           econ_t *        p,
                                const   uint32_t        raw )
{
    float   ppm = raw;

    ppm     *= p->cal.tg;
    ppm     += p->cal.ofst;

    return( (int32_t) (ppm + 0.5f) );
}
