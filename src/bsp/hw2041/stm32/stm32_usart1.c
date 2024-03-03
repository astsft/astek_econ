/**
  * @file    stm32_usart1.c
  * @brief   USART1 services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* LOCAL FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
    LL_GPIO_InitTypeDef   gpio;

    gpio.Pin        = LL_GPIO_PIN_10 | LL_GPIO_PIN_9;
    gpio.Mode       = LL_GPIO_MODE_ALTERNATE;
    gpio.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio.Pull       = LL_GPIO_PULL_NO;
    gpio.Alternate  = LL_GPIO_AF_7;
    LL_GPIO_Init( GPIOA, &gpio );
}


static
int
init_uart(                      const   size_t          baudrate,
                                const   size_t          data_bits,
                                const   float           stop_bits,
                                const   char            parity )
{
    uint32_t                d, s, p;
    LL_USART_InitTypeDef    uart    = {0};


    switch( data_bits )
    {
        case 9:     d   = LL_USART_DATAWIDTH_9B;    break;
        case 8:     d   = LL_USART_DATAWIDTH_8B;    break;
        case 7:     d   = LL_USART_DATAWIDTH_7B;    break;
        default:    d   = LL_USART_DATAWIDTH_8B;    break;
    }

    switch( parity )
    {
        case 'O':   p   = LL_USART_PARITY_ODD;      break;
        case 'E':   p   = LL_USART_PARITY_EVEN;     break;
        case 'N':   p   = LL_USART_PARITY_NONE;     break;
        default:    p   = LL_USART_PARITY_NONE;     break;
    }

    if(         stop_bits == 2.0f ) s   = LL_USART_STOPBITS_2;
    else if(    stop_bits == 1.5f ) s   = LL_USART_STOPBITS_1_5;
    else if(    stop_bits == 1.0f ) s   = LL_USART_STOPBITS_1;
    else if(    stop_bits == 0.5f ) s   = LL_USART_STOPBITS_0_5;
    else                            s   = LL_USART_STOPBITS_1;

    LL_APB2_GRP1_ForceReset( LL_APB2_GRP1_PERIPH_USART1 );
    __DSB();
    LL_APB2_GRP1_ReleaseReset( LL_APB2_GRP1_PERIPH_USART1 );
    __DSB();

    uart.BaudRate               = baudrate;
    uart.DataWidth              = d;
    uart.StopBits               = s;
    uart.Parity                 = p;
    uart.TransferDirection      = LL_USART_DIRECTION_TX_RX;
    uart.HardwareFlowControl    = LL_USART_HWCONTROL_NONE;
    uart.OverSampling           = LL_USART_OVERSAMPLING_16;
    LL_USART_Init( USART1, &uart );

    LL_USART_ConfigAsyncMode( USART1 );
    //LL_USART_EnableRxTimeout( USART1 );
    //LL_USART_SetRxTimeout( USART1, 20 );
    LL_USART_Enable( USART1 );

    return( 0 );
}

/*
static
int
init_dma_xmit( void )
{
    uint32_t    config  =   LL_DMA_DIRECTION_MEMORY_TO_PERIPH   |
                            LL_DMA_PRIORITY_LOW                 |
                            LL_DMA_MODE_NORMAL                  |
                            LL_DMA_PERIPH_NOINCREMENT           |
                            LL_DMA_MEMORY_INCREMENT             |
                            LL_DMA_PDATAALIGN_BYTE              |
                            LL_DMA_MDATAALIGN_BYTE;

    LL_DMA_DisableStream(       DMA2, LL_DMA_STREAM_7                   );
    LL_DMA_SetChannelSelection( DMA2, LL_DMA_STREAM_7, LL_DMA_CHANNEL_4 );
    LL_DMA_ConfigTransfer(      DMA2, LL_DMA_STREAM_7, config           );
    LL_DMA_DisableFifoMode(     DMA2, LL_DMA_STREAM_7                   );

    return( 0 );
}
*/
/*
static
int
init_dma_recv( void )
{
    uint32_t    config  =   LL_DMA_DIRECTION_PERIPH_TO_MEMORY   |
                            LL_DMA_PRIORITY_LOW                 |
                            LL_DMA_MODE_NORMAL                  |
                            LL_DMA_PERIPH_NOINCREMENT           |
                            LL_DMA_MEMORY_INCREMENT             |
                            LL_DMA_PDATAALIGN_BYTE              |
                            LL_DMA_MDATAALIGN_BYTE;


    LL_DMA_DisableStream(       DMA2, LL_DMA_STREAM_2                   );
    LL_DMA_SetChannelSelection( DMA2, LL_DMA_STREAM_2, LL_DMA_CHANNEL_4 );
    LL_DMA_ConfigTransfer(      DMA2, LL_DMA_STREAM_2, config           );
    LL_DMA_DisableFifoMode(     DMA2, LL_DMA_STREAM_2                   );

    return( 0 );
}
*/

