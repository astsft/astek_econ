/**
  * @file    gt9xx.c
  * @brief   GOODIX GT9xx Touchpad driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "gt9xx.h"
#include "gt911_config.c"


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

        gt9xx_config_upload( gt911_cfg_data_rotate180 );

	return( error );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
int
gt9xx_config_upload(                            uint8_t *       data )
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
        
        gt9xx_x_write( GT9XX_REG_CONFIG_VERSION, data, GT9XX_CONFIG_SIZE );


        return( 0 );
}
