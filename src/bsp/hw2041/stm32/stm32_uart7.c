/**
  * @file    stm32_uart7.c
  * @brief   UART7 routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  UART_HandleTypeDef      huart7;
static  DMA_HandleTypeDef       hdma_uart7_rx;
static  DMA_HandleTypeDef       hdma_uart7_tx;

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
        //PF6     ------> UART7_RX
        //PF7     ------> UART7_TX
        //PF8     ------> UART7_DE

        GPIO_InitTypeDef        pins    = {     .Pin       =    GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8,
	                                        .Mode      =    GPIO_MODE_AF_PP,
	                                        .Pull      =    GPIO_NOPULL,
	                                        .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                        .Alternate =    GPIO_AF8_UART7 };

        HAL_GPIO_Init( GPIOF, &pins );
}


static
int
init_uart( void )
{
        __HAL_RCC_UART7_CLK_ENABLE();
        __HAL_RCC_UART7_FORCE_RESET();
        __HAL_RCC_UART7_RELEASE_RESET();

        huart7.Instance                         = UART7;
        //huart7.Init.BaudRate                    = baudrate;
        huart7.Init.WordLength                  = UART_WORDLENGTH_8B;
        huart7.Init.StopBits                    = UART_STOPBITS_1;
        huart7.Init.Parity                      = UART_PARITY_NONE;
        huart7.Init.Mode                        = UART_MODE_TX_RX;
        huart7.Init.HwFlowCtl                   = UART_HWCONTROL_NONE;
        huart7.Init.OverSampling                = UART_OVERSAMPLING_16;
        huart7.Init.OneBitSampling              = UART_ONE_BIT_SAMPLE_DISABLE;
        huart7.AdvancedInit.AdvFeatureInit      = UART_ADVFEATURE_NO_INIT;

        if( HAL_RS485Ex_Init( &huart7, UART_DE_POLARITY_HIGH, 0, 0 ) != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


static
int
init_dma_rx( void )
{
        hdma_uart7_rx.Instance                  = DMA1_Stream3;
        hdma_uart7_rx.Init.Channel              = DMA_CHANNEL_5;
        hdma_uart7_rx.Init.Direction            = DMA_PERIPH_TO_MEMORY;
        hdma_uart7_rx.Init.PeriphInc            = DMA_PINC_DISABLE;
        hdma_uart7_rx.Init.MemInc               = DMA_MINC_ENABLE;
        hdma_uart7_rx.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
        hdma_uart7_rx.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
        hdma_uart7_rx.Init.Mode                 = DMA_NORMAL;
        hdma_uart7_rx.Init.Priority             = DMA_PRIORITY_LOW;
        hdma_uart7_rx.Init.FIFOMode             = DMA_FIFOMODE_DISABLE;

        if( HAL_DMA_Init( &hdma_uart7_rx ) != HAL_OK )
        {
                return( -1 );
        }

        __HAL_LINKDMA( &huart7, hdmarx, hdma_uart7_rx );

        return( 0 );
}


static
int
init_dma_tx( void )
{
        hdma_uart7_tx.Instance                  = DMA1_Stream1;
        hdma_uart7_tx.Init.Channel              = DMA_CHANNEL_5;
        hdma_uart7_tx.Init.Direction            = DMA_MEMORY_TO_PERIPH;
        hdma_uart7_tx.Init.PeriphInc            = DMA_PINC_DISABLE;
        hdma_uart7_tx.Init.MemInc               = DMA_MINC_ENABLE;
        hdma_uart7_tx.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
        hdma_uart7_tx.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
        hdma_uart7_tx.Init.Mode                 = DMA_NORMAL;
        hdma_uart7_tx.Init.Priority             = DMA_PRIORITY_LOW;
        hdma_uart7_tx.Init.FIFOMode             = DMA_FIFOMODE_DISABLE;

        if( HAL_DMA_Init( &hdma_uart7_tx ) != HAL_OK )
        {
                return( -1 );
        }

        __HAL_LINKDMA( &huart7, hdmatx, hdma_uart7_tx );

        return( 0 );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
void
stm32_uart7_deinit( void )
{
        __HAL_RCC_UART7_FORCE_RESET();
        __HAL_RCC_UART7_CLK_DISABLE();

        //PF6     ------> UART7_RX
        //PF7     ------> UART7_TX
        //PF8     ------> UART7_DE
        HAL_GPIO_DeInit( GPIOF, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 );
}


int
stm32_uart7_init( void )
{
    init_uart();
    init_dma_rx();
    init_dma_tx();
    init_io();

    NVIC_SetPriority(   UART7_IRQn, STM32_NVIC_PRIO_UART7                   );
    NVIC_EnableIRQ(     UART7_IRQn                                          );

    NVIC_SetPriority(   DMA1_Stream3_IRQn, STM32_NVIC_PRIO_UART7_DMA_RECV   );
    NVIC_EnableIRQ(     DMA1_Stream3_IRQn                                   );

    NVIC_SetPriority(   DMA1_Stream1_IRQn, STM32_NVIC_PRIO_UART7_DMA_XMIT   );
    NVIC_EnableIRQ(     DMA1_Stream1_IRQn                                   );

    //__HAL_UART_CLEAR_IDLEFLAG( &huart );
    __HAL_UART_ENABLE_IT( &huart7, UART_IT_IDLE );

    return( 0 );
}


void
stm32_uart7_config_baudrate(            const   size_t          baudrate )
{
        huart7.Init.BaudRate    = baudrate;
}


/******************************************************************************/
/* Receive related                                                            */
/******************************************************************************/
int
stm32_uart7_recv_dma(                           uint8_t *           data,
                                                size_t              size )
{
        HAL_UART_AbortReceive( &huart7 );

        if( HAL_UART_Receive_DMA( &huart7, data, size  ) != HAL_OK )
        {
                return( -1 );
        }

        return( 0 );
}


/******************************************************************************/
/* Transmit related                                                           */
/******************************************************************************/
int
stm32_uart7_xmit_dma(                   const   uint8_t *               data,
                                                size_t                  size )
{
        HAL_StatusTypeDef       resp;


        //HAL_UART_AbortTransmit( &huart7 );
        resp    = HAL_UART_Transmit_DMA( &huart7, (uint8_t *) data, size );

        return( resp == HAL_OK ? 0 : 1 );
}


/******************************************************************************/
/* Interrupt Service Routines                                                 */
/******************************************************************************/

int
stm32_uart7_isr( void )
{
        int     idle    = 0;


        if( __HAL_UART_GET_FLAG( &huart7, UART_FLAG_IDLE ) )
        {
                idle    = 1;
                __HAL_UART_CLEAR_IDLEFLAG( &huart7 );
        }


	HAL_UART_IRQHandler( &huart7 );

        return( idle );
}


/*******************************************************************************
* DMA RX
*******************************************************************************/
uint32_t
stm32_uart7_dma_rx_get_ndtr( void )
{
        return( __HAL_DMA_GET_COUNTER( &hdma_uart7_rx ) );
}


void
stm32_uart7_dma_rx_isr( void )
{
        HAL_DMA_IRQHandler( huart7.hdmarx );
}


/*******************************************************************************
* DMA TX
*******************************************************************************/
void
stm32_uart7_dma_tx_isr( void )
{
        HAL_DMA_IRQHandler( huart7.hdmatx );
}
