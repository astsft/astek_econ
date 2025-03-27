/**
  * @file    task_ibus.c
  * @brief   Task Internal Bus
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <math.h>
#include <stdbool.h>
#include "stm32.h"
#include "cmsis_os.h"
#include "modbus.h"
#include "modbus\client\mdbs_dev_o2mb.h"
#include "config.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "beep\beep.h"
#include "app_pipe.h"
#include "internal_uart.h"
#include "hw_config.h"
#include "hw_cl420.h"
#include "hw_relay.h"

#ifdef EXT_FLASH
  #include "hw2331_ext_flash.h"
#endif


/*******************************************************************************
* PUBLIC VARIABLES
*******************************************************************************/
extern  QueueHandle_t           que_mdbs_clnt_hndl;
extern  QueueHandle_t           que_hmi_hndl;
extern  QueueHandle_t           que_ibus_hndl;
extern  dev_t                   dev;

        TimerHandle_t           tmr_1sec_hndl;
        osStaticTimerDef_t      tmr_1sec_qcb;
        StaticTimer_t           tmr_1sec_alloc;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#pragma data_alignment = 32
uint16_t    mdbs_sens_raw[ MDBS_RTU_ADU_SIZEOF / 2];

#pragma data_alignment = 32
uint8_t     mdbs_adu_xmit[ MDBS_RTU_ADU_SIZEOF ];

#pragma data_alignment = 32
uint8_t     mdbs_adu_recv[ MDBS_RTU_ADU_SIZEOF ];

static void set_ext_relay_status (int status);

int modbus_send_receive_data (uint8_t *mdbs_send_buf, size_t adu_len, size_t *recv_len, uint8_t *mdbs_resv_buf);

/*******************************************************************************
* PRIVATE FUNCTIONS - MODBUS
*******************************************************************************/
//#pragma inline=forced
static
int
modbus_client_read(                     const   uint8_t         dev_addr,
                                                //uint8_t *       adu,
                                        const   uint16_t        reg_addr,
                                        const   size_t          regs_cnt,
                                                uint16_t *      reg16 )
{
                BaseType_t      rcvd;
                os_user_tag_t   tag;
    volatile    uint32_t        cnt;
    volatile    size_t          len;
    app_pipe_t      queue_data;
    //uint32_t        rem;


    cnt     = modbus_rtu_client_hreg_rqst( mdbs_adu_xmit, dev_addr, reg_addr, regs_cnt );

    SCB_CleanDCache_by_Addr( (uint32_t *) mdbs_adu_xmit, MDBS_RTU_ADU_SIZEOF );
    internal_uart_xmit( mdbs_adu_xmit, cnt );

    xQueueReset( que_mdbs_clnt_hndl );

    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );
    internal_uart_recv( mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );

    rcvd = xQueueReceive( que_mdbs_clnt_hndl, &queue_data, pdMS_TO_TICKS(500) );
    tag = queue_data.tag;

    if( rcvd == pdFALSE )
    {
        return( -1 );
    }

    if( tag == OS_USER_TAG_UART4_RECV_TOUT )
    {
        cnt = MDBS_RTU_ADU_SIZEOF - internal_uart_rx_get_ndtr();
        //TRACE( "rem: %d\n", cnt );
        len = mdbs_rtu_xfer( dev_addr, mdbs_adu_recv, cnt, reg16 );

        if( len != MDBS_ERR_NONE )
        {
            return( -3 );
        }
    }

    return( 0 );
}


//#pragma inline=forced
static
int
modbus_client_write(                    const   uint8_t         dev_addr,
                                                //uint8_t *       adu,
                                        const   uint16_t        reg_addr,
                                        const   size_t          regs_cnt,
                                                uint16_t *      mdbs_raw )
{
                BaseType_t      received;
                os_user_tag_t   sync;
    volatile    size_t          len;
    volatile    uint32_t        rem;
    volatile    uint32_t        cnt;
    app_pipe_t queue_data;


    len = modbus_rtu_client_write_multiple_registers( dev_addr, mdbs_adu_xmit, reg_addr, regs_cnt, mdbs_raw );

    SCB_CleanDCache_by_Addr( (uint32_t *) mdbs_adu_xmit, MDBS_RTU_ADU_SIZEOF );
    internal_uart_xmit( mdbs_adu_xmit, len );
    
    xQueueReset( que_mdbs_clnt_hndl );

    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );
    internal_uart_recv( mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );

    received        =   xQueueReceive( que_mdbs_clnt_hndl, &queue_data, pdMS_TO_TICKS(500)  );
    sync = queue_data.tag;

    if( received == pdFALSE )
    {
        return( -1 );
    }

    //if( queue_data.tag == OS_USER_TAG_UART4_RECV_IDLE )
    //{
    //    osDelay( 10 );
    //
    //    rem = stm32_uart4_dma_recv_remainder();
    //    cnt = MDBS_RTU_ADU_SIZEOF - rem;
    //    //SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_adu_recv, cnt );
    //    len = mdbs_rtu_xfer(    dev_addr,
    //                            mdbs_adu_recv,
    //                            MDBS_RTU_ADU_SIZEOF - rem,
    //                            mdbs_raw );
    //
    //    //APP_TRACE( "mdbs_rtu_xfer(), len = %d\n", len );
    //
    //    if( len != MDBS_ERR_NONE )
    //    {
    //            return( -2 );
    //    }
    //}

    return( 0 );
}
static char chardig (char v)
{
  v-='0';
  if(v>41) return v-39;
  if(v>9) return v-7;
  return v;
}

