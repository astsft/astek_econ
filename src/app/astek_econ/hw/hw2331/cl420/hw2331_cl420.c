#include "hw_cl420.h"
#include "../../dev/dev.h"
#include "asback.h"

#define CONFIG_ASBACK_CL420_CAL_A_DEFAULT   1.0f
#define CONFIG_ASBACK_CL420_CAL_B_DEFAULT   1.0f
#define CL_PWM_PERIOD           0x0FFF

extern  dev_t                   dev;

static  const   asback_cl420_drv_t      asback_cl420_ch1_drv    =
{
    .set    = stm32_tim4_ch1_set,
    .get    = stm32_tim4_ch1_get,
};

static          asback_cl420_cal_t      asback_cl420_ch1_cal    =
{
    .a      = CONFIG_ASBACK_CL420_CAL_A_DEFAULT,
    .b      = CONFIG_ASBACK_CL420_CAL_B_DEFAULT,
};

static          asback_cl420_t          asback_cl420_ch1        =
{
    .drv    = &asback_cl420_ch1_drv,
    .cal    = &asback_cl420_ch1_cal,
};

static  const   asback_cl420_drv_t      asback_cl420_ch2_drv    =
{
    .set    = stm32_tim4_ch2_set,
    .get    = stm32_tim4_ch2_get,
};

static          asback_cl420_cal_t      asback_cl420_ch2_cal    =
{
    .a      = CONFIG_ASBACK_CL420_CAL_A_DEFAULT,
    .b      = CONFIG_ASBACK_CL420_CAL_B_DEFAULT,
};

static          asback_cl420_t          asback_cl420_ch2        =
{
    .drv    = &asback_cl420_ch2_drv,
    .cal    = &asback_cl420_ch2_cal,
};


int cloop_hw_init (void)
{
  asback_cl420_init( CL_PWM_PERIOD );
  
  return 0;
}

int cloop_get_info(void)
{
  //device_id
  dev.cloop->info.device_id    = CL420_DEV_ID;
      
  //hw_id
  dev.cloop->info.hardware_id  = CL420_HW_ID;    
  
  return 0;
}

int cloop_get_cal(void)
{
  dev.cloop->cal_ch1[0].timestamp.i32 = dev.nvm.get(NVM_REG_CL420_CH1_CAL0_TIMESTAMP);
  dev.cloop->cal_ch1[0].uA.i32        = dev.nvm.get(NVM_REG_CL420_CH1_CAL0_UA);
  dev.cloop->cal_ch1[0].raw.i32       = dev.nvm.get(NVM_REG_CL420_CH1_CAL0_RAW);    
  dev.cloop->cal_ch1[1].timestamp.i32 = dev.nvm.get(NVM_REG_CL420_CH1_CAL1_TIMESTAMP);
  dev.cloop->cal_ch1[1].uA.i32        = dev.nvm.get(NVM_REG_CL420_CH1_CAL1_UA);
  dev.cloop->cal_ch1[1].raw.i32       = dev.nvm.get(NVM_REG_CL420_CH1_CAL1_RAW);    
  
  dev.cloop->cal_ch2[0].timestamp.i32 = dev.nvm.get(NVM_REG_CL420_CH2_CAL0_TIMESTAMP);
  dev.cloop->cal_ch2[0].uA.i32        = dev.nvm.get(NVM_REG_CL420_CH2_CAL0_UA);
  dev.cloop->cal_ch2[0].raw.i32       = dev.nvm.get(NVM_REG_CL420_CH2_CAL0_RAW);    
  dev.cloop->cal_ch2[1].timestamp.i32 = dev.nvm.get(NVM_REG_CL420_CH2_CAL1_TIMESTAMP);
  dev.cloop->cal_ch2[1].uA.i32        = dev.nvm.get(NVM_REG_CL420_CH2_CAL1_UA);
  dev.cloop->cal_ch2[1].raw.i32       = dev.nvm.get(NVM_REG_CL420_CH2_CAL1_RAW);      
  
  // channel 1
  asback_cl420_ch2.cal->uA[0] = dev.cloop->cal_ch1[0].uA.i32;
  asback_cl420_ch2.cal->raw[0] = dev.cloop->cal_ch1[0].raw.i32;
  asback_cl420_ch2.cal->uA[1] = dev.cloop->cal_ch1[1].uA.i32;
  asback_cl420_ch2.cal->raw[1] = dev.cloop->cal_ch1[1].raw.i32;     
  asback_cl420_cal_restore(&asback_cl420_ch2);
  
  // channel 2
  asback_cl420_ch1.cal->uA[0] = dev.cloop->cal_ch2[0].uA.i32;
  asback_cl420_ch1.cal->raw[0] = dev.cloop->cal_ch2[0].raw.i32;
  asback_cl420_ch1.cal->uA[1] = dev.cloop->cal_ch2[1].uA.i32;
  asback_cl420_ch1.cal->raw[1] = dev.cloop->cal_ch2[1].raw.i32;     
  asback_cl420_cal_restore(&asback_cl420_ch1);  
  
  return 0;
}

