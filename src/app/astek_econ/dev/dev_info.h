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
typedef union
{
    char        c_tag[12];
    uint32_t    u32[3];
} dev_tag_t;

typedef struct  dev_info_s
{
    uint16_t        device_id;
    uint16_t        hardware_id;
    uint32_t        sn;
    uint16_t *      uid_u16[ 8];
    uint32_t *      uid_u32[ 4];
    dev_tag_t *     tag;
    char            real_firmware_id[64];           
    char const *    firmware_str;
    char const *    hardware_str;  
    char const *    device_str;  
} dev_info_t;

#endif  //DEV_INFO_H
