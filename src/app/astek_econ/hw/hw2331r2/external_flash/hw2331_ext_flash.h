#ifndef  _HW_EXT_FLASH_H_
#define  _HW_EXT_FLASH_H_

#include <stdint.h>
#include <stdlib.h>
#include "flash_spi.h"

void flash_write_param(size_t param_id, const uint32_t value);
uint32_t flash_read_param(size_t param_id);

#endif // _HW_EXT_FLASH_H_