int cloop_get_range(void)
{
  dev.cl420.range[0].ppm  = dev.nvm.get(NVM_REG_RANGE_R1_PPM); 
  dev.cl420.range[1].ppm  = dev.nvm.get(NVM_REG_RANGE_R2_PPM);        
  dev.cl420.range[2].ppm  = dev.nvm.get(NVM_REG_RANGE_R3_PPM);
  
  dev.cl420.range_idx = dev.nvm.get(NVM_REG_RANGE_IDX);      
  dev.cl420.range[0].units = dev.nvm.get(NVM_REG_RANGE_UNITS);
  dev.cl420.range[1].units = dev.nvm.get(NVM_REG_RANGE_UNITS);
  dev.cl420.range[2].units = dev.nvm.get(NVM_REG_RANGE_UNITS);
  
  return 0;
}

int cloop_set_range(void)
{
  dev.nvm.put(NVM_REG_RANGE_R1_PPM, dev.cl420.range[0].ppm);
  dev.nvm.put(NVM_REG_RANGE_R2_PPM, dev.cl420.range[1].ppm);
  dev.nvm.put(NVM_REG_RANGE_R3_PPM, dev.cl420.range[2].ppm);
  dev.nvm.put(NVM_REG_RANGE_IDX, dev.cl420.range_idx);
  dev.nvm.put(NVM_REG_RANGE_UNITS, dev.cl420.range[0].units);
  
  return 0;
}

int cloop_set_raw(uint8_t ch, uint32_t raw)
{
  if (ch == 1)
  {
    asback_cl420_set_raw( &asback_cl420_ch2, raw );
  }
  else
  {
    asback_cl420_set_raw( &asback_cl420_ch1, raw );
  }
  
  return 0;
}

int cloop_get_raw(uint8_t ch, uint32_t* current_raw)
{
  if (ch == 1)
  {
    *current_raw = asback_cl420_get_raw( &asback_cl420_ch2 );
  }
  else
  {
    *current_raw = asback_cl420_get_raw( &asback_cl420_ch1 );
  }
  
  return 0;
}

int cloop_set_uA(uint8_t ch, uint32_t uA)
{
  if (ch == 1)
  {
    asback_cl420_set_uA( &asback_cl420_ch2, uA );
  }
  else
  {
    asback_cl420_set_uA( &asback_cl420_ch1, uA );
  }
  
  return 0;
}

int cloop_get_uA(uint8_t ch, uint32_t *uA)
{
  if (ch == 1)
  {
    *uA = asback_cl420_get_uA( &asback_cl420_ch2 );
  }
  else
  {
    *uA = asback_cl420_get_uA( &asback_cl420_ch1 );
  }
  
  return 0;
}

