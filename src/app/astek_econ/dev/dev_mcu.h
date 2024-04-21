/**
  * @file    dev_mcu.c
  * @brief   Device MCU Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef DEV_MCU_H
#define DEV_MCU_H


/*******************************************************************************
* TYPES
*******************************************************************************/
typedef struct  dev_mcu_s
{
    struct
    {
        size_t          rx_idle_cnt;
        size_t          rcvd;
        uint8_t         rxd;
    } usart1;
    
    struct
    {
        size_t          rx_idle_cnt;
        size_t          rcvd;
        uint8_t         rxd;
    } usart2;    

    //struct
    //{
    //    size_t          rx_idle_cnt;
    //    bool            rx_idle_flag;
    //} usart2;

    struct
    {
        int32_t         temp_digc;
        int32_t         vref_mv;
        bool            ready_flag;
    } adc1;

    struct
    {
        size_t          rx_idle_cnt;
        int             (* set_date) ( const int, const int, const int );
        int             (* set_time) ( const int, const int, const int );
        time_t          (* get_timestamp) (void);
        int             (* get_date) ( uint8_t *, uint8_t *, uint8_t * );
        int             (* get_time) ( uint8_t *, uint8_t *, uint8_t * );
        int             (* get_year) ( void );
        int             (* get_month) ( void );
        int             (* get_day) ( void );
        int             (* get_hour) ( void );
        int             (* get_minute) ( void );
        int             (* get_second) ( void );
    } rtc;

} dev_mcu_t;


#endif  //DEV_MCU_H
