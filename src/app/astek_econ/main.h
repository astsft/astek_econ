/**
  * @file    main.h
  * @brief   Main Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef MAIN_H
#define MAIN_H

/*******************************************************************************
*
*******************************************************************************/
#include <time.h>
#include "GUI.h"
#include "trace/trace.h"


/*******************************************************************************
*
*******************************************************************************/
void    task_start( const void * );

void    task_ibus( const void * );
void    task_ibus_sens_conf_lpf_update( const uint16_t, const uint16_t );
void    task_ibus_sens_conf_comp_update( const uint16_t, const uint16_t );
void    task_ibus_cl420_ch2_update( void );
void    task_ibus_boards_init(void);


void    task_m2m( const void * );

void    task_hmi( const void * );
void    task_hmi_cal_update( const int );
void    task_hmi_cal_restore( const int );
//void    task_hmi_cal_backup_restore( const int );
//void    task_hmi_cal_factory_settings_update( const int );

void    task_net( const void * );

#endif  //MAIN_H
