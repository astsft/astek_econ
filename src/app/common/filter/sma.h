/**
  * @file    sma.h
  * @brief   Simple Moving Average Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef SMA_H
#define SMA_H

#include "stdint.h"

#define LPF_BUF_SIZE                    256
#define LPF_ORDER_MAX                   4

typedef struct  sma_s
{
    int32_t *           buf;
    uint32_t            buf_size;
    uint32_t            idx;
    int32_t             sum;
    int32_t             result;
} sma_t;

extern sma_t lpf_sma[ LPF_ORDER_MAX ];
extern int32_t lpf_sma_buf[ LPF_ORDER_MAX ][ LPF_BUF_SIZE ];

/*******************************************************************************
*
*******************************************************************************/
void sma16_init( sma_t* lpf_sma, int32_t* lpf_sma_buf, uint32_t buf_size);
int32_t sma16_filter( sma_t* p, int32_t x);

#endif  //SMA_H
