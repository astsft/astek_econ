/**
  * @file    modbus.h
  * @brief   MODBUS protocol header
  * @author  AST Soft <info@ast-soft.ru>
  */


#ifndef MODBUS_H
#define MODBUS_H


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>




//#define MDBS_TBL_OFFSET_MAX                     9998    //0x270E
#define MDBS_ADDR_BROADCAST                     0

#define MDBS_ASCII_ADU_SIZEOF                   513

#define MDBS_RTU_ADU_SIZEOF                     256
#define MDBS_RTU_DEV_ADDR_SIZEOF                1
#define MDBS_RTU_RESP_LENGTH_NONE               0
#define MDBS_RTU_RESP_LENGTH_EXCEPTION          3
#define MDBS_RTU_RESP_LENGTH_FUNC_05            5
#define MDBS_RTU_RESP_LENGTH_FUNC_06            5
#define MDBS_RTU_RESP_LENGTH_FUNC_16            5


typedef enum    mdbs_func_e
{
    MDBS_FUNC_READ_COILS                        = 0x01,
    MDBS_FUNC_READ_DISCRETE_INPUTS              = 0x02,
    MDBS_FUNC_READ_HOLDING_REGISTERS            = 0x03,
    MDBS_FUNC_READ_INPUT_REGISTERS              = 0x04,
    MDBS_FUNC_WRITE_SINGLE_COIL                 = 0x05,
    MDBS_FUNC_WRITE_SINGLE_REGISTER             = 0x06,
    MDBS_FUNC_READ_EXCEPTION_STATUS             = 0x07,
    MDBS_FUNC_DIAGNOSTICS                       = 0x08,
    MDBS_FUNC_GET_COMM_EVENT_COUNTER            = 0x0B,
    MDBS_FUNC_GET_COMM_EVENT_LOG                = 0x0C,
    MDBS_FUNC_WRITE_MULTIPLE_COILS              = 0x0F,
    MDBS_FUNC_WRITE_MULTIPLE_REGISTERS          = 0x10,
    MDBS_FUNC_REPORT_SERVER_ID                  = 0x11,
    MDBS_FUNC_READ_FILE_RECORD                  = 0x14,
    MDBS_FUNC_WRITE_FILE_RECORD                 = 0x15,
    MDBS_FUNC_MASK_WRITE_REGISTER               = 0x16,
    MDBS_FUNC_READ_WRITE_MULTIPLE_REGISTERS     = 0x17,
    MDBS_FUNC_READ_FIFO_QUEUE                   = 0x18,
    MDBS_FUNC_ENCAPSULATED_INTERFACE_TRANSPORT  = 0x2B,
    MDBS_FUNC_CANOPEN_GENERAL_REFERENCE         = 0x2B,
} mdbs_func_t;
        
typedef enum mdbs_diag_subfunc_e 
{
    MDBS_DIAG_SUBFUNC_RETURN_QUERY_DATA         = 0x00,
    MDBS_DIAG_SUBFUNC_RESTART_COMM_OPT          = 0x01,  
    MDBS_DIAG_SUBFUNC_RETURN_DIAG_REG           = 0x02,
    MDBS_DIAG_SUBFUNC_CHANGE_ASCII_INP_DELIM    = 0x03,
    MDBS_DIAG_SUBFUNC_FORCE_LISTEN_ONLY_MODE    = 0x04,        
    /* 05...09 RESERVED*/        
    MDBS_DIAG_SUBFUNC_CLEAR_CNTS_DIAG_REG       = 0x0A,        
    MDBS_DIAG_SUBFUNC_RETURN_BUS_MESS_CNT       = 0x0B,        
    MDBS_DIAG_SUBFUNC_RETURN_BUS_COMM_ERR_CNT   = 0x0C,      
    MDBS_DIAG_SUBFUNC_RETURN_BUS_EXC_ERR_CNT    = 0x0D,                
    MDBS_DIAG_SUBFUNC_RETURN_SERV_MESS_CNT      = 0x0E,                
    MDBS_DIAG_SUBFUNC_RETURN_SERV_NORSP_CNT     = 0x0F,                        
    MDBS_DIAG_SUBFUNC_RETURN_SERV_NAK_CNT       = 0x10,
    MDBS_DIAG_SUBFUNC_RETURN_SERV_BUSY_CNT      = 0x11,        
    MDBS_DIAG_SUBFUNC_BUS_CHR_OVR_CNT           = 0x12,
    /* 19 RESERVED*/
    MDBS_DIAG_SUBFUNC_CLEAR_OVR_CNT_FLG         = 0x14,
} mdbs_diag_subfunc_t;  

