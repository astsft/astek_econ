/**
  * @file    stm32_i2c3.c
  * @brief   I2C3 services
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
    LL_GPIO_InitTypeDef     gpio;


    gpio.Pin        = LL_GPIO_PIN_8;
    gpio.Mode       = LL_GPIO_MODE_ALTERNATE;
    gpio.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    gpio.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    gpio.Pull       = LL_GPIO_PULL_UP;
    gpio.Alternate  = LL_GPIO_AF_4;
    LL_GPIO_Init( GPIOA, &gpio );

    gpio.Pin        = LL_GPIO_PIN_9;
    LL_GPIO_Init( GPIOC, &gpio );
}


static
void
init_i2c( void )
{
    LL_I2C_InitTypeDef  i2c3;


    i2c3.PeripheralMode     = LL_I2C_MODE_I2C;
    i2c3.Timing             = 0x00A09BEA;
    i2c3.AnalogFilter       = LL_I2C_ANALOGFILTER_DISABLE;
    i2c3.DigitalFilter      = 0;
    i2c3.OwnAddress1        = 0x0000;
    i2c3.TypeAcknowledge    = LL_I2C_ACK;
    i2c3.OwnAddrSize        = LL_I2C_OWNADDRESS1_7BIT;
    LL_I2C_Init( I2C3, &i2c3 );

    //LL_I2C_SetTiming(I2C3, I2C_TIMING );
}

/*******************************************************************************
* GLOBAL FUNCTIONS
*******************************************************************************/
int
stm32_i2c3_init( void )
{
    init_io();
    init_i2c();

    return( 0 );
}
