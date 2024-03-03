/**
  * @file       bsp_beep.h
  * @brief      Board Support Package, Beep routines
  * @author     Igor T. <research.tahoe@gmail.com>
  */


//#include "bsp_beep.h"
//#include "app_error.h"
#include "stm32.h"


static  TIM_HandleTypeDef       htim;
static  DMA_HandleTypeDef       hdma_ch2;
static  DMA_HandleTypeDef       hdma_ch3;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
	GPIO_InitTypeDef        pins            =   {   .Pin            =    GPIO_PIN_1 | GPIO_PIN_2,
	                                                .Mode           =    GPIO_MODE_AF_PP,
	                                                .Pull           =    GPIO_NOPULL,
	                                                .Speed          =    GPIO_SPEED_FREQ_LOW,
                                                        .Alternate      =    GPIO_AF2_TIM5 };

	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init( GPIOA, &pins );
}


static
int
init_timer(                     const   uint32_t        freq_hz )
{
        TIM_MasterConfigTypeDef         cfg;
        TIM_OC_InitTypeDef              cmp;
        TIM_BreakDeadTimeConfigTypeDef  dtc;
        const   uint32_t                pclk1_hz        = SystemCoreClock / 4;
        const   uint32_t                tone_freq_hz    = freq_hz;
        const   uint32_t                period_ticks    = 256;
        const   uint32_t                magic_number    = 2;
        const   uint32_t                prescaler_raw   = (pclk1_hz * magic_number) / (tone_freq_hz * period_ticks);


        __HAL_RCC_TIM5_CLK_ENABLE();

        htim.Instance                   = TIM5;
        htim.Init.Prescaler             =  prescaler_raw;
        htim.Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim.Init.Period                = period_ticks;
        htim.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim.Init.RepetitionCounter     = 0;
        htim.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;

        if( HAL_TIM_PWM_Init( &htim ) != HAL_OK )
        {
                //app_error_handler();
                return( -1 );
        }

        //cfg.MasterOutputTrigger         = TIM_TRGO_RESET;
        cfg.MasterOutputTrigger         = TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             = TIM_MASTERSLAVEMODE_DISABLE;

        if( HAL_TIMEx_MasterConfigSynchronization( &htim, &cfg ) != HAL_OK )
        {
                //app_error_handler();
                return( -2 );
        }

        cmp.OCMode                      = TIM_OCMODE_PWM1;
        cmp.Pulse                       = htim.Init.Period / 2;
        cmp.OCPolarity                  = TIM_OCPOLARITY_HIGH;
        cmp.OCNPolarity                 = TIM_OCNPOLARITY_HIGH;
        cmp.OCFastMode                  = TIM_OCFAST_DISABLE;
        cmp.OCIdleState                 = TIM_OCIDLESTATE_RESET;
        cmp.OCNIdleState                = TIM_OCNIDLESTATE_RESET;

        if( HAL_TIM_PWM_ConfigChannel( &htim, &cmp, TIM_CHANNEL_2 ) != HAL_OK )
        {
                //app_error_handler();
                return( -3 );
        }

        cmp.OCPolarity                  = TIM_OCPOLARITY_LOW;
        cmp.OCNPolarity                 = TIM_OCPOLARITY_LOW;

        if( HAL_TIM_PWM_ConfigChannel( &htim, &cmp, TIM_CHANNEL_3 ) != HAL_OK )
        {
                //app_error_handler();
                return( -4 );
        }

        dtc.OffStateRunMode             = TIM_OSSR_DISABLE;
        dtc.OffStateIDLEMode            = TIM_OSSI_DISABLE;
        dtc.LockLevel                   = TIM_LOCKLEVEL_OFF;
        dtc.DeadTime                    = 0;
        dtc.BreakState                  = TIM_BREAK_DISABLE;
        dtc.BreakPolarity               = TIM_BREAKPOLARITY_HIGH;
        dtc.AutomaticOutput             = TIM_AUTOMATICOUTPUT_DISABLE;

        if( HAL_TIMEx_ConfigBreakDeadTime( &htim, &dtc ) != HAL_OK )
        {
                //app_error_handler();
                return( -5 );
        }

        return( 0 );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
 * @brief Beep initialization
 */
int
stm32_tim5_init(                const   uint32_t        freq_hz )
{
        init_timer( freq_hz );
        init_io();

        return( 0 );
}


int
stm32_tim5_pwm_start( void )
{
        if( HAL_OK  != HAL_TIM_PWM_Start( &htim, TIM_CHANNEL_2 ) )
        {
                //app_error_handler();
                return( -1 );
        }

        if( HAL_OK  != HAL_TIM_PWM_Start( &htim, TIM_CHANNEL_3 ) )
        {
                //app_error_handler();
                return( -2 );
        }

        return( 0 );
}


int
stm32_tim5_pwm_stop( void )
{
        if( HAL_OK  != HAL_TIM_PWM_Stop( &htim, TIM_CHANNEL_2 ) )
        {
                //app_error_handler();
                return( -1 );
        }

        if( HAL_OK  != HAL_TIM_PWM_Stop( &htim, TIM_CHANNEL_3 ) )
        {
                //app_error_handler();
                return( -2 );
        }

        return( 0 );
}


/**
 * @brief Beep enable/disable at preset tone
 */
/*
void
bsp_beep_tone_set(                      const   bool            enable )
{
        if( enable )
        {
                if( HAL_OK  != HAL_TIM_PWM_Start( &htim, TIM_CHANNEL_2 ) )
                {
                        app_error_handler();
                }

                if( HAL_OK  != HAL_TIM_PWM_Start( &htim, TIM_CHANNEL_3 ) )
                {
                        app_error_handler();
                }
        }
        else
        {
                if( HAL_OK  != HAL_TIM_PWM_Stop( &htim, TIM_CHANNEL_2 ) )
                {
                        app_error_handler();
                }

                if( HAL_OK  != HAL_TIM_PWM_Stop( &htim, TIM_CHANNEL_3 ) )
                {
                        app_error_handler();
                }
        }
}
*/

/**
 * @brief Beep tone at arbitary frequency
 */
/*
void
bsp_beep_tone_play(                     const   uint32_t        freq_hz )
{
}
*/

/**
 * @brief Beep wave play
 */
/*
void
bsp_beep_wave_play(                     const   uint32_t *      data,
                                        const   size_t          len )
{
}
*/

/*******************************************************************************
* BEEP ISR
*******************************************************************************/
/*
void
bsp_beep_isr( void )
{
}
*/