static char LRC (char *str)
{
  char val=0;
  while (*str)
  {
    val+=(chardig(*str)<<4)|chardig(*(str+1));
    str+=2;
  }
  return (char)(-((signed char)val));
}

static
int
modbus_ascii_client_read( void )
{
                BaseType_t      rcvd;
                app_pipe_t result;
                result.tag = 0;
                
    volatile    uint32_t        cnt;
    volatile    size_t          len;

#ifdef EKON_2025     
                char            str[]   = ":010300270002D3\r\n";
#else
                char            str[]   = ":010300000002FA\r\n";
#endif
                //":010300000002FA\r\n" :-cmd, 01-addr, 03-fun, 0000-PAdr, 0002-PNum, FA-CRC, \r\n - CRLF

                int             meas;
                int             temp;
                int             lrc;
                
                typedef union 
                {
                  float           d_f;
                  uint32_t        d_u;
                } data_t;               
                
                data_t          meas_t;
                data_t          temp_t;                
                
    //0x4146B9EC - raw O2 (float) (12.42039)
    //0x442b2ccd - temp 684.7

    //char test[]   = ":0103084146B9EC442B2CCDFC\r\n";
    //sscanf((char *) test, ":010308%08x%08x%02x\r\n", &meas_t.d_u, &temp_t.d_u, &lrc );   
    //meas = (int)(meas_t.d_f*1000);
    //temp = (int)(temp_t.d_f*10);  
    //            
    //dev.sens->meas.raw          = meas;
    //dev.sens->meas.raw_t        = temp_t.d_u;   
    //dev.sens->meas.raw_t_conv   = temp;                   
    //
    //memcpy(dev.sens->meas.get_str, test, 25);
                    
    //lrc = LRC("010300270002");                          

    strncpy( (char *) mdbs_adu_xmit, str, sizeof(str) );

    SCB_CleanDCache_by_Addr( (uint32_t *) mdbs_adu_xmit, MDBS_RTU_ADU_SIZEOF );
    internal_uart_xmit( mdbs_adu_xmit, sizeof(str) );

    xQueueReset( que_mdbs_clnt_hndl );

    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );
    internal_uart_recv( mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );

    rcvd = xQueueReceive( que_mdbs_clnt_hndl, &result, pdMS_TO_TICKS(1000) );

    if( rcvd == pdFALSE )
    {
        return( -1 );
    }

    if( result.tag == OS_USER_TAG_UART4_RECV_TOUT )
    {
        cnt = MDBS_RTU_ADU_SIZEOF - internal_uart_rx_get_ndtr();
        //TRACE( "rem: %d\n", cnt );
        //len = mdbs_rtu_xfer( dev_addr, mdbs_adu_recv, cnt, reg16 );

        //for( int i = 0; i < cnt; i++ )
        //{
        //    //TRACE( "%02X", *(mdbs_adu_recv + i));
        //    TRACE( "%c", *(mdbs_adu_recv + i));
        //}
        //TRACE( "\n" );

#ifdef EKON_2025
         sscanf((char *) mdbs_adu_recv, ":010308%08x%08x%02x\r\n", &temp_t.d_u, &meas_t.d_u, &lrc );   
         meas = (int)(meas_t.d_f*1000);
         temp = (int)(temp_t.d_f*10);          
#else
         sscanf( (char *) mdbs_adu_recv, ":010304%04x%04x%02x\r\n", &meas, &temp, &lrc );         
#endif        
            
        dev.sens->meas.raw          = meas;
        dev.sens->meas.raw_t        = temp;   
        dev.sens->meas.ppm.integral = econ_raw2ppm( dev.sens, meas );

        dev.sens->meas.digc.integral    = temp / 10;
        dev.sens->meas.digc.fractional  = temp % 10;
    }

    return( 0 );

}