typedef enum mdbs_mei_type_e 
{
  MDBS_MEI_TYPE_CANOPEN_GENERAL_REFERENCE                       = 0x0D,
  MDBS_MEI_TYPE_READ_DEVICE_ID                                  = 0x0E
} mdbs_mei_type_t;


typedef enum    mdbs_err_e
{
        MDBS_ERR_NONE                                           = 0x00,
        MDBS_ERR_ILLEGAL_FUNCTION                               = 0x01,
        MDBS_ERR_ILLEGAL_DATA_ADDRESS                           = 0x02,
        MDBS_ERR_ILLEGAL_DATA_VALUE                             = 0x03,
        MDBS_ERR_SERVER_DEVICE_FAILURE                          = 0x04,
        MDBS_ERR_ACKNOWLEDGE                                    = 0x05,
        MDBS_ERR_SERVER_DEVICE_BUSY                             = 0x06,
        MDBS_ERR_MEMORY_PARITY_ERROR                            = 0x08,
        MDBS_ERR_GATEWAY_PATH_UNAVAILABLE                       = 0x0A,
        MDBS_ERR_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND        = 0x0B,
} mdbs_err_t;

typedef enum mdbs_coil_state_e
{
        MBS_COIL_STATE_ON                                       = 0xFF00,
        MBS_COIL_STATE_OFF                                      = 0x0000,
} mdbs_coil_state_t;


typedef enum    mdbs_ctl_e
{
        MDBS_CTL_SET_DEVICE_ADDR,
} mdbs_ctl_t;



/*******************************************************************************
* MODBUS MODULE STRUCTURES
*******************************************************************************/
typedef struct {
size_t
(* modbus_rtu_client_diagnostics_rqst)(              uint8_t *  adu,
                                    const   uint8_t    dev_addr,
                                            uint16_t sub_function,
                                            uint8_t * data,
                                            uint32_t data_cnt );

size_t
(* modbus_rtu_client_diagnostics_return_query_data_rqst)(          uint8_t *  adu,
                                                  const   uint8_t    dev_addr,
                                                          uint8_t * data,
                                                          uint32_t data_cnt );

size_t
(* modbus_rtu_client_diagnostics_restart_comm_options_with_log_rqst) (     uint8_t *  adu,
                                                          const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_restart_comm_options_without_log_rqst) ( uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_diag_reg_rqst) ( uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_change_ascii_delimiter_rqst) (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr,
                                                          uint8_t    ascii_delim );

size_t
(* modbus_rtu_client_diagnostics_force_listen_only_mode_rqst) (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_clear_cnts_diag_reg_rqst) (       uint8_t *  adu,
                                                  const   uint8_t    dev_addr );  

size_t
(* modbus_rtu_client_read_exception_status_rqst) (         uint8_t *  adu,
                                         const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_mess_cnt_rqst) (           uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_comm_error_cnt_rqst) (     uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_exc_error_cnt_rqst) (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_serv_mess_cnt_rqst) (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_serv_norsp_cnt_rqst) (     uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_serv_nak_cnt_rqst) (       uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_serv_busy_cnt_rqst) (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_return_char_overrun_cnt_rqst) (   uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_diagnostics_clear_overrun_cnt_flag_rqst) (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr );
}  client_diagnostic_s;

