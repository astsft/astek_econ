/**
  * @file    non_isolated_uart.c
  * @brief   non isolated uart communications
  * @author  NKP144
  */

#include "stm32.h"
#include "external_uart.h"

void external_uart_init (void)
{
  stm32_uart7_init(); 
}

void external_uart_config_baudrate(uint32_t baudrate )
{
  stm32_uart7_config_baudrate( baudrate );
}

void external_uart_recv ( uint8_t *data, size_t size )
{
  stm32_uart7_recv_dma( data, size );
}

void external_uart_xmit ( uint8_t *data, size_t size )
{
  stm32_uart7_xmit_dma(data, size);
}

uint32_t external_uart_rx_get_ndtr( void )
{
  return stm32_uart7_dma_rx_get_ndtr();
}