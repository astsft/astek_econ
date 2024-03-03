/**
  * @file       stm32_reset.h
  * @brief      Global MCU Reset
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
 * @brief Restart MCU
 */
void
stm32_reset( void )
{
    NVIC_SystemReset();
}
