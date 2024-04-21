/**
  * @file    gt9xx.h
  * @brief   GOODIX GT9xx Touchpad driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef GT9XX_H
#define GT9XX_H


#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define GT9XX_CONFIG_SIZE       (0x8100 - 0x8047)       //185


typedef enum    gt9xx_reg_e
{
        GT9XX_REG_COMMAND                       = 0x8040,
        GT9XX_REG_CONFIG_VERSION                = 0x8047,
        GT9XX_REG_TOUCH_NUMBER                  = 0x804C,
        GT9XX_REG_MODULE_SWITCH1                = 0x804D,
        GT9XX_REG_MODULE_SWITCH2                = 0x804E,
        GT9XX_REG_CONFIG_FRESH                  = 0x8100,
        GT9XX_REG_PRODUCT_ID                    = 0x8140,
        GT9XX_REG_FIRMWARE_VERSION              = 0x8144,
        GT9XX_REG_READ_XY                       = 0x814E,
} gt9xx_reg_t;



typedef struct  gt9xx_cfg_s
{
        uint8_t         command_check;  //For commands greater than 0x07, it is required to write the command to 0x8046 before writing to 0x8040, to improve anti-ESD capability.
        uint8_t         config_version;
        uint16_t        x_output_max;           //Resolution of X axis
        uint16_t        y_output_max;           //Resolution of Y axis
        uint8_t         touch_number;           //Touch points supported: 1 to 5

        union
        {
                uint8_t         u08;
                struct
                {
                        uint8_t         int_triggering  :       2; //00: rising edge
                                                                   //01: falling edge
                                                                   //02: Low level
                                                                   //03: High level
                        uint8_t         sito            :       1; //Software noise reduction
                        uint8_t         x2y             :       1; //X,Y axis switch-over
                        uint8_t         stretch_rank    :       2;
                        uint8_t         sensor_resersal :       1; //X2X
                        uint8_t         driver_resersal :       1; //Y2Y
                };
        } module_switch1;

        union
        {
                uint8_t         u08;

                struct
                {
                        uint8_t         touch_key       :       1;
                        uint8_t         hotknot_en      :       1;
                        uint8_t         approch_en      :       1;
                        uint8_t         reserved3       :       1;
                        uint8_t         reserved4       :       1;
                        uint8_t         firstfilter_dis :       1;
                        uint8_t         reserved6       :       1;
                        uint8_t         reserved7       :       1;
                };
        } module_switch2;

} gt9xx_cfg_t;



/*******************************************************************************
* FUNCTION PROTOTYPES
*******************************************************************************/
int
gt9xx_init( void );

int
gt9xx_reset( void );
/*
int
gt9xx_write( uint16_t reg, uint8_t* buf, uint8_t len );

int
gt9xx_read( uint16_t reg, uint8_t* buf, uint8_t len );
*/
int
gt9xx_config_upload( uint8_t * data );

int
gt9xx_checksum( const uint8_t * data, size_t size );


/*******************************************************************************
* REG X
*******************************************************************************/
extern  int     gt9xx_x_init( void );
extern  void    gt9xx_x_enable( const bool );
extern  int     gt9xx_x_write( uint16_t reg, uint8_t* buf, uint8_t len );
extern  int     gt9xx_x_read( uint16_t reg, uint8_t* buf, uint8_t len );

void gt9xx_x_i2c_ev_isr( void );
void gt9xx_x_i2c_er_isr( void );

#endif	//GT9XX_H
