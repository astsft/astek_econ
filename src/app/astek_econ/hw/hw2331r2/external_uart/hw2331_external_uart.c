/**
  * @file    external_uart.c
  * @brief   external bus communication
  * @author  Konstantin.Nikonov@pmi-ai.ru
  */

#include "stm32.h"
#include "external_uart.h"
#include "app_pipe.h"
#include "cmsis_os.h"
#include "dev/dev.h"

extern  QueueHandle_t           que_m2m_hndl;
extern dev_t    dev;

void external_uart_init (void)
{
  uint32_t baudrate = 0;
  uint8_t data_bits = 0;
  float stop_bits = 0;
  char parity = 'N';
  
  switch( dev.cfg.ext_mdbs_cfg.data_width )
  {
      case 0:     data_bits = 8;    break;
      case 1:     data_bits = 7;    break;
      case 2:     data_bits = 9;    break;
      default:    data_bits = 8;    break;
  }
  
  switch( dev.cfg.ext_mdbs_cfg.stop_bits )
  {
      case 0:     stop_bits = 1.0;    break;
      case 1:     stop_bits = 2.0;    break;
      default:    stop_bits = 1.0;    break;
  }  
  
  switch( dev.cfg.ext_mdbs_cfg.parity )
  {
      case 0:     parity = 'N';    break;
      case 1:     parity = 'E';    break;
      case 2:     parity = 'O';    break;
      default:    parity = 'N';    break;
  }    
  
  switch( dev.cfg.ext_mdbs_cfg.baudrate )
  {
      case 0:     baudrate = 9600;     break;
      case 1:     baudrate = 19200;    break;
      case 2:     baudrate = 38400;    break;
      case 3:     baudrate = 57600;    break;      
      case 4:     baudrate = 115200;   break;            
      default:    baudrate = 19200;    break;
  } 
  
  if (parity != 'N')
    data_bits += 1;
  stm32_usart1_init( baudrate, data_bits, stop_bits, parity );
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