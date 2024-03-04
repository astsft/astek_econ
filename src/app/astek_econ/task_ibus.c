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
static  uint16_t    mdbs_sens_raw[ MDBS_RTU_ADU_SIZEOF / 2];

#pragma data_alignment = 32
static  uint8_t     mdbs_adu_xmit[ MDBS_RTU_ADU_SIZEOF ];

#pragma data_alignment = 32
static  uint8_t     mdbs_adu_recv[ MDBS_RTU_ADU_SIZEOF ];


static int mdb_relay1_set_close(void);
static int mdb_relay2_set_close(void);
static int mdb_relay1_set_open(void);
static int mdb_relay2_set_open(void);
static void set_mdb_relay_status (int status);

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
    stm32_uart4_xmit_dma( mdbs_adu_xmit, cnt );

    xQueueReset( que_mdbs_clnt_hndl );

    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );
    stm32_uart4_recv_dma( mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );

    rcvd = xQueueReceive( que_mdbs_clnt_hndl, &queue_data, pdMS_TO_TICKS(500) );
    tag = queue_data.tag;

    if( rcvd == pdFALSE )
    {
        return( -1 );
    }

    if( tag == OS_USER_TAG_UART4_RECV_TOUT )
    {
        cnt = MDBS_RTU_ADU_SIZEOF - stm32_uart4_dma_recv_remainder();
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

    volatile    size_t          len;
    volatile    uint32_t        rem;
    volatile    uint32_t        cnt;
    app_pipe_t queue_data;


    len = modbus_rtu_client_write_multiple_registers( dev_addr, mdbs_adu_xmit, reg_addr, regs_cnt, mdbs_raw );

    SCB_CleanDCache_by_Addr( (uint32_t *) mdbs_adu_xmit, MDBS_RTU_ADU_SIZEOF );
    stm32_uart4_xmit_dma( mdbs_adu_xmit, len );
    
    xQueueReset( que_mdbs_clnt_hndl );

    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );
    stm32_uart4_recv_dma( mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );

    received        =   xQueueReceive( que_mdbs_clnt_hndl, &queue_data, pdMS_TO_TICKS(500)  );

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

static
int
modbus_ascii_client_read( void )
{
                BaseType_t      rcvd;
                app_pipe_t result;
                result.tag = 0;
                
    volatile    uint32_t        cnt;
    volatile    size_t          len;

                char            str[]   = ":010300000002FA\r\n";

                int             meas;
                int             temp;
                int             lrc;


    strncpy( (char *) mdbs_adu_xmit, str, sizeof(str) );

    SCB_CleanDCache_by_Addr( (uint32_t *) mdbs_adu_xmit, MDBS_RTU_ADU_SIZEOF );
    stm32_uart4_xmit_dma( mdbs_adu_xmit, sizeof(str) );

    xQueueReset( que_mdbs_clnt_hndl );

    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );
    stm32_uart4_recv_dma( mdbs_adu_recv, MDBS_RTU_ADU_SIZEOF );

    rcvd = xQueueReceive( que_mdbs_clnt_hndl, &result, pdMS_TO_TICKS(1000) );

    if( rcvd == pdFALSE )
    {
        return( -1 );
    }

    if( result.tag == OS_USER_TAG_UART4_RECV_TOUT )
    {
        cnt = MDBS_RTU_ADU_SIZEOF - stm32_uart4_dma_recv_remainder();
        //TRACE( "rem: %d\n", cnt );
        //len = mdbs_rtu_xfer( dev_addr, mdbs_adu_recv, cnt, reg16 );

        //for( int i = 0; i < cnt; i++ )
        //{
        //    //TRACE( "%02X", *(mdbs_adu_recv + i));
        //    TRACE( "%c", *(mdbs_adu_recv + i));
        //}
        //TRACE( "\n" );

        sscanf( (char *) mdbs_adu_recv, ":010304%04x%04x%02x\r\n", &meas, &temp, &lrc );

        //uint32_t    ppm = meas * 10;
        //TRACE( "meas: %d\ttemp: %d\tlrc: %02X\tppm: %d\n", meas, temp, lrc, ppm );

        dev.sens->meas.raw          = meas;
        dev.sens->meas.ppm.integral = econ_raw2ppm( dev.sens, meas );

        dev.sens->meas.digc.integral    = temp / 10;
        dev.sens->meas.digc.fractional  = temp % 10;


        //if( len != MDBS_ERR_NONE )
        //{
        //    return( -3 );
        //}
    }

    return( 0 );

}


