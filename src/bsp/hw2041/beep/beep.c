/**
  * @file       beep.c
  * @brief      Beep services
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"
#include "beep.h"


#pragma pack(4)
static  ui_beep_t       ui_beep = { .init           =   stm32_tim5_init,
	                                .start          =   stm32_tim5_pwm_start,
	                                .stop           =   stm32_tim5_pwm_stop,
	                                .tick           =   0 };
#pragma pack()


/**
 * @brief       Initialization
 */
void
beep_init(                      const   uint32_t        freq_hz )
{
        ui_beep_t *     p       = &( ui_beep );


        p->init( freq_hz );
}


/**
 * @brief       SysTick hook
 */
void beep_hook( void )
{
    ui_beep_t *     p       = &( ui_beep );


	if( p->tick > 0 )
	{
		p->tick--;

		if( p->tick == 0 )
		{
            p->stop();
		}
	}
}


/**
 * @brief       Play
 */
void
beep_play(                           const   size_t          ticks )
{
        ui_beep_t *     p       = &( ui_beep );


        p->tick         =   ticks;
        p->start();
}
