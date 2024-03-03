/**
  * @file    dev_info.c
  * @brief   Device Info Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef DEV_INFO_H
#define DEV_INFO_H


/*******************************************************************************
* TYPES
*******************************************************************************/
typedef struct  dev_info_s
{
    uint16_t        device_id;
    uint16_t        hardware_id;
    uint16_t *      serial_num_u16[ 8];
    uint32_t *      serial_num_u32[ 4];
    char            real_firmware_id[64];
    char const *    firmware_str;
    char const *    hardware_str;  
    char const *    device_str;  
} dev_info_t;

#endif  //DEV_INFO_H