typedef struct {
size_t
(* modbus_rtu_client_read_coils_rqst) (           uint8_t *       adu,
                                const   uint8_t         dev_addr,
                                const   uint16_t        addr,
                                        size_t          cnt );

size_t
(* modbus_rtu_client_read_discrete_inputs_rqst)(                 uint8_t *       adu,
                                                const   uint8_t         dev_addr,
                                                const   uint16_t        addr,
                                                        size_t          cnt );

size_t
(* modbus_rtu_client_read_holding_registers_rqst) (         uint8_t *       adu,
                                          const   uint8_t         dev_addr,
                                          const   uint16_t        addr,
                                                  size_t          cnt );

size_t
(* modbus_rtu_client_read_input_registers_rqst)(                 uint8_t *       adu,
                                                const   uint8_t         dev_addr,
                                                const   uint16_t        addr,
                                                        size_t          cnt );

size_t
(* modbus_rtu_client_write_single_coil_rqst)(            uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                uint16_t        value );

size_t
(* modbus_rtu_client_write_single_register_rqst) (        uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                uint16_t        value );  
  
client_diagnostic_s *client_diagnostic_rqst;

size_t
(* modbus_rtu_client_get_comm_event_cnt_rqst) (             uint8_t *  adu,
                                           const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_get_comm_event_log_rqst) (             uint8_t *  adu,
                                           const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_write_multiple_coils_rqst) (           uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint16_t   addr, 
                                                   uint16_t   cnt, 
                                                   uint8_t * outputs_value );

size_t
(* modbus_rtu_client_write_multiple_registers_rqst) (  uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint16_t   addr, 
                                                   uint16_t   cnt, 
                                                   uint8_t * outputs_value );

size_t
(* modbus_rtu_client_report_server_id_rqst) (               uint8_t *  adu,
                                           const   uint8_t    dev_addr );

size_t
(* modbus_rtu_client_read_file_record_rqst) (               uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   byte_cnt, 
                                                   uint8_t   ref_type, 
                                                   uint16_t  file_number,
                                                   uint16_t  record_number,
                                                   uint16_t  record_len);

size_t
(* modbus_rtu_client_write_file_record_rqst) (              uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   rqst_data_len, 
                                                   uint8_t   ref_type, 
                                                   uint16_t  file_number,
                                                   uint16_t  record_number,
                                                   uint16_t  record_len,
                                                   uint8_t * data );

size_t
(* modbus_rtu_client_mask_write_register_rqst) (            uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   ref_addr, 
                                                   uint16_t  and_mask,
                                                   uint16_t  or_mask );

size_t
(* modbus_rtu_client_read_write_multiple_registers_rqst) (       uint8_t *  adu,
                                                const   uint8_t    dev_addr,
                                                        uint16_t   read_addr, 
                                                        uint16_t   read_qnt,
                                                        uint16_t   write_addr, 
                                                        uint16_t   write_qnt,
                                                        uint16_t   write_byte_cnt,
                                                        uint8_t *  registers_value );

size_t
(* modbus_rtu_client_read_fifo_queue_rqst) (                uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                           const   uint16_t   fifo_addr );

size_t
(* modbus_rtu_client_enc_int_transp_can_gen_ref_rqst) (                 uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                           const   uint8_t *  data,
                                                   uint8_t    bytes_cnt );

size_t
(* modbus_rtu_client_enc_int_transp_read_dev_id_rqst) (     uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t    dev_id_code,
                                                   uint8_t    object_id );
  
mdbs_err_t
(* modbus_rtu_client_response_parse) ( const   uint8_t,
                                                  const   uint8_t *,
                                                  const   size_t,
                                                          uint16_t *);
} modbus_client_s;

typedef struct {
  uint8_t todo;  
} modbus_server_s;

typedef struct {
  modbus_client_s *modbus_client;
  modbus_server_s *modbus_server;
} modbus_module_s;

extern modbus_module_s modbus_module;