/**
* @brief Send data to modbus UART and wait answer
* @param mdbs_adu - data
* @param adu_len - len
* @param recv_len - receive data len
*/
int modbus_send_receive_data (uint8_t *mdbs_send_buf, size_t adu_len, size_t *recv_len, uint8_t *mdbs_resv_buf)
{ 
    BaseType_t      rcvd;
    
    app_pipe_t result;
    result.tag = 0;
    
    volatile    uint32_t        cnt;
  
    xQueueReset( que_mdbs_clnt_hndl );
    
    SCB_CleanDCache_by_Addr( (uint32_t *) mdbs_send_buf, MDBS_RTU_ADU_SIZEOF );
    internal_uart_xmit( mdbs_send_buf, adu_len );
      
    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_resv_buf, MDBS_RTU_ADU_SIZEOF );
    internal_uart_recv( mdbs_resv_buf, MDBS_RTU_ADU_SIZEOF );
    
    rcvd = xQueueReceive( que_mdbs_clnt_hndl, &result, pdMS_TO_TICKS(500) );  
    
    if( rcvd == pdFALSE )
    {
        return( -1 );
    }
    
    if( result.tag == OS_USER_TAG_UART4_RECV_TOUT )
    {
        *recv_len = MDBS_RTU_ADU_SIZEOF - internal_uart_rx_get_ndtr();
    }

    return( 0 );      
}

/*******************************************************************************
* RELAY PROCESS
*******************************************************************************/
typedef enum 
{
  EXTERNAL_1 = 0x00,
  EXTERNAL_2,
  EXTERNAL_3,
  EXTERNAL_4,
  EXTERNAL_5,
  EXTERNAL_6,  
} Outputs_swithes_e;

static void send_cmd_for_relay_open (uint8_t relay_num)
{
    dev.ext_relay->link_err = relay_set_state(relay_num, false);
    set_ext_relay_status(dev.ext_relay->link_err);
}

static void send_cmd_for_relay_close (uint8_t relay_num)
{
  
    dev.ext_relay->link_err = relay_set_state(relay_num, true);
    set_ext_relay_status(dev.ext_relay->link_err);   
}

/* Controlling the status of external outputs */
static void set_external_switch_open (uint8_t external_num)
{
  switch (external_num) {
  case EXTERNAL_1:     send_cmd_for_relay_open(EXTERNAL_1);
    break;
  case EXTERNAL_2:     send_cmd_for_relay_open(EXTERNAL_2);
    break;
  case EXTERNAL_3:     send_cmd_for_relay_open(EXTERNAL_3);
    break;
  case EXTERNAL_4:     send_cmd_for_relay_open(EXTERNAL_4);
    break;
  case EXTERNAL_5:     send_cmd_for_relay_open(EXTERNAL_5);
    break;
  case EXTERNAL_6:     send_cmd_for_relay_open(EXTERNAL_6);
    break;    
  }
}

static void set_external_switch_close (uint8_t external_num)
{
  switch (external_num) {
  case EXTERNAL_1:      send_cmd_for_relay_close(EXTERNAL_1);
    break;
  case EXTERNAL_2:      send_cmd_for_relay_close(EXTERNAL_2);
    break;
  case EXTERNAL_3:      send_cmd_for_relay_close(EXTERNAL_3);
    break;
  case EXTERNAL_4:      send_cmd_for_relay_close(EXTERNAL_4);
    break;
  case EXTERNAL_5:      send_cmd_for_relay_close(EXTERNAL_5);
    break;
  case EXTERNAL_6:      send_cmd_for_relay_close(EXTERNAL_6);
    break;    
  }
}


static void relay_threshold_process(uint8_t relay_num)
{
  static relay_position_e previous_position[6] = {UNKNOWN_POSITION, UNKNOWN_POSITION, UNKNOWN_POSITION, UNKNOWN_POSITION, UNKNOWN_POSITION, UNKNOWN_POSITION};
  
  int32_t     meas_ppm = dev.sens->meas.ppm.integral;
  int32_t     thres_ppm =  (int32_t)dev.ext_relay->relay[relay_num].ppm.ppm_f;
  int32_t     hyst_ppm = (int32_t)dev.ext_relay->relay[relay_num].hyst_ppm.ppm_f;
  
  
  switch (dev.ext_relay->relay[relay_num].thld_type)
  {
  case LOW_LEVEL_THLD_TYPE:
      if (meas_ppm < thres_ppm - hyst_ppm)
      {
        if (dev.ext_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {
            previous_position[relay_num] = SWITCH_OPEN;
            set_external_switch_open(relay_num);
          }
        }
        else 
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close(relay_num);
          }
        }
      }
      
      if (meas_ppm > thres_ppm + hyst_ppm)
      {
        if (dev.ext_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close(relay_num);
          }
        }
        else
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {
            previous_position[relay_num] = SWITCH_OPEN;          
            set_external_switch_open(relay_num);
          }
        }
      }
    break;
    
  case HI_LEVEL_THLD_TYPE:
      if (meas_ppm > thres_ppm + hyst_ppm)
      {
        if (dev.ext_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {
            previous_position[relay_num] = SWITCH_OPEN; 
            set_external_switch_open(relay_num);
          }
        }
        else
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {          
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close(relay_num);
          }
        }
      }
    
      if (meas_ppm < thres_ppm - hyst_ppm)
      {        
        if (dev.ext_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {           
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close(relay_num);
          }
        }
        else
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {          
            previous_position[relay_num] = SWITCH_OPEN; 
            set_external_switch_open(relay_num);
          }
        }
      }
    break;
  }
}

