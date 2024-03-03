/**
  * @file    time.c
  * @brief   CLIB time() implementation
  * @brief   The "time" function returns the current calendar time.
  * @brief   The time is measured in seconds since the first of January 1970.
  * @brief   (time_t)-1 should be returned if the calendar time is not available.
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>
#include "stm32.h"


#pragma module_name = "?time"

#if _DLIB_TIME_USES_64
__time64_t (__time64)( __time64_t *     t )
  #include "time64.h"
#else
__time32_t (__time32)( __time32_t *     t )
  #include "time32.h"
#endif
{
    time_t  raw = stm32_rtc_get_timestamp();

    if( t != NULL )
    {
        *t  = raw;
    }

    return( raw );
}
