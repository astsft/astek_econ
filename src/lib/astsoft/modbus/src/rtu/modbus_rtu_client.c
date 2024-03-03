/**
  * @file    modbus_rtu_client.c
  * @brief   MODBUS RTU protocol, master routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "modbus.h"

/*******************************************************************************
* STRUCTURES
*******************************************************************************/
client_diagnostic_s client_diagnostic_rqst =
{
  modbus_rtu_client_diagnostics_rqst,
  modbus_rtu_client_diagnostics_return_query_data_rqst,
  modbus_rtu_client_diagnostics_restart_comm_options_with_log_rqst,
  modbus_rtu_client_diagnostics_restart_comm_options_without_log_rqst,
  modbus_rtu_client_diagnostics_return_diag_reg_rqst,
  modbus_rtu_client_diagnostics_change_ascii_delimiter_rqst,
  modbus_rtu_client_diagnostics_force_listen_only_mode_rqst,
  modbus_rtu_client_diagnostics_clear_cnts_diag_reg_rqst,
  modbus_rtu_client_read_exception_status_rqst,
  modbus_rtu_client_diagnostics_return_mess_cnt_rqst,
  modbus_rtu_client_diagnostics_return_comm_error_cnt_rqst,
  modbus_rtu_client_diagnostics_return_exc_error_cnt_rqst,
  modbus_rtu_client_diagnostics_return_serv_mess_cnt_rqst,
  modbus_rtu_client_diagnostics_return_serv_norsp_cnt_rqst,
  modbus_rtu_client_diagnostics_return_serv_nak_cnt_rqst,
  modbus_rtu_client_diagnostics_return_serv_busy_cnt_rqst,
  modbus_rtu_client_diagnostics_return_char_overrun_cnt_rqst,
  modbus_rtu_client_diagnostics_clear_overrun_cnt_flag_rqst  
};

modbus_client_s modbus_client =
{
  modbus_rtu_client_read_coils_rqst,
  modbus_rtu_client_read_discrete_inputs_rqst,
  modbus_rtu_client_read_holding_registers_rqst,
  modbus_rtu_client_read_input_registers_rqst,
  modbus_rtu_client_write_single_coil_rqst,
  modbus_rtu_client_write_single_register_rqst,  
  
  .client_diagnostic_rqst = &client_diagnostic_rqst,
  
  modbus_rtu_client_get_comm_event_cnt_rqst,
  modbus_rtu_client_get_comm_event_log_rqst,
  modbus_rtu_client_write_multiple_coils_rqst,
  modbus_rtu_client_write_multiple_registers_rqst,
  modbus_rtu_client_report_server_id_rqst,
  modbus_rtu_client_read_file_record_rqst,
  modbus_rtu_client_write_file_record_rqst,
  modbus_rtu_client_mask_write_register_rqst,
  modbus_rtu_client_read_write_multiple_registers_rqst,
  modbus_rtu_client_read_fifo_queue_rqst,
  modbus_rtu_client_enc_int_transp_can_gen_ref_rqst,
  modbus_rtu_client_enc_int_transp_read_dev_id_rqst, 
  modbus_rtu_client_response_parse
};

modbus_server_s modbus_server;

modbus_module_s modbus_module = 
{
  .modbus_client = &modbus_client,
  .modbus_server = &modbus_server
};

/*******************************************************************************
* NEW FUNCTIONS
*******************************************************************************/

/**
  * @brief Function adds data to adu buffer 
  * @param mdbs_func modbus function
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start coil/reg address
  * @param cnt quantity of coils or registers to read
  */
static size_t 
add_adu_data_read_rqst (               uint8_t *       adu,
                                       const   mdbs_func_t     mdbs_func,
                                       const   uint8_t         dev_addr,
                                       const   uint16_t        addr,
                                               size_t          cnt)
{
          size_t                  len     = 0;
          uint16_t                crc;
  
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = mdbs_func;
          adu[ len++ ]    = addr >> 8;
          adu[ len++ ]    = addr & 0x00FF;
          adu[ len++ ]    = cnt >> 8;
          adu[ len++ ]    = cnt & 0x00FF;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
  
  return len;
}

/**
  * @brief Function adds data to adu buffer 
  * @param mdbs_func modbus function
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start coil/reg address
  * @param value quantity of single coil or register to write
  */
static size_t 
add_adu_data_write_single_rqst (                   uint8_t *       adu,
                                           const   mdbs_func_t     mdbs_func,
                                           const   uint8_t         dev_addr,
                                           const   uint16_t        addr,
                                                   uint16_t        value)
{
          size_t                  len     = 0;
          uint16_t                crc;
  
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = mdbs_func;
          adu[ len++ ]    = addr >> 8;
          adu[ len++ ]    = addr & 0x00FF;
          adu[ len++ ]    = value >> 8;
          adu[ len++ ]    = value & 0x00FF;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
  
          return len;
}

/**
  * @brief Function adds data to adu buffer in diagnostic function
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param mdbs_subfunc modbus diagnostic sub-function
  * @param value quantity of single coil or register to write
  */
static size_t 
add_adu_diagnostic_fun_data_rqst (         uint8_t *       adu,
                                   const   uint8_t         dev_addr,
                                   const   mdbs_diag_subfunc_t     mdbs_subfunc,
                                           uint16_t        data)
{
          size_t                  len     = 0;
          uint16_t                crc;
  
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_DIAGNOSTICS;
          adu[ len++ ]    = 0x00;
          adu[ len++ ]    = mdbs_subfunc;
          adu[ len++ ]    = data >> 8;
          adu[ len++ ]    = data & 0x00FF;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
          return len;
}