static void relay_alarm_process(uint8_t relay_num)
{
  static uint32_t previous_warning_status[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  static uint32_t previous_error_status[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  static uint32_t previous_state[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  
  static uint8_t power_on_flag[6] = {1,1,1,1,1,1};
  
  if (power_on_flag[relay_num])
  {
    previous_error_status[relay_num] = ~dev.state.error_status;
    previous_warning_status[relay_num] = ~dev.state.warnings_status;
    power_on_flag[relay_num] = 0;
    previous_state[relay_num] = dev.ext_relay->relay[relay_num].relay_state;
  }
  
  if ((previous_error_status[relay_num] != dev.state.error_status) || (previous_warning_status[relay_num] != dev.state.warnings_status) || previous_state[relay_num] != dev.ext_relay->relay[relay_num].relay_state)
  {  
    switch (dev.ext_relay->relay[relay_num].relay_state) {
    case NORMAL_OPEN_STATE:
        // 1. Check alarm or not
        // 2. set state
        if ((dev.state.warnings_status != 0) || (dev.state.error_status != 0))
          set_external_switch_close(relay_num); 
        else
          set_external_switch_open(relay_num); // no error
      break;
      
    case NORMAL_CLOSE_STATE:
        if ((dev.state.warnings_status != 0) || (dev.state.error_status != 0))
          set_external_switch_open(relay_num); 
        else 
          set_external_switch_close(relay_num); // no error
      break;
    }
  }
  
  previous_warning_status[relay_num] = dev.state.warnings_status;
  previous_error_status[relay_num] = dev.state.error_status;
  previous_state[relay_num] = dev.ext_relay->relay[relay_num].relay_state;

}

static void relay_not_active_process(uint8_t relay_num)
{
  static relay_state_e previous_state[6] = {UNKNOWN_STATE, UNKNOWN_STATE, UNKNOWN_STATE, UNKNOWN_STATE, UNKNOWN_STATE, UNKNOWN_STATE};
  
  if (previous_state[relay_num] != dev.ext_relay->relay[relay_num].relay_state)
  {  
    switch (dev.ext_relay->relay[relay_num].relay_state) {
    case NORMAL_OPEN_STATE:
        set_external_switch_open(relay_num); 
      break;
      
    case NORMAL_CLOSE_STATE:
        set_external_switch_close(relay_num); 
      break;
    }
  }
  
  previous_state[relay_num] = dev.ext_relay->relay[relay_num].relay_state;
  
}

static void relay_process (uint8_t relay_num)
{
  // Relay mode
  switch (dev.ext_relay->relay[relay_num].relay_mode) {
  case THRESHOLD_MODE: relay_threshold_process(relay_num);
      break;
      
  case ERROR_MODE: relay_alarm_process(relay_num);
      break;
      
  case NOT_ACTIVE_MODE: relay_not_active_process (relay_num);
      break;
      
  default:
      break;
  }
  
}

static void relays_process(void)
{
  // Relay 0
  relay_process(0);
    
  // Relay 1
  relay_process(1);  
}

static 
int
relay_init(void)
{ 
  return relay_get_info ();  
}

static uint32_t get_dev_errors_status (void)
{
  return dev.state.error_status;
}

static uint32_t get_dev_warnings_status (void)
{
  return dev.state.warnings_status;  
}

static void set_sensor_status (int status)
{ 
  status == 0 ? (dev.state.error_status &=~ SENSON_LINK_ERR) : (dev.state.error_status |= SENSON_LINK_ERR);
}

static void set_cloop_status (int status)
{
  status == 0 ? (dev.state.error_status &=~ CLOOP_LINK_ERR) : (dev.state.error_status |= CLOOP_LINK_ERR);
}

static void set_ext_relay_status (int status)
{
  
#ifndef NO_RELAY  
  status == 0 ? (dev.state.error_status &=~ EXT_RELAY_LINK_ERR) : (dev.state.error_status |= EXT_RELAY_LINK_ERR);
#else
  dev.state.error_status &=~ EXT_RELAY_LINK_ERR; 
#endif
    
}

/*******************************************************************************
* PRIVATE FUNCTIONS - SENSOR
*******************************************************************************/
/*
static
int
sensor_init(                                    sens_t *            p )
{
    int     err;


    err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0000, 16, dev.sens->info.raw );
    TRACE("%d\t", err); for(int i = 0; i < 16; i++){TRACE("%04X", *(dev.sens->info.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0200, 16, dev.sens->conf.raw );
    TRACE("%d\t", err); for(int i = 0; i < 16; i++){TRACE("%04X", *(dev.sens->conf.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0C00, 16, dev.sens->cal.zero.raw );
    TRACE("%d\t", err); for(int i = 0; i < 16; i++){TRACE("%04X", *(dev.sens->cal.zero.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0C10, 16, dev.sens->cal.span.raw );
    TRACE("%d\t", err); for(int i = 0; i < 16; i++){TRACE("%04X", *(dev.sens->cal.span.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0D00, 16, dev.sens->cal_back.zero.raw );
    TRACE("%d\t", err); for(int i = 0; i < 16; i++){TRACE("%04X", *(dev.sens->cal_back.zero.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0D10, 16, dev.sens->cal_back.span.raw );
    TRACE("%d\t", err); for(int i = 0; i < 16; i++){TRACE("%04X", *(dev.sens->cal_back.span.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0C80, 16, dev.sens->cal_factory.zero.raw );
    TRACE("%d\t", err); for(int i = 0; i < 16; i++){TRACE("%04X", *(dev.sens->cal_factory.zero.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    return( err );
}
*/

static
int
cl420_ch1_update(                       dev_cl420_t *   p,
                                        econ_t *        sens )
{
    int         err = 0;
    int32_t     ppm         = sens->meas.ppm.integral;
    uint32_t    range_ppm   = p->range[ p->range_idx ].ppm;
    if (ppm < 0) ppm = 0;

    p->range[ p->range_idx ].mb_output.uA   = 4000 + (16000 * ppm) / range_ppm;
    
    if (p->range[ p->range_idx ].mb_output.uA < MIN_LOW_CURRENT_LEVEL) p->range[ p->range_idx ].mb_output.uA = MIN_LOW_CURRENT_LEVEL; 
  
    if (p->range[ p->range_idx ].mb_output.uA > MAX_HI_CURRENT_LEVEL) p->range[ p->range_idx ].mb_output.uA = MAX_HI_CURRENT_LEVEL;     
    
    if (dev.state.error_status)
    {
      if (dev.cloop->error_level == LOW_ERROR_LEVEL)
        p->range[ p->range_idx ].mb_output.uA = LOW_ERROR_CURRENT_LEVEL;    
      else
        p->range[ p->range_idx ].mb_output.uA = HI_ERROR_CURRENT_LEVEL;
    }     
    
    if (dev.cloop->cloop_state != CLOOP_FREEZ)
    {
      err = cloop_set_uA(1, p->range[ p->range_idx ].mb_output.uA);
      if( err )
      {
          return( err );
      }
    }

    return( 0 );
}


static
int
cl420_ch2_update(                       dev_cl420_t *   p )
{
    cl420_output_t  out;
    int         err;

    switch( p->range_idx )
    {
        case DEV_RANGE_IDX_R3:
            out.uA  = 20000;
            break;
        case DEV_RANGE_IDX_R2:
            out.uA  = 12000;
            break;
        case DEV_RANGE_IDX_R1:
        default:
            out.uA  = 4000;
            break;
    }

    err = cloop_set_uA(2, out.uA);
    if( err )
    {
        return( err );
    }

    return( 0 );
}

static 
int
cloop_set_cal_4ma (uint8_t channel)
{
    int     err;

    err = set_cal_4ma (channel);
   
    return( err );
}

static 
int
cloop_set_cal_20ma (uint8_t channel)
{
    int     err;

    err = set_cal_20ma(channel);
   
    return( err );  
}

static 
int
cloop_set_raw_4mA (uint8_t channel)
{
    int     err;
    
    if (channel == 1)
      err = cloop_set_raw(1, dev.cloop->cal_ch1[0].raw.u16[0]);    
    else 
      err = cloop_set_raw(2, dev.cloop->cal_ch2[0].raw.u16[0]);      
   
    return( err );  
}

static 
int
cloop_set_raw_20mA (uint8_t channel)
{ 
    int     err;
    
    if (channel == 1)
      err = cloop_set_raw(1, dev.cloop->cal_ch1[1].raw.u16[0]);
    else
      err = cloop_set_raw(2, dev.cloop->cal_ch2[1].raw.u16[0]);      
                  
    return( err );    
}

static 
int
cloop_init (void)
{
  int err = 0;
  uint8_t need_write_range = 0;
  
  err = cloop_get_info();
  if( err == 0) err = cloop_get_cal();
  
  if (err == 0) err = cloop_get_range();
  
  if (err == 0)
  {
    if ((dev.cl420.range[0].ppm <= 0) || (dev.cl420.range[0].ppm > 999999))
    {
      need_write_range = 1;
      dev.cl420.range[0].ppm = range_r0_value_default;
    }
    if ((dev.cl420.range[1].ppm <= 0) || (dev.cl420.range[1].ppm > 999999))
    {
      need_write_range = 1;
      dev.cl420.range[1].ppm = range_r1_value_default;      
    }
    if ((dev.cl420.range[2].ppm <= 0) || (dev.cl420.range[2].ppm > 999999))
    {
      need_write_range = 1;
      dev.cl420.range[2].ppm = range_r2_value_default;      
    }
    if (dev.cl420.range_idx > 2)
    {
      dev.cl420.range_idx = range_idx_default;
      need_write_range = 1;
    }
    for (int i = 0; i < 3; i++)
    {
      if(dev.cl420.range[i].units >= CL420_UNITS_MAX)
      {
        dev.cl420.range[i].units = range_units_default;
        need_write_range = 1;
      }
    }
    
    if (need_write_range) 
      err = cloop_set_range();
  }
    
  return err;
}

/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
void
task_ibus_sens_conf_lpf_update( const   uint16_t        cutoff,
                                const   uint16_t        order )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_SENS_CONF_FILTER;

    //dev.sens->conf.lpf_cutoff   = cutoff;
    //dev.sens->conf.lpf_order    = order;
    xQueueSend( que_ibus_hndl, &result, NULL );
}

void
task_ibus_boards_init( void )
{
    BaseType_t result;
    app_pipe_t queue_data;
    queue_data.tag    = OS_USER_TAG_IBUS_BOARDS_INIT;   
    
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }    
}

void
task_ibus_cl420_ch2_update( void )
{
    BaseType_t result;
    app_pipe_t queue_data;
    queue_data.tag    = OS_USER_TAG_IBUS_CL420_CH2_UPDATE;   
    
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );  
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }        
}

