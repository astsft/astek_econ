/**
  * @file    bsp_stm32_rtc.c
  * @brief   Real Time Clock servives
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <stdbool.h>
#include "stm32.h"


/*******************************************************************************
* PRIVATE CONSTANTS
*******************************************************************************/
#define RTC_ASYNCH_PREDIV               127
#define RTC_SYNCH_PREDIV                255


/*******************************************************************************
* PRIVATE VRIABLES
*******************************************************************************/
static  RTC_HandleTypeDef       hrtc;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
int
init_lse( void )
{
        RCC_OscInitTypeDef              osc;
        RCC_PeriphCLKInitTypeDef        pck;


        //RTC LSE
        HAL_RCC_GetOscConfig( &osc );

        if( osc.LSEState != RCC_LSE_ON )
        {
                __HAL_RCC_LSEDRIVE_CONFIG( RCC_LSEDRIVE_HIGH );

                osc.OscillatorType              = RCC_OSCILLATORTYPE_LSE;
                osc.PLL.PLLState                = RCC_PLL_NONE;
                osc.LSEState                    = RCC_LSE_ON;
                osc.LSIState                    = RCC_LSI_OFF;

                if( HAL_RCC_OscConfig( &osc ) != HAL_OK )
                {
                        return( -1 );
                }

                __HAL_RCC_LSEDRIVE_CONFIG( RCC_LSEDRIVE_LOW );
        }

        HAL_RCCEx_GetPeriphCLKConfig( &pck );

        if( pck.RTCClockSelection != RCC_RTCCLKSOURCE_LSE )
        {
                pck.PeriphClockSelection        = RCC_PERIPHCLK_RTC;
                pck.RTCClockSelection           = RCC_RTCCLKSOURCE_LSE;
                if( HAL_RCCEx_PeriphCLKConfig( &pck ) != HAL_OK )
                {
                        return( -2 );
                }
        }

        return( 0 );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
void
HAL_RTC_MspInit(                        RTC_HandleTypeDef *     hrtc )
{
        //To change the source clock of the RTC feature (LSE, LSI):
        // - enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
        // - enable write access using HAL_PWR_EnableBkUpAccess() function before to configure the RTC clock source (to be done once after reset)
        // - reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and __HAL_RCC_BACKUPRESET_RELEASE()
        // - configure the needed RTc clock source
        __HAL_RCC_PWR_CLK_ENABLE();
        HAL_PWR_EnableBkUpAccess();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();
          // Enable backup regulator
        LL_PWR_EnableBkUpRegulator();
        // Wait until backup regulator is initialized
        while (!LL_PWR_IsActiveFlag_BRR()){};
        //After a system reset, the application can read the INITS flag in the RTC_ISR register to
        //check if the calendar has been initialized or not. If this flag equals 0, the calendar has not
        //been initialized since the year field is set at its Backup domain reset default value (0x00).
        //To read the calendar after initialization, the software must first check that the RSF flag is set
        //in the RTC_ISR register.
        if( HAL_IS_BIT_CLR( RTC->ISR, RTC_FLAG_INITS ) )
        {
                __HAL_RCC_BACKUPRESET_FORCE();
                __HAL_RCC_BACKUPRESET_RELEASE();

                init_lse();

                __HAL_RCC_RTC_ENABLE();
                stm32_rtc_set_date( 51, 01, 01 );
        }

        //enable RTC peripheral Clocks
        __HAL_RCC_RTC_ENABLE();
}


int
stm32_rtc_init( void )
{
        hrtc.Instance                   = RTC;
        hrtc.Init.HourFormat            = RTC_HOURFORMAT_24;
        hrtc.Init.AsynchPrediv          = RTC_ASYNCH_PREDIV;
        hrtc.Init.SynchPrediv           = RTC_SYNCH_PREDIV;
        hrtc.Init.OutPut                = RTC_OUTPUT_DISABLE;
        hrtc.Init.OutPutPolarity        = RTC_OUTPUT_POLARITY_HIGH;
        hrtc.Init.OutPutType            = RTC_OUTPUT_TYPE_OPENDRAIN;

        if( HAL_RTC_Init( &hrtc ) != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_set_date(                     const   int             year,
                                        const   int             month,
                                        const   int             day )
{
        RTC_DateTypeDef         date;


        date.Year       =   year;
        date.Month      =   month;
        date.Date       =   day;
        date.WeekDay    = 0;

        if( HAL_RTC_SetDate( &hrtc, &date, RTC_FORMAT_BIN ) != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_set_time(                     const   int             hour,
                                        const   int             minute,
                                        const   int             second )
{
        RTC_TimeTypeDef         time;


        time.Hours              = hour;
        time.Minutes            = minute;
        time.Seconds            = second;
        time.SubSeconds         = 0x00;
        time.TimeFormat         = RTC_HOURFORMAT12_AM;
        time.DayLightSaving     = RTC_DAYLIGHTSAVING_NONE ;
        time.StoreOperation     = RTC_STOREOPERATION_RESET;

        if( HAL_RTC_SetTime( &hrtc, &time, RTC_FORMAT_BIN ) != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
time_t
stm32_rtc_get_timestamp( void )
{
                RTC_DateTypeDef         date;
                RTC_TimeTypeDef         time;
        struct  tm                      ts;


        //UM1725:
        //You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values
        //in the higher-order calendar shadow registers to ensure consistency between
        //the time and date values. Reading RTC current time locks the values in
        //calendar shadow registers until Current date is read.
        HAL_RTC_GetTime( &hrtc, &time, RTC_FORMAT_BIN );
        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );

        ts.tm_sec       =   time.Seconds;
        ts.tm_min       =   time.Minutes;
        ts.tm_hour      =   time.Hours;
        ts.tm_mday      =   date.Date;
        ts.tm_mon       =   date.Month - 1;
        ts.tm_year      =   date.Year + 70;
        ts.tm_isdst     =   0;

	return( mktime( &ts ) );
}


/*******************************************************************************
* UM1725:
* You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values
* in the higher-order calendar shadow registers to ensure consistency between
* the time and date values. Reading RTC current time locks the values in
* calendar shadow registers until Current date is read.
*******************************************************************************/

/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_date(                             uint8_t *       year,
                                                uint8_t *       month,
                                                uint8_t *       monthday )
{

        RTC_TimeTypeDef         time;
        RTC_DateTypeDef         date;


        HAL_RTC_GetTime( &hrtc, &time, RTC_FORMAT_BIN );
        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );

        *year           = date.Year;
        *month          = date.Month;
        *monthday       = date.Date;

        return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_time(                             uint8_t *       hours,
                                                uint8_t *       minutes,
                                                uint8_t *       seconds )
{
        RTC_TimeTypeDef         time;
        RTC_DateTypeDef         date;


        HAL_RTC_GetTime( &hrtc, &time, RTC_FORMAT_BIN );
        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );

        *hours          = time.Hours;
        *minutes        = time.Minutes;
        *seconds        = time.Seconds;

        return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_second( void )
{
        RTC_TimeTypeDef         time;
        RTC_DateTypeDef         date;


        HAL_RTC_GetTime( &hrtc, &time, RTC_FORMAT_BIN );
        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );
        return( time.Seconds );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_minute( void )
{
        RTC_TimeTypeDef         time;
        RTC_DateTypeDef         date;


        HAL_RTC_GetTime( &hrtc, &time, RTC_FORMAT_BIN );
        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );
        return( time.Minutes );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_hour( void )
{
        RTC_TimeTypeDef         time;
        RTC_DateTypeDef         date;


        HAL_RTC_GetTime( &hrtc, &time, RTC_FORMAT_BIN );
        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );
        return( time.Hours );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_day( void )
{
        RTC_DateTypeDef         date;


        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );
        return( date.Date );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_month( void )
{
        RTC_DateTypeDef         date;


        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );
        return( date.Month );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_rtc_get_year( void )
{
        RTC_DateTypeDef         date;


        HAL_RTC_GetDate( &hrtc, &date, RTC_FORMAT_BIN );
        return( date.Year );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
void
stm32_rtc_bckp_put(                             size_t          addr,
                                        const   uint32_t        data )
{
        HAL_RTCEx_BKUPWrite( &hrtc, addr, data );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
uint32_t
stm32_rtc_bckp_get(                             size_t          addr )
{
        return HAL_RTCEx_BKUPRead( &hrtc, addr );
}