int set_cal_4ma(uint8_t ch)
{
  uint32_t current_raw;
  cloop_get_raw(ch, &current_raw);
  
  if (ch == 1)
  {
    dev.nvm.put(NVM_REG_CL420_CH1_CAL0_TIMESTAMP,  dev.mcu->rtc.get_timestamp());
    dev.nvm.put(NVM_REG_CL420_CH1_CAL0_UA, 4000);
    dev.nvm.put(NVM_REG_CL420_CH1_CAL0_RAW, current_raw);
    asback_cl420_ch2.cal->uA[0] = 4000;
    asback_cl420_ch2.cal->raw[0] = current_raw;
    asback_cl420_cal_restore(&asback_cl420_ch2);
  }
  else 
  {
    dev.nvm.put(NVM_REG_CL420_CH2_CAL0_TIMESTAMP,  dev.mcu->rtc.get_timestamp());
    dev.nvm.put(NVM_REG_CL420_CH2_CAL0_UA, 4000);
    dev.nvm.put(NVM_REG_CL420_CH2_CAL0_RAW, current_raw);
    asback_cl420_ch1.cal->uA[0] = 4000;
    asback_cl420_ch1.cal->raw[0] = current_raw;    
    asback_cl420_cal_restore(&asback_cl420_ch1);   
  }
  
  return 0;
}

int set_cal_20ma(uint8_t ch)
{
  uint32_t current_raw;
  cloop_get_raw(ch, &current_raw);
  
  if (ch == 1)
  {
    dev.nvm.put(NVM_REG_CL420_CH1_CAL1_TIMESTAMP,  dev.mcu->rtc.get_timestamp());
    dev.nvm.put(NVM_REG_CL420_CH1_CAL1_UA, 20000);
    dev.nvm.put(NVM_REG_CL420_CH1_CAL1_RAW, current_raw);
    asback_cl420_ch2.cal->uA[1] = 20000;
    asback_cl420_ch2.cal->raw[1] = current_raw;    
    asback_cl420_cal_restore(&asback_cl420_ch2);
  }
  else 
  {
    dev.nvm.put(NVM_REG_CL420_CH2_CAL1_TIMESTAMP,  dev.mcu->rtc.get_timestamp());
    dev.nvm.put(NVM_REG_CL420_CH2_CAL1_UA, 20000);
    dev.nvm.put(NVM_REG_CL420_CH2_CAL1_RAW, current_raw);
    asback_cl420_ch1.cal->uA[1] = 20000;
    asback_cl420_ch1.cal->raw[1] = current_raw;        
    asback_cl420_cal_restore(&asback_cl420_ch1);
  }
  
  return 0;
}

int coarse_increase_4ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] += 2;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] += 2;
  }
  
  return 0;
}

int fine_increase_4ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] += 1;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] += 1;
  }  
  
  return 0;
}

int coarse_increase_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] += 2;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] += 2;
  }  
  
  return 0;
}

int fine_increase_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] += 1;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] += 1;
  }    
  
  return 0;
}

int coarse_decrease_4ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] -= 2;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] -= 2;
  }  
  
  return 0;
}

int fine_decrease_4ma_raw(uint8_t ch)
{
    if (ch == 1)
  {
    dev.cloop->cal_ch1[0].raw.u16[0] -= 1;
  }
  else
  {
    dev.cloop->cal_ch2[0].raw.u16[0] -= 1;
  }  
  
  return 0;
}

int coarse_decrease_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] -= 2;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] -= 2;
  }
  
  return 0;
}

int fine_decrease_20ma_raw(uint8_t ch)
{
  if (ch == 1)
  {
    dev.cloop->cal_ch1[1].raw.u16[0] -= 1;
  }
  else
  {
    dev.cloop->cal_ch2[1].raw.u16[0] -= 1;
  }     
  
  return 0;
}