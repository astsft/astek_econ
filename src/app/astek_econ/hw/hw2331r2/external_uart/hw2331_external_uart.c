/**
  * @file    external_uart.c
  * @brief   external bus communication
  * @author  Konstantin.Nikonov@pmi-ai.ru
  */

#include "stm32.h"
#include "external_uart.h"
#include "app_pipe.h"
#include "cmsis_os.h"

extern  QueueHandle_t           que_m2m_hndl;

void external_uart_init (void)
{
  stm32_usart1_init( 19200, 8, 1.0, 'N' );
}

void external_uart_config_baudrate(uint32_t baudrate )
{
  
}

void external_uart_recv ( uint8_t *data, size_t size )
{
  stm32_usart1_recv_dma( data, size );
}

void external_uart_xmit ( uint8_t *data, size_t size )
{
  stm32_usart1_xmit_dma(data, size);
}

uint32_t external_uart_rx_get_ndtr( void )
{
  return stm32_usart1_dma_recv_remainder();
}

void
stm32_usart1_rto_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_UART7_RECV;   

    xQueueSendFromISR( que_m2m_hndl, &result, NULL );
}

void
stm32_usart1_error_hook(         uint32_t sts_reg )
{
    TRACE( "USART1->ISR: %08X ORE=%c, NE=%c, FE=%c, PE=%c\n",
            sts_reg,
            sts_reg & USART_ISR_ORE ? '1' : '0',
            sts_reg & USART_ISR_NE ? '1' : '0',
            sts_reg & USART_ISR_FE ? '1' : '0',
            sts_reg & USART_ISR_PE ? '1' : '0'  );
}