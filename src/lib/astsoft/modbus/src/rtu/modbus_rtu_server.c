/**
  * @file    modbus_rtu_server.c
  * @brief   MODBUS RTU Slave routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "modbus.h"


/**
  * @brief      Set error in raw buffer
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_error(                                 uint8_t *               pdu,
                                        const   mdbs_err_t              err )
{
        pdu[ 0]         |=  0x80;
        pdu[ 1]         = (uint8_t) err;

        return( MDBS_RTU_RESP_LENGTH_EXCEPTION );
}


/**
  * @brief      01 (0x01) Read Coils
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_coils(                       uint8_t *               pdu )
{
        mdbs_err_t      err;
        size_t          len             = 1;
        uint16_t        reg;
        uint16_t        addr            = (pdu[ 1] << 8) | (pdu[ 2] & 0xFF );
        uint16_t        regs_cnt        = (pdu[ 3] << 8) | (pdu[ 4] & 0xFF );


        pdu[ len++ ]    = regs_cnt*2;        //Set responce length

        while( regs_cnt-- )
        {
                err     = mdbs_coil_read( addr++, &reg );

                if( err != MDBS_ERR_NONE )
                {
                        len     = mdbs_rtu_error( pdu, err );
                        return( len );
                }

                pdu[ len++ ]    = reg >> 8;
                pdu[ len++ ]    = reg & 0xFF;
        }

        return( len );
}


/**
  * @brief      02 (0x02) Read Discrete Inputs
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_discrete_inputs(             uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      03 (0x03) Read Holding Registers
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_holding_regs(                uint8_t *               pdu )
{
        mdbs_err_t      err;
        size_t          len             = 1;
        uint16_t        reg;
        uint16_t        addr            = (pdu[ 1] << 8) | (pdu[ 2] & 0xFF );
        uint16_t        regs_cnt        = (pdu[ 3] << 8) | (pdu[ 4] & 0xFF );


        pdu[ len++ ]    = regs_cnt*2;        //Set responce length

        while( regs_cnt-- )
        {
                err     = mdbs_hreg_read( addr++, &reg );

                if( err != MDBS_ERR_NONE )
                {
                        len     = mdbs_rtu_error( pdu, err );
                        return( len );
                }

                pdu[ len++ ]    = reg >> 8;
                pdu[ len++ ]    = reg & 0xFF;
        }

        return( len );
}


/**
  * @brief      04 (0x04) Read Input Registers
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_input_regs(                  uint8_t *               pdu )
{
        mdbs_err_t      err;
        size_t          len             = 1;
        uint16_t        reg;
        uint16_t        addr            = (pdu[ 1] << 8) | (pdu[ 2] & 0xFF );
        uint16_t        regs_cnt        = (pdu[ 3] << 8) | (pdu[ 4] & 0xFF );


        pdu[ len++ ]    = regs_cnt*2;        //Set responce length

        while( regs_cnt-- )
        {
                err     = mdbs_ireg_read( addr++, &reg );

                if( err != MDBS_ERR_NONE )
                {
                        len     = mdbs_rtu_error( pdu, err );
                        return( len );
                }

                pdu[ len++ ]    = reg >> 8;
                pdu[ len++ ]    = reg & 0xFF;
        }

        return( len );
}


/**
  * @brief      05 (0x05) Write Single Coil
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_write_single_coil(                uint8_t *               pdu )
{
        mdbs_err_t      err;
        uint16_t        addr            = (pdu[ 1] << 8) | (pdu[ 2] & 0xFF );
        uint16_t        reg             = (pdu[ 3] << 8) | (pdu[ 4] & 0xFF );
        size_t          len             = MDBS_RTU_RESP_LENGTH_FUNC_05;


        err     = mdbs_coil_write( addr, &reg );


        if( err != MDBS_ERR_NONE )
        {
                len     = mdbs_rtu_error( pdu, err );
                return( len );
        }

        //pdu[ len++ ]    = reg >> 8;
        //pdu[ len++ ]    = reg & 0xFF;

        return( len );
}


/**
  * @brief      06 (0x06) Write Single Register
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_write_single_reg(                 uint8_t *               pdu )
{
        mdbs_err_t      err;
        uint16_t        addr    = (pdu[ 1] << 8) | (pdu[ 2] & 0xFF );
        uint16_t        reg     = (pdu[ 3] << 8) | (pdu[ 4] & 0xFF );
        size_t          len             = MDBS_RTU_RESP_LENGTH_FUNC_06;


        err     = mdbs_hreg_write( addr, &reg );

        if( err != MDBS_ERR_NONE )
        {
                len     = mdbs_rtu_error( pdu, err );
                return( len );
        }

        return( len );
}


/**
  * @brief      07 (0x07) Read Exception Status (Serial Line only)
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_exception_status(            uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      08 (0x08) Diagnostics (Serial Line only)
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_diagnostics(                      uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      11 (0x0B) Get Comm Event Counter (Serial Line only)
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_get_comm_event_counter(           uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      12 (0x0C) Get Comm Event Log
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_get_comm_event_log(               uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      15 (0x0F) Write Multiple Coils
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_write_multiple_coils(             uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      16 (0x10) Write Multiple registers
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_write_multiple_regs(              uint8_t *               pdu )
{
        mdbs_err_t      err;
        size_t          idx             = 6;    //pdu data starting index to write
        uint16_t        reg;
        uint16_t        addr            = (pdu[ 1] << 8) | (pdu[ 2] & 0xFF );
        uint16_t        regs_cnt        = (pdu[ 3] << 8) | (pdu[ 4] & 0xFF );
        size_t          len             = MDBS_RTU_RESP_LENGTH_FUNC_16;


        while( regs_cnt-- )
        {
                reg     =  pdu[ idx++ ] << 8;
                reg     |= pdu[ idx++ ] & 0xFF;

                err     = mdbs_hreg_write( addr++, &reg );

                if( err != MDBS_ERR_NONE )
                {
                        len     = mdbs_rtu_error( pdu, err );
                        return( len );
                }
        }

        return( len );
}


/**
  * @brief      17 (0x11) Report Server ID (Serial Line only)
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_report_server_id(                 uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      20 (0x14) Read File Record
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_file_record(                 uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      21 (0x15) Write File Record
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_write_file_record(                uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      22 (0x16) Mask Write Register
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_mask_write_register(              uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      23 (0x17) Read/Write Multiple registers
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_write_multiple_regs(         uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      24 (0x18) Read FIFO Queue
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_read_fifo_queue(                  uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      43/13 (0x2B/0x0D) CANopen General Reference Request and Response PDU
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_func_canopen_general_rqst_resp_pdu(    uint8_t *               pdu )
{
        return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      43/14 (0x2B/0x0E) Read Device Identification
  * @retval     Raw request length
  */
