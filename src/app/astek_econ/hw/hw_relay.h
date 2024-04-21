#ifndef  _HW_RELAY_H_
#define  _HW_RELAY_H_

#include <stdint.h>
#include <stdbool.h>

int relay_hw_init (void);
int relay_get_info (void);
int relay_set_state(const uint8_t relay_num, const bool state);
int relay_get_state(const uint8_t relay_num, bool *state);

#endif //_HW_RELAY_H_