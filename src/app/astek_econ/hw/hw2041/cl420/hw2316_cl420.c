#include "hw_cl420.h"
#include "modbus.h"
#include "modbus\client\mdbs_dev_o2mb.h"
#include "cmsis_os.h"
#include "../../dev/dev.h"

extern uint8_t     mdbs_adu_xmit[ MDBS_RTU_ADU_SIZEOF ];
extern uint8_t     mdbs_adu_recv[ MDBS_RTU_ADU_SIZEOF ];
extern uint16_t    mdbs_sens_raw[ MDBS_RTU_ADU_SIZEOF / 2];

extern  dev_t                   dev;

extern int modbus_send_receive_data (uint8_t *mdbs_send_buf, size_t adu_len, size_t *recv_len, uint8_t *mdbs_resv_buf);

int cloop_hw_init (void)
{
  return 0;
}

int cloop_get_info(void)
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

int cloop_get_cal(void)
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
      dev.cloop->cal_ch1[0].timestamp.u16[ 1]        = mdbs_sens_raw[ 0];
      dev.cloop->cal_ch1[0].timestamp.u16[ 0]        = mdbs_sens_raw[ 1];
      dev.cloop->cal_ch1[0].uA.u16[ 0]               = mdbs_sens_raw[ 8];    
      dev.cloop->cal_ch1[0].raw.u16[ 0]              = mdbs_sens_raw[ 9];    
      dev.cloop->cal_ch1[1].timestamp.u16[ 1]        = mdbs_sens_raw[ 16];
      dev.cloop->cal_ch1[1].timestamp.u16[ 0]        = mdbs_sens_raw[ 17];
      dev.cloop->cal_ch1[1].uA.u16[ 0]               = mdbs_sens_raw[ 24];    
      dev.cloop->cal_ch1[1].raw.u16[ 0]              = mdbs_sens_raw[ 25];    
    taskEXIT_CRITICAL();


    return( err );  
}

int cloop_get_range(void)
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

int cloop_set_range(void)
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

int cloop_set_raw(uint8_t ch, uint32_t raw)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t reg_addr;
    if (ch == 1)
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_RAW_INT_HI;
    else 
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_RAW_INT_HI;
                
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_register_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                reg_addr,
                                                                                (uint16_t)raw);
    
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

int cloop_get_raw(uint8_t ch, uint32_t* current_raw)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t reg_addr;
    
    if (ch == 1)
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_RAW_INT_HI;
    else 
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_RAW_INT_HI;
                
    adu_len = modbus_module.modbus_client->modbus_rtu_client_read_holding_registers_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                reg_addr,
                                                                                2); 
    
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
      *current_raw  = mdbs_sens_raw[0];
      *current_raw <<= 16; 
      *current_raw |= mdbs_sens_raw[1];    
    taskEXIT_CRITICAL();
    
    return( err );    
}

int cloop_set_uA(uint8_t ch, uint32_t uA)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;

    uint16_t reg_addr;
    if (ch == 1)
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_uA_INT_HI;
    else 
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_uA_INT_HI;     
    
   adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_register_rqst(mdbs_adu_xmit,
                                                                                        CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                        reg_addr,
                                                                                        (uint16_t)uA);
        
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
   
   return err;      
}


int cloop_get_uA(uint8_t ch, uint32_t *uA)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t reg_addr;
    
    if (ch == 1)
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL1_CURRENT_uA_INT_HI;
    else 
      reg_addr = MODBUS_HREG_CLOOP_CHANNEL2_CURRENT_uA_INT_HI;
                
    adu_len = modbus_module.modbus_client->modbus_rtu_client_read_holding_registers_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                reg_addr,
                                                                                2); 
    
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
      *uA  = mdbs_sens_raw[0];
      *uA <<= 16; 
      *uA |= mdbs_sens_raw[1];    
    taskEXIT_CRITICAL();
    
    return( err );      
}

int set_cal_4ma(uint8_t ch)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint8_t     send_buf[4] = {0};
    uint16_t    uA_4 = 4000;
    
    uint16_t reg_addr;
    if (ch == 1)
      reg_addr = MODBUS_HREG_CLOOP_CAL_CHANNEL1_4UA_HI;
    else 
      reg_addr = MODBUS_HREG_CLOOP_CAL_CHANNEL2_4UA_HI;    
    
    send_buf[0] =  uA_4 >> 8;
    send_buf[1] =  uA_4 & 0x00FF;
    send_buf[2] =  0x00;
    send_buf[3] =  0x00;
        
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_multiple_registers_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                reg_addr,
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

int set_cal_20ma(uint8_t ch)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint8_t     send_buf[4] = {0};
    uint16_t    uA_20 = 20000;
   
    uint16_t reg_addr;
    if (ch == 1)
      reg_addr = MODBUS_HREG_CLOOP_CAL_CHANNEL1_20UA_HI;
    else 
      reg_addr = MODBUS_HREG_CLOOP_CAL_CHANNEL2_20UA_HI;      
    
    send_buf[0] =  uA_20 >> 8;
    send_buf[1] =  uA_20 & 0x00FF;
    send_buf[2] =  0x00;
    send_buf[3] =  0x00;
        
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_multiple_registers_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_CLOOP,
                                                                                reg_addr,
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

int coarse_increase_4ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] += 100;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] += 100;
  }
  
  return 0;
}

int fine_increase_4ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] += 10;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] += 10;
  }  
  
  return 0;
}

int coarse_increase_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] += 100;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] += 100;
  }  
  
  return 0;
}

int fine_increase_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] += 10;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] += 10;
  }    
  
  return 0;
}

int coarse_decrease_4ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] -= 100;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] -= 100;
  }  
  
  return 0;
}

int fine_decrease_4ma_raw(uint8_t ch)
{
    if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] -= 10;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] -= 10;
  }  
  
  return 0;
}

int coarse_decrease_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] -= 100;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] -= 100;
  }
  
  return 0;
}

int fine_decrease_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] -= 10;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] -= 10;
  }     
  
  return 0;
}
