/**
  * @file       config_ui.h
  * @brief      User Interface Key CTL services
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#ifndef  CONFIG_UI_H
#define  CONFIG_UI_H


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


#endif	//CONFIG_UI_H
