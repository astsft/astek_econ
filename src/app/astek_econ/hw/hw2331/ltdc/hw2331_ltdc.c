#include "hw_ltdc.h"
#include "stm32.h"

void ltdc_swap_x(uint8_t swap_flag)
{
  stm32_ltdc_swap_x( !swap_flag );
}

void ltdc_swap_y(uint8_t swap_flag)
{
  stm32_ltdc_swap_y( swap_flag );
}
