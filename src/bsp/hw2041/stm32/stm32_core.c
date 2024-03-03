/**
  * @file    bsp_stm32_clk.c
  * @brief   MCU Clock Tree
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


void
stm32_core_icache_enable( void )
{
    SCB_EnableICache();
}


void
stm32_core_icache_disable( void )
{
    SCB_DisableICache();
}


void
stm32_core_dcache_enable( void )
{
    SCB_EnableDCache();
}


void
stm32_core_dcache_disable( void )
{
    SCB_DisableDCache();
}
