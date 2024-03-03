/**
  * @file    dev_safe.c
  * @brief   Device Safe Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef DEV_SAFE_H
#define DEV_SAFE_H


#include <stdint.h>


/*******************************************************************************
* TYPES
*******************************************************************************/
typedef struct
{
    union
    {
        uint32_t        u32;
        uint8_t         u08[ 4];
    };
} dev_safe_password_t;


typedef struct  dev_safe_s
{
    struct
    {
        dev_safe_password_t         password;
    } user;

    struct
    {
        dev_safe_password_t         password;
    } master;

} dev_safe_t;

#endif  //DEV_SAFE_H
