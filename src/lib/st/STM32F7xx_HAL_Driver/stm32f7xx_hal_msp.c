/**
  * @file    stm32f7xx_hal_msp.c
  * @brief   HAL MSP module
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "stm32f7xx_hal.h"


void
HAL_MspInit( void )
{
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOH_CLK_ENABLE();

        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_DMA1_FORCE_RESET();
        __HAL_RCC_DMA1_RELEASE_RESET();

        __HAL_RCC_DMA2_CLK_ENABLE();
        __HAL_RCC_DMA2_FORCE_RESET();
        __HAL_RCC_DMA2_RELEASE_RESET();
}


/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
}


/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
}
