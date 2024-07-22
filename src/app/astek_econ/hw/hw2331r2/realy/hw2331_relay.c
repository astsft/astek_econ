#include "hw_relay.h"
#include "../../dev/dev.h"
#include "asback.h"

extern dev_t   dev;
static uint8_t relay_raw;

int relay_hw_init (void)
{
  asback_relay_init();
  
  return 0;
}

int relay_get_info (void)
{
    dev.ext_relay->info.device_id   = RELAY_DEV_ID;
    dev.ext_relay->info.hardware_id = RELAY_HW_ID;
    
    return 0;   
}

int relay_set_state(const uint8_t relay_num, const bool state)
{
  asback_relay_t  relay;  
  
  switch (relay_num) {
  case 0: relay = ASBACK_RELAY_1;
          break;
  case 1: relay = ASBACK_RELAY_2;
          break;
  case 2: relay = ASBACK_RELAY_3;
          break;
  case 3: relay = ASBACK_RELAY_4;
          break;
  case 4: relay = ASBACK_RELAY_5;
          break;
  case 5: relay = ASBACK_RELAY_6;
          break;
  default: return -1;    
  }
    
  asback_relay_set(relay, state);
}

int relay_get_state(const uint8_t relay_num, bool *state)
{
  asback_relay_t  relay;  
  
  switch (relay_num) {
  case 0: relay = ASBACK_RELAY_1;
          break;
  case 1: relay = ASBACK_RELAY_2;
          break;
  case 2: relay = ASBACK_RELAY_3;
          break;
  case 3: relay = ASBACK_RELAY_4;
          break;
  case 4: relay = ASBACK_RELAY_5;
          break;
  case 5: relay = ASBACK_RELAY_6;
          break;
  default: return -1;    
  }  
  
 *state = asback_relay_get(relay);
}

