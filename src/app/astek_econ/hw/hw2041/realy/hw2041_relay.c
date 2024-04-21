#include "hw_relay.h"


#include "modbus.h"
#include "modbus\client\mdbs_dev_o2mb.h"
#include "cmsis_os.h"
#include "../../dev/dev.h"

extern uint8_t     mdbs_adu_xmit[ MDBS_RTU_ADU_SIZEOF ];
extern uint8_t     mdbs_adu_recv[ MDBS_RTU_ADU_SIZEOF ];
extern uint16_t    mdbs_sens_raw[ MDBS_RTU_ADU_SIZEOF / 2];

extern  dev_t                   dev;

extern int modbus_send_receive_data (uint8_t *mdbs_send_buf, size_t adu_len, size_t *recv_len, uint8_t *mdbs_resv_buf);

int relay_hw_init (void)
{
  return 0;
}

int relay_get_info (void)
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
      dev.ext_relay->info.device_id        = mdbs_sens_raw[  0];
      dev.ext_relay->info.hardware_id      = mdbs_sens_raw[  1];
    taskEXIT_CRITICAL();

    return( err );   
  
}

int relay_set_state(const uint8_t relay_num, const bool state)
{
    int     err;
    size_t      adu_len;
    size_t      recv_len = 0;
    uint16_t    write_data;
    uint16_t    reg_add;
    
    if (relay_num == 0)
      reg_add = MODBUS_COIL_MDB_RELAY_1;
    else if (relay_num == 1)
      reg_add = MODBUS_COIL_MDB_RELAY_2;
    else 
      return 1;
        
    if (state == true)
      write_data = 0xFF00;
    else
      write_data = 0x0000;
                    
    adu_len = modbus_module.modbus_client->modbus_rtu_client_write_single_coil_rqst(mdbs_adu_xmit,
                                                                                CFG_MDBS_DEV_ADDR_MDB_RELAY,
                                                                                reg_add,
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

int relay_get_state(const uint8_t relay_num, bool *state)
{
  return 0;
}

