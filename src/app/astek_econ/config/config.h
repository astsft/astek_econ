/**
  * @file    config.h
  * @brief   Master Config file
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  CONFIG_H
#define  CONFIG_H


#define CONFIG_MCU_SYSTICK_HZ           100ul

#define CONFIG_DEVICE_ID                0xDEC0
#define CONFIG_DEVICE_STR               "astek_econ"
#define CONFIG_HARDWARE_ID              0x2041
#define CONFIG_HARDWARE_STR             "hw2041"

#define CONFIG_THLD_LTCH_DELAY_SMPL     4

#define CONFIG_LOG_DATA_SIZE            64

#define CONFIG_FIRMWARE_ID              "astekecon.1.0"

//#define NO_RELAY

#include "config_mdbs.h"
#include "config_ser.h"
#include "config_ui.h"
#include "config_lcd.h"


#endif	//CONFIG_H
