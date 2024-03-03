/**
  * @file    gt9xx_chksum.c
  * @brief   GOODIX GT9xx Touchpad driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "gt9xx.h"


/*******************************************************************************
* GLOBAL FUNCTIONS
*******************************************************************************/
int
gt9xx_checksum(                 const   uint8_t *       data,
                                        size_t          size )
{
        int             i,
                        raw_cfg_len;
        uint8_t         check_sum       = 0;


        raw_cfg_len     = size - 2;
        for( i = 0; i < raw_cfg_len; i++ )
        {
                check_sum       += data[ i];
        }

        check_sum       = (~check_sum) + 1;

        return( check_sum );
}
