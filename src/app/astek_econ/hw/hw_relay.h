#ifndef  _HW_RELAY_H_
#define  _HW_RELAY_H_

#include <stdint.h>
#include <stdbool.h>

#if (defined(ASBACK_HW2353REV3) || defined(ASBACK_HW2353REV4))
typedef enum 
{
  EXTERNAL_1 = 0x00,
  EXTERNAL_2,
  EXTERNAL_3,
  EXTERNAL_4,
  EXTERNAL_5,
  EXTERNAL_6,  
  EXTERNAL_7,
  EXTERNAL_8
} Outputs_swithes_e;
#elif defined(ASBACK_HW2353REV5)
typedef enum 
{ 
  EXTERNAL_8 = 0,
  EXTERNAL_1,
  EXTERNAL_2,
  EXTERNAL_3,
  EXTERNAL_4,
  EXTERNAL_5,
  EXTERNAL_6,  
  EXTERNAL_7,
} Outputs_swithes_e;
#else
typedef enum 
{
  EXTERNAL_1 = 0x00,
  EXTERNAL_2,
  EXTERNAL_3,
  EXTERNAL_4,
} Outputs_swithes_e;
#endif

int relay_hw_init (void);
int relay_get_info (void);
int relay_set_state(const uint8_t relay_num, const bool state);
int relay_get_state(const uint8_t relay_num, bool *state);

#endif //_HW_RELAY_H_