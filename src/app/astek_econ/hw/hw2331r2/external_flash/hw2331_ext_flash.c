#include "hw2331_ext_flash.h"
#include "../../dev/nvm/nvm.h"
#include "flash_spi.h"

void flash_write_param(size_t param_id, const uint32_t value)
{
  uint32_t addr = param_id * 4;
  flash_write_data(addr, 4, (uint8_t*)&value);  
}


uint32_t flash_read_param(uint32_t param_id)
{
  uint32_t addr = param_id * 4;
  uint32_t temp_data = 0;
  flashReadData(addr, 4, (uint8_t*)&temp_data);

  return temp_data;
}