void send_cmd_for_cloop_set_raw_4mA (uint8_t channel)
{
    BaseType_t result;
    app_pipe_t queue_data;
    if (channel == 1)
      queue_data.tag    = OS_USER_TAG_CAL_CLOOP_CHANNEL1_SET_RAW_4MA;        
    else
      queue_data.tag    = OS_USER_TAG_CAL_CLOOP_CHANNEL2_SET_RAW_4MA;
      
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
 
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }        
    
    osDelay(3);   
}

void send_cmd_for_cloop_set_raw_20mA (uint8_t channel)
{
    BaseType_t result;  
    app_pipe_t queue_data;
    if (channel == 1)
      queue_data.tag    = OS_USER_TAG_CAL_CLOOP_CHANNEL1_SET_RAW_20MA;          
    else
      queue_data.tag    = OS_USER_TAG_CAL_CLOOP_CHANNEL2_SET_RAW_20MA;
    
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );

    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }            
    
    osDelay(3);   
}

void send_cmd_for_cloop_4mA_write (uint8_t channel)
{
    BaseType_t result;  
    app_pipe_t queue_data;  
    if (channel == 1)
      queue_data.tag      = OS_USER_TAG_CAL_CLOOP_CHANNEL1_4MA_WRITE;          
    else 
      queue_data.tag      = OS_USER_TAG_CAL_CLOOP_CHANNEL2_4MA_WRITE;
    
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
 
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }                
    
    osDelay(3);   
}

