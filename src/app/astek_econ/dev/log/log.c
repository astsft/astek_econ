/**
  * @file    log.c
  * @brief   Log Functions
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "dev/log/log.h"
#include "dev/dev.h"

extern  dev_t           dev;

/*******************************************************************************
*
*******************************************************************************/
int 
log_write_event(log_t * log, log_source_t source, uint8_t event)
{
  log_prepare_data (&log->last_log_data, source, event);
  log_push_data (log, &log->last_log_data);
  
  return 0;
}

int
log_push_data(                           log_t *                 log,
                                    log_data_t *            data )
{
    /* calc write addr */
    uint32_t write_addr = log->head * sizeof(log_data_t) + LOG_START_ADDR;
    log->head++;
    
    /* check write index */
    if (log->head >= LOG_INDEX_MAX) log->head = 0; 
    
    memcpy((uint32_t *)write_addr, data, sizeof (log_data_t));
    
    return( 0 );
}

void copy_log_data (log_data_t *src, log_data_t *dst)
{
    memcpy(dst, src, sizeof(log_data_t)); 
}
         
void log_prepare_data (log_data_t *data, log_source_t source, uint8_t event)
{
    data->timestamp = dev.mcu->rtc.get_timestamp();
    data->source = source;
    data->event = event;     
    
    switch(data->source)
    {
    case LOG_SOURCE_SENSOR:
      switch (data->event)
      {
      case LOG_SENSOR_EVENT_LINK_ERR:
          data->event_type = LOG_TYPE_ERROR;
         break;

      default:
          data->event_type = LOG_TYPE_WARNING;
          break;          
      }
      break;
      
    case LOG_SOURCE_CLOOP:
      switch (data->event)
      {
      case LOG_CLOOP_EVENT_LINK_ERR:
          data->event_type = LOG_TYPE_ERROR;
          break;
          
      default:
          data->event_type = LOG_TYPE_WARNING;
          break;
      }
      break;
      
    case LOG_SOURCE_EXT_RELAY:
      switch (data->event)
      {
      case LOG_EXT_RELAY_EVENT_LINK_ERR:
          data->event_type = LOG_TYPE_ERROR;
          break;
          
      default:
          data->event_type = LOG_TYPE_WARNING;
          break;
      }
      break;      
            
    case LOG_SOURCE_SYSTEM:
      switch (data->event)
      {
                
      default:
          data->event_type = LOG_TYPE_WARNING;
          break;
      }      
      break;      
    }   
}

int     log_read_event(log_data_t *data, size_t idx)
{
  /* check write index */
 if (idx >= LOG_INDEX_MAX) return -1; 

  uint32_t write_addr = idx * sizeof(log_data_t) + LOG_START_ADDR;
  
  memcpy(data, (uint32_t *)write_addr, sizeof(log_data_t));  
  
  /* check data for valid */
  /* check timestamp for valid*/
  if (!((data->timestamp >= 0) && (data->timestamp <= 2147472000))) return -1;
  
  /* check source for valid */
  if (data->source >= LOG_MAX_SOURCE) return -1;
  
  /* check event for valid  */
  if (data->event_type >= LOG_MAX_TYPE) return -1;
  
  return 0;
}

/*
* Search last data index
*/
int     log_init (log_t * log)
{
  log->head = 0;
  memset(&log->last_log_data, 0x00, sizeof(log_data_t));
  memset(&log->previous_log_data, 0x00, sizeof(log_data_t));
  
  /* Read data first*/
  log_read_event(&log->previous_log_data, log->head);
  
  /*Check timestamp for valid*/
  if (!((log->previous_log_data.timestamp >= 1685566800) && (log->previous_log_data.timestamp <= 2147472000))) return 0;
  
  /* check source for valid */
  if (log->last_log_data.source >= LOG_MAX_SOURCE) return 0;
  
  /* check event for valid  */
  if (log->last_log_data.event_type >= LOG_MAX_TYPE) return 0;

  /* find last valid index */
  for (int i=0; i < LOG_INDEX_MAX; i++)
  {  
    if (i == (LOG_INDEX_MAX - 1))
    {
      log->head = 0; 
      return 0;
    }
    
    /* Read data second*/
    log_read_event(&log->last_log_data, (log->head + 1));
    
    /*Check timestamp for valid*/
    if (!((log->last_log_data.timestamp >= 1685566800) && (log->last_log_data.timestamp <= 2147472000))) 
    {
      log->head++;
      return 0;
    }
    
    /* check source for valid */
    if (log->last_log_data.source >= LOG_MAX_SOURCE)
    {
      log->head++;
      return 0;
    }
  
    /* check event for valid  */
    if (log->last_log_data.event_type >= LOG_MAX_TYPE)
    {
      log->head++;
      return 0;
    }

    /* compare timestamp */
    if (log->last_log_data.timestamp < log->previous_log_data.timestamp)
    {
      log->head++;
      return 0;
    }
    
    copy_log_data (&log->last_log_data, &log->previous_log_data);
    
    log->head++;
  }
}