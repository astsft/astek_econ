/**
  * @file    trace.h
  * @brief   Trace/Debug
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef TRACE_H
#define TRACE_H


#include <stdio.h>
#include <stdint.h>
//#include "ff.h"
//#include "app_trace_emwin.h"


static
void app_trace_error(const uint32_t error_code,
                     const uint32_t line_num,
                     const char *file_name)
{
	printf("\n\rERROR: 0x%0X,\t%s, line %d\r\n",
	       error_code,
	       file_name,
	       line_num);
}


#ifdef  NDEBUG
# define APP_TRACE_INIT()
# define APP_TRACE(...)
# define APP_TRACE_ERROR(ERR_CODE)
# define TRACE(...)
#else
# define APP_TRACE_INIT()
# define APP_TRACE(...)                 printf(__VA_ARGS__)
# define APP_TRACE_ERROR(ERR_CODE)      app_trace_error((ERR_CODE), __LINE__, __FILE__)
# define TRACE(...)                     printf(__VA_ARGS__)
#endif

#endif	//TRACE_H
