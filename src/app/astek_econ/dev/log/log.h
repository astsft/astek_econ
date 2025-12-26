/**
  * @file    log.h
  * @brief   Log Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef LOG_H
#define LOG_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "stm32.h"

#define LOG_START_ADDR  (BKPSRAM_BASE + 16)
#define LOG_INDEX_MAX   510

typedef enum log_source_e
{
  LOG_SOURCE_SENSOR = 0,
  LOG_SOURCE_CLOOP,
  LOG_SOURCE_PID,
  LOG_SOURCE_SYSTEM,
  LOG_SOURCE_EXT_RELAY,
  
  LOG_MAX_SOURCE
} log_source_t;

typedef enum log_type_e
{
  LOG_TYPE_WARNING = 0,
  LOG_TYPE_ERROR,
  
  LOG_MAX_TYPE
} log_type_t;

typedef enum log_sensor_event_e
{
  LOG_SENSOR_EVENT_LINK_OK = 0,
  LOG_SENSOR_EVENT_LINK_ERR,
  LOG_SENSOR_EVENT_VALUE_TOO_HIGH_WAR,
  LOG_SENSOR_EVENT_VALUE_TOO_LOW_WAR,
  LOG_SENSOR_EVENT_VALUE_OK,  
  LOG_SENSOR_EVENT_ZERO_CALIBRATION,  
  LOG_SENSOR_EVENT_SPAN_CALIBRATION,
  LOG_SENSOR_EVENT_CALIBRATION_RESTORE,
  LOG_SENSOR_EVENT_LINEARIZATION1,
  LOG_SENSOR_EVENT_LINEARIZATION2,  
  LOG_SENSOR_EVENT_CHAR1,
  LOG_SENSOR_EVENT_CHAR2,    
  LOG_SENSOR_EVENT_ZERO_OFFSET,  
  LOG_SENSOR_EVENT_ZERO_FACTORY_UPDATE,
  LOG_SENSOR_EVENT_ZERO_FACTORY_RESTORE,  
  LOG_SENSOR_EVENT_SPAN_FACTORY_UPDATE,
  LOG_SENSOR_EVENT_ZERO_CALIBRATION_RESTORE,
  LOG_SENSOR_EVENT_SPAN_CALIBRATION_RESTORE,  
  
  LOG_MAX_LOG_SENSOR_EVENT
} log_sensor_event_t;

typedef enum log_cloop_event_e
{
  LOG_CLOOP_EVENT_LINK_OK = 0,
  LOG_CLOOP_EVENT_LINK_ERR,
  LOG_CLOOP_EVENT_CLOOP_CALIBRATION_4MA,
  LOG_CLOOP_EVENT_CLOOP_CALIBRATION_20MA,   

  
  LOG_MAX_LOG_CLOOP_EVENT
} log_cloop_event_t;

typedef enum log_ext_relay_event_e
{
  LOG_EXT_RELAY_EVENT_LINK_OK = 0,
  LOG_EXT_RELAY_EVENT_LINK_ERR,
  
  LOG_MAX_LOG_EXT_RELAY_EVENT
} log_mdb_relay_event_t;

typedef enum log_pid_event_e
{
  LOG_PID_EVENT_LINK_OK = 0,
  LOG_PID_EVENT_LINK_ERR,
  LOG_PID_EVENT_TEMP_MEASURE_OK,
  LOG_PID_EVENT_TEMP_MEASURE_ERR,
  LOG_PID_EVENT_TEMP_TOO_LOW_ERR,
  LOG_PID_EVENT_TEMP_TOO_HIGH_ERR, 
  LOG_PID_EVENT_TEMP_NOT_IN_RANGE_LOW_WAR,    
  LOG_PID_EVENT_TEMP_NOT_IN_RANGE_HIGH_WAR,
  
  LOG_MAX_LOG_PID_EVENT
} log_pid_event_t;

typedef enum log_system_event_e
{
  LOG_SYSTEM_EVENT_START = 0,
  LOG_SYSTEM_EVENT_FACTORY_RESET,
  LOG_SYSTEM_EVENT_VALIDATION_START,
  LOG_SYSTEM_EVENT_VALIDATION_STOP,
  LOG_SYSTEM_EVENT_VALIDATION_ERROR,  
  LOG_SYSTEM_EVENT_VALIDATION_PASSED,
  LOG_SYSTEM_EVENT_CALIBRATION_ZERO_START,
  LOG_SYSTEM_EVENT_CALIBRATION_SPAN_START,
  LOG_SYSTEM_EVENT_CALIBRATION_PASSED,
  LOG_SYSTEM_EVENT_CALIBRATION_STOP,
  LOG_SYSTEM_EVENT_CALIBRATION_ERROR,    

  LOG_MAX_LOG_SYSTEM_EVENT
} log_system_event_t;

#pragma pack ( push, 1 )
typedef struct  log_data_s
{
    uint32_t            timestamp;
    uint8_t             source;
    uint8_t             event;
    uint8_t             event_type;      
    uint8_t             reserved;
} log_data_t;
#pragma pack( pop )

typedef struct  log_s
{
    log_data_t *        buf;
    size_t              buf_size;
    size_t              head;
    log_data_t          last_log_data;
    log_data_t          previous_log_data;
} log_t;


/*******************************************************************************
* FUNCTION PROTOTYPE
*******************************************************************************/
int     log_write_event(log_t * log, log_source_t source, uint8_t event);
int     log_read_event(log_data_t *data, size_t idx);
void    log_prepare_data (log_data_t *data, log_source_t source, uint8_t event);
int     log_push_data( log_t * log, log_data_t *data);
int     log_init (log_t * log);
void    copy_log_data (log_data_t *src, log_data_t *dst);



#endif //LOG_H
