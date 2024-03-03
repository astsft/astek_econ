/**
  * @file    stm32_mpu.c
  * @brief   Memory Protection Unit
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"

/*
AN4838 - Managing memory protection unit in STM32 MCUs

3.1 Memory types

There are three common memory types:
• Normal memory: allows the load and store of bytes, half-words and words to be arranged by the CPU in
an efficient manner (the compiler is not aware of memory region types). For the normal memory region the
load/store is not necessarily performed by the CPU in the order listed in the program.
• Device memory: within the device region, the loads and stores are done strictly in order. This is to ensure the
registers are set in the proper order.
• Strongly ordered memory: everything is always done in the programmatically listed order, where the CPU
waits the end of load/store instruction execution (effective bus access) before executing the next instruction
in the program stream. This can cause a performance hit.


The S field is for a shareable memory region: the memory system provides data synchronization between
bus masters in a system with multiple bus masters, for example, a processor with a DMA controller. A
strongly-ordered memory is always shareable. If multiple bus masters can access a non-shareable memory
region, the software must ensure the data coherency between the bus masters. The STM32F7 Series and
STM32H7 Series do not support hardware coherency. The S field is equivalent to non-cacheable memory.


For Cortex-M7, TCMs memories always behave as non-cacheable, non-shared normal memories, irrespective
of the memory type attributes defined in the MPU for a memory region containing addresses held in the TCM.
Otherwise, the access permissions associated with an MPU region in the TCM address space are treated in the
same way as addresses outside the TCM address space.
*/

/*
AN4838 - Managing memory protection unit in STM32 MCUs

Table 4. Cache properties and shareability

TEX     C   B   Memory Type         Description                     Shareable
--------------------------------------------------------------------------------
000     0   0   Strongly Ordered    Strongly Ordered                    Yes
000     0   1   Device              Shared Device                       Yes
000     1   0   Normal              Write through, no write allocate    S bit
000     1   1   Normal              Write-back, no write allocate       S bit
001     0   0   Normal              Non-cacheable                       S bit
001     0   1   Reserved            Reserved                            Reserved
001     1   0   Undefined           Undefined                           Undefined
001     1   1   Normal              Write-back, write and read allocate S bit
010     0   0   Device              Non-shareable device                No
010     0   1   Reserved            Reserved                            Reserved
*/


void
stm32_mpu_init( void )
{
    //Configure the MPU as Strongly ordered for not defined regions
    uint32_t    whole   =   LL_MPU_REGION_SIZE_4GB          |
                            LL_MPU_REGION_NO_ACCESS         |
                            LL_MPU_ACCESS_NOT_BUFFERABLE    |
                            LL_MPU_ACCESS_NOT_CACHEABLE     |
                            LL_MPU_ACCESS_SHAREABLE         |
                            LL_MPU_TEX_LEVEL0               |
                            LL_MPU_INSTRUCTION_ACCESS_DISABLE;

    uint32_t    flash   =   LL_MPU_REGION_SIZE_1MB          |
                            LL_MPU_REGION_FULL_ACCESS       |
                            LL_MPU_ACCESS_NOT_BUFFERABLE    |
                            LL_MPU_ACCESS_NOT_CACHEABLE     |
                            LL_MPU_ACCESS_SHAREABLE         |
                            LL_MPU_TEX_LEVEL0               |
                            LL_MPU_INSTRUCTION_ACCESS_ENABLE;

    uint32_t    sram    =   LL_MPU_REGION_SIZE_512KB        |
                            LL_MPU_REGION_FULL_ACCESS       |
                            LL_MPU_ACCESS_BUFFERABLE        |
                            LL_MPU_ACCESS_CACHEABLE         |
                            LL_MPU_ACCESS_SHAREABLE         |
                            LL_MPU_TEX_LEVEL1               |
                            LL_MPU_INSTRUCTION_ACCESS_DISABLE;

    uint32_t    periph  =   LL_MPU_REGION_SIZE_512MB        |
                            LL_MPU_REGION_FULL_ACCESS       |
                            LL_MPU_ACCESS_BUFFERABLE        |
                            LL_MPU_ACCESS_NOT_CACHEABLE     |
                            LL_MPU_ACCESS_SHAREABLE         |
                            LL_MPU_TEX_LEVEL0               |
                            LL_MPU_INSTRUCTION_ACCESS_DISABLE;

    uint32_t    fmc     =   LL_MPU_REGION_SIZE_8KB          |
                            LL_MPU_REGION_FULL_ACCESS       |
                            LL_MPU_ACCESS_BUFFERABLE        |
                            LL_MPU_ACCESS_NOT_CACHEABLE     |
                            LL_MPU_ACCESS_SHAREABLE         |
                            LL_MPU_TEX_LEVEL0               |
                            LL_MPU_INSTRUCTION_ACCESS_DISABLE;

    uint32_t    sdram   =   LL_MPU_REGION_SIZE_256MB        |
                            LL_MPU_REGION_FULL_ACCESS       |
                            LL_MPU_ACCESS_BUFFERABLE        |
                            LL_MPU_ACCESS_CACHEABLE         |
                            LL_MPU_ACCESS_NOT_SHAREABLE     |
                            LL_MPU_TEX_LEVEL0               |
                            LL_MPU_INSTRUCTION_ACCESS_DISABLE;

    LL_MPU_Disable();
    LL_MPU_ConfigRegion( LL_MPU_REGION_NUMBER0, 0x87, 0x00000000ul, whole   );
    LL_MPU_ConfigRegion( LL_MPU_REGION_NUMBER1, 0x00, 0x08000000ul, flash   );
    LL_MPU_ConfigRegion( LL_MPU_REGION_NUMBER2, 0x00, 0x20000000ul, sram    );
    LL_MPU_ConfigRegion( LL_MPU_REGION_NUMBER3, 0x00, 0x40000000ul, periph  );
    LL_MPU_ConfigRegion( LL_MPU_REGION_NUMBER4, 0x00, 0xA0000000ul, fmc     );
    LL_MPU_ConfigRegion( LL_MPU_REGION_NUMBER5, 0x00, 0xC0000000ul, sdram   );
    LL_MPU_Enable( LL_MPU_CTRL_PRIVILEGED_DEFAULT );
}
