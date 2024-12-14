/**
  * @file    touch.c
  * @brief   Touchpad
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "touch.h"
#include "gt9xx.h"
#include "DIALOG.h"
#include "config.h"
#include <stdio.h>

/*******************************************************************************
* TOUCH
*******************************************************************************/
#define CT_MAX_TOUCH    5

#define GT911_MAX_WIDTH                 800    	//Touchscreen pad max width
#define GT911_MAX_HEIGHT                480	//Touchscreen pad max height




typedef struct
{
	uint8_t Touch;
	uint8_t TouchpointFlag;
	uint8_t TouchCount;

	uint8_t Touchkeytrackid[CT_MAX_TOUCH];
	uint16_t X[CT_MAX_TOUCH];
	uint16_t Y[CT_MAX_TOUCH];
	uint16_t S[CT_MAX_TOUCH];
}GT911_Dev;



/* touch global variables*/
GT911_Dev Dev_Now = {0};
GT911_Dev Dev_Backup = {0};

int cntTouchDetected = 0;
//extern int cntTouchDetected = 0;

uint32_t tick = 0;
int pressed = 0;
//extern int pressed = 0;

        int point[2] = {0}; //touched point [x, y];
//extern  int point[2]; //touched point [x, y];

        int pointX = 0; // touched X
        int pointY = 0; // touched Y
//extern  int pointX; // touched X
//extern  int pointY; // touched Y
/* eof touch global variables*/

extern GT911_Dev Dev_Now;
extern GT911_Dev Dev_Backup;


void
ui_touch_scan( void )
{
	uint8_t buf[41]; // to read full info about touching;
	uint8_t Clearbuf = 0;
        //TS_StateTypeDef ts;
	//GUI_PID_STATE   TS_State        = {0, 0, 0, 0};


	if (Dev_Now.Touch == 1)
	{
		Dev_Now.Touch = 0;
		gt9xx_x_read(gt9xx_i2c_addr, GT9XX_REG_READ_XY, buf, 1 );


		if ((buf[0]&0x80) == 0x00) // check is data relevant;
		{
			gt9xx_x_write(gt9xx_i2c_addr, GT9XX_REG_READ_XY, (uint8_t *)&Clearbuf, 1 );
		}
		else
		{
			Dev_Now.TouchpointFlag = buf[0];
			Dev_Now.TouchCount = buf[0]&0x0f;

			if (Dev_Now.TouchCount > 5)
			{
				gt9xx_x_write(gt9xx_i2c_addr, GT9XX_REG_READ_XY, (uint8_t *)&Clearbuf, 1 );
				return ;
			}

			gt9xx_x_read(gt9xx_i2c_addr, GT9XX_REG_READ_XY+1, &buf[1], Dev_Now.TouchCount*8 );
			gt9xx_x_write(gt9xx_i2c_addr, GT9XX_REG_READ_XY, (uint8_t *)&Clearbuf, 1 );

			Dev_Now.Touchkeytrackid[0] = buf[1];
			Dev_Now.X[0] = ((uint16_t)buf[3] << 8) + buf[2];
			Dev_Now.Y[0] = ((uint16_t)buf[5] << 8) + buf[4];
			Dev_Now.S[0] = ((uint16_t)buf[7] << 8) + buf[6];

			Dev_Now.Touchkeytrackid[1] = buf[9];
			Dev_Now.X[1] = ((uint16_t)buf[11] << 8) + buf[10];
			Dev_Now.Y[1] = ((uint16_t)buf[13] << 8) + buf[12];
			Dev_Now.S[1] = ((uint16_t)buf[15] << 8) + buf[14];

			Dev_Now.Touchkeytrackid[2] = buf[17];
			Dev_Now.X[2] = ((uint16_t)buf[19] << 8) + buf[18];
			Dev_Now.Y[2] = ((uint16_t)buf[21] << 8) + buf[20];
			Dev_Now.S[2] = ((uint16_t)buf[23] << 8) + buf[22];

			Dev_Now.Touchkeytrackid[3] = buf[25];
			Dev_Now.X[3] = ((uint16_t)buf[27] << 8) + buf[26];
			Dev_Now.Y[3] = ((uint16_t)buf[29] << 8) + buf[28];
			Dev_Now.S[3] = ((uint16_t)buf[31] << 8) + buf[30];

			Dev_Now.Touchkeytrackid[4] = buf[33];
			Dev_Now.X[4] = ((uint16_t)buf[35] << 8) + buf[34];
			Dev_Now.Y[4] = ((uint16_t)buf[37] << 8) + buf[36];
			Dev_Now.S[4] = ((uint16_t)buf[39] << 8) + buf[38];

			for ( int i=0;i<Dev_Now.TouchCount;i++)
			{
				Dev_Backup.X[i] = Dev_Now.X[i];
				Dev_Backup.Y[i] = Dev_Now.Y[i];
				Dev_Backup.TouchCount = Dev_Now.TouchCount;
			}

			if( Dev_Now.TouchCount == 0 )
			{
				pressed = 0; // it means, surface unpressed;

				#if 1
				pointX = -1;
				pointY = -1;

                                GUI_TOUCH_StoreState(pointX, pointY);
/*
                                TS_State.x              = pointX;
                                TS_State.y              = pointY;
                                TS_State.Layer          = 0;
                                TS_State.Pressed        = 0;
                                GUI_TOUCH_StoreStateEx( &TS_State );
*/
				#endif
			}

                        else
			//if (Dev_Now.TouchCount == 1)
			{
				//if (!pressed)
				{
					pressed = 1;
					cntTouchDetected++;

					point[0] = Dev_Now.X[0];
					point[1] = Dev_Now.Y[0];

                                        #if 1
                                        pointX                  = Dev_Now.X[0];
                                        pointY                  = Dev_Now.Y[0];
                                        //TS_State.x              = pointX;
                                        //TS_State.y              = pointY;
                                        //TS_State.Layer          = 1;
                                        //TS_State.Pressed        = 1;
                                        //GUI_TOUCH_StoreStateEx( &TS_State );

                                        GUI_TOUCH_StoreState( pointX, pointY );
                                        #endif
				}
			}
		}
	}
}


void
ui_touch_int( void )
{
        Dev_Now.Touch   = 1;
}


static
int
ui_touch_check_id_and_set_addr( void )
{
    int result = 0;
    uint8_t     product_id[ 5]    = {0};

    result = gt9xx_x_read(gt9xx_i2c_addr, GT9XX_REG_PRODUCT_ID, product_id, 4 );
    
    if (result == 0) return 0;
    
    gt9xx_i2c_addr = TOUCH_GT9xx_I2C_ADDRESS_ALT;
    
    result = gt9xx_x_read(gt9xx_i2c_addr, GT9XX_REG_PRODUCT_ID, product_id, 4 );

    if (result == 0) return 0;
    
    result = -1;
    
    return result;
}


/*******************************************************************************
*
*******************************************************************************/
int
ui_touch_init( void )
{
    if( gt9xx_init() != 0 )
    {
            return( -1 );
    }
    
    ui_touch_check_id_and_set_addr();
    
    gt9xx_reverseXY(gt9xx_i2c_addr, REVERSE_TOUCH_BYTE);  

    return( 0 );
}
