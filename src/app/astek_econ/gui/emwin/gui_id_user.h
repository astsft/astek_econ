/**
  * @file    gui_id_user.h
  * @brief   GUI ID User Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  GUI_ID_USER_H
#define  GUI_ID_USER_H


#define WM_MESSAGE_HEADER_LABEL_UPDATE          (WM_USER + 0x100)

#define GUI_ID_WIN_SBAR                         (GUI_ID_USER + 0x100)

typedef enum
{
    GUI_ID_TEXT_AUTO                = (GUI_ID_USER + 0x200),
    GUI_ID_TEXT_BACKPLANE,
    //GUI_ID_TEXT_CAL1,
    //GUI_ID_TEXT_CAL2,
    GUI_ID_TEXT_CYCLE,
    GUI_ID_TEXT_DATE_LABEL,
    GUI_ID_TEXT_DATE_VALUE,
    GUI_ID_TEXT_DAY,
    GUI_ID_TEXT_DEC_LABEL,
    GUI_ID_TEXT_DEC_VALUE,
    GUI_ID_TEXT_DEVICE_SERNUM,
    GUI_ID_TEXT_DIGC,
    GUI_ID_TEXT_ERROR,
    GUI_ID_TEXT_FILTER,
    GUI_ID_TEXT_FIRMWARE_VER,
    GUI_ID_TEXT_FOCUS0,
    GUI_ID_TEXT_FOCUS1,
    GUI_ID_TEXT_FOCUS2,
    GUI_ID_TEXT_FOCUS3,
    GUI_ID_TEXT_FOCUS4,
    GUI_ID_TEXT_FOCUS5,
    GUI_ID_TEXT_HOUR_METER,
    GUI_ID_TEXT_HOUR,
    GUI_ID_TEXT_LABEL_SYSTEM,
    GUI_ID_TEXT_LABEL_LANGUAGE,
    GUI_ID_TEXT_LABEL_PASSWORD,
    GUI_ID_TEXT_LAUNCH,
    GUI_ID_TEXT_MCU_DIGC,
    GUI_ID_TEXT_MCU_VDDA,
    GUI_ID_TEXT_MEAS_LABEL,
    GUI_ID_TEXT_MEAS_VALUE,
    GUI_ID_TEXT_MEAS_UNITS,
    GUI_ID_TEXT_MINUTES,
    GUI_ID_TEXT_MONTH,
    GUI_ID_TEXT_mV_LABEL,
    GUI_ID_TEXT_mV_VALUE,
    GUI_ID_TEXT_ORDER,
    GUI_ID_TEXT_POINT,
    GUI_ID_TEXT_PRES_LABEL,
    GUI_ID_TEXT_PRES_VALUE,
    GUI_ID_TEXT_RANGE_LABEL,
    GUI_ID_TEXT_RANGE_VALUE,
    GUI_ID_TEXT_RAW_LABEL,
    GUI_ID_TEXT_RAW_VALUE,
    GUI_ID_TEXT_REMOTELY,
    GUI_ID_TEXT_RESTORE,
    GUI_ID_TEXT_RUNS_COUNTER,
    GUI_ID_TEXT_SBAR_HEADER,
    GUI_ID_TEXT_SBAR_TIME,
    GUI_ID_TEXT_SBAR_ERR_X,
    GUI_ID_TEXT_SBAR_ERR_E,
    GUI_ID_TEXT_SBAR_ERR_B,
    GUI_ID_TEXT_SBAR_ERR_C,
    GUI_ID_TEXT_SBAR_ERR_S,
    GUI_ID_TEXT_SBAR_ERR_M,
    GUI_ID_TEXT_SBAR_STS0,
    GUI_ID_TEXT_SECONDS,
    GUI_ID_TEXT_SLOPE_LABEL,
    GUI_ID_TEXT_SLOPE_VALUE,
    GUI_ID_TEXT_SPAN,
    GUI_ID_TEXT_SPAN_BACK,
    GUI_ID_TEXT_TEMP_LABEL,
    GUI_ID_TEXT_TEMP_VALUE,
    GUI_ID_TEXT_TIME_LABEL,
    GUI_ID_TEXT_TIME_VALUE,
    GUI_ID_TEXT_UNITS,
    GUI_ID_TEXT_YEAR,
    GUI_ID_TEXT_ZERO,
    GUI_ID_TEXT_ZERO_BACK,
    GUI_ID_TEXT_REAL_FW_VER,

} gui_id_text_t;


typedef enum
{
    GUI_ID_BUTTON_AUTO                  = (GUI_ID_USER + 0x330),
    GUI_ID_BUTTON_AUTOLAUNCH,
    GUI_ID_BUTTON_BACK,
    GUI_ID_BUTTON_CALIBRATION,
    GUI_ID_BUTTON_CANCEL,
    GUI_ID_BUTTON_CL420,
    GUI_ID_BUTTON_CYCLE,
    GUI_ID_BUTTON_DATE,
    GUI_ID_BUTTON_DIAGNOSTICS,
    GUI_ID_BUTTON_DISCRETE_OUTPUT,
    GUI_ID_BUTTON_DUMMY,
    GUI_ID_BUTTON_ENTER,
    GUI_ID_BUTTON_ESCAPE,
    GUI_ID_BUTTON_FACTORY_ZERO,
    GUI_ID_BUTTON_FILTER,
    GUI_ID_BUTTON_FILTER_DEC,
    GUI_ID_BUTTON_FILTER_INC,
    //GUI_ID_BUTTON_HELP,
    GUI_ID_BUTTON_INFO,
    GUI_ID_BUTTON_LANGUAGE,
    GUI_ID_BUTTON_LAUNCH,
    GUI_ID_BUTTON_LEFT,
    GUI_ID_BUTTON_MEASURE,
    //GUI_ID_BUTTON_OUTPUT,
    GUI_ID_BUTTON_PASSWORD,
    GUI_ID_BUTTON_RANGE,
    GUI_ID_BUTTON_REMOTELY,
    GUI_ID_BUTTON_RESET,
    GUI_ID_BUTTON_RESET_PASSWORD,
    GUI_ID_BUTTON_RESET_SETTINGS,
    GUI_ID_BUTTON_RESTORE,
    GUI_ID_BUTTON_RIGHT,
    GUI_ID_BUTTON_SENSOR,
    GUI_ID_BUTTON_SERVICE,
    GUI_ID_BUTTON_SPAN,
    GUI_ID_BUTTON_SYSTEM,
    GUI_ID_BUTTON_TIME,
    GUI_ID_BUTTON_ZERO,
    GUI_ID_BUTTON_RELAY,
    GUI_ID_BUTTON_CURRENT_LOOP,
    GUI_ID_BUTTON_RELAY_1,
    GUI_ID_BUTTON_RELAY_2,
    GUI_ID_BUTTON_RELAY_MODE,
    GUI_ID_BUTTON_RELAY_STATE,
    GUI_ID_BUTTON_RELAY_THRESHOLD,
    GUI_ID_BUTTON_CLOOP_ERROR_LEVEL,
    GUI_ID_BUTTON_CLOOP_CAL,
    GUI_ID_BUTTON_CLOOP_RANGE,
    GUI_ID_BUTTON_CLOOP_4MA,
    GUI_ID_BUTTON_CLOOP_20MA,
    GUI_ID_BUTTON_COURSE_DOWN,
    GUI_ID_BUTTON_COURSE_UP,
    GUI_ID_BUTTON_FINE_DOWN,
    GUI_ID_BUTTON_FINE_UP,
    
} gui_id_button_t;


typedef enum
{
    GUI_ID_LISTWHEEL4               = 0x264,
    GUI_ID_LISTWHEEL5,
    GUI_ID_LISTWHEEL6,
} gui_id_listwheel_t;;


#endif  //GUI_ID_USER_H