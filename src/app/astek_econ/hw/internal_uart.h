/**
  * @file    internal_uart.h
  * @brief   internal bus communications
  * @author  Konstantin.Nikonov@pmi-ai.ru
  */

#include <stdint.h>

void internal_uart_init( const   size_t baudrate,
                         const   size_t data_bits,
                         const   float  stop_bits,
                         const   char   parity);

void internal_uart_config_baudrate( uint32_t baudrate );

void internal_uart_recv ( uint8_t *data, size_t size );

void internal_uart_xmit (uint8_t *data, size_t size);

uint32_t internal_uart_rx_get_ndtr( void );