#ifndef  _HW_CL420_H_
#define  _HW_CL420_H_

#include <stdint.h>

int cloop_hw_init (void);
int cloop_get_info(void);
int cloop_get_cal(void);
int cloop_get_range(void);
int cloop_set_range(void);
int cloop_set_raw(uint8_t ch, uint32_t raw);
int cloop_get_raw(uint8_t ch, uint32_t* current_raw);
int cloop_set_uA(uint8_t ch, uint32_t uA);
int cloop_get_uA(uint8_t ch, uint32_t *uA);
int set_cal_4ma(uint8_t ch);
int set_cal_20ma(uint8_t ch);
int coarse_increase_4ma_raw(uint8_t ch);
int fine_increase_4ma_raw(uint8_t ch);
int coarse_decrease_4ma_raw(uint8_t ch);
int fine_decrease_4ma_raw(uint8_t ch);
int coarse_increase_20ma_raw(uint8_t ch);
int fine_increase_20ma_raw(uint8_t ch);
int coarse_decrease_20ma_raw(uint8_t ch);
int fine_decrease_20ma_raw(uint8_t ch);

#endif //_HW_CL420_H_
