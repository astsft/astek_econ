/**
  * @file       gt9xx_x.h
  * @brief      Board Support Package, Touch Sense Panel routines
  * @author     Igor T. <research.tahoe@gmail.com>
  */


//#include "stm32f7xx_hal.h"
#include "gt9xx.h"
#include "string.h"
#include "config.h"
#include "stm32.h"

#include "os\os_user.h"
#include "app_pipe.h"
#include "cmsis_os.h"


//#define TOUCH_ADDRESS           CONFIG_UI_TOUCH_GT9xx_I2C_ADDRESS

#define I2C_WAIT_TIMEOUT        500

static  I2C_HandleTypeDef       hi2c;
extern QueueHandle_t            que_hmi_hndl;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/
int     gt9xx_x_init( void );
int     gt9xx_x_write(uint8_t addr, uint16_t reg, uint8_t* buf, uint8_t len );
int     gt9xx_x_read(uint8_t addr, uint16_t reg, uint8_t* buf, uint8_t len );
void    gt9xx_x_i2c_ev_isr( void );
void    gt9xx_x_i2c_er_isr( void );


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
gt9xx_x_init_i2c( void )
{
	__HAL_RCC_I2C3_CLK_ENABLE();


	hi2c.Instance                   = I2C3;
	hi2c.Init.Timing                = 0x00A09BEA;
	hi2c.Init.OwnAddress1           = 0;
	hi2c.Init.AddressingMode        = I2C_ADDRESSINGMODE_7BIT;
	hi2c.Init.DualAddressMode       = I2C_DUALADDRESS_DISABLE;
	hi2c.Init.OwnAddress2           = 0;
	hi2c.Init.OwnAddress2Masks      = I2C_OA2_NOMASK;
	hi2c.Init.GeneralCallMode       = I2C_GENERALCALL_DISABLE;
	hi2c.Init.NoStretchMode         = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init( &hi2c );

	HAL_I2CEx_ConfigAnalogFilter( &hi2c, I2C_ANALOGFILTER_DISABLE );

	HAL_I2CEx_ConfigDigitalFilter( &hi2c, 0 );


    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	/* set priority depending on FREERTOS requirements!!! */
    NVIC_SetPriority(   I2C3_EV_IRQn, STM32_NVIC_PRIO_I2C3_EV       );
    NVIC_EnableIRQ(     I2C3_EV_IRQn                                );
    NVIC_SetPriority(   I2C3_ER_IRQn, STM32_NVIC_PRIO_I2C3_ER       );
    NVIC_EnableIRQ(     I2C3_ER_IRQn                                );
}


static
void
gt9xx_x_init_int( void )
{
	GPIO_InitTypeDef        gpio    = {0};


	gpio.Pin        = GPIO_PIN_8;
	//gpio.Mode       = GPIO_MODE_IT_FALLING;
	gpio.Mode       = GPIO_MODE_IT_RISING;
	//gpio.Pull       = GPIO_PULLUP;
	gpio.Pull       = GPIO_NOPULL;
	gpio.Speed      = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( GPIOC, &gpio );

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//set priority depending on FREERTOS requirements
	//HAL_NVIC_SetPriority( EXTI9_5_IRQn, 9, 0 );
	//HAL_NVIC_EnableIRQ( EXTI9_5_IRQn );
       
        NVIC_SetPriority(   EXTI9_5_IRQn, STM32_NVIC_PRIO_EXTI9_5       );
        NVIC_EnableIRQ(     EXTI9_5_IRQn                                );
	EXTI->IMR |= EXTI_IMR_IM8;
}


