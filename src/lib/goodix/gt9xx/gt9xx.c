/**
  * @file    gt9xx.c
  * @brief   GOODIX GT9xx Touchpad driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "gt9xx.h"

/*******************************************************************************
* UI TOUCHSCREEN
*******************************************************************************/
uint8_t gt9xx_i2c_addr = TOUCH_GT9xx_I2C_ADDRESS;

/*******************************************************************************
* GLOBAL FUNCTIONS
*******************************************************************************/
/**
  * @brief  Initializes the gt9xx
  * @param  None
  * @retval error (true : false)
  */
int
gt9xx_init( void )
{
	int     error   = gt9xx_x_init();

	return( error );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
gt9xx_config_upload(  uint8_t         addr,
                      uint8_t *       data )
{
        //uint8_t         gt_data[ GT9XX_CONFIG_SIZE ];
        //uint8_t         checksum;


        //memcpy( gt_data, gt911_cfg_data, GT9XX_CONFIG_SIZE-1 );
        //checksum                        = gt9xx_checksum( data, GT9XX_CONFIG_SIZE-1 );
        //data[ GT9XX_CONFIG_SIZE-1 ]     = checksum;
        //gt9xx_write( GT9XX_REG_CONFIG_VERSION, data, GT9XX_CONFIG_SIZE );
        //gt9xx_x_write( GT9XX_REG_CONFIG_VERSION, gt911_cfg_data, GT9XX_CONFIG_SIZE );
  
  
  
        //memcpy( gt_data, data, GT9XX_CONFIG_SIZE-1 );
        //checksum = gt9xx_checksum( data, GT9XX_CONFIG_SIZE-1 );        
        //gt_data[ GT9XX_CONFIG_SIZE-1 ]     = checksum;
        //gt9xx_x_write( GT9XX_REG_CONFIG_VERSION, gt_data, GT9XX_CONFIG_SIZE );        
        
        gt9xx_x_write( addr, GT9XX_REG_CONFIG_VERSION, data, GT9XX_CONFIG_SIZE );


        return( 0 );
}

int
gt9xx_reverseXY(uint8_t addr, uint8_t reverse_byte)
{
  uint8_t temp_buf[GT9XX_CONFIG_SIZE] = {0};
  uint8_t switch1_reg = 0x01;

  gt9xx_x_read(addr, GT9XX_REG_CONFIG_VERSION, temp_buf, GT9XX_CONFIG_SIZE);
  temp_buf[6] = reverse_byte;
  int crc = gt9xx_checksum(temp_buf, GT9XX_CONFIG_SIZE - 1);
  temp_buf[GT9XX_CONFIG_SIZE - 1] = crc;
  gt9xx_x_write( addr, GT9XX_REG_CONFIG_VERSION, temp_buf, GT9XX_CONFIG_SIZE );     
  gt9xx_x_write( addr, GT9XX_REG_CONFIG_FRESH, &switch1_reg, 1 ); 

  return( 0 );  
}