/*******************************************************************************
* Modbus Function prototypes
*******************************************************************************/
uint16_t    modbus_crc( const uint8_t *, size_t );
char        modbus_lrc( const char * );

size_t
mdbs_rtu_slave(                                 const   uint8_t         dev_addr,
                                                        uint8_t *       raw,
                                                        size_t          len_rqst );

/*******************************************************************************
* Modbus Device Function prototypes
*******************************************************************************/
mdbs_err_t
mdbs_coil_read(                                 const   size_t          addr,
                                                        uint16_t *      data );

mdbs_err_t
mdbs_coil_write(                                const   size_t          idx,
                                                const   uint16_t *      data );

mdbs_err_t
mdbs_dinp_read(                                 const   size_t          idx,
                                                        uint16_t *      data );

mdbs_err_t
mdbs_dinp_write(                                const   size_t          idx,
                                                        uint16_t *      data );

mdbs_err_t
mdbs_ireg_read(                                 const   size_t          idx,
                                                        uint16_t *      data );

mdbs_err_t
mdbs_ireg_write(                                const   size_t          idx,
                                                const   uint16_t *      data );

mdbs_err_t
mdbs_hreg_read(                                 const   size_t          idx,
                                                        uint16_t *      data );

mdbs_err_t
mdbs_hreg_write(                                const   size_t          idx,
                                                const   uint16_t *      data );


/*******************************************************************************
* Modbus RTU Master related
*******************************************************************************/
size_t
modbus_rtu_client_coil_read(                   uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                size_t          cnt );

size_t
modbus_rtu_client_rqst(                         uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   mdbs_func_t     func,
                                        const   uint16_t        reg_addr,
                                        const   size_t          regs_cnt );

size_t
modbus_rtu_client_ireg_rqst(                    uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                size_t          cnt );

size_t
modbus_rtu_client_hreg_rqst(                    uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                        const   size_t          cnt );

size_t
modbus_rtu_client_write_multiple_registers(     const   uint8_t         dev_addr,
                                                        uint8_t *       adu,
                                                const   uint16_t        addr,
                                                const   size_t          cnt_regs,
                                                const   uint16_t *      data );


size_t
mdbs_rtu_xfer(                                  const   uint8_t         dev_addr,
                                                const   uint8_t *       adu,
                                                const   size_t          cnt,
                                                        uint16_t *      data );

/*******************************************************************************
*
*******************************************************************************/
size_t
mdbs_mei_read_device_identification(                    uint8_t *       pdu );


/*******************************************************************************
* NEW FUNCTIONS
*******************************************************************************/
size_t
modbus_rtu_client_read_coils_rqst(           uint8_t *       adu,
                                const   uint8_t         dev_addr,
                                const   uint16_t        addr,
                                        size_t          cnt );

size_t
modbus_rtu_client_read_discrete_inputs_rqst(                 uint8_t *       adu,
                                                const   uint8_t         dev_addr,
                                                const   uint16_t        addr,
                                                        size_t          cnt );

size_t
modbus_rtu_client_read_holding_registers_rqst(         uint8_t *       adu,
                                          const   uint8_t         dev_addr,
                                          const   uint16_t        addr,
                                                  size_t          cnt );

size_t
modbus_rtu_client_read_input_registers_rqst(                 uint8_t *       adu,
                                                const   uint8_t         dev_addr,
                                                const   uint16_t        addr,
                                                        size_t          cnt );

size_t
modbus_rtu_client_write_single_coil_rqst(            uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                uint16_t        value );

size_t
modbus_rtu_client_write_single_register_rqst(        uint8_t *       adu,
                                        const   uint8_t         dev_addr,
                                        const   uint16_t        addr,
                                                uint16_t        value );

size_t
modbus_rtu_client_diagnostics_rqst(              uint8_t *  adu,
                                    const   uint8_t    dev_addr,
                                            uint16_t sub_function,
                                            uint8_t * data,
                                            uint32_t data_cnt );