/**
  * @brief Function adds 2 variable of data to pdu buffer 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
static void 
add_two_var_from_pdu_to_data_resp (  const  uint8_t *       pdu,
                                            uint16_t *      data)
{
          size_t          idx     = 1;
  
          uint16_t      var = pdu[idx++];
          var <<= 8;
          var |= pdu[idx++];
          
          *data++    = var;
          
          var = pdu[idx++];
          var <<= 8;
          var |= pdu[idx++];
          
          *data++    = var;
}


/**
  * @brief Read Coils 0x01 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start coils address
  * @param cnt quantity of coils to read
  */
size_t
modbus_rtu_client_read_coils_rqst (           uint8_t *       adu,
                                const   uint8_t         dev_addr,
                                const   uint16_t        addr,
                                        size_t          cnt )
{
        size_t                  len     = 0;
        
        if ((cnt >= 1) && (cnt <= 2000)) /* Checking for the correct counter */ 
        {
          len = add_adu_data_read_rqst (adu, MDBS_FUNC_READ_COILS, 
                              dev_addr, addr, cnt);
        }

        return( len );
}


/**
  * @brief Read Discrete Inputs 0x02 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start discrete_inputs address
  * @param cnt quantity of discrete_inputs to read
  */
size_t
modbus_rtu_client_read_discrete_inputs_rqst (                 uint8_t *       adu,
                                                const   uint8_t         dev_addr,
                                                const   uint16_t        addr,
                                                        size_t          cnt )
{
        size_t                  len     = 0;
        
        if ((cnt >= 1) && (cnt <= 2000)) /* Checking for the correct counter */ 
        {
          len = add_adu_data_read_rqst (adu, MDBS_FUNC_READ_DISCRETE_INPUTS, 
                              dev_addr, addr, cnt);
        }

        return( len );
}

/**
  * @brief Read Holding Registers 0x03 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start holding registers address
  * @param cnt quantity of holding registers to read
  */
size_t
modbus_rtu_client_read_holding_registers_rqst (         uint8_t *       adu,
                                          const   uint8_t         dev_addr,
                                          const   uint16_t        addr,
                                                  size_t          cnt )
{
        size_t                  len     = 0;
        
        if ((cnt >= 1) && (cnt <= 125)) /* Checking for the correct counter */ 
        {
          len = add_adu_data_read_rqst (adu, MDBS_FUNC_READ_HOLDING_REGISTERS, 
                              dev_addr, addr, cnt);   
        }

        return( len );
}

/**
  * @brief Read Input Registers 0x04 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start holding registers address
  * @param cnt quantity of holding registers to read
  */
size_t
modbus_rtu_client_read_input_registers_rqst (                 uint8_t *       adu,
                                                const   uint8_t         dev_addr,
                                                const   uint16_t        addr,
                                                        size_t          cnt )
{
        size_t                  len     = 0;
        
        if ((cnt >= 1) && (cnt <= 125)) /* Checking for the correct counter */ 
        {
          len = add_adu_data_read_rqst (adu, MDBS_FUNC_READ_INPUT_REGISTERS, 
                              dev_addr, addr, cnt);   
        }

        return( len );
}

/**
  * @brief Write Single Coil 0x05 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr single coil address
  * @param value new value of coil
  */
size_t
modbus_rtu_client_write_single_coil_rqst (            uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                uint16_t        value)
{
        size_t                  len     = 0;

        len = add_adu_data_write_single_rqst(adu, MDBS_FUNC_WRITE_SINGLE_COIL,
                                             dev_addr, addr, value); 
               
        return( len );
}

/**
  * @brief Write Single Register 0x06 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr single register address
  * @param value new value of register
  */
size_t
modbus_rtu_client_write_single_register_rqst (        uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                uint16_t        value)
{
        size_t                  len     = 0;
        
        len = add_adu_data_write_single_rqst(adu, MDBS_FUNC_WRITE_SINGLE_REGISTER,
                                             dev_addr, addr, value); 
          
        return( len );
}


/**
  * @brief Read Exception Status (Serial Line only) 0x07 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_read_exception_status_rqst (         uint8_t *  adu,
                                         const   uint8_t    dev_addr)
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_READ_EXCEPTION_STATUS;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
          return len;
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Universal function ( NOT RECOMENDED TO USE )
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param sub-function code
  * @param data data buf for transmit
  * @param data_cnt quantity of data
  */
size_t
modbus_rtu_client_diagnostics_rqst (              uint8_t *  adu,
                                    const   uint8_t    dev_addr,
                                            uint16_t sub_function,
                                            uint8_t * data,
                                            uint32_t data_cnt)
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_DIAGNOSTICS;
          adu[ len++ ]    = sub_function >> 8;
          adu[ len++ ]    = sub_function & 0x00FF;
          memcpy(&adu[len-1], data, data_cnt);
          len += data_cnt;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
          return len;
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Query Data Sub-function 0x00.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param data data buf for transmit
  * @param data_cnt quantity of data
  */
