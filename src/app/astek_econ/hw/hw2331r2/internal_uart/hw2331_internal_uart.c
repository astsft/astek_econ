/**
  * @file    internal_uart.c
  * @brief   internal bus communication
  * @author  Konstantin.Nikonov@pmi-ai.ru
  */

#include "stm32.h"
#include "internal_uart.h"
#include "trace/trace.h"
#include "app_pipe.h"
#include "cmsis_os.h"

extern  QueueHandle_t           que_mdbs_clnt_hndl;

void internal_uart_init (       const   size_t          baudrate,
                                const   size_t          data_bits,
                                const   float           stop_bits,
                                const   char            parity)
{
  stm32_uart4_init( baudrate, data_bits, stop_bits, parity );
}

void internal_uart_config_baudrate(uint32_t baudrate )
{
  
}

void internal_uart_recv ( uint8_t *data, size_t size )
{
  stm32_uart4_recv_dma( data, size );
}

void internal_uart_xmit ( uint8_t *data, size_t size )
{
  stm32_uart4_xmit_dma(data, size);
}

uint32_t internal_uart_rx_get_ndtr( void )
{
  return stm32_uart4_dma_recv_remainder();
}

void
stm32_uart4_idle_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_UART4_RECV_IDLE; 
    
    xQueueSendFromISR( que_mdbs_clnt_hndl, &result, NULL );
}

void
stm32_uart4_rto_hook( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_UART4_RECV_TOUT; 
  
    xQueueSendFromISR( que_mdbs_clnt_hndl, &result, NULL );
}

void
stm32_uart4_error_hook(         uint32_t sts_reg )
{
    TRACE( "UART4->ISR: %08X ORE=%c, NE=%c, FE=%c, PE=%c\n",
            sts_reg,
            sts_reg & USART_ISR_ORE ? '1' : '0',
            sts_reg & USART_ISR_NE ? '1' : '0',
            sts_reg & USART_ISR_FE ? '1' : '0',
            sts_reg & USART_ISR_PE ? '1' : '0'  );
}

void
stm32_uart4_dma_xmit_error_hook( void )
{
    TRACE("stm32_uart4_dma_xmit_error_hook\n");
}

void
stm32_uart4_dma_xmit_dm_error_hook( void )
{
    TRACE("stm32_uart4_dma_xmit_dm_error_hook\n");
}

void
stm32_uart4_dma_xmit_fifo_error_hook( void )
{
    TRACE("stm32_uart4_dma_xmit_fifo_error_hook\n");
}

void
stm32_uart4_dma_recv_error_hook( void )
{
    TRACE("stm32_uart4_dma_recv_error_hook\n");
}

void
stm32_uart4_dma_recv_dm_error_hook( void )
{
    TRACE("stm32_uart4_dma_recv_dm_error_hook\n");
}

void
stm32_uart4_dma_recv_fifo_error_hook( void )
{
    TRACE("stm32_uart4_dma_recv_fifo_error_hook\n");
}