static
void
gt9xx_x_init_io( void )
{
        GPIO_InitTypeDef        GPIO_InitStruct = {0};


		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**I2C3 GPIO Configuration
		PA8     ------> I2C3_SCL
		PC9     ------> I2C3_SDA
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
 * @brief Initialization
 */
int
gt9xx_x_init( void )
{
        gt9xx_x_init_i2c();
        gt9xx_x_init_io();
        gt9xx_x_init_int();

        return( 0 );
}


/**
 * @brief
 */
int
gt9xx_x_write(                                  uint8_t         addr,
                                                uint16_t        reg,
                                                uint8_t *       buf,
                                                uint8_t         len )
{
	uint32_t tick = 0;
	I2C3->CR2 = 0;
	I2C3->CR2 = ((uint8_t) CONFIG_UI_TOUCH_GT9xx_I2C_ADDRESS) | I2C_CR2_AUTOEND | ((len + 2) << I2C_CR2_NBYTES_Pos);
	I2C3->CR2 |= I2C_CR2_START;

	for( int i = 0; i < 2; i++)
	{
		tick = HAL_GetTick();
		while (! (I2C3->ISR & I2C_FLAG_TXE))
		{
			if(I2C2->ISR & I2C_ISR_NACKF)
				return 1;
			if ( HAL_GetTick() - tick > I2C_WAIT_TIMEOUT)
				return 3;
		}

		I2C3->TXDR = (uint8_t) (reg >>  8 * (1 - i)) & 0xFF;
	}

	for( int i = 0; i < len; i++)
	{
		tick = HAL_GetTick();
		while (! (I2C3->ISR & I2C_FLAG_TXE))
		{
			if(I2C2->ISR & I2C_ISR_NACKF)
				return 1;
			if ( HAL_GetTick() - tick > I2C_WAIT_TIMEOUT)
				return 3;
		}
		I2C3->TXDR = (uint8_t) *buf++;
	}

	return 0;
}


/**
 * @brief
 */
int
gt9xx_x_read(                                   uint8_t         addr,
                                                uint16_t        reg,
                                                uint8_t *       buf,
                                                uint8_t         len )
{
	uint32_t tick = 0;
	if (len == 0)
	 return 1;

	I2C3->CR2 = 0;
	I2C3->CR2 = ((uint8_t) CONFIG_UI_TOUCH_GT9xx_I2C_ADDRESS) | I2C_CR2_AUTOEND | (2 << I2C_CR2_NBYTES_Pos);
	I2C3->CR2 |= I2C_CR2_START;
	for( int i = 0; i < 2; i++)
	{
		tick = HAL_GetTick();
		while (! (I2C3->ISR & I2C_FLAG_TXE))
		{
			if(I2C3->ISR & I2C_ISR_NACKF)
				return 2;
			if ( HAL_GetTick() - tick > I2C_WAIT_TIMEOUT)
				return 3;
		}

		I2C3->TXDR = (uint8_t) (reg >>  8 * (1 - i)) & 0xFF;
	}

	tick = HAL_GetTick();

	//wait end of transmition - STOP condition
	while (! (I2C3->ISR & I2C_FLAG_BUSY))
	{
		if ( HAL_GetTick() - tick > I2C_WAIT_TIMEOUT)
			return 3;
	}


	I2C3->CR2 = 0;
	I2C3->CR2 = ((uint8_t) CONFIG_UI_TOUCH_GT9xx_I2C_ADDRESS) | I2C_CR2_AUTOEND | (len << I2C_CR2_NBYTES_Pos)
				| I2C_CR2_RD_WRN;
	I2C3->CR2 |= I2C_CR2_START;

	for( int i = 0; i < len; i++)
	{
		tick = HAL_GetTick();
		while (! (I2C3->ISR & I2C_FLAG_RXNE))
		{
			if(I2C3->ISR & (I2C_ISR_NACKF))
				return 1;
			if ( HAL_GetTick() - tick > I2C_WAIT_TIMEOUT)
			return 3;
		}

		buf[i] = I2C3->RXDR;
	}

	tick = HAL_GetTick();
	while (! (I2C3->ISR & I2C_FLAG_BUSY))
	{
		if ( HAL_GetTick() - tick > I2C_WAIT_TIMEOUT)
			return 3;
	}

	return 0;
}



/*******************************************************************************
* PUBLIC ISR FUNCTIONS
*******************************************************************************/
/**
 * @brief
 */
void
gt9xx_x_i2c_ev_isr( void )
{
	HAL_I2C_EV_IRQHandler( &hi2c );
}


/**
 * @brief
 */
void
gt9xx_x_i2c_er_isr( void )
{
	HAL_I2C_ER_IRQHandler( &hi2c );
}

void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
  app_pipe_t result;
  result.tag    = OS_USER_TAG_TOUCH_INT;  
  
  if (GPIO_Pin == GPIO_PIN_8)
  {
         xQueueSendFromISR( que_hmi_hndl, &result, NULL );
  }                   
}
