/**
  * @file    isolated_uart.h
  * @brief   isolated uart communications
  * @author  NKP144
  */

#include <stdint.h>

void external_uart_init(void);

void external_uart_config_baudrate(uint32_t baudrate );

void external_uart_recv ( uint8_t *data, size_t size );

void external_uart_xmit (uint8_t *data, size_t size);

uint32_t external_uart_rx_get_ndtr( void );