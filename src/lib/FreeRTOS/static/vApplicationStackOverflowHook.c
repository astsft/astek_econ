/**
  * @file   vApplicationStackOverflowHook.c
  * @link   www.freertos.org/a00110.html
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "cmsis_os.h"
#include "stm32\stm32.h"


void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);


__weak
void
vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    //Run time stack overflow checking is performed if configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.
    //This hook function is called if a stack overflow is detected.

    #ifdef  NDEBUG
    NVIC_SystemReset();
    #else
    __asm volatile ("bkpt #0\n");   // Break into the debugger
    #endif //NDEBUG
}


