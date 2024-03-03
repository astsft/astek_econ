/**
  * @file    stm32_fmc.c
  * @brief   Flexible Memory Controller
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "mt48lc/mt48lc4m32b2.h"
#include "stm32.h"


#define FMC_NBL0_Pin                    GPIO_PIN_0
#define FMC_NBL0_GPIO_Port              GPIOE
#define FMC_NBL1_Pin                    GPIO_PIN_1
#define FMC_NBL1_GPIO_Port              GPIOE


static  SDRAM_HandleTypeDef     hsdram1;
//static  uint32_t                FMC_Initialized = 0;


static
void
init_io( void )
{
        GPIO_InitTypeDef        gpio    = {0};

/*
        if( FMC_Initialized )
        {
                return;
        }

        FMC_Initialized = 1;
*/


    /** FMC GPIO Configuration
    PF0   ------> FMC_A0
    PF1   ------> FMC_A1
    PF2   ------> FMC_A2
    PF3   ------> FMC_A3
    PF4   ------> FMC_A4
    PF5   ------> FMC_A5
    PC0   ------> FMC_SDNWE
    PC2   ------> FMC_SDNE0
    PC3   ------> FMC_SDCKE0
    PF11   ------> FMC_SDNRAS
    PF12   ------> FMC_A6
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PF15   ------> FMC_A9
    PG0   ------> FMC_A10
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG2   ------> FMC_A12
    PG4   ------> FMC_BA0
    PG5   ------> FMC_BA1
    PG8   ------> FMC_SDCLK
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PG15   ------> FMC_SDNCAS
    PE0   ------> FMC_NBL0
    PE1   ------> FMC_NBL1
    */

        gpio.Pin        =   GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                            GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_11 | GPIO_PIN_12 |
                            GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate  = GPIO_AF12_FMC;
        HAL_GPIO_Init( GPIOF, &gpio );

        gpio.Pin        = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate  = GPIO_AF12_FMC;
        HAL_GPIO_Init( GPIOC, &gpio );

        gpio.Pin        =   GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_4  |
                            GPIO_PIN_5  | GPIO_PIN_8  | GPIO_PIN_15;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate  = GPIO_AF12_FMC;
        HAL_GPIO_Init( GPIOG, &gpio );

        gpio.Pin        =   GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 |
                                    GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
                                    GPIO_PIN_15 | FMC_NBL0_Pin | FMC_NBL1_Pin;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate  = GPIO_AF12_FMC;
        HAL_GPIO_Init( GPIOE, &gpio );

        gpio.Pin        = GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_14 |
                          GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
        gpio.Mode        = GPIO_MODE_AF_PP;
        gpio.Pull        = GPIO_NOPULL;
        gpio.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate   = GPIO_AF12_FMC;
        HAL_GPIO_Init( GPIOD, &gpio );
}


/* FMC initialization function */
static
int
init( void )
{
        FMC_SDRAM_TimingTypeDef timing  = {0};


        __HAL_RCC_FMC_CLK_ENABLE();

        hsdram1.Instance                = FMC_SDRAM_DEVICE;
        hsdram1.Init.SDBank             = FMC_SDRAM_BANK1;
        hsdram1.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
        hsdram1.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;
        hsdram1.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
        hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
        hsdram1.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
        hsdram1.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
        hsdram1.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
        hsdram1.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
        hsdram1.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

/*
        hsdram1.Instance                = FMC_SDRAM_DEVICE;
        hsdram1.Init.SDBank             = FMC_SDRAM_BANK1;
        hsdram1.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
        hsdram1.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;
        hsdram1.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
        hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_2;
        hsdram1.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
        hsdram1.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
        hsdram1.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
        hsdram1.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
        hsdram1.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;
*/


        timing.ExitSelfRefreshDelay     =  7;
        timing.LoadToActiveDelay        =  2;
        timing.RCDDelay                 =  2;
        timing.RowCycleDelay            =  7;
        timing.RPDelay                  =  2;
        timing.SelfRefreshTime          =  4;
        timing.WriteRecoveryTime        =  3;

/*
        timing.ExitSelfRefreshDelay     =  6;
        timing.LoadToActiveDelay        =  3;
        timing.RCDDelay                 =  3;
        timing.RowCycleDelay            =  6;
        timing.RPDelay                  =  3;
        timing.SelfRefreshTime          =  4;
        timing.WriteRecoveryTime        =  4;
*/
    if( HAL_SDRAM_Init( &hsdram1, &timing ) != HAL_OK )
    {
        return( -1 );
    }

    return( 0 );
}


/**
  * @brief  Initializes FMC for SDRAM device.
  * @retval SDRAM status
  */
int
stm32_fmc_init( void )
{
    init_io();
    init();
    NVIC_SetPriority( FMC_IRQn, STM32_NVIC_PRIO_FMC );
    NVIC_EnableIRQ( FMC_IRQn );
    mt48lc4m32b2_init( &hsdram1 );

    return( 0 );
}


void
stm32_fmc_isr( void )
{
    HAL_SDRAM_IRQHandler( &hsdram1 );
}
