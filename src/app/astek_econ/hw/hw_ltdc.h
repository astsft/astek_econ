#ifndef  _HW_LTDC_H_
#define  _HW_LTDC_H_

#include <stdint.h>

void ltdc_swap_x(uint8_t swap_flag);
void ltdc_swap_y(uint8_t swap_flag);
void ltdc_reset_swap(uint8_t swap_flag);

#endif // _HW_LTDC_H_