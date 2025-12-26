/**
  * @file    os_user.h
  * @brief   OS User Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef OS_USER_H
#define OS_USER_H


typedef enum    sys_sync_e
{
    OS_USER_TAG_TMR_1SEC,
    OS_USER_TAG_USART1_RECV_IDLE,
    OS_USER_TAG_USART1_RECV_RXNE,
    OS_USER_TAG_UART4_RECV_IDLE,
    OS_USER_TAG_UART4_RECV_TOUT,
    OS_USER_TAG_UART4_XMIT_FULL,
    OS_USER_TAG_UART7_RECV,
    OS_USER_TAG_CAL0_UPDATE,
    OS_USER_TAG_CAL1_UPDATE,
    OS_USER_TAG_CAL0_RESTORE,
    OS_USER_TAG_CAL1_RESTORE,
    OS_USER_TAG_CAL0_FACTORY_UPDATE,
    OS_USER_TAG_CAL1_FACTORY_UPDATE,
    OS_USER_TAG_CAL0_FACTORY_RESTORE,
    OS_USER_TAG_CAL1_FACTORY_RESTORE,
    OS_USER_TAG_TRIM_READ,
    OS_USER_TAG_TOUCH_INT,
    OS_USER_TAG_SENS_CONF_FILTER,
    OS_USER_TAG_IBUS_REFRESH,
    OS_USER_TAG_IBUS_CL420_CH1_UPDATE,
    OS_USER_TAG_IBUS_CL420_CH2_UPDATE,
    OS_USER_TAG_IBUS_SHOW_VALIDATION,
    OS_USER_TAG_RELAY1_OPEN,
    OS_USER_TAG_RELAY2_OPEN,
    OS_USER_TAG_RELAY1_CLOSE,
    OS_USER_TAG_RELAY2_CLOSE,
    OS_USER_TAG_CAL_CLOOP_CHANNEL1_4MA_WRITE,   
    OS_USER_TAG_CAL_CLOOP_CHANNEL2_4MA_WRITE,
    OS_USER_TAG_CAL_CLOOP_CHANNEL1_20MA_WRITE, 
    OS_USER_TAG_CAL_CLOOP_CHANNEL2_20MA_WRITE,     
    OS_USER_TAG_CAL_CLOOP_CHANNEL1_SET_RAW_4MA,
    OS_USER_TAG_CAL_CLOOP_CHANNEL2_SET_RAW_4MA,    
    OS_USER_TAG_CAL_CLOOP_CHANNEL1_SET_RAW_20MA,
    OS_USER_TAG_CAL_CLOOP_CHANNEL2_SET_RAW_20MA,  
    OS_USER_TAG_CAL_CLOOP_CHANNEL1_SET_CURRENT,
    OS_USER_TAG_CAL_CLOOP_CHANNEL2_SET_CURRENT,
    OS_USER_TAG_IBUS_BOARDS_INIT,
    OS_USER_TAG_IBUS_SHOW_CALIBRATION_ZERO,
    OS_USER_TAG_IBUS_SHOW_CALIBRATION_SPAN,
    OS_USER_TAG_CAL_CLOOP_SET_RANGE,
    OS_USER_TAG_KEYBOARD_RECV_IDLE,
    OS_USER_TAG_KEYBOARD_RECV_RXNE,
    OS_USER_TAG_WRITE_PARAM_TO_EXT_FLASH,
    OS_USER_TAG_EXT_MDBS_REINIT,
    OS_USER_TAG_ETH_START,
    OS_USER_TAG_ETH_INT,
    OS_USER_TAG_ETH_CHANGE_PARAM,    
    OS_USER_TAG_ETH_QUEUE_TIMEOUT,
    OS_USER_TAG_VALIDATION_START,    
    OS_USER_TAG_VALIDATION_STOP,
    OS_USER_TAG_VALIDATION_SWITCH_FOR_MEASURE,    
    OS_USER_TAG_VALIDATION_ERROR,    
    OS_USER_TAG_VALIDATION_PASSED,  
    OS_USER_TAG_REMOTE_VALIDATION_START,
    OS_USER_TAG_REMOTE_VALIDATION_BREAK,    
    OS_USER_TAG_CALIBRATION_ZERO_START,
    OS_USER_TAG_CALIBRATION_SPAN_START,
    OS_USER_TAG_CALIBRATION_STOP,
    OS_USER_TAG_CALIBRATION_ERROR,
    OS_USER_TAG_CALIBRATION_PASSED,
    OS_USER_TAG_CALIBRATION_SWITCH_FOR_MEASURE,
    OS_USER_TAG_REMOTE_ZERO_CALIBRATION_START,
    OS_USER_TAG_REMOTE_SPAN_CALIBRATION_START,
    OS_USER_TAG_REMOTE_CALIBRATION_BREAK,
    
} os_user_tag_t;

void send_cmd_for_cloop_set_raw_4mA (uint8_t channel);
void send_cmd_for_cloop_set_raw_20mA (uint8_t channel);
void send_cmd_for_cloop_4mA_write (uint8_t channel);
void send_cmd_for_cloop_20mA_write (uint8_t channel);
void send_cmd_for_cloop_write_range (void);
void send_cmd_for_nvm_write_param (const uint32_t param_id, const uint32_t param_value);
void task_m2m_ext_mdbs_reinit(void);
void send_cmd_to_net_task (os_user_tag_t tag);
void send_cmd_for_cloop_set_current(uint8_t channel, uint32_t uA);

void send_cmd_for_validation_start(void);
void send_cmd_for_validation_stop(void);
void send_cmd_for_validation_return_to_measure (void);
void send_cmd_for_validation_error(void);
void send_cmd_for_validation_passed(void);
void send_cmd_for_remote_validation_start (void);
void send_cmd_for_remote_validation_break (void);

void send_cmd_for_calibration_zero_start (void);
void send_cmd_for_calibration_span_start (void);
void send_cmd_for_calibration_stop (void);
void send_cmd_for_calibration_return_to_measure (void);
void send_cmd_for_calibration_error(void);
void send_cmd_for_calibration_passed (void);
void send_cmd_for_remote_calibration_break(void);
void send_cmd_for_remote_calibration_span_start(void);
void send_cmd_for_remote_calibration_zero_start(void);

#endif //OS_USER_H
