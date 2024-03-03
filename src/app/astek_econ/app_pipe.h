/**
  * @file    app_pipe.c
  * @brief   Pipe services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef APP_PIPE_H
#define APP_PIPE_H


#include <stdint.h>
#include <stdio.h>
#include "os\os_user.h"


/*typedef enum    app_pipe_tag_e
{
        APP_PIPE_TAG_ERROR,
        APP_PIPE_TAG_SENS_01_DATA,
        APP_PIPE_TAG_CONFIG_SET,

        APP_PIPE_TAG_SER1_DATA,
        APP_PIPE_TAG_SER1_IDLE,
        APP_PIPE_TAG_SER1_ERR,

        APP_PIPE_TAG_SER2,
        APP_PIPE_TAG_SER2_ERR,
        APP_PIPE_TAG_SER2_IDLE,

        APP_PIPE_TAG_RELAY_01,
        APP_PIPE_TAG_RELAY_02,

        APP_PIPE_TAG_UI_TRIM_0,
        APP_PIPE_TAG_UI_TRIM_1,

        APP_PIPE_TAG_SER1,
        //APP_PIPE_TAG_SER2,
        APP_PIPE_TAG_SER3,
        APP_PIPE_TAG_SER4,
        APP_PIPE_TAG_USB_RECV,
        APP_PIPE_TAG_CLI,
        APP_PIPE_TAG_STORAGE_CLOSE,
        APP_PIPE_TAG_STORAGE_OPEN,
        APP_PIPE_TAG_STORAGE_TOGGLE,
} app_pipe_tag_t;*/


typedef	struct	app_pipe_s
{
        //app_pipe_tag_t          tag;
        os_user_tag_t           tag;
        void *                  data;
        size_t                  cnt;
} app_pipe_t;


#endif  //APP_PIPE_H