void send_cmd_for_cloop_20mA_write (uint8_t channel)
{
    BaseType_t result;  
    app_pipe_t queue_data;  
    if (channel == 1)
      queue_data.tag      = OS_USER_TAG_CAL_CLOOP_CHANNEL1_20MA_WRITE;          
    else
      queue_data.tag      = OS_USER_TAG_CAL_CLOOP_CHANNEL2_20MA_WRITE;
    
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
 
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }                
    
    osDelay(3);       
}

void send_cmd_for_cloop_write_range (void)
{
    BaseType_t result;  
    app_pipe_t queue_data;  
    queue_data.tag      = OS_USER_TAG_CAL_CLOOP_SET_RANGE;          
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
 
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }                
    
    osDelay(3);           
}

void send_cmd_for_nvm_write_param (const uint32_t param_id, const uint32_t param_value)
{
#ifdef EXT_FLASH
    BaseType_t result;  
    app_pipe_t queue_data;  
    queue_data.tag      = OS_USER_TAG_WRITE_PARAM_TO_EXT_FLASH; 
    queue_data.param_id = param_id;
    queue_data.param_data = param_value;
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
 
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }                
    
    osDelay(3);              
#else
    dev.nvm.put( param_id, param_value);    
#endif  
}

/*******************************************************************************
*
*******************************************************************************/
static
void
vTimerCallback1SecExpired(              TimerHandle_t           xTimer )
{
    app_pipe_t result;
    result.tag  = OS_USER_TAG_TMR_1SEC;
    xQueueSend( que_ibus_hndl, &result, NULL );
}

#ifdef EXT_FLASH

static void write_data_to_ext_flash(const uint32_t param_id, const uint32_t param_value)
{
  flash_write_param(param_id, param_value);
}

#endif