/**
* @brief Send data to modbus UART and wait answer
* @param mdbs_adu - data
* @param adu_len - len
* @param recv_len - receive data len
*/
static int modbus_send_receive_data (uint8_t *mdbs_send_buf, size_t adu_len, size_t *recv_len, uint8_t *mdbs_resv_buf)
{ 
    BaseType_t      rcvd;
    
    app_pipe_t result;
    result.tag = 0;
    
    volatile    uint32_t        cnt;
  
    xQueueReset( que_mdbs_clnt_hndl );
    
    SCB_CleanDCache_by_Addr( (uint32_t *) mdbs_send_buf, MDBS_RTU_ADU_SIZEOF );
    stm32_uart4_xmit_dma( mdbs_send_buf, adu_len );
      
    SCB_InvalidateDCache_by_Addr( (uint32_t *) mdbs_resv_buf, MDBS_RTU_ADU_SIZEOF );
    stm32_uart4_recv_dma( mdbs_resv_buf, MDBS_RTU_ADU_SIZEOF );
    
    rcvd = xQueueReceive( que_mdbs_clnt_hndl, &result, pdMS_TO_TICKS(500) );  
    
    if( rcvd == pdFALSE )
    {
        return( -1 );
    }
    
    if( result.tag == OS_USER_TAG_UART4_RECV_TOUT )
    {
        *recv_len = MDBS_RTU_ADU_SIZEOF - stm32_uart4_dma_recv_remainder();
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
  PURGE,
  PROBE
} Outputs_swithes_e;

static void send_cmd_for_mdb_relay1_open (void)
{
    //app_pipe_t result;
    //result.tag    = OS_USER_TAG_RELAY1_OPEN;          
    //xQueueSendToFront( que_ibus_hndl, &result, NULL );
    //
    //osDelay(1);   

    dev.mdb_relay->link_err = mdb_relay1_set_open();
    set_mdb_relay_status(dev.mdb_relay->link_err);
}

static void send_cmd_for_mdb_relay2_open (void)
{
    //app_pipe_t result;
    //result.tag    = OS_USER_TAG_RELAY2_OPEN;          
    //xQueueSendToFront( que_ibus_hndl, &result, NULL );
    //
    //osDelay(1);   
    
    dev.mdb_relay->link_err = mdb_relay2_set_open();
    set_mdb_relay_status(dev.mdb_relay->link_err);  
}

static void send_cmd_for_mdb_relay1_close (void)
{
    //app_pipe_t result;
    //result.tag    = OS_USER_TAG_RELAY1_CLOSE;          
    //xQueueSendToFront( que_ibus_hndl, &result, NULL );
    //
    //osDelay(1);   
  
    dev.mdb_relay->link_err = mdb_relay1_set_close();
    set_mdb_relay_status(dev.mdb_relay->link_err);   
}

static void send_cmd_for_mdb_relay2_close (void)
{
    //app_pipe_t result;
    //result.tag    = OS_USER_TAG_RELAY2_CLOSE;          
    //xQueueSendToFront( que_ibus_hndl, &result, NULL );
    //
    //osDelay(1);   
  
    dev.mdb_relay->link_err = mdb_relay2_set_close();
    set_mdb_relay_status(dev.mdb_relay->link_err);
}

/* Controlling the status of external outputs */
static void set_external_switch_open (uint8_t external_num)
{
  switch (external_num) {
  case EXTERNAL_1:     send_cmd_for_mdb_relay1_open();
    break;
  case EXTERNAL_2:     send_cmd_for_mdb_relay2_open();
    break;
  }
}

static void set_external_switch_close (uint8_t external_num)
{
  switch (external_num) {
  case EXTERNAL_1:      send_cmd_for_mdb_relay1_close();
    break;
  case EXTERNAL_2:      send_cmd_for_mdb_relay2_close();
    break;
  }
}


static void relay_threshold_process(uint8_t relay_num)
{
  static relay_position_e previous_position[2] = {UNKNOWN_POSITION, UNKNOWN_POSITION};
  
  int32_t     meas_ppm = dev.sens->meas.ppm.integral;
  int32_t     thres_ppm =  (int32_t)dev.mdb_relay->relay[relay_num].ppm.ppm_f;
  int32_t     hyst_ppm = (int32_t)dev.mdb_relay->relay[relay_num].hyst_ppm.ppm_f;
  
  
  switch (dev.mdb_relay->relay[relay_num].thld_type)
  {
  case LOW_LEVEL_THLD_TYPE:
      if (meas_ppm < thres_ppm - hyst_ppm)
      {
        if (dev.mdb_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {
            previous_position[relay_num] = SWITCH_OPEN;
            set_external_switch_open((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
        else 
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
      }
      
      if (meas_ppm > thres_ppm + hyst_ppm)
      {
        if (dev.mdb_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
        else
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {
            previous_position[relay_num] = SWITCH_OPEN;          
            set_external_switch_open((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
      }
    break;
    
  case HI_LEVEL_THLD_TYPE:
      if (meas_ppm > thres_ppm + hyst_ppm)
      {
        if (dev.mdb_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {
            previous_position[relay_num] = SWITCH_OPEN; 
            set_external_switch_open((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
        else
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {          
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
      }
    
      if (meas_ppm < thres_ppm - hyst_ppm)
      {        
        if (dev.mdb_relay->relay[relay_num].relay_state == NORMAL_OPEN_STATE)
        {
          if (previous_position[relay_num] != SWITCH_CLOSE)
          {           
            previous_position[relay_num] = SWITCH_CLOSE;
            set_external_switch_close((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
        else
        {
          if (previous_position[relay_num] != SWITCH_OPEN)
          {          
            previous_position[relay_num] = SWITCH_OPEN; 
            set_external_switch_open((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2);
          }
        }
      }
    break;
  }
}

static void relay_alarm_process(uint8_t relay_num)
{
  static uint32_t previous_warning_status[2] = {0x00, 0x00};
  static uint32_t previous_error_status[2] = {0x00, 0x00};
  static uint8_t power_on_flag[2] = {1,1};
  
  if (power_on_flag[relay_num])
  {
    previous_error_status[relay_num] = ~dev.state.error_status;
    previous_warning_status[relay_num] = ~dev.state.warnings_status;
    power_on_flag[relay_num] = 0;
  }
  
  if ((previous_error_status[relay_num] != dev.state.error_status) || (previous_warning_status[relay_num] != dev.state.warnings_status))
  {  
    switch (dev.mdb_relay->relay[relay_num].relay_state) {
    case NORMAL_OPEN_STATE:
        // 1. Check alarm or not
        // 2. set state
        if ((dev.state.warnings_status != 0) || (dev.state.error_status != 0))
          set_external_switch_close((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2); 
        else
          set_external_switch_open((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2); // no error
      break;
      
    case NORMAL_CLOSE_STATE:
        if ((dev.state.warnings_status != 0) || (dev.state.error_status != 0))
          set_external_switch_open((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2); 
        else 
          set_external_switch_close((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2); // no error
      break;
    }
  }
  
  previous_warning_status[relay_num] = dev.state.warnings_status;
  previous_error_status[relay_num] = dev.state.error_status;

}

static void relay_not_active_process(uint8_t relay_num)
{
  static relay_state_e previous_state[2] = {UNKNOWN_STATE, UNKNOWN_STATE};
  
  if (previous_state[relay_num] != dev.mdb_relay->relay[relay_num].relay_state)
  {  
    switch (dev.mdb_relay->relay[relay_num].relay_state) {
    case NORMAL_OPEN_STATE:
        set_external_switch_open((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2); 
      break;
      
    case NORMAL_CLOSE_STATE:
        set_external_switch_close((relay_num == 0) ? EXTERNAL_1 : EXTERNAL_2); 
      break;
    }
  }
  
  previous_state[relay_num] = dev.mdb_relay->relay[relay_num].relay_state;
  
}

static void relay_process (uint8_t relay_num)
{
  // Relay mode
  switch (dev.mdb_relay->relay[relay_num].relay_mode) {
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
mdb_relay1_set_open(void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t    write_data = 0xFF00;
                    
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_coil_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                                MODBUS_COIL_MDB_RELAY_1,
                                                                                write_data);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
    }        
   
    return( err );     
}

static 
int
mdb_relay2_set_open(void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t    write_data = 0xFF00;
                    
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_coil_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                                MODBUS_COIL_MDB_RELAY_2,
                                                                                write_data);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
    }        
   
    return( err );     
}

static 
int
mdb_relay1_set_close(void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t    write_data = 0x0000;
                    
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_coil_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                                MODBUS_COIL_MDB_RELAY_1,
                                                                                write_data);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
    }        
   
    return( err );     
}

static 
int
mdb_relay2_set_close(void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t    write_data = 0x0000;
                    
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_coil_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                                MODBUS_COIL_MDB_RELAY_2,
                                                                                write_data);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
    }        
   
    return( err );     
}

static 
int
mdb_relay_init(void)
{                   
    int     err = 0;
    size_t      adu_len;
    size_t      recv_len = 0;
    

    adu_len = modbus_module.modbus_client->modbus_rtu_client_read_holding_registers_rqst (mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                                0x0000,
                                                                                32);    

    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
         
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }
    
    if( err != 0 )
    { 
        return( err );
    }


    taskENTER_CRITICAL();
      dev.cloop->info.device_id        = mdbs_sens_raw[  0];
      dev.cloop->info.hardware_id      = mdbs_sens_raw[  1];
    taskEXIT_CRITICAL();

    return( err );   
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

static void set_mdb_relay_status (int status)
{
#ifndef NO_RELAY  
  status == 0 ? (dev.state.error_status &=~ MDB_RELAY_LINK_ERR) : (dev.state.error_status |= MDB_RELAY_LINK_ERR);
#else
  dev.state.error_status &=~ MDB_RELAY_LINK_ERR; 
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
    uint8_t     mdbs_addr   = 0x40;
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
      err = modbus_client_write( mdbs_addr, 0x0804, 4, p->range[ p->range_idx ].mb_output.raw );
      TRACE("%d\t", err); for(int i = 0; i < 4; i++){TRACE("%04X", *(p->range[ p->range_idx ].mb_output.raw+i));} TRACE("\n");
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
    uint8_t     mdbs_addr   = 0x40;

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

    err = modbus_client_write( mdbs_addr, 0x0814, 4, out.raw );
    TRACE("%d\t", err); for(int i = 0; i < 4; i++){TRACE("%04X", *(p->range[ p->range_idx ].mb_output.raw+i));} TRACE("\n");
    if( err )
    {
        return( err );
    }

    return( 0 );
}


static 
int
cloop_get_info(void)
{
    int     err = 0;
    size_t      adu_len;
    size_t      recv_len = 0;
    

    adu_len = modbus_module.modbus_client->modbus_rtu_client_read_holding_registers_rqst (mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                0x0000,
                                                                                32);    

    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
         
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }
    
    if( err != 0 )
    { 
        return( err );
    }


    taskENTER_CRITICAL();
      dev.cloop->info.device_id        = mdbs_sens_raw[  0];
      dev.cloop->info.hardware_id      = mdbs_sens_raw[  1];
    taskEXIT_CRITICAL();


    return( err );
}

static 
int
cloop_get_cal(void)
{
    int     err = 0;
    size_t      adu_len;
    size_t      recv_len = 0;
    

    adu_len = modbus_module.modbus_client->modbus_rtu_client_read_holding_registers_rqst (mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                MODBUS_HREG_CLOOP_CAL_CHANNEL1_TIMESTAMP_HI,
                                                                                32);    

    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
         
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }
    
    if( err != 0 )
    { 
        return( err );
    }


    taskENTER_CRITICAL();
      dev.cloop->cal[0].timestamp.u16[ 1]        = mdbs_sens_raw[ 0];
      dev.cloop->cal[0].timestamp.u16[ 0]        = mdbs_sens_raw[ 1];
      dev.cloop->cal[0].uA.u16[ 0]               = mdbs_sens_raw[ 8];    
      dev.cloop->cal[0].raw.u16[ 0]              = mdbs_sens_raw[ 9];    
      dev.cloop->cal[1].timestamp.u16[ 1]        = mdbs_sens_raw[ 16];
      dev.cloop->cal[1].timestamp.u16[ 0]        = mdbs_sens_raw[ 17];
      dev.cloop->cal[1].uA.u16[ 0]               = mdbs_sens_raw[ 24];    
      dev.cloop->cal[1].raw.u16[ 0]              = mdbs_sens_raw[ 25];    
    taskEXIT_CRITICAL();


    return( err );
  
}

static 
int
cloop_set_cal_4ma (void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint8_t     send_buf[4] = {0};
    uint16_t    uA_4 = 4000;
    
    send_buf[0] =  uA_4 >> 8;
    send_buf[1] =  uA_4 & 0x00FF;
    send_buf[2] =  0x00;
    send_buf[3] =  0x00;
        
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_multiple_registers_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                MODBUS_HREG_CLOOP_CAL_CHANNEL1_4UA_HI,
                                                                                2,
                                                                                send_buf);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }       
   
    return( err );
}

static 
int
cloop_set_cal_20ma (void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint8_t     send_buf[4] = {0};
    uint16_t    uA_20 = 20000;
    
    send_buf[0] =  uA_20 >> 8;
    send_buf[1] =  uA_20 & 0x00FF;
    send_buf[2] =  0x00;
    send_buf[3] =  0x00;
        
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_multiple_registers_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                MODBUS_HREG_CLOOP_CAL_CHANNEL1_20UA_HI,
                                                                                2,
                                                                                send_buf);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }        
   
    return( err );  
}

static 
int
cloop_set_raw_4mA (void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
                
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_register_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_RAW_INT_HI,
                                                                                dev.cloop->cal[0].raw.u16[0]);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }
   
    return( err );  
}

static 
int
cloop_set_raw_20mA (void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
                
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_register_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_RAW_INT_HI,
                                                                                dev.cloop->cal[1].raw.u16[0]);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }        
   
    return( err );    
}

static 
int
cloop_set_range (void)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    
    uint8_t     send_buf[28] = {0};
   
    send_buf[0] =  0;   // fractional
    send_buf[1] =  0;
    send_buf[2] =  0;
    send_buf[3] =  0;
    send_buf[4] =  dev.cl420.range[0].ppm >> 24;                // integral
    send_buf[5] =  dev.cl420.range[0].ppm >> 16 & 0x00FF;
    send_buf[6] =  dev.cl420.range[0].ppm >> 8 & 0x00FF;
    send_buf[7] =  dev.cl420.range[0].ppm & 0x00FF;
    send_buf[8] =  0;  // fractional
    send_buf[9] =  0;
    send_buf[10] = 0;
    send_buf[11] = 0;
    send_buf[12] =  dev.cl420.range[1].ppm >> 24;               // integral
    send_buf[13] =  dev.cl420.range[1].ppm >> 16 & 0x00FF;
    send_buf[14] =  dev.cl420.range[1].ppm >> 8 & 0x00FF;
    send_buf[15] =  dev.cl420.range[1].ppm & 0x00FF;
    send_buf[16] =  0;  // fractional
    send_buf[17] =  0;
    send_buf[18] =  0;
    send_buf[19] =  0;
    send_buf[20] =  dev.cl420.range[2].ppm >> 24;               // integral
    send_buf[21] =  dev.cl420.range[2].ppm >> 16 & 0x00FF;
    send_buf[22] =  dev.cl420.range[2].ppm >> 8 & 0x00FF;
    send_buf[23] =  dev.cl420.range[2].ppm & 0x00FF;    
    send_buf[24] = 0x00;    
    send_buf[25] = dev.cl420.range_idx;
    send_buf[26] = 0x00;
    send_buf[27] = dev.cl420.range[0].units;     
      
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_multiple_registers_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                MODBUS_HREG_CONF_RNG1_PPM_FRC_HI,
                                                                                14,
                                                                                send_buf);
    
    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
    
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }        
   
    return( err );    
}


static 
int
cloop_get_range (void)
{
    int     err = 0;
    size_t      adu_len;
    size_t      recv_len = 0;
    

    adu_len = modbus_module.modbus_client->modbus_rtu_client_read_holding_registers_rqst (mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                MODBUS_HREG_CONF_RNG1_PPM_FRC_HI,
                                                                                14);    

    err = modbus_send_receive_data(mdbs_adu_xmit, adu_len, &recv_len, mdbs_adu_recv);
         
    if (err == 0)
    {
      taskENTER_CRITICAL();
      err = modbus_module.modbus_client->modbus_rtu_client_response_parse(CFG_MDBS_DEV_ADDR_CLOOP,
                                                                    mdbs_adu_recv,
                                                                    recv_len,
                                                                    mdbs_sens_raw);
      taskEXIT_CRITICAL();
    }
    
    if( err != 0 )
    { 
        return( err );
    }


    taskENTER_CRITICAL();
      dev.cl420.range[0].ppm  = mdbs_sens_raw[2];
      dev.cl420.range[0].ppm <<= 16; 
      dev.cl420.range[0].ppm |= mdbs_sens_raw[3];    

      dev.cl420.range[1].ppm  = mdbs_sens_raw[6];
      dev.cl420.range[1].ppm <<= 16; 
      dev.cl420.range[1].ppm |= mdbs_sens_raw[7];    
            
      dev.cl420.range[2].ppm  = mdbs_sens_raw[10];
      dev.cl420.range[2].ppm <<= 16; 
      dev.cl420.range[2].ppm |= mdbs_sens_raw[11];    
      
      dev.cl420.range_idx = mdbs_sens_raw[12];      
      dev.cl420.range[0].units = mdbs_sens_raw[13]; 
      dev.cl420.range[1].units = mdbs_sens_raw[13]; 
      dev.cl420.range[2].units = mdbs_sens_raw[13];
    taskEXIT_CRITICAL();
    
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

void send_cmd_for_cloop_set_raw_4mA (void)
{
    BaseType_t result;
    app_pipe_t queue_data;
    queue_data.tag    = OS_USER_TAG_CAL_CLOOP_SET_RAW_4MA;        
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
 
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }        
    
    osDelay(3);   
}

void send_cmd_for_cloop_set_raw_20mA (void)
{
    BaseType_t result;  
    app_pipe_t queue_data;
    queue_data.tag    = OS_USER_TAG_CAL_CLOOP_SET_RAW_20MA;          
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );

    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }            
    
    osDelay(3);   
}

void send_cmd_for_cloop_4mA_write (void)
{
    BaseType_t result;  
    app_pipe_t queue_data;  
    queue_data.tag      = OS_USER_TAG_CAL_CLOOP_4MA_WRITE;          
    result = xQueueSendToFront( que_ibus_hndl, &queue_data, NULL );
 
    if (result != pdTRUE) 
    {
      xQueueReset(que_ibus_hndl);
      xQueueSend( que_ibus_hndl, &queue_data, NULL );
    }                
    
    osDelay(3);   
}

void send_cmd_for_cloop_20mA_write (void)
{
    BaseType_t result;  
    app_pipe_t queue_data;  
    queue_data.tag      = OS_USER_TAG_CAL_CLOOP_20MA_WRITE;          
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

    stm32_uart4_init( 9600, 8, 1.0, 'N' );  

    sens->link_err      = 1;
    set_sensor_status(dev.sens->link_err);
    sens->meas.ppm.integral   = 0;
    sens->meas.ppm.fractional = 0;
    
    dev.cloop->link_err = 1;
    set_cloop_status(dev.cloop->link_err);
    dev.mdb_relay->link_err = 1;
    set_mdb_relay_status(dev.mdb_relay->link_err);    
    
    queue_data.tag = OS_USER_TAG_IBUS_BOARDS_INIT;
    xQueueSend( que_ibus_hndl, &queue_data, NULL );
    
    task_ibus_cl420_ch2_update();

    while( true )
    {
        xQueueReceive( que_ibus_hndl, &queue_data, portMAX_DELAY  );
        tag = queue_data.tag;
        
        previous_error_status = get_dev_errors_status(); 
        previous_warning_status = get_dev_warnings_status();

        switch( tag )
        {
            case OS_USER_TAG_TMR_1SEC:
                    stm32_uart4_init( 9600, 8, 1.0, 'N' );
                    sens->link_err  = modbus_ascii_client_read();
                    osDelay( 150 );
                    sens->link_err  = modbus_ascii_client_read();
                    set_sensor_status(dev.sens->link_err);
                    osDelay( 150 );
                    
                    stm32_uart4_init(19200, 8, 1.0, 'N');                    
                    dev.cloop->link_err = cl420_ch1_update( dev_cl420, sens );
                    set_cloop_status(dev.cloop->link_err);       
                    
                    queue_data.tag = OS_USER_TAG_IBUS_REFRESH;
                    xQueueSend( que_hmi_hndl, &queue_data, NULL );                                           
                break;
                
            case OS_USER_TAG_IBUS_CL420_CH2_UPDATE:
                stm32_uart4_init(19200, 8, 1.0, 'N');
                dev.cloop->link_err = cl420_ch2_update( dev_cl420 );
                set_cloop_status(dev.cloop->link_err); 
                break;
                
            case OS_USER_TAG_IBUS_BOARDS_INIT:
                //dev.sens->link_err  = sensor_init( sens );      
                //set_sensor_status(dev.sens->link_err);
                stm32_uart4_init(19200, 8, 1.0, 'N');
                dev.cloop->link_err = cloop_init();
                set_cloop_status(dev.cloop->link_err); 
                dev.mdb_relay->link_err = mdb_relay_init();
                set_mdb_relay_status(dev.mdb_relay->link_err); 
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
              
            case OS_USER_TAG_CAL_CLOOP_4MA_WRITE:
              stm32_uart4_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_cal_4ma();
              if (dev.cloop->link_err == 0)
                log_write_event(&dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_CLOOP_CALIBRATION_4MA);
              set_cloop_status(dev.cloop->link_err);     
              break;
              
            case OS_USER_TAG_CAL_CLOOP_20MA_WRITE:
              stm32_uart4_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_cal_20ma();
              if (dev.cloop->link_err == 0)
                log_write_event(&dev.log, LOG_SOURCE_CLOOP, LOG_CLOOP_EVENT_CLOOP_CALIBRATION_20MA);              
              set_cloop_status(dev.cloop->link_err);   
              break;
              
            case OS_USER_TAG_CAL_CLOOP_SET_RAW_4MA:
              stm32_uart4_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_raw_4mA();
              set_cloop_status(dev.cloop->link_err);  
              break;
              
            case OS_USER_TAG_CAL_CLOOP_SET_RAW_20MA:
              stm32_uart4_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_raw_20mA();
              set_cloop_status(dev.cloop->link_err);  
              break;    
              
            case OS_USER_TAG_CAL_CLOOP_SET_RANGE:
              stm32_uart4_init(19200, 8, 1.0, 'N');
              dev.cloop->link_err = cloop_set_range();  
              set_cloop_status(dev.cloop->link_err);
              break;              

            default:
                break;
        }
        
        stm32_uart4_init(19200, 8, 1.0, 'N');
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
