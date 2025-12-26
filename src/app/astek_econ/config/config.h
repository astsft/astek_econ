/**
  * @file    config.h
  * @brief   Master Config file
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include "hw_config.h"

#ifndef  CONFIG_H
#define  CONFIG_H


#define CONFIG_MCU_SYSTICK_HZ           100ul

#define CONFIG_DEVICE_ID                0xEC0
#define CONFIG_DEVICE_STR               "astek_econ"

#define CONFIG_THLD_LTCH_DELAY_SMPL     4

#define CONFIG_LOG_DATA_SIZE            64

#define CONFIG_FIRMWARE_ID              "astekecon.1.0"

//#define NO_RELAY
/*******************************************************************************
* MODBUS
*******************************************************************************/
#define CFG_MDBS_BAUDRATE                       115200
//#define CFG_MDBS_DEV_ADDR_SENS                  13
#define CFG_MDBS_DEV_ADDR_SENS                  32
#define CFG_MDBS_DEV_ADDR_MDB_RELAY             48
#define CFG_MDBS_DEV_ADDR_CLOOP                 64
/*******************************************************************************
* EXT_MODBUS
*******************************************************************************/
#define CFG_RTU_MDBS_DEV_ADDR                 1
#define CFG_TCP_MDBS_DEV_ADDR                 1

/*******************************************************************************
* LCD
*******************************************************************************/
#define LCD_SPEC_XSIZE                  800
#define LCD_SPEC_YSIZE                  480

#define LCD_SPEC_HSYNC_PULSE_WIDTH      70
#define LCD_SPEC_HSYNC_BACK_PORCH       160
#define LCD_SPEC_HSYNC_FRONT_PORCH      160
#define LCD_SPEC_HSYNC_ACTIVE_WIDTH     (LCD_SPEC_XSIZE + LCD_SPEC_HSYNC_BACK_PORCH)
#define LCD_SPEC_HSYNC_PERIOD_TOTAL     (LCD_SPEC_XSIZE + LCD_SPEC_HSYNC_BACK_PORCH + LCD_SPEC_HSYNC_FRONT_PORCH)

#define LCD_SPEC_VSYNC_PULSE_WIDTH      10
#define LCD_SPEC_VSYNC_BACK_PORCH       23
#define LCD_SPEC_VSYNC_FRONT_PORCH      12
#define LCD_SPEC_VSYNC_ACTIVE_WIDTH     (LCD_SPEC_YSIZE + LCD_SPEC_VSYNC_BACK_PORCH)
#define LCD_SPEC_VSYNC_PERIOD_TOTAL     (LCD_SPEC_YSIZE + LCD_SPEC_VSYNC_BACK_PORCH + LCD_SPEC_VSYNC_FRONT_PORCH)

/*******************************************************************************
* SDRAM
*******************************************************************************/
#define CONFIG_SDRAM_START_ADDR             0xC0000000

/*******************************************************************************
* UI KEYBOARD
*******************************************************************************/
#define CONFIG_UI_KEYBOARD_USART_BAUDRATE       115200

/*******************************************************************************
* UI LED
*******************************************************************************/
#define CONFIG_UI_LED_FLSH_SHRT_mSEC            40
#define CONFIG_UI_LED_FLSH_LONG_mSEC            200

/*******************************************************************************
* UI TOUCHSCREEN
*******************************************************************************/
#define CONFIG_UI_TOUCH_GT9xx_I2C_ADDRESS       0xBA

/*******************************************************************************
* UI COLOR THEME
*******************************************************************************/
#define CONFIG_UI_COLOR_DEBUG                   0

#if (CONFIG_UI_COLOR_DEBUG==1)
#define CONFIG_UI_COLOR_BACKPLANE               GUI_DARKRED
#define CONFIG_UI_COLOR_BACKGROUND              GUI_DARKBLUE
#define CONFIG_UI_COLOR_WHEEL_UNSEL             GUI_DARKGRAY
#define CONFIG_UI_COLOR_WHEEL_SEL               GUI_GRAY
#else
#define CONFIG_UI_COLOR_BACKPLANE               GUI_GRAY_2F
#define CONFIG_UI_COLOR_BACKGROUND              GUI_BLACK
#define CONFIG_UI_COLOR_WHEEL_UNSEL             GUI_DARKGRAY
#define CONFIG_UI_COLOR_WHEEL_SEL               GUI_GRAY
#endif

/*******************************************************************************
* TEMP
*******************************************************************************/
#define COLOR_CONVERSION_0          GUICC_M565
#define DISPLAY_DRIVER_0            GUIDRV_LIN_16
#define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_RGB565
#define DMA_COLOR_MODE              DMA2D_RGB565
#define GUI_NUM_LAYERS              1

#endif	//CONFIG_H
