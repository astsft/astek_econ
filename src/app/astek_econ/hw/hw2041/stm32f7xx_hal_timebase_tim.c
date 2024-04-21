/**
  * @file    stm32f7xx_hal_timebase_tim.c
  * @brief   HAL time base based on the hardware TIM Template.
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32f7xx_hal.h"
#include "stm32.h"


static  TIM_HandleTypeDef       htim;


/**
  * @brief  This function configures the TIM6 as a time base source.
  *         The time source is configured to have 1ms time base with a dedicated
  *         Tick interrupt priority.
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
/*
HAL_StatusTypeDef
HAL_InitTick(                           uint32_t        TickPriority )
{
    RCC_ClkInitTypeDef      clkconfig;
    uint32_t                uwTimclock, uwAPB1Prescaler = 0U;
    uint32_t                uwPrescalerValue = 0U;
    uint32_t                pFLatency;


    //NVIC_SetPriority( TIM6_DAC_IRQn, STM32_NVIC_PRIO_TIM6 );
    //NVIC_EnableIRQ( TIM6_DAC_IRQn );


        __HAL_RCC_TIM6_CLK_ENABLE();

        HAL_RCC_GetClockConfig( &clkconfig, &pFLatency );

        uwAPB1Prescaler = clkconfig.APB1CLKDivider;     //Get APB1 prescaler

        if( uwAPB1Prescaler == RCC_HCLK_DIV1 )
        {
                uwTimclock      = HAL_RCC_GetPCLK1Freq();
        }
        else
        {
                uwTimclock      = 2 * HAL_RCC_GetPCLK1Freq();
        }

        //Compute the prescaler value to have TIM6 counter clock equal to 1MHz
        uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);


        // Initialize TIMx peripheral as follow:
        // + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
        // + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
        // + ClockDivision = 0
        // + Counter direction = Up
        htim.Instance                   = TIM6;
        htim.Init.Period                = (1000000U / 1000U) - 1U;
        htim.Init.Prescaler             = uwPrescalerValue;
        htim.Init.ClockDivision         = 0;
        htim.Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;
        if( HAL_TIM_Base_Init( &htim ) == HAL_OK )
        {
                return( HAL_TIM_Base_Start_IT( &htim ) );
        }

        return( HAL_ERROR );
}
*/


HAL_StatusTypeDef
HAL_InitTick(                           uint32_t        TickPriority )
{
        //Configure the SysTick to have interrupt in 1ms time basis
        if( HAL_SYSTICK_Config( SystemCoreClock / (1000U / uwTickFreq) ) > 0U )
        {
                return HAL_ERROR;
        }

        //Configure the SysTick IRQ priority
        if( TickPriority < (1UL << __NVIC_PRIO_BITS) )
        {
                HAL_NVIC_SetPriority( SysTick_IRQn, TickPriority, 0U );
                uwTickPrio = TickPriority;
        }
        else
        {
                return HAL_ERROR;
        }

        return( HAL_OK );
}


/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */


void HAL_SuspendTick(void)
{
        __HAL_TIM_DISABLE_IT( &htim, TIM_IT_UPDATE );
}


/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
        __HAL_TIM_ENABLE_IT( &htim, TIM_IT_UPDATE );
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void
HAL_TIM_PeriodElapsedCallback(          TIM_HandleTypeDef *     htim )
{
    HAL_IncTick();
}


/**
  * @brief  HAL timebase interrupt request handling
  * @param  None
  * @retval None
  */
void
stm32fxx_hal_timebase_tim_isr( void )
{
    HAL_TIM_IRQHandler( &htim );
}