/*******************************************************************************
*
*******************************************************************************/
void
task_ibus(                      const   void *          argument )
{
    app_pipe_t      queue_data;
    os_user_tag_t   tag;
    uint32_t previous_error_status = 0;    
    uint32_t previous_warning_status = 0;    
    uint32_t error_status = 0;
    uint32_t warning_status = 0;  
    uint8_t power_on_flag = 1;
    uint16_t        u16;
    econ_t *        sens        = dev.sens;
    dev_cl420_t *   dev_cl420   = &(dev.cl420);

    dev_init( &dev );
    
    (void) argument;

    tmr_1sec_hndl   = xTimerCreateStatic(   "TMR1SEC",                      //const char * const pcTimerName,
                                            pdMS_TO_TICKS( 1000 ),          //const TickType_t xTimerPeriod,
                                            pdTRUE,                         //const UBaseType_t uxAutoReload,
                                            ( void * ) 0,                   //void * const pvTimerID,
                                            vTimerCallback1SecExpired,      //TimerCallbackFunction_t pxCallbackFunction
                                            &tmr_1sec_alloc                  //StaticTimer_t *pxTimerBuffer
                                            );
    xTimerStart( tmr_1sec_hndl, 0 );

    osDelay( 250 );

    internal_uart_init( 9600, 8, 1.0, 'N' );  

    sens->link_err      = 1;
    set_sensor_status(dev.sens->link_err);
    sens->meas.ppm.integral   = 0;
    sens->meas.ppm.fractional = 0;
    
    dev.cloop->link_err = 1;
    set_cloop_status(dev.cloop->link_err);
    dev.ext_relay->link_err = 1;
    set_ext_relay_status(dev.ext_relay->link_err);    
    
    cloop_hw_init();
    relay_hw_init();
    task_ibus_boards_init();

    while( true )
    {
        xQueueReceive( que_ibus_hndl, &queue_data, portMAX_DELAY  );
        tag = queue_data.tag;
        
        previous_error_status = get_dev_errors_status(); 
        previous_warning_status = get_dev_warnings_status();

        switch( tag )
        {
            case OS_USER_TAG_TMR_1SEC:
                    internal_uart_init( 9600, 8, 1.0, 'N' );
                    sens->link_err  = modbus_ascii_client_read();
                    osDelay( 150 );
                    sens->link_err  = modbus_ascii_client_read();
                    set_sensor_status(dev.sens->link_err);
                    osDelay( 150 );
                    
                    internal_uart_init(19200, 8, 1.0, 'N');                    
                    dev.cloop->link_err = cl420_ch1_update( dev_cl420, sens );
                    set_cloop_status(dev.cloop->link_err);       
                    
                    queue_data.tag = OS_USER_TAG_IBUS_REFRESH;
                    xQueueSend( que_hmi_hndl, &queue_data, NULL );                                           
                break;
                
            case OS_USER_TAG_IBUS_CL420_CH2_UPDATE:
                internal_uart_init(19200, 8, 1.0, 'N');
                dev.cloop->link_err = cl420_ch2_update( dev_cl420 );
                set_cloop_status(dev.cloop->link_err); 
                break;
                
            case OS_USER_TAG_IBUS_BOARDS_INIT:
                //dev.sens->link_err  = sensor_init( sens );      
                //set_sensor_status(dev.sens->link_err);
                internal_uart_init(19200, 8, 1.0, 'N');
                dev.cloop->link_err = cloop_init();
                set_cloop_status(dev.cloop->link_err); 
                dev.ext_relay->link_err = relay_init();
                set_ext_relay_status(dev.ext_relay->link_err); 
                task_ibus_cl420_ch2_update();
                break;                

            case OS_USER_TAG_SENS_CONF_FILTER:
                //dev.sens->link_err  = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0200, 16, dev.sens->conf.raw );
                //set_sensor_status(dev.sens->link_err); 
                //if( dev.sens->link_err )
                //    break;
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0200, 16, dev.sens->conf.raw );
                //set_sensor_status(dev.sens->link_err);
                break;

            case OS_USER_TAG_CAL0_UPDATE:
                //dev.sens->link_err = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0C00, 16, dev.sens->cal.zero.raw );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_ZERO_CALIBRATION);
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0D00, 16, dev.sens->cal_back.zero.raw );
                //set_sensor_status(dev.sens->link_err);
                dev.sens->cal.zero.raw.i32  = dev.sens->meas.raw;
                dev_zero_update( &dev );
                log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_ZERO_CALIBRATION);                
                break;

            case OS_USER_TAG_CAL1_UPDATE:
                //dev.sens->link_err = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0C10, 16, dev.sens->cal.span.raw );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_SPAN_CALIBRATION); 
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0D10, 16, dev.sens->cal_back.span.raw );
                //set_sensor_status(dev.sens->link_err);
                dev.sens->cal.span.raw.i32  = dev.sens->meas.raw;
                dev_span_update( &dev );              
                log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_SPAN_CALIBRATION);                 
                break;

            case OS_USER_TAG_CAL1_FACTORY_UPDATE:
                //dev.sens->link_err = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0E10, 16, dev.sens->cal_factory.span.raw );
                //if( dev.sens->link_err )
                //    break;
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0E10, 16, dev.sens->cal_factory.span.raw );
                break;

            case OS_USER_TAG_CAL0_FACTORY_UPDATE:
                //dev.sens->link_err = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0C80, 16, dev.sens->cal_factory.zero.raw );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_ZERO_FACTORY_UPDATE);
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0C80, 16, dev.sens->cal_factory.zero.raw );
                //set_sensor_status(dev.sens->link_err);
                break;

            case OS_USER_TAG_CAL0_RESTORE:
                //u16 = 0xACDC;
                //dev.sens->link_err = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0D0F, 1, &u16 );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_ZERO_CALIBRATION_RESTORE);
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0C00, 16, dev.sens->cal.zero.raw );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0D00, 16, dev.sens->cal_back.zero.raw );
                //set_sensor_status(dev.sens->link_err);
                dev_zero_restore(&dev, 0xACDC);
                log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_ZERO_CALIBRATION_RESTORE);                
                break;

            case OS_USER_TAG_CAL1_RESTORE:
                //u16 = 0xACDC;
                //dev.sens->link_err = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0D1F, 1, &u16 );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_SPAN_CALIBRATION_RESTORE);
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0C10, 16, dev.sens->cal.span.raw );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0D10, 16, dev.sens->cal_back.span.raw );
                //set_sensor_status(dev.sens->link_err);
                dev_span_restore(&dev, 0xACDC);
                log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_SPAN_CALIBRATION_RESTORE);                
                break;

            case OS_USER_TAG_CAL0_FACTORY_RESTORE:
                //u16 = 0xACDC;
                //dev.sens->link_err = modbus_client_write( CFG_MDBS_DEV_ADDR_SENS, 0x0D8F, 1, &u16 );
                //set_sensor_status(dev.sens->link_err);
                //if( dev.sens->link_err )
                //    break;
                //log_write_event(&dev.log, LOG_SOURCE_SENSOR, LOG_SENSOR_EVENT_ZERO_FACTORY_RESTORE);
                //dev.sens->link_err = modbus_client_read( CFG_MDBS_DEV_ADDR_SENS, 0x0C00, 16, dev.sens->cal.zero.raw );
                //set_sensor_status(dev.sens->link_err);
                break;                      
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL1_4MA_WRITE:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_cal_4ma(1);
              if (dev.cloop->link_err == 0)
                log_write_event(&dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_CLOOP_CALIBRATION_4MA);
              set_cloop_status(dev.cloop->link_err);                 
              break;
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL2_4MA_WRITE:
              dev.cloop->link_err = cloop_set_cal_4ma(2);
              if (dev.cloop->link_err == 0)
                log_write_event(&dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_CLOOP_CALIBRATION_4MA);
              set_cloop_status(dev.cloop->link_err);                 
              break;              
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL1_20MA_WRITE:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_cal_20ma(1);
              if (dev.cloop->link_err == 0)
                log_write_event(&dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_CLOOP_CALIBRATION_20MA);              
              set_cloop_status(dev.cloop->link_err);                 
              break;
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL2_20MA_WRITE:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_cal_20ma(2);
              if (dev.cloop->link_err == 0)
                log_write_event(&dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_CLOOP_CALIBRATION_20MA);              
              set_cloop_status(dev.cloop->link_err);                 
              break;              
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL1_SET_RAW_4MA:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_raw_4mA(1);
              set_cloop_status(dev.cloop->link_err);                 
              break;
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL2_SET_RAW_4MA:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_raw_4mA(2);
              set_cloop_status(dev.cloop->link_err);                 
              break;              
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL1_SET_RAW_20MA:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_raw_20mA(1);
              set_cloop_status(dev.cloop->link_err);                 
              break;    
              
            case OS_USER_TAG_CAL_CLOOP_CHANNEL2_SET_RAW_20MA:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_raw_20mA(2);
              set_cloop_status(dev.cloop->link_err);                 
              break;                  
              
            case OS_USER_TAG_CAL_CLOOP_SET_RANGE:
              internal_uart_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_range();  
              set_cloop_status(dev.cloop->link_err);
              break;   
              
#ifdef EXT_FLASH
            case OS_USER_TAG_WRITE_PARAM_TO_EXT_FLASH:
              write_data_to_ext_flash(queue_data.param_id, queue_data.param_data);
              break;
#endif              

            default:
                break;
        }
        
        internal_uart_init(19200, 8, 1.0, 'N');
        relays_process();
        
        error_status = get_dev_errors_status();
        warning_status = get_dev_warnings_status();
        
        if (power_on_flag)
        {
          previous_error_status = ~error_status;
          previous_warning_status = ~warning_status;
          power_on_flag = 0;
        }
        
        if ((previous_error_status != error_status) || (previous_warning_status != warning_status))
          dev_find_and_write_error_warnings(&dev.log, previous_error_status, error_status, previous_warning_status, warning_status);        
    }
}