size_t
modbus_rtu_client_diagnostics_return_query_data_rqst (          uint8_t *  adu,
                                                  const   uint8_t    dev_addr,
                                                          uint8_t * data,
                                                          uint32_t data_cnt)
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_DIAGNOSTICS;
          adu[ len++ ]    = 0x00;
          adu[ len++ ]    = MDBS_DIAG_SUBFUNC_RETURN_QUERY_DATA;
          memcpy(&adu[len-1], data, data_cnt);
          len += data_cnt;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Restart Communications Option. 0x01
  *                             With Communications Event Log. Data = 0xFF00
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_restart_comm_options_with_log_rqst (       uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                                 MDBS_DIAG_SUBFUNC_RESTART_COMM_OPT,
                                                 0xFF00);  
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Restart Communications Option. 0x01
  *                             Without Communications Event Log. Data = 0x0000
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_restart_comm_options_without_log_rqst ( uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                                 MDBS_DIAG_SUBFUNC_RESTART_COMM_OPT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Diagnostic Register 0x02.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_diag_reg_rqst ( uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                             MDBS_DIAG_SUBFUNC_RETURN_DIAG_REG,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Change ASCII Input Delimiter 0x03.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param ascii_delim new ascii delimeter
  */
size_t
modbus_rtu_client_diagnostics_change_ascii_delimiter_rqst (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr,
                                                          uint8_t    ascii_delim )
{
          size_t                  len     = 0;
          uint16_t data = ascii_delim;
          data <<= 8;
          data &= 0xFF00;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_CHANGE_ASCII_INP_DELIM,
                                                 data);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Force Listen Only Mode 0x04.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_force_listen_only_mode_rqst (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_FORCE_LISTEN_ONLY_MODE,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Clear Counters and Diagnostic Register 0x0A.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_clear_cnts_diag_reg_rqst (       uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_CLEAR_CNTS_DIAG_REG,
                                                 0x0000 );              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Bus Message Count 0x0B.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_mess_cnt_rqst (           uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_RETURN_BUS_MESS_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Bus Communication Error Count 0x0C.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_comm_error_cnt_rqst (     uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_RETURN_BUS_COMM_ERR_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Bus Exception Error Count 0x0D.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_exc_error_cnt_rqst (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_RETURN_BUS_EXC_ERR_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Server Message Count 0x0E.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_serv_mess_cnt_rqst (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_RETURN_SERV_MESS_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Server No Response Count 0x0F.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_serv_norsp_cnt_rqst (     uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_RETURN_SERV_NORSP_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Server NAK Count 0x10.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_serv_nak_cnt_rqst (       uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_RETURN_SERV_NAK_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Server Busy Count 0x11.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_serv_busy_cnt_rqst (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_RETURN_SERV_BUSY_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Return Bus Character Overrun Count 0x12.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_return_char_overrun_cnt_rqst (   uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_BUS_CHR_OVR_CNT,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Diagnostics (Serial Line only) 0x08. Clear Overrun Counter and Flag 0x14.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_diagnostics_clear_overrun_cnt_flag_rqst (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          
          len = add_adu_diagnostic_fun_data_rqst(adu, dev_addr,
                                      MDBS_DIAG_SUBFUNC_CLEAR_OVR_CNT_FLG,
                                                 0x0000);              
          return len;  
}

/**
  * @brief Get Comm Event Counter (Serial Line only) 0x0B.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_get_comm_event_cnt_rqst (             uint8_t *  adu,
                                           const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_GET_COMM_EVENT_COUNTER;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
          return len; 
}

/**
  * @brief Get Comm Event Log (Serial Line only) 0x0C.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_get_comm_event_log_rqst (             uint8_t *  adu,
                                           const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_GET_COMM_EVENT_LOG;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
          return len; 
}

/**
  * @brief Write Multiple Coils 0x0F.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start coils address
  * @param cnt quantity of coils to write
  * @param outputs_value Output values buf
  */
size_t
modbus_rtu_client_write_multiple_coils_rqst (           uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint16_t   addr, 
                                                   uint16_t   cnt, 
                                                   uint8_t * outputs_value )
{
          size_t                  len     = 0;
          uint16_t                crc;
          uint16_t                addition = 0;
          
          if ((cnt >= 1) && (cnt <= 0x07B0))
          {
            addition = ((cnt / 8) == 0) ? 0 : 1;
                      
            adu[ len++ ]    = dev_addr;
            adu[ len++ ]    = MDBS_FUNC_WRITE_MULTIPLE_COILS;
            adu[ len++ ]    = addr >> 8;
            adu[ len++ ]    = addr & 0x00FF;
            adu[ len++ ]    = cnt >> 8;
            adu[ len++ ]    = cnt & 0x00FF;   
            adu[ len++ ]    = (cnt / 8) + addition;
            memcpy(&adu[len], outputs_value, adu[len-1]);
            len += adu[len-1];
            crc             = modbus_crc( adu, len );
            adu[ len++ ]    = crc >> 8;
            adu[ len++ ]    = crc & 0xFF;
          }
          else len = 0;
  
          return len; 
}

/**
  * @brief Write Multiple Registers 0x10.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start register address
  * @param cnt quantity of registers to write
  * @param outputs_value Output values buf
  */
size_t
modbus_rtu_client_write_multiple_registers_rqst (  uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint16_t   addr, 
                                                   uint16_t   cnt, 
                                                   uint8_t * outputs_value )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          if ((cnt >= 1) && (cnt <= 0x007B))
          {
            adu[ len++ ]    = dev_addr;
            adu[ len++ ]    = MDBS_FUNC_WRITE_MULTIPLE_REGISTERS;
            adu[ len++ ]    = addr >> 8;
            adu[ len++ ]    = addr & 0x00FF;
            adu[ len++ ]    = cnt >> 8;
            adu[ len++ ]    = cnt & 0x00FF;   
            adu[ len++ ]    = cnt * 2 ;
            memcpy(&adu[len], outputs_value, adu[len-1]);
            len += adu[len-1];
            crc             = modbus_crc( adu, len );
            adu[ len++ ]    = crc >> 8;
            adu[ len++ ]    = crc & 0xFF;
          }
          else len = 0;
  
          return len; 
}

/**
  * @brief Report Server ID (Serial Line only) 0x11.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_report_server_id_rqst (               uint8_t *  adu,
                                           const   uint8_t    dev_addr )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_REPORT_SERVER_ID;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
          
          return len; 
}

/**
  * @brief Read File Record 0x14.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_read_file_record_rqst (               uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   byte_cnt, 
                                                   uint8_t   ref_type, 
                                                   uint16_t  file_number,
                                                   uint16_t  record_number,
                                                   uint16_t  record_len )
{
          size_t                  len     = 0;
          
          return len; 
}

/**
  * @brief Write File Record 0x15.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_write_file_record_rqst (              uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   rqst_data_len, 
                                                   uint8_t   ref_type, 
                                                   uint16_t  file_number,
                                                   uint16_t  record_number,
                                                   uint16_t  record_len,
                                                   uint8_t * data )
{
          size_t                  len     = 0;
          
          return len; 
}

/**
  * @brief Mask Write Register 0x16.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param and_mask
  * @param or_mask
  */
size_t
modbus_rtu_client_mask_write_register_rqst (            uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   ref_addr, 
                                                   uint16_t  and_mask,
                                                   uint16_t  or_mask )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_MASK_WRITE_REGISTER;
          adu[ len++ ]    = ref_addr >> 8;
          adu[ len++ ]    = ref_addr & 0x00FF;
          adu[ len++ ]    = and_mask >> 8;
          adu[ len++ ]    = and_mask & 0x00FF;
          adu[ len++ ]    = or_mask >> 8;
          adu[ len++ ]    = or_mask & 0x00FF;
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
          
          return len; 
}

/**
  * @brief Read/Write Multiple Registers 0x17.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param read_addr start register address to read
  * @param read_qnt quantity of registers to read
  * @param write_addr start register address
  * @param write_qnt quantity of registers to write
  * @param write_byte_qnt quantity of registers to write
  * @param registers_value Output values buf
  */
size_t
modbus_rtu_client_read_write_multiple_registers_rqst (       uint8_t *  adu,
                                                const   uint8_t    dev_addr,
                                                        uint16_t   read_addr, 
                                                        uint16_t   read_qnt,
                                                        uint16_t   write_addr, 
                                                        uint16_t   write_qnt,
                                                        uint16_t   write_byte_cnt,
                                                        uint8_t *  registers_value )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          if (!((read_qnt >= 1) && (read_qnt <= 0x7D))) return 0;
          if (!((write_qnt >= 1) && (write_qnt <= 0x79))) return 0;          
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_READ_WRITE_MULTIPLE_REGISTERS;
          adu[ len++ ]    = read_addr >> 8;
          adu[ len++ ]    = read_addr & 0x00FF;
          adu[ len++ ]    = read_qnt >> 8;
          adu[ len++ ]    = read_qnt & 0x00FF;   
          adu[ len++ ]    = write_addr >> 8;
          adu[ len++ ]    = write_addr & 0x00FF;
          adu[ len++ ]    = write_qnt >> 8;
          adu[ len++ ]    = write_qnt & 0x00FF;             
          adu[ len++ ]    = write_qnt * 2 ;
          memcpy(&adu[len], registers_value, adu[len-1]);
          len += adu[len-1];
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
  
          return len; 
}


/**
  * @brief Read FIFO Queue 0x18.
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  */
size_t
modbus_rtu_client_read_fifo_queue_rqst (                uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                           const   uint16_t   fifo_addr )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_READ_FIFO_QUEUE;
          adu[ len++ ]    = fifo_addr >> 8;
          adu[ len++ ]    = fifo_addr & 0xFF;          
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
          
          return len; 
}

/**
  * @brief Encapsulated Interface Transport 0x2B. CANopen General Reference Request and Response PDU 0x0D
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param data MEI type specific data
  * @param bytes_cnt data bytes cnt
  */
size_t
modbus_rtu_client_enc_int_transp_can_gen_ref_rqst (                 uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                           const   uint8_t *  data,
                                                   uint8_t    bytes_cnt )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_ENCAPSULATED_INTERFACE_TRANSPORT;
          adu[ len++ ]    = MDBS_MEI_TYPE_CANOPEN_GENERAL_REFERENCE;
          memcpy(&adu[len], data, bytes_cnt);
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
          
          return len; 
}

/**
  * @brief Encapsulated Interface Transport 0x2B. Read Device Identification 0x0E
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param dev_id_code Read Device ID code
  * @param object_id Object Id
  */
size_t
modbus_rtu_client_enc_int_transp_read_dev_id_rqst (     uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t    dev_id_code,
                                                   uint8_t    object_id )
{
          size_t                  len     = 0;
          uint16_t                crc;
          
          adu[ len++ ]    = dev_addr;
          adu[ len++ ]    = MDBS_FUNC_ENCAPSULATED_INTERFACE_TRANSPORT;
          adu[ len++ ]    = MDBS_MEI_TYPE_READ_DEVICE_ID;
          adu[ len++ ]    = dev_id_code;
          adu[ len++ ]    = object_id;        
          crc             = modbus_crc( adu, len );
          adu[ len++ ]    = crc >> 8;
          adu[ len++ ]    = crc & 0xFF;
          
          return len; 
}

/*******************************************************************************
* RESPONSE
*******************************************************************************/
/**
  * @brief      Read Coils 0x01. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Data of requested coils
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_read_coils_resp(         const   uint8_t *       pdu,
                                                   uint8_t *       data )
{
                uint16_t        cnt             = pdu[ 1];
                size_t          idx             = 2;
                uint8_t         coil;
        const   size_t          coils_max        = 2000;


        if( cnt > coils_max )
        {
                return( MDBS_ERR_SERVER_DEVICE_FAILURE );
        }

        while( cnt-- )
        {
                coil     =   pdu[ idx++ ];
                *data++ =   coil;
        }

        return( MDBS_ERR_NONE );
}

/**
  * @brief      Read Discrete Inputs 0x02. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Data of requested discrete inputs
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_read_discrete_inputs_resp(    const   uint8_t *       pdu,
                                                        uint8_t *      data )
{
                uint16_t        cnt             = pdu[ 1];
                size_t          idx             = 2;
                uint8_t         discrete_input;
        const   size_t          discrete_input_max        = 2000;


        if( cnt > discrete_input_max )
        {
                return( MDBS_ERR_SERVER_DEVICE_FAILURE );
        }

        while( cnt-- )
        {
                discrete_input     =   pdu[ idx++ ];
                *data++ =   discrete_input;
        }

        return( MDBS_ERR_NONE );
}

/**
  * @brief      Read Holding Registers 0x03. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Data of requested holding registers
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_read_holding_registers_resp(         const   uint8_t *       pdu,
                                                        uint16_t *      data )
{
                uint16_t        cnt             = pdu[ 1];
                size_t          idx             = 2;
                uint16_t        reg;
        const   size_t          regs_max        = 125;


        if( cnt/2 > regs_max )
        {
                return( MDBS_ERR_SERVER_DEVICE_FAILURE );
        }

        while( cnt != 0 )
        {
                reg     =   pdu[ idx++ ] << 8;
                reg     |=  pdu[ idx++ ] & 0xFF;
                *data++ =   reg;
                cnt -= 2;
        }

        return( MDBS_ERR_NONE );
}

/**
  * @brief      Read Input Registers 0x04. Response. 
  * @param pdu  Modbus PDU data of response
  * @param data Data of requested input registers
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_read_input_registers_resp(     const   uint8_t *       pdu,
                                                        uint16_t *      data )
{
                uint16_t        cnt             = pdu[ 1];
                size_t          idx             = 2;
                uint16_t        reg;
        const   size_t          regs_max        = 125;


        if( cnt/2 > regs_max )
        {
                return( MDBS_ERR_SERVER_DEVICE_FAILURE );
        }

        while( cnt != 0 )
        {
                reg     =   pdu[ idx++ ] << 8;
                reg     |=  pdu[ idx++ ] & 0xFF;
                *data++ =   reg;
                cnt -= 2;
        }

        return( MDBS_ERR_NONE );
}

/**
  * @brief      Write single Coil 0x05. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Output Address 2 bytes + Output Value 2 bytes of writed coil 
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_write_single_coil_resp(      const   uint8_t *       pdu,
                                                       uint16_t *       data )
{
        uint16_t         coil_output_value;

/*        
        coil_output_address = pdu[ idx++ ];
        coil_output_address <<= 8;
        coil_output_address |= pdu[ idx++ ];
        *data++ = coil_output_address;
                
        coil_output_value = pdu[ idx++ ];
        coil_output_value <<= 8;
        coil_output_value |= pdu[ idx++ ];
*/
        
        add_two_var_from_pdu_to_data_resp(pdu, data);
        
        coil_output_value = data[1];
        
        if ((coil_output_value != 0x0000) && (coil_output_value != 0xFF00))
        {
                return( MDBS_ERR_ILLEGAL_DATA_VALUE );          
        }
        
        *data++ = coil_output_value;
        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Write single Register 0x06. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Register Address 2 bytes + Register Value 2 bytes of writed register 
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_write_single_register_resp(  const   uint8_t *       pdu,
                                                       uint16_t *      data )
{      
        add_two_var_from_pdu_to_data_resp(pdu, data);
        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Read Exception Status (Serial Line only) 0x07. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Value 1 byte
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_read_exception_status_resp(  const   uint8_t *       pdu,
                                                       uint8_t *      data )
{
        size_t          idx             = 1;

        *data = pdu[ idx++ ];
                        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Diagnostics (Serial Line only). 08 (0x08). Response 
  * @param pdu  Modbus PDU data of response
  * @param data Sub-function 2 bytes, Data N x 2 Bytes
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_diagnostics_resp(  const   uint8_t *       pdu,
                                             uint16_t *      data )
{
        size_t          idx             = 1;

        *data++ = pdu[ idx++ ];  // Sub-function
        
        switch (pdu[ idx-1 ])
        {
        case MDBS_DIAG_SUBFUNC_RETURN_QUERY_DATA:  /* TODO DATA len maybe ANY*/

        case MDBS_DIAG_SUBFUNC_RESTART_COMM_OPT:  
        case MDBS_DIAG_SUBFUNC_RETURN_DIAG_REG:  
        case MDBS_DIAG_SUBFUNC_CHANGE_ASCII_INP_DELIM:
        case MDBS_DIAG_SUBFUNC_CLEAR_CNTS_DIAG_REG:  
        case MDBS_DIAG_SUBFUNC_RETURN_BUS_MESS_CNT:  
        case MDBS_DIAG_SUBFUNC_RETURN_BUS_COMM_ERR_CNT:
        case MDBS_DIAG_SUBFUNC_RETURN_BUS_EXC_ERR_CNT:  
        case MDBS_DIAG_SUBFUNC_RETURN_SERV_MESS_CNT:  
        case MDBS_DIAG_SUBFUNC_RETURN_SERV_NORSP_CNT:   
        case MDBS_DIAG_SUBFUNC_RETURN_SERV_NAK_CNT:
        case MDBS_DIAG_SUBFUNC_RETURN_SERV_BUSY_CNT:
        case MDBS_DIAG_SUBFUNC_BUS_CHR_OVR_CNT:
        case MDBS_DIAG_SUBFUNC_CLEAR_OVR_CNT_FLG:
          *data++ = pdu[ idx++ ];
        break;
        
        case MDBS_DIAG_SUBFUNC_FORCE_LISTEN_ONLY_MODE:
        break;
 
        }
                        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Get Comm Event Counter (Serial Line only) 0x0B. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Status 2 bytes + Event Count 2 bytes  
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_get_comm_event_counter_resp(  const   uint8_t *       pdu,
                                                        uint16_t *      data )
{  
        add_two_var_from_pdu_to_data_resp(pdu, data);
        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Get Comm Event Counter (Serial Line only) 0x0B. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Status 2 bytes + Event Count 2 bytes + Message Count 2 bytes + Events 
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_get_comm_event_log_resp(  const   uint8_t *       pdu,
                                                    uint16_t *      data )
{
        size_t          idx             = 1;
        uint8_t         byte_cnt = pdu[idx++];
        uint16_t        status;
        uint16_t        event_count;
        uint16_t        message_count;

        status = pdu[ idx++ ];
        status <<= 8;
        status |= pdu[ idx++ ];
        *data++ = status;
                
        event_count = pdu[ idx++ ];
        event_count <<= 8;
        event_count |= pdu[ idx++ ];
                
        *data++ = event_count;
        
        message_count = pdu[ idx++ ];
        message_count <<= 8;
        message_count |= pdu[ idx++ ];
                
        *data++ = message_count;        
               
        memcpy(data, &pdu[idx], (byte_cnt - 6));
        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Write multiple Coils 0x0F. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Coils Address 2 bytes + Quantity of Outputs 2 bytes  
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_write_multiple_coils_resp(  const   uint8_t *       pdu,
                                                      uint16_t *      data )
{    
        add_two_var_from_pdu_to_data_resp(pdu, data);
        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Write multiple Registers 0x10. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Register Address 2 bytes + Register Quantity 2 bytes 
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_write_multiple_registers_resp(  const   uint8_t *       pdu,
                                                          uint16_t *      data )
{
        add_two_var_from_pdu_to_data_resp(pdu, data);
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Report Server ID (Serial Line only) 0x11. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Byte Count + Server ID + Run status + Addition data 
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_reposrt_server_id_resp(  const   uint8_t *       pdu,
                                                   uint8_t *      data )
{
        size_t          idx             = 1;
               
        memcpy(data, &pdu[idx], pdu[idx]);
        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Mask Write Register 0x16. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Reference Address 2 bytes + And_Mask 2 bytes + Or_Mask
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_mask_write_register_resp(  const   uint8_t *       pdu,
                                                     uint16_t *      data )
{
        size_t          idx             = 1;
        uint16_t        ref_address;
        uint16_t        and_mask;
        uint16_t        or_mask;

        ref_address = pdu[ idx++ ];
        ref_address <<= 8;
        ref_address |= pdu[ idx++ ];
        *data++ = ref_address;
                
        and_mask = pdu[ idx++ ];
        and_mask <<= 8;
        and_mask |= pdu[ idx++ ];
                
        *data++ = and_mask;
        
        or_mask = pdu[ idx++ ];
        or_mask <<= 8;
        or_mask |= pdu[ idx++ ];
                
        *data++ = or_mask;

        return( MDBS_ERR_NONE );
}

/**
  * @brief      Read/Write Multiple Registers 0x17. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Read Registers Value
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_read_write_multiple_register_resp(  const   uint8_t *       pdu,
                                                              uint16_t *      data )
{
        size_t          idx             = 1;
        uint8_t         byte_cnt = pdu[idx++];
        uint16_t        register_value;
          
        for (int i = 0; i < (byte_cnt / 2); i++)
        {
          register_value = pdu[idx++];
          register_value <<= 8;
          register_value |= pdu[idx++];
          *data++ = register_value; 
        }
        
        return( MDBS_ERR_NONE );
}

/**
  * @brief      Read/Write Multiple Registers 0x17. Response 
  * @param pdu  Modbus PDU data of response
  * @param data Read Registers Value
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_client_read_fifo_queue_resp(  const   uint8_t *       pdu,
                                                 uint16_t *      data )
{
        size_t          idx             = 1;
        uint16_t        byte_cnt;
        uint16_t        fifo_cnt;
        uint16_t        fifo_value;
        
        byte_cnt = pdu[idx++];
        byte_cnt <<=8;
        byte_cnt |= pdu[idx++];
        
        fifo_cnt = pdu[idx++];
        fifo_cnt <<=8;
        fifo_cnt |= pdu[idx++];        
          
        *data++ = fifo_cnt;
          
        for (int i = 0; i < fifo_cnt; i++)
        {
          fifo_value = pdu[idx++];
          fifo_value <<= 8;
          fifo_value |= pdu[idx++];
          *data++ = fifo_value; 
        }
        
        return( MDBS_ERR_NONE );
}

/*******************************************************************************
* MODBUS RTU CLIENT RESPONSE PARSE (XFER)
*******************************************************************************/
mdbs_err_t
modbus_rtu_client_response_parse(           const   uint8_t         dev_addr,
                                            const   uint8_t *       adu,
                                            const   size_t          cnt,
                                                    uint16_t *      data )
{
        const   uint8_t *       pdu     = adu + MDBS_RTU_DEV_ADDR_SIZEOF;
                mdbs_err_t err = 0;

                if( adu[ 0] != dev_addr )
                {
                  return (mdbs_err_t) -2;  //-2
                }
                
                if( modbus_crc( adu, cnt ) != 0 )
                {
                  return (mdbs_err_t) -1;  //-1
                }

                if( pdu[ 0] & 0x80 )
                {                                                                      
                  return( (mdbs_err_t) pdu[1] ); // Modbus Exception code
                }

                switch( pdu[ 0] )
                {
                        case 0x01: err = modbus_rtu_client_read_coils_resp (             pdu, (uint8_t *)data ); break;
                        case 0x02: err = modbus_rtu_client_read_discrete_inputs_resp (   pdu, (uint8_t *)data ); break;
                        case 0x03: err = modbus_rtu_client_read_holding_registers_resp ( pdu, data ); break;
                        case 0x04: err = modbus_rtu_client_read_input_registers_resp (   pdu, data ); break;
                        case 0x05: err = modbus_rtu_client_write_single_coil_resp (      pdu, data ); break;
                        case 0x06: err = modbus_rtu_client_write_single_register_resp (  pdu, data ); break;
                        case 0x07: err = modbus_rtu_client_read_exception_status_resp( pdu, (uint8_t *)data );       break;
                        case 0x08: err = modbus_rtu_client_diagnostics_resp( pdu, data );       break;
                        case 0x0B: err = modbus_rtu_client_get_comm_event_counter_resp( pdu, data );      break;
                        case 0x0C: err = modbus_rtu_client_get_comm_event_log_resp( pdu, data );       break;
                        case 0x0F: err = modbus_rtu_client_write_multiple_coils_resp(  pdu, data );            break;
                        case 0x10: err = modbus_rtu_client_write_multiple_registers_resp (   pdu, data);       break;
                        case 0x11: err = modbus_rtu_client_reposrt_server_id_resp( pdu, (uint8_t*) data );       break;
                        //case 0x14: len = modbus_rtu_func_read_file_record(      p, pdu );       break;
                        //case 0x15: len = modbus_rtu_func_write_file_record(     p, pdu );       break;
                        case 0x16: err = modbus_rtu_client_mask_write_register_resp( pdu, data );       break;
                        case 0x17: err = modbus_rtu_client_read_write_multiple_register_resp( pdu, data );    break;
                        case 0x18: err = modbus_rtu_client_read_fifo_queue_resp(  pdu, data );       break;
                        //case 0x2B: len = modbus_rtu_func_mei_transport(         p, pdu );       break;
                        default:
                                err = MDBS_ERR_ILLEGAL_FUNCTION; // illegal_function
                                break;
                }

        
        return err ;
}

/*******************************************************************************
* REFERENCE FUNCTION
*******************************************************************************/
/**
  * @brief Read Coils 0x01 
  * @param adu buffer where data will be written for transmission
  * @param dev_addr device address
  * @param addr start coil address
  * @param cnt quantity of coils to read
  */
size_t
modbus_rtu_client_coil_read(                            uint8_t *       adu,
                                                const   uint8_t         dev_addr,
                                                const   uint16_t        addr,
                                                        size_t          cnt )
{
        size_t                  len     = 0;
        uint16_t                crc;


        //adu[ len++ ]    = *( p->addr );
        adu[ len++ ]    = dev_addr;
        adu[ len++ ]    = MDBS_FUNC_READ_COILS;
        adu[ len++ ]    = addr >> 16;
        adu[ len++ ]    = addr & 0xFFFF;
        adu[ len++ ]    = cnt >> 16;
        adu[ len++ ]    = cnt & 0xFFFF;
        crc             = modbus_crc( adu, len );
        adu[ len++ ]    = crc >> 8;
        adu[ len++ ]    = crc & 0xFF;

        return( len );
}




/*******************************************************************************
* OLD FUNCTIONS 
*******************************************************************************/
/*
size_t
modbus_rtu_client_rqst_ireg(            const   uint8_t         dev_addr,
                                                uint8_t *       adu,
                                        const   uint16_t        addr,
                                                size_t          cnt )
{
        size_t                  len     = 0;
        uint16_t                crc;


        //adu[ len++ ]    = *( p->addr );
        adu[ len++ ]    = dev_addr;
        adu[ len++ ]    = MDBS_FUNC_READ_DISCRETE_INPUTS;
        adu[ len++ ]    = addr >> 16;
        adu[ len++ ]    = addr & 0xFFFF;
        adu[ len++ ]    = cnt >> 16;
        adu[ len++ ]    = cnt & 0xFFFF;
        crc             = mdbs_crc( adu, len );
        adu[ len++ ]    = crc >> 8;
        adu[ len++ ]    = crc & 0xFF;

        return( len );
}
*/

size_t
modbus_rtu_client_rqst(                 uint8_t *       adu,
                                const   uint8_t         dev_addr,
                                const   mdbs_func_t     func,
                                const   uint16_t        reg_addr,
                                const   size_t          regs_cnt )
{
        size_t                  len     = 0;
        uint16_t                crc;


        adu[ len++ ]    = dev_addr;
        adu[ len++ ]    = func;
        adu[ len++ ]    = (reg_addr + 0) >> 8;
        adu[ len++ ]    = (reg_addr + 0) & 0xFF;
        adu[ len++ ]    = regs_cnt >> 8;
        adu[ len++ ]    = regs_cnt & 0xFF;
        crc             = modbus_crc( adu, len );
        adu[ len++ ]    = crc >> 8;
        adu[ len++ ]    = crc & 0xFF;

        return( len );
}


size_t
modbus_rtu_client_ireg_rqst(                    uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        reg_addr,
                                        const   size_t          regs_cnt )
{
        size_t                  len     = 0;
        uint16_t                crc;


        adu[ len++ ]    = dev_addr;
        adu[ len++ ]    = MDBS_FUNC_READ_INPUT_REGISTERS;
        adu[ len++ ]    = (reg_addr + 0) >> 8;
        adu[ len++ ]    = (reg_addr + 0) & 0xFF;
        adu[ len++ ]    = regs_cnt >> 8;
        adu[ len++ ]    = regs_cnt & 0xFF;
        crc             = modbus_crc( adu, len );
        adu[ len++ ]    = crc >> 8;
        adu[ len++ ]    = crc & 0xFF;

        return( len );
}


size_t
modbus_rtu_client_hreg_rqst(                    uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                        const   size_t          cnt )
{
        size_t                  len     = 0;
        uint16_t                crc;


        //adu[ len++ ]    = *( p->addr );
        adu[ len++ ]    = dev_addr;
        adu[ len++ ]    = MDBS_FUNC_READ_HOLDING_REGISTERS;
        adu[ len++ ]    = (addr + 0) >> 8;
        adu[ len++ ]    = (addr + 0) & 0xFF;
        adu[ len++ ]    = cnt >> 8;
        adu[ len++ ]    = cnt & 0xFF;
        crc             = modbus_crc( adu, len );
        adu[ len++ ]    = crc >> 8;
        adu[ len++ ]    = crc & 0xFF;

        return( len );
}


/**
  * @brief      Function Code 16 (0x10) Write Multiple registers
  * @retval     Error status
  */
size_t
//modbus_rtu_master_write_multiple_registers(     const   modbus_t *      p,
modbus_rtu_client_write_multiple_registers(     const   uint8_t         dev_addr,
                                                        uint8_t *       adu,
                                                const   uint16_t        addr,
                                                const   size_t          cnt_regs,
                                                const   uint16_t *      data )
{
        uint16_t        crc;
        size_t          len             = 0;
        size_t          cnt_bytes       = cnt_regs * 2;


        //adu[ len++ ]    = *( p->addr );
        adu[ len++ ]    = dev_addr;
        adu[ len++ ]    = MDBS_FUNC_WRITE_MULTIPLE_REGISTERS;
        adu[ len++ ]    = (addr + 0) >> 8;
        adu[ len++ ]    = (addr + 0) & 0xFF;
        adu[ len++ ]    = cnt_regs >> 8;
        adu[ len++ ]    = cnt_regs & 0xFF;
        adu[ len++ ]    = cnt_bytes;

        for( int i = 0; i < cnt_regs; i++ )
        {
                adu[ len++ ]    = *(data + i) >> 8;
                adu[ len++ ]    = *(data + i) & 0xFF;
        }

        crc             = modbus_crc( adu, len );
        adu[ len++ ]    = crc >> 8;
        adu[ len++ ]    = crc & 0xFF;

        return( len );
}


/**
  * @brief      Function Code 03 (0x03) Read Holding Registers
  * @retval     Error status
  */
static
mdbs_err_t
modbus_rtu_resp_fc03(                           const   uint8_t *       pdu,
                                                        uint16_t *      data )
{
                uint16_t        cnt             = pdu[ 1];
                size_t          idx             = 2;
                uint16_t        reg;
        const   size_t          regs_max        = 125;


        if( cnt/2 > regs_max )
        {
                return( MDBS_ERR_SERVER_DEVICE_FAILURE );
        }

        while( cnt != 0 )
        {
                reg     =   pdu[ idx++ ] << 8;
                reg     |=  pdu[ idx++ ] & 0xFF;
                *data++ =   reg;
                cnt -= 2; 
        }

        return( MDBS_ERR_NONE );
}


static
mdbs_err_t
modbus_rtu_resp_fc04(                           const   uint8_t *       pdu,
                                                        uint16_t *      data )
{
                uint16_t        cnt             = pdu[ 1];
                size_t          idx             = 2;
                uint16_t        reg;
        const   size_t          regs_max        = 125;


        if( cnt/2 > regs_max )
        {
                return( MDBS_ERR_SERVER_DEVICE_FAILURE );
        }

        while( cnt != 0 )
        {
                reg     =   pdu[ idx++ ] << 8;
                reg     |=  pdu[ idx++ ] & 0xFF;
                *data++ =   reg;
                cnt -= 2;
        }

        return( MDBS_ERR_NONE );
}


size_t
//modbus_rtu_xfer(                                const   modbus_t *      p,
mdbs_rtu_xfer(                                  const   uint8_t         dev_addr,
                                                const   uint8_t *       adu,
                                                const   size_t          cnt,
                                                        uint16_t *      data )
{
        const   uint8_t *       pdu     = adu + MDBS_RTU_DEV_ADDR_SIZEOF;
                size_t          len     = 0;
                //modbus_err_t    err;

        //printf( "adu[ 0] = %d\n", adu[ 0] );
        //printf( "*(p->addr) = %d\n", *(p->addr) );
        //printf( "pdu[ 0] = %d\n", pdu[ 0] );
        //printf( "cnt = %d\n", cnt );

        //if( adu[ 0] == *(p->addr) )
        if( adu[ 0] == dev_addr )
        {
                if( modbus_crc( adu, cnt ) != 0 )
                {
                        return( MDBS_RTU_RESP_LENGTH_NONE );
                }

                if( pdu[ 0] & 0x80 )
                {
                        //err   = mdbs_err();
                        return( MDBS_RTU_RESP_LENGTH_NONE );
                }

                switch( pdu[ 0] )
                {
                        //case 0x01: len = modbus_rtu_func_read_coils(            p, pdu );       break;
                        //case 0x02: len = modbus_rtu_func_read_discrete_inputs(  p, pdu );       break;
                        //case 0x03: err = modbus_rtu_resp_fc03( p, pdu, data );  break;
                        case 0x03: len = modbus_rtu_resp_fc03( pdu, data );  break;
                        case 0x04: len = modbus_rtu_resp_fc04( pdu, data );  break;
                        //case 0x05: len = modbus_rtu_func_write_single_coil(     p, pdu );       break;
                        //case 0x06: len = modbus_rtu_func_write_single_reg(      p, pdu );       break;
                        //case 0x07: len = modbus_rtu_func_read_exception_status( p, pdu );       break;
                        //case 0x08: len = modbus_rtu_func_diagnostics(           p, pdu );       break;
                        //case 0x0B: len = modbus_rtu_func_get_comm_event_counter( p, pdu );      break;
                        //case 0x0C: len = modbus_rtu_func_get_comm_event_log(    p, pdu );       break;
                        //case 0x0F: len = modbus_rtu_func_write_multiple_coils(  p, pdu );       break;
                        //case 0x10: len = modbus_rtu_func_write_multiple_regs(   p, pdu );       break;
                        //case 0x11: len = modbus_rtu_func_report_server_id(      p, pdu );       break;
                        //case 0x14: len = modbus_rtu_func_read_file_record(      p, pdu );       break;
                        //case 0x15: len = modbus_rtu_func_write_file_record(     p, pdu );       break;
                        //case 0x16: len = modbus_rtu_func_mask_write_register(   p, pdu );       break;
                        //case 0x17: len = modbus_rtu_func_read_write_multiple_regs( p, pdu );    break;
                        //case 0x18: len = modbus_rtu_func_read_fifo_queue(       p, pdu );       break;
                        //case 0x2B: len = modbus_rtu_func_mei_transport(         p, pdu );       break;
                        default:
                                //len = modbus_rtu_error( pdu, MODBUS_ERR_ILLEGAL_FUNCTION );
                                break;
                }

        }
        else
        {
                len     = MDBS_RTU_RESP_LENGTH_NONE;
        }

        return( len );
}