/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_usart1_init(              const   size_t          baudrate,
                                const   size_t          data_bits,
                                const   float           stop_bits,
                                const   char            parity )
{
    init_uart( baudrate, data_bits, stop_bits, parity );
    //init_dma_xmit();
    //init_dma_recv();
    init_io();

    NVIC_SetPriority( DMA2_Stream7_IRQn, STM32_NVIC_PRIO_USART1_DMA_XMIT );
    NVIC_EnableIRQ( DMA2_Stream7_IRQn );
    LL_DMA_EnableIT_TC( DMA2, LL_DMA_STREAM_7 );
    LL_DMA_EnableIT_TE( DMA2, LL_DMA_STREAM_7 );

    NVIC_SetPriority( DMA2_Stream2_IRQn, STM32_NVIC_PRIO_USART1_DMA_RECV );
    NVIC_EnableIRQ( DMA2_Stream2_IRQn );
    //LL_DMA_EnableIT_TC( DMA2, LL_DMA_STREAM_2 );
    LL_DMA_EnableIT_TE( DMA2, LL_DMA_STREAM_2 );

    NVIC_SetPriority( USART1_IRQn, STM32_NVIC_PRIO_USART1 );
    NVIC_EnableIRQ( USART1_IRQn );
    LL_USART_ClearFlag_ORE( USART1 );
    LL_USART_ClearFlag_NE( USART1 );
    LL_USART_ClearFlag_FE( USART1 );
    LL_USART_ClearFlag_PE( USART1 );
    LL_USART_ClearFlag_RTO( USART1 );
    LL_USART_ClearFlag_IDLE( USART1 );

    //LL_USART_EnableIT_IDLE( USART1 );
    //LL_USART_EnableIT_RXNE( USART1 );
    //LL_USART_EnableIT_RTO( USART1 );
    LL_USART_EnableIT_ERROR( USART1 );

    return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_usart1_xmit_dma(          const   uint8_t *       buf,
                                const   size_t          cnt )
{
    //while( LL_DMA_GetDataLength( DMA2, LL_DMA_STREAM_7 ) > 0 );

    __DSB();

    LL_DMA_DisableStream( DMA2, LL_DMA_STREAM_7 );

    LL_DMA_ConfigAddresses( DMA2,
                            LL_DMA_STREAM_7,
                (uint32_t)  buf,
                            LL_USART_DMA_GetRegAddr( USART1, LL_USART_DMA_REG_DATA_TRANSMIT ),
                            //LL_DMA_GetDataTransferDirection( DMA2, LL_DMA_STREAM_7 ) );
                            LL_DMA_DIRECTION_MEMORY_TO_PERIPH );


    LL_DMA_SetDataLength(   DMA2, LL_DMA_STREAM_7, cnt );
    LL_USART_EnableDMAReq_TX( USART1 );

    LL_DMA_ClearFlag_TC4( DMA2 );
    LL_DMA_ClearFlag_HT4( DMA2 );
    LL_DMA_ClearFlag_TE4( DMA2 );
    LL_DMA_ClearFlag_DME4( DMA2 );
    LL_DMA_ClearFlag_FE4( DMA2 );

    LL_DMA_EnableStream( DMA2, LL_DMA_STREAM_7 );

    return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
void
stm32_usart1_recv( void )
{
    LL_DMA_DisableStream( DMA2, LL_DMA_STREAM_2 );
    __DSB();

    LL_USART_EnableIT_RXNE( USART1 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
stm32_usart1_recv_dma(          const   uint8_t *   buf,
                                const   size_t      cnt )
{
    LL_DMA_DisableStream( DMA2, LL_DMA_STREAM_2 );
    __DSB();

    LL_DMA_ConfigAddresses( DMA2,
                            LL_DMA_STREAM_2,
                            LL_USART_DMA_GetRegAddr( USART1, LL_USART_DMA_REG_DATA_RECEIVE ),
                (uint32_t)  buf,
                            LL_DMA_GetDataTransferDirection( DMA2, LL_DMA_STREAM_2) );

    LL_DMA_SetDataLength(   DMA2,
                            LL_DMA_STREAM_2,
                            cnt );

    LL_DMA_ClearFlag_TC2( DMA2 );
    LL_DMA_ClearFlag_HT2( DMA2 );
    LL_DMA_ClearFlag_TE2( DMA2 );
    LL_DMA_ClearFlag_DME2( DMA2 );
    LL_DMA_ClearFlag_FE2( DMA2 );
    LL_USART_ClearFlag_ORE( USART1 );
    LL_USART_EnableDMAReq_RX( USART1 );
    __DSB();
    LL_DMA_EnableStream( DMA2, LL_DMA_STREAM_2 );
    return( 0 );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
uint32_t
stm32_usart1_dma_xmit_remainder( void )
{
    uint32_t    remainder;

    remainder   = LL_DMA_GetDataLength( DMA2, LL_DMA_STREAM_7 );
    __DSB();
    return( remainder );
}

/**
  * @brief
  * @param  None
  * @retval None
  */
uint32_t
stm32_usart1_dma_recv_remainder( void )
{
    return( LL_DMA_GetDataLength( DMA2, LL_DMA_STREAM_2 ) );
}


/*******************************************************************************
* INTERRUPT SERVICE ROUTINES
*******************************************************************************/
/**
  * @brief
  * @param  None
  * @retval None
  */
void
stm32_usart1_isr( void )
{
    volatile    uint32_t    isr = USART1->ISR;


    if( (isr & USART_ISR_RXNE) && LL_USART_IsEnabledIT_RXNE( USART1 ) )
    {
        //RXNE flag will be cleared by reading of DR register (done in call)
        stm32_usart1_rxne_hook();
    }

    if( (isr & USART_ISR_IDLE) && LL_USART_IsEnabledIT_IDLE( USART1 ) )
    {
        LL_USART_ClearFlag_IDLE( USART1 );
        stm32_usart1_idle_hook();
    }

    if( (isr & USART_ISR_RTOF) && LL_USART_IsEnabledIT_RTO( USART1 ) )
    {
        LL_USART_ClearFlag_RTO( USART1 );
        stm32_usart1_rto_hook();
    }



    if( LL_USART_IsEnabledIT_ERROR( USART1 ) )
    {
        if( isr & USART_ISR_ORE )
        {
            LL_USART_ClearFlag_ORE( USART1 );
        }

        if( isr & USART_ISR_NE )
        {
            LL_USART_ClearFlag_NE( USART1 );
        }

        if( isr & USART_ISR_FE )
        {
            LL_USART_ClearFlag_FE( USART1 );
        }

        if( isr & USART_ISR_PE )
        {
            LL_USART_ClearFlag_PE( USART1 );
        }

        if( isr & (USART_ISR_ORE | USART_ISR_NE | USART_ISR_FE | USART_ISR_PE) )
        {
            stm32_usart1_error_hook( isr );
        }
    }

    if( (isr & USART_ISR_TXE) && LL_USART_IsEnabledIT_TXE( USART1 ) )
    {
        //TXE flag will be automatically cleared when writing new data in DR register
        stm32_usart1_txe_hook();
    }

    if( (isr & USART_ISR_TC) && LL_USART_IsEnabledIT_TC( USART1 ) )
    {
        LL_USART_ClearFlag_TC( USART1 );
        stm32_usart1_tc_hook();
    }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void
stm32_usart1_dma_xmit_isr( void )
{
    volatile    uint32_t    isr = DMA2->HISR;


    if( (isr & DMA_HISR_TCIF7) && LL_DMA_IsEnabledIT_TC(DMA2, LL_DMA_STREAM_7) )
    {
        LL_DMA_ClearFlag_TC7( DMA2 );
        stm32_usart1_dma_xmit_full_hook();
    }

    if( (isr & DMA_HISR_HTIF7) && LL_DMA_IsEnabledIT_HT(DMA2, LL_DMA_STREAM_7) )
    {
        LL_DMA_ClearFlag_HT7( DMA2 );
        stm32_usart1_dma_xmit_half_hook();
    }

    if( (isr & DMA_HISR_TEIF7) && LL_DMA_IsEnabledIT_TE(DMA2, LL_DMA_STREAM_7) )
    {
        LL_DMA_ClearFlag_TE7( DMA2 );
        LL_DMA_DisableStream( DMA2, LL_DMA_STREAM_7 );
        stm32_usart1_dma_xmit_error_hook();
    }

    if( (isr & DMA_HISR_DMEIF7) && LL_DMA_IsEnabledIT_DME(DMA2, LL_DMA_STREAM_7) )
    {
        LL_DMA_ClearFlag_DME7( DMA2 );
        stm32_usart1_dma_xmit_dm_error_hook();
    }

    if( (isr & DMA_HISR_FEIF7) && LL_DMA_IsEnabledIT_FE(DMA2, LL_DMA_STREAM_7) )
    {
        LL_DMA_ClearFlag_FE7( DMA2 );
        stm32_usart1_dma_xmit_fifo_error_hook();
    }
}


/**
  * @brief
  * @param  None
  * @retval None
  */
void
stm32_usart1_dma_recv_isr( void )
{
    if( LL_DMA_IsActiveFlag_TC2( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_TC2( DMA2 );
        stm32_usart1_dma_recv_full_hook();
    }

    if( LL_DMA_IsActiveFlag_HT2( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_HT2( DMA2 );
        stm32_usart1_dma_recv_half_hook();
    }

    if( LL_DMA_IsActiveFlag_TE2( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_TE2( DMA2 );
        LL_DMA_DisableStream( DMA2, LL_DMA_STREAM_2 );
        stm32_usart1_dma_recv_error_hook();
    }

    if( LL_DMA_IsActiveFlag_DME2( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_DME2( DMA2 );
        stm32_usart1_dma_recv_dm_error_hook();
    }

    if( LL_DMA_IsActiveFlag_FE2( DMA2 ) == 1 )
    {
        LL_DMA_ClearFlag_FE2( DMA2 );
        stm32_usart1_dma_recv_fifo_error_hook();
    }
}


/*******************************************************************************
* HOOKS
*******************************************************************************/
__weak  void    stm32_usart1_txe_hook( void ) {}
__weak  void    stm32_usart1_tc_hook( void ) {}
__weak  void    stm32_usart1_rxne_hook( void ) {}
__weak  void    stm32_usart1_idle_hook( void ) {}
__weak  void    stm32_usart1_rto_hook( void ) {}
__weak  void    stm32_usart1_error_hook( uint32_t sts_reg ) {}
__weak  void    stm32_usart1_dma_xmit_full_hook( void ) {}
__weak  void    stm32_usart1_dma_xmit_half_hook( void ) {}
__weak  void    stm32_usart1_dma_xmit_error_hook( void ) {}
__weak  void    stm32_usart1_dma_xmit_dm_error_hook( void ) {}
__weak  void    stm32_usart1_dma_xmit_fifo_error_hook( void ) {}
__weak  void    stm32_usart1_dma_recv_full_hook( void ) {}
__weak  void    stm32_usart1_dma_recv_half_hook( void ) {}
__weak  void    stm32_usart1_dma_recv_error_hook( void ) {}
__weak  void    stm32_usart1_dma_recv_dm_error_hook( void ) {}
__weak  void    stm32_usart1_dma_recv_fifo_error_hook( void ) {}
