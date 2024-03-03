/**
  * @file       beep.h
  * @brief      Beep services
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#ifndef  BEEP_H
#define  BEEP_H


/*******************************************************************************
*
*******************************************************************************/
#include <string.h>
#include <stdint.h>
#include "config.h"


/*******************************************************************************
*
*******************************************************************************/
#define BEEP_mSEC_TO_TICKS( msec )   ( ((size_t) msec * CONFIG_MCU_SYSTICK_HZ) / 1000 )

#define BEEP_TYPE_CONFIRM       BEEP_mSEC_TO_TICKS( 400 )
#define BEEP_TYPE_EMPTY         BEEP_mSEC_TO_TICKS(   5 )
#define BEEP_TYPE_ERROR         BEEP_mSEC_TO_TICKS(   4 )
//#define BEEP_SHRT          BEEP_mSEC_TO_TICKS(  40 )
#define BEEP_SHRT          BEEP_mSEC_TO_TICKS(  50 )


/*******************************************************************************
*
*******************************************************************************/
typedef int (* beep_init_t  ) ( const uint32_t );
typedef int (* beep_start_t ) (void);
typedef int (* beep_stop_t  ) (void);


typedef	struct	beep_s
{
	beep_init_t     init;
	beep_start_t    start;
	beep_stop_t     stop;
	size_t          tick;
} ui_beep_t;


/*******************************************************************************
*
*******************************************************************************/
void    beep_init( const uint32_t );

void
beep_hook( void );

void
beep_play(                           const   size_t          ticks );


#endif	//BEEP_H
