/**
  * @file       bsp_stm32_reset.h
  * @brief      Global MCU Reset
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include "stm32.h"


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
 * @brief Read Unique device ID
 */
int
stm32_device_uid(                       uint32_t *      uid,
                                        size_t          uid_countof )
{
        if( uid_countof < 4 )
        {
                return( -1 );
        }

        *uid++  = HAL_GetUIDw2();
        *uid++  = HAL_GetUIDw1();
        *uid++  = HAL_GetUIDw0();

        return( 0 );
}


/**
 * @brief Read Unique device ID as string
 */
int
stm32_device_uid_str(               char *      str,
                                    size_t      n )
{
        return  snprintf(       str,
                                n,
                                "%04X %04X %04X %04X %04X %04X\0",
                                HAL_GetUIDw2() >> 16,
                                HAL_GetUIDw2() & 0xFFFF,
                                HAL_GetUIDw1() >> 16,
                                HAL_GetUIDw1() & 0xFFFF,
                                HAL_GetUIDw0() >> 16,
                                HAL_GetUIDw0() & 0xFFFF );
}
