/**
  * @file    cl420.c
  * @brief   Current Loop 4-20mA
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "cl420.h"


/**
  * @brief
  * @param  None
  * @retval None
  */
uint16_t
cl420_ppb2uA(                           cl420_t *       p,
                                const   uint32_t        ppb )
{
    uint16_t    uA  = 0;
        //uint32_t        ppm     = p->sens.oxgn_ppm.i32;
/*
        uint32_t        range   = 20000L - 4000L;
        switch( p->scale.idx )
        {
                case 3:
                        p->cloop.value_uA       =  (ppm * 250000) / range;
                        break;

                case 2:
                        p->cloop.value_uA       =  (ppm * 100000) / range;
                        break;

                case 1:
                        p->cloop.value_uA       =  (ppm *   1000) / range;
                        break;

                case 0:
                        p->cloop.value_uA       =  (ppm * 250000) / range;
                        break;

                default:
                        break;
        }
*/

/*
        p->cloop.value_uA       =  (ppm / 10);

        if( p->cloop.value_uA > 3000 )
        {
                p->cloop.value_uA       -= 3000;
        }
*/
    return( uA );
}