size_t
modbus_rtu_client_diagnostics_return_query_data_rqst(          uint8_t *  adu,
                                                  const   uint8_t    dev_addr,
                                                          uint8_t * data,
                                                          uint32_t data_cnt );

size_t
modbus_rtu_client_diagnostics_restart_comm_options_with_log_rqst (     uint8_t *  adu,
                                                          const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_restart_comm_options_without_log_rqst ( uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_diag_reg_rqst ( uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_change_ascii_delimiter_rqst (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr,
                                                          uint8_t    ascii_delim );

size_t
modbus_rtu_client_diagnostics_force_listen_only_mode_rqst (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_clear_cnts_diag_reg_rqst (       uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_read_exception_status_rqst(         uint8_t *  adu,
                                         const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_mess_cnt_rqst (           uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_comm_error_cnt_rqst (     uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_exc_error_cnt_rqst (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_serv_mess_cnt_rqst (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_serv_norsp_cnt_rqst (     uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_serv_nak_cnt_rqst (       uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_serv_busy_cnt_rqst (      uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_return_char_overrun_cnt_rqst (   uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_diagnostics_clear_overrun_cnt_flag_rqst (    uint8_t *  adu,
                                                  const   uint8_t    dev_addr );

size_t
modbus_rtu_client_get_comm_event_cnt_rqst (             uint8_t *  adu,
                                           const   uint8_t    dev_addr );

size_t
modbus_rtu_client_get_comm_event_log_rqst (             uint8_t *  adu,
                                           const   uint8_t    dev_addr );

size_t
modbus_rtu_client_write_multiple_coils_rqst (           uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint16_t   addr, 
                                                   uint16_t   cnt, 
                                                   uint8_t * outputs_value );

size_t
modbus_rtu_client_write_multiple_registers_rqst (  uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint16_t   addr, 
                                                   uint16_t   cnt, 
                                                   uint8_t * outputs_value );

size_t
modbus_rtu_client_report_server_id_rqst (               uint8_t *  adu,
                                           const   uint8_t    dev_addr );

size_t
modbus_rtu_client_read_file_record_rqst (               uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   byte_cnt, 
                                                   uint8_t   ref_type, 
                                                   uint16_t  file_number,
                                                   uint16_t  record_number,
                                                   uint16_t  record_len);

size_t
modbus_rtu_client_write_file_record_rqst (              uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   rqst_data_len, 
                                                   uint8_t   ref_type, 
                                                   uint16_t  file_number,
                                                   uint16_t  record_number,
                                                   uint16_t  record_len,
                                                   uint8_t * data );

size_t
modbus_rtu_client_mask_write_register_rqst (            uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t   ref_addr, 
                                                   uint16_t  and_mask,
                                                   uint16_t  or_mask );

size_t
modbus_rtu_client_read_write_multiple_registers_rqst (       uint8_t *  adu,
                                                const   uint8_t    dev_addr,
                                                        uint16_t   read_addr, 
                                                        uint16_t   read_qnt,
                                                        uint16_t   write_addr, 
                                                        uint16_t   write_qnt,
                                                        uint16_t   write_byte_cnt,
                                                        uint8_t *  registers_value );

size_t
modbus_rtu_client_read_fifo_queue_rqst (                uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                           const   uint16_t   fifo_addr );

size_t
modbus_rtu_client_enc_int_transp_can_gen_ref_rqst (                 uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                           const   uint8_t *  data,
                                                   uint8_t    bytes_cnt );

size_t
modbus_rtu_client_enc_int_transp_read_dev_id_rqst (     uint8_t *  adu,
                                           const   uint8_t    dev_addr,
                                                   uint8_t    dev_id_code,
                                                   uint8_t    object_id );

mdbs_err_t
modbus_rtu_client_response_parse(             const   uint8_t         dev_addr,
                                            const   uint8_t *       adu,
                                            const   size_t          cnt,
                                                    uint16_t *      data );



#endif  //MODBUS_H
