/**
  * @file    stm32fxx_it.c
  * @brief   Interrupt Service Routines
  * @author  Konstantin.Nikonov@pmi-ai.ru  
  */


#include "stm32.h"
#include "cmsis_os.h"
#include "beep\beep.h"
#include "gt9xx.h"

void    NMI_Handler( void );
void    HardFault_Handler( void );
void    MemManage_Handler( void );
void    BusFault_Handler( void );
void    UsageFault_Handler( void );
void    SVC_Handler( void );
void    DebugMon_Handler( void );
void    PendSV_Handler( void );
void    SysTick_Handler( void );
void    USART1_IRQHandler( void );
void    UART4_IRQHandler( void );
void    USART2_IRQHandler( void );
void    DMA1_Stream2_IRQHandler( void );
void    DMA1_Stream4_IRQHandler( void );
void    DMA1_Stream5_IRQHandler( void );
void    DMA1_Stream6_IRQHandler( void );
void    DMA2_Stream2_IRQHandler( void );
void    DMA2_Stream7_IRQHandler( void );
void    FMC_IRQHandler( void );
void    LTDC_IRQHandler( void );
void    LTDC_ER_IRQHandler( void );
void    EXTI9_5_IRQHandler( void );
void    I2C3_EV_IRQHandler( void );
void    I2C3_ER_IRQHandler( void );
void    EXTI0_IRQHandler( void );
/*******************************************************************************
* Cortex Processor Interruption and Exception Handlers
*******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void
NMI_Handler( void )
{
    #ifdef  NDEBUG
    NVIC_SystemReset();
    #else
    __asm volatile ("bkpt #0\n");   // Break into the debugger
    #endif //NDEBUG
}


/**
  * @brief This function handles Hard fault interrupt.
  */
void
HardFault_Handler( void )
{
    #ifdef  NDEBUG
    NVIC_SystemReset();
    #else
    //__disable_irq();
	__asm volatile ("bkpt #0\n");         // Break into the debugger
    #endif //NDEBUG
}


/**
  * @brief This function handles Memory management fault.
  */
void
MemManage_Handler( void )
{
    #ifdef  NDEBUG
    NVIC_SystemReset();
    #else
    __asm volatile ("bkpt #0\n");   // Break into the debugger
    #endif //NDEBUG
}


/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void
BusFault_Handler( void )
{
    #ifdef  NDEBUG
    NVIC_SystemReset();
    #else
    __asm volatile ("bkpt #0\n");   // Break into the debugger
    #endif //NDEBUG
}


/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void
UsageFault_Handler( void )
{
        #ifdef  NDEBUG
        NVIC_SystemReset();
        #else
        __asm volatile ("bkpt #0\n");   // Break into the debugger
        #endif //NDEBUG
}


/**
  * @brief This function handles Debug monitor.
  */
void
DebugMon_Handler( void )
{
        #ifdef  NDEBUG
        NVIC_SystemReset();
        #else
        __asm volatile ("bkpt #0\n");   // Break into the debugger
        #endif //NDEBUG
}


/*******************************************************************************
* SYS_TICK
*******************************************************************************/
/**
  * @brief This function handles System tick timer.
  */
void
SysTick_Handler( void )
{
    osSystickHandler();
    beep_hook();
    //HAL_IncTick();
}

/******************************************************************************/
/* RTC                                                                        */
/******************************************************************************/


/*******************************************************************************
* USART1
*******************************************************************************/
void
USART1_IRQHandler( void )
{
    stm32_usart1_isr();
}


void
DMA2_Stream2_IRQHandler( void )
{
    stm32_usart1_dma_recv_isr();
}


void
DMA2_Stream7_IRQHandler( void )
{
    stm32_usart1_dma_xmit_isr();
}

/*******************************************************************************
* USART2
*******************************************************************************/
void
USART2_IRQHandler( void )
{
    stm32_usart2_isr();
}


void
DMA1_Stream5_IRQHandler( void )
{
    stm32_usart2_dma_recv_isr();
}


void
DMA1_Stream6_IRQHandler( void )
{
    stm32_usart2_dma_xmit_isr();
}

/*******************************************************************************
* UART4
*******************************************************************************/
void
UART4_IRQHandler( void )
{
    stm32_uart4_isr();
}

void
DMA1_Stream2_IRQHandler( void )
{
    stm32_uart4_dma_recv_isr();
}

void
DMA1_Stream4_IRQHandler( void )
{
    stm32_uart4_dma_xmit_isr();
}



/*******************************************************************************
* SDRAM
*******************************************************************************/
/**
  * @brief This function handles FMC global interrupt.
  */
void
FMC_IRQHandler( void )
{
    stm32_fmc_isr();
}


/*******************************************************************************
* LCD
*******************************************************************************/
/**
  * @brief This function handles LTDC global interrupt.
  */
void
LTDC_IRQHandler( void )
{
  stm32_ltdc_isr();
}


/**
  * @brief This function handles LTDC global error interrupt.
  */
void
LTDC_ER_IRQHandler( void )
{
  stm32_ltdc_er_isr();
}


/*******************************************************************************
* TOUCH SENS
*******************************************************************************/
/**
  * @brief
  */
void
I2C3_EV_IRQHandler(void)
{
    gt9xx_x_i2c_ev_isr();
}

void
I2C3_ER_IRQHandler(void)
{
    gt9xx_x_i2c_er_isr();
}

void
EXTI9_5_IRQHandler( void )
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

/*******************************************************************************
* ETH 
*******************************************************************************/
/**
  * @brief
  */

void
EXTI0_IRQHandler( void )
{
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
    stm32_exti0_isr();
  }
}
