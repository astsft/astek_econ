#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );


/* Hook prototypes */
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);


__weak void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}


static  StaticTask_t    xIdleTaskTCBBuffer;
static  StackType_t     xIdleStack[ configMINIMAL_STACK_SIZE ];


void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
        *ppxIdleTaskTCBBuffer   =   &xIdleTaskTCBBuffer;
        *ppxIdleTaskStackBuffer =   &xIdleStack[0];
        *pulIdleTaskStackSize   =   configMINIMAL_STACK_SIZE;
}
