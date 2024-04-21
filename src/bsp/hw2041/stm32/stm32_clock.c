/**
  * @file    bsp_stm32_clock.c
  * @brief   MCU Clock Tree
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


int
stm32_clock_init( void )
{
    RCC_OscInitTypeDef              osc     = {0};
    RCC_ClkInitTypeDef              clk     = {0};
    RCC_PeriphCLKInitTypeDef        pck     = {0};
    
    HAL_Init();

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_CRC_CLK_ENABLE();     //Enable CRC to Unlock GUI

    //HAL_PWR_EnableBkUpAccess();

    //Configure the main internal regulator output voltage
    __HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );


    //Initializes the CPU, AHB and APB busses clocks
    osc.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
    osc.HSEState            = RCC_HSE_ON;
    osc.PLL.PLLState        = RCC_PLL_ON;
    osc.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLM            = 16;
    osc.PLL.PLLN            = 400;
    osc.PLL.PLLP            = RCC_PLLP_DIV2;
    osc.PLL.PLLQ            = 2;
    if( HAL_RCC_OscConfig( &osc ) != HAL_OK )
    {
            return( -1 );
    }

    //Activate the OverDrive to reach the 216 MHz Frequency
    if( HAL_PWREx_EnableOverDrive() != HAL_OK )
    {
            return( -2 );
    }

    //Initializes the CPU, AHB and APB busses clocks
    clk.ClockType           = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource        = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider       = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider      = RCC_HCLK_DIV4;
    clk.APB2CLKDivider      = RCC_HCLK_DIV2;
    if( HAL_RCC_ClockConfig( &clk, FLASH_LATENCY_7 ) != HAL_OK )
    {
            return( -3 );
    }

    //LCD
    pck.PeriphClockSelection        = RCC_PERIPHCLK_LTDC;
    //pck.PLLSAI.PLLSAIN              = 100;
    pck.PLLSAI.PLLSAIN              = 200;
    pck.PLLSAI.PLLSAIP              = RCC_PLLSAIP_DIV2;
	pck.PLLSAI.PLLSAIQ              = 2;
    pck.PLLSAI.PLLSAIR              = 5;
    pck.PLLSAIDivQ                  = 1;
    pck.PLLSAIDivR                  = RCC_PLLSAIDIVR_2;
    HAL_RCCEx_PeriphCLKConfig( &pck );

    LL_AHB1_GRP1_EnableClock(   LL_AHB1_GRP1_PERIPH_GPIOA   |
                                LL_AHB1_GRP1_PERIPH_GPIOB   |
                                LL_AHB1_GRP1_PERIPH_GPIOC   |
                                LL_AHB1_GRP1_PERIPH_GPIOD   |
                                LL_AHB1_GRP1_PERIPH_GPIOE   |
                                LL_AHB1_GRP1_PERIPH_GPIOF   |
                                LL_AHB1_GRP1_PERIPH_GPIOG   |
                                LL_AHB1_GRP1_PERIPH_GPIOH   |
                                LL_AHB1_GRP1_PERIPH_DMA1    |
                                LL_AHB1_GRP1_PERIPH_DMA2    |
                                LL_AHB1_GRP1_PERIPH_DMA2D   );

    LL_APB1_GRP1_EnableClock(   LL_APB1_GRP1_PERIPH_UART4   );


    LL_APB2_GRP1_EnableClock(   LL_APB2_GRP1_PERIPH_LTDC    |
                                LL_APB2_GRP1_PERIPH_USART1  );

    LL_RCC_SetUARTClockSource( LL_RCC_UART4_CLKSOURCE_PCLK1 );

    LL_AHB1_GRP1_ForceReset( LL_AHB1_GRP1_PERIPH_DMA1 );
    __DSB();
    LL_AHB1_GRP1_ReleaseReset( LL_AHB1_GRP1_PERIPH_DMA1 );
    __DSB();

    LL_AHB1_GRP1_ForceReset( LL_AHB1_GRP1_PERIPH_DMA2 );
    __DSB();
    LL_AHB1_GRP1_ReleaseReset( LL_AHB1_GRP1_PERIPH_DMA2 );
    __DSB();

    LL_AHB1_GRP1_ForceReset( LL_AHB1_GRP1_PERIPH_DMA2D );
    __DSB();
    LL_AHB1_GRP1_ReleaseReset( LL_AHB1_GRP1_PERIPH_DMA2D );
    __DSB();

    return( 0 );
}