__weak
//static
size_t
mdbs_mei_read_device_identification(       uint8_t *               pdu )
{
    return( mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION ) );
}


/**
  * @brief      43 (0x2B) Encapsulated Interface Transport
  * @retval     Raw request length
  * @note       MEI = MODBUS Encapsulated Interface
  */
static
size_t
mdbs_rtu_func_mei_transport(                    uint8_t *               pdu )
{
        const   uint8_t *       mei_type        = &pdu[ 1];
                size_t          len;


        switch( *mei_type )
        {
                case 0x0D:
                        len     = mdbs_rtu_func_canopen_general_rqst_resp_pdu( pdu );
                        break;

                case 0x0E:
                        len     = mdbs_mei_read_device_identification( pdu );
                        break;

                default:
                        mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION );
                        len     = MDBS_RTU_RESP_LENGTH_EXCEPTION ;
        }

        return( len );
}


/**
  * @brief      0 (0x00) Broadcast
  * @retval     Raw request length
  */
static
size_t
mdbs_rtu_broadcast(                             uint8_t *               pdu )
{
        return( MDBS_RTU_RESP_LENGTH_NONE );
}


/**
  * @brief      Modbus RTU slave entry
  * @retval     Raw request length
  */
size_t
mdbs_rtu_slave(                         const   uint8_t                 dev_addr,
                                                uint8_t *               adu,
                                                size_t                  cnt )
{
        uint8_t *               pdu     = adu + MDBS_RTU_DEV_ADDR_SIZEOF;
        size_t                  len;
        uint16_t                crc;


        if( modbus_crc( adu, cnt ) != 0 )
        {
                return( MDBS_RTU_RESP_LENGTH_NONE );
        }

        if( adu[ 0] == dev_addr )
        {
                switch( pdu[ 0] )
                {
                        case 0x01: len = mdbs_rtu_func_read_coils(            pdu );    break;
                        case 0x02: len = mdbs_rtu_func_read_discrete_inputs(  pdu );    break;
                        case 0x03: len = mdbs_rtu_func_read_holding_regs(     pdu );    break;
                        case 0x04: len = mdbs_rtu_func_read_input_regs(       pdu );    break;
                        case 0x05: len = mdbs_rtu_func_write_single_coil(     pdu );    break;
                        case 0x06: len = mdbs_rtu_func_write_single_reg(      pdu );    break;
                        case 0x07: len = mdbs_rtu_func_read_exception_status( pdu );    break;
                        case 0x08: len = mdbs_rtu_func_diagnostics(           pdu );    break;
                        case 0x0B: len = mdbs_rtu_func_get_comm_event_counter( pdu );   break;
                        case 0x0C: len = mdbs_rtu_func_get_comm_event_log(    pdu );    break;
                        case 0x0F: len = mdbs_rtu_func_write_multiple_coils(  pdu );    break;
                        case 0x10: len = mdbs_rtu_func_write_multiple_regs(   pdu );    break;
                        case 0x11: len = mdbs_rtu_func_report_server_id(      pdu );    break;
                        case 0x14: len = mdbs_rtu_func_read_file_record(      pdu );    break;
                        case 0x15: len = mdbs_rtu_func_write_file_record(     pdu );    break;
                        case 0x16: len = mdbs_rtu_func_mask_write_register(   pdu );    break;
                        case 0x17: len = mdbs_rtu_func_read_write_multiple_regs( pdu ); break;
                        case 0x18: len = mdbs_rtu_func_read_fifo_queue(       pdu );    break;
                        case 0x2B: len = mdbs_rtu_func_mei_transport(         pdu );    break;
                        default:
                                len = mdbs_rtu_error( pdu, MDBS_ERR_ILLEGAL_FUNCTION );
                                break;
                }

                len             += MDBS_RTU_DEV_ADDR_SIZEOF;
                crc             =   modbus_crc( adu, len );
                adu[ len++ ]    =   crc >> 8;
                adu[ len++ ]    =   crc & 0xFF;
        }
        else if( adu[ 0] == MDBS_ADDR_BROADCAST )
        {
                len     = mdbs_rtu_broadcast( pdu );
        }
        else
        {
                len     = MDBS_RTU_RESP_LENGTH_NONE;
        }

        return( len );
}
