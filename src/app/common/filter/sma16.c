/**
  * @file    sma16.c
  * @brief   Simple Moving Average Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <stdint.h>
#include "sma.h"
#include "string.h"

void
sma16_init(                         sma_t*    p,
                                    int32_t*  buf,
                                    uint32_t  buf_size )
{
    memset ( buf, 0, sizeof(int32_t) * buf_size );
    p->buf          = buf;
    p->buf_size     = buf_size;
    p->idx          = 0;
    p->sum          = 0;
    p->result       = 0;
}

int32_t
sma16_filter( sma_t* p,  int32_t x)
{
    p->sum                  += x;
    p->sum                  -= p->buf[ p->idx ];
    p->buf[ p->idx ]        =  x;
    p->idx++;
    if( p->idx >= p->buf_size )
    {
        p->idx                  = 0;
    }
    
    p->result = p->sum / p->buf_size;

    return( p->result );
}


