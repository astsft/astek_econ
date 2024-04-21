/**
  * @file    app_isr_hw2041.c
  * @brief   Interrupt Service Routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"
#include "modbus.h"
#include "cmsis_os.h"
#include "beep\beep.h"
#include "os\os_user.h"
#include "app_pipe.h"


extern LTDC_HandleTypeDef       hltdc;
extern volatile GUI_TIMER_TIME  OS_TimeMS;
extern QueueHandle_t            que_m2m_hndl;
extern QueueHandle_t            que_hmi_hndl;


void    NMI_Handler( void );
void    HardFault_Handler( void );
void    MemManage_Handler( void );
void    BusFault_Handler( void );
void    UsageFault_Handler( void );
void    SVC_Handler( void );
void    DebugMon_Handler( void );
void    PendSV_Handler( void );
void    SysTick_Handler( void );
void    TIM6_DAC_IRQHandler( void );
void    USART1_IRQHandler( void );
//void    USART2_IRQHandler( void );
void    UART4_IRQHandler( void );
void    UART7_IRQHandler( void );
void    DMA1_Stream0_IRQHandler( void );
void    DMA1_Stream1_IRQHandler( void );
void    DMA1_Stream2_IRQHandler( void );
void    DMA1_Stream3_IRQHandler( void );
void    DMA1_Stream4_IRQHandler( void );
void    DMA1_Stream5_IRQHandler( void );
void    DMA2_Stream2_IRQHandler( void );
void    DMA2_Stream7_IRQHandler( void );

void    FMC_IRQHandler( void );
void    LTDC_IRQHandler( void );
void    LTDC_ER_IRQHandler( void );

void    EXTI9_5_IRQHandler( void );
void    I2C3_EV_IRQHandler( void );
void    I2C3_ER_IRQHandler( void );

void    gt9xx_x_i2c_ev_isr( void );
void    gt9xx_x_i2c_er_isr( void );

void    hard_fault_handler(                     uint32_t *      arg     );

/**
 * @brief Hard Fault handler
 */
void
hard_fault_handler(                             uint32_t *      arg     )
{
	volatile uint32_t stack_ptr;
	volatile uint32_t stacked_r0;
	volatile uint32_t stacked_r1;
	volatile uint32_t stacked_r2;
	volatile uint32_t stacked_r3;
	volatile uint32_t stacked_r12;
	volatile uint32_t stacked_lr;
	volatile uint32_t stacked_pc;
	volatile uint32_t stacked_psr;

	stack_ptr = (uint32_t)arg;
	stacked_r0 = arg[0];
	stacked_r1 = arg[1];
	stacked_r2 = arg[2];
	stacked_r3 = arg[3];
	stacked_r12 = arg[4];
	stacked_lr = arg[5];
	stacked_pc = arg[6];
	stacked_psr = arg[7];

	APP_TRACE("\r\n--- Hard fault handler ---\n");
	APP_TRACE("\nstck_ptr\t= 0x%08X", stack_ptr);
	APP_TRACE("\nR0\t= 0x%08X", stacked_r0);
	APP_TRACE("\nR1\t= 0x%08X", stacked_r1);
	APP_TRACE("\nR2\t= 0x%08X", stacked_r2);
	APP_TRACE("\nR3\t= 0x%08X", stacked_r3);
	APP_TRACE("\nR12\t= 0x%08X", stacked_r12);
	APP_TRACE("\nLR[R14]\t= 0x%08X\tsubroutine call return address", stacked_lr);
	APP_TRACE("\nPC[R15]\t= 0x%08X\tprogram counter", stacked_pc);
	APP_TRACE("\nPSR\t= 0x%08X", stacked_psr);
	APP_TRACE("\nBFAR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED38))));
	APP_TRACE("\nCFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED28))));
	APP_TRACE("\nHFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED2C))));
	APP_TRACE("\nDFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED30))));
	APP_TRACE("\nAFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED3C))));
	APP_TRACE("\nSCB_SHCSR\t= 0x%08X", SCB->SHCSR);
	// memstat();
	__asm volatile ("bkpt #0\n");         // Break into the debugger
}


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
    HAL_IncTick();
}


/*******************************************************************************
* HAL TIMEBASE
*******************************************************************************/
void
TIM6_DAC_IRQHandler( void )
{
    stm32fxx_hal_timebase_tim_isr();
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
* UART 7
*******************************************************************************/
void
UART7_IRQHandler( void )
{
    app_pipe_t result;
    result.tag    = OS_USER_TAG_UART7_RECV;   
  
    int             valid   = stm32_uart7_isr();

    if( valid )
    {
        xQueueSendFromISR( que_m2m_hndl, &result, NULL );
    }
}

void
DMA1_Stream3_IRQHandler( void )
{
    stm32_uart7_dma_rx_isr();
}

void
DMA1_Stream1_IRQHandler( void )
{
    stm32_uart7_dma_tx_isr();
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
    HAL_LTDC_IRQHandler( &hltdc );
}


/**
  * @brief This function handles LTDC global error interrupt.
  */
void
LTDC_ER_IRQHandler( void )
{
    HAL_LTDC_IRQHandler( &hltdc );
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


/**0
  * @brief
  */
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
