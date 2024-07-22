/**
  * @file    GUIConf.c
  * @brief   Display controller initialization ( emWin V5.28 )
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdint.h>
#include "GUI.h"


#define SDRAM_BASE                      0xC0000000
#define GUI_MEMORY_COUNTOF              (8*1024*1024)


#pragma data_alignment=32
uint32_t *    gui_memory = (uint32_t *)(SDRAM_BASE);


/**
  * @brief  Set up the available memory for the GUI. Called during the initialization.
  * @param  None
  * @retval None
  */
void
GUI_X_Config( void )
{
    GUI_ALLOC_AssignMemory( (uint32_t *) gui_memory, GUI_MEMORY_COUNTOF );
}
