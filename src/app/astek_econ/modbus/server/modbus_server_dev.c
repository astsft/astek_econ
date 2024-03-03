/**
  * @file    mdbs_dev.c
  * @brief   MODBUS Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "modbus.h"
#include "modbus\server\modbus_server_dev.h"
#include "main.h"
#include "dev\dev.h"


extern  dev_t          dev;



/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_coil_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_coil_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_dinp_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_dinp_write(                        const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_ireg_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_ireg_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


mdbs_err_t
mdbs_hreg_read(                         const   size_t                  addr,
                                                uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;


        switch( addr )
        {
                case MDBS_TERM_HREG_DEVICE_ID_HI:
                case MDBS_TERM_HREG_DEVICE_ID_LO:
                case MDBS_TERM_HREG_HARDWARE_ID_HI:
                case MDBS_TERM_HREG_HARDWARE_ID_LO:
                case MDBS_TERM_HREG_FIRMWARE_ID_HI:
                case MDBS_TERM_HREG_FIRMWARE_ID_MI:
                case MDBS_TERM_HREG_FIRMWARE_ID_LO:
                case MDBS_TERM_HREG_RESERVED_07:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SERIAL_NUM_00:
                case MDBS_TERM_HREG_SERIAL_NUM_01:
                case MDBS_TERM_HREG_SERIAL_NUM_02:
                case MDBS_TERM_HREG_SERIAL_NUM_03:
                case MDBS_TERM_HREG_SERIAL_NUM_04:
                case MDBS_TERM_HREG_SERIAL_NUM_05:
                case MDBS_TERM_HREG_SERIAL_NUM_06:
                case MDBS_TERM_HREG_SERIAL_NUM_07:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_ERR_CODE_LAST:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_STARTS_COUNTER:
                        *data   = dev.cfg.starts_count;
                        break;

                case MDBS_TERM_HREG_RESERVED_12:
                case MDBS_TERM_HREG_RESERVED_13:
                case MDBS_TERM_HREG_RESERVED_14:
                case MDBS_TERM_HREG_RESERVED_15:
                case MDBS_TERM_HREG_RESERVED_16:
                case MDBS_TERM_HREG_RESERVED_17:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_MCU_TEMP_DIGC:
                        //*data   = dev.sens->meas.mcu_temp_digc;
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_MCU_VDDA_mV:
                        //*data   = dev.sens->meas.mcu_vdda_mv;
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FV_HI:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FV_LO:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FT_HI:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FT_LO:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FP_HI:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FP_LO:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_STS_RESERVED_0x200:
                case MDBS_TERM_HREG_STS_RESERVED_0x201:
                case MDBS_TERM_HREG_STS_RESERVED_0x202:
                case MDBS_TERM_HREG_STS_RESERVED_0x203:
                case MDBS_TERM_HREG_STS_RESERVED_0x204:
                case MDBS_TERM_HREG_STS_RESERVED_0x205:
                case MDBS_TERM_HREG_STS_RESERVED_0x206:
                case MDBS_TERM_HREG_STS_RESERVED_0x207:
                case MDBS_TERM_HREG_STS_RESERVED_0x208:
                case MDBS_TERM_HREG_STS_RESERVED_0x209:
                case MDBS_TERM_HREG_STS_RESERVED_0x20A:
                case MDBS_TERM_HREG_STS_RESERVED_0x20B:
                case MDBS_TERM_HREG_STS_RESERVED_0x20C:
                case MDBS_TERM_HREG_STS_RESERVED_0x20D:
                case MDBS_TERM_HREG_STS_RESERVED_0x20E:
                case MDBS_TERM_HREG_STS_RESERVED_0x20F:
                case MDBS_TERM_HREG_STS_RESERVED_0x210:
                case MDBS_TERM_HREG_STS_RESERVED_0x211:
                case MDBS_TERM_HREG_STS_RESERVED_0x212:
                case MDBS_TERM_HREG_STS_RESERVED_0x213:
                case MDBS_TERM_HREG_STS_RESERVED_0x214:
                case MDBS_TERM_HREG_STS_RESERVED_0x215:
                case MDBS_TERM_HREG_STS_RESERVED_0x216:
                case MDBS_TERM_HREG_STS_RESERVED_0x217:
                case MDBS_TERM_HREG_STS_RESERVED_0x218:
                case MDBS_TERM_HREG_STS_RESERVED_0x219:
                case MDBS_TERM_HREG_STS_RESERVED_0x21A:
                case MDBS_TERM_HREG_STS_RESERVED_0x21B:
                case MDBS_TERM_HREG_STS_RESERVED_0x21C:
                case MDBS_TERM_HREG_STS_RESERVED_0x21D:
                case MDBS_TERM_HREG_STS_RESERVED_0x21E:
                case MDBS_TERM_HREG_STS_RESERVED_0x21F:
                        *data   = 0;
                        break;


                case MDBS_TERM_HREG_STS_RESERVED_0x400:
                case MDBS_TERM_HREG_STS_RESERVED_0x401:
                case MDBS_TERM_HREG_STS_RESERVED_0x402:
                case MDBS_TERM_HREG_STS_RESERVED_0x403:
                case MDBS_TERM_HREG_STS_RESERVED_0x404:
                case MDBS_TERM_HREG_STS_RESERVED_0x405:
                case MDBS_TERM_HREG_STS_RESERVED_0x406:
                case MDBS_TERM_HREG_STS_RESERVED_0x407:
                case MDBS_TERM_HREG_STS_RESERVED_0x408:
                case MDBS_TERM_HREG_STS_RESERVED_0x409:
                case MDBS_TERM_HREG_STS_RESERVED_0x40A:
                case MDBS_TERM_HREG_STS_RESERVED_0x40B:
                case MDBS_TERM_HREG_STS_RESERVED_0x40C:
                case MDBS_TERM_HREG_STS_RESERVED_0x40D:
                case MDBS_TERM_HREG_STS_RESERVED_0x40E:
                case MDBS_TERM_HREG_STS_RESERVED_0x40F:
                case MDBS_TERM_HREG_STS_RESERVED_0x410:
                case MDBS_TERM_HREG_STS_RESERVED_0x411:
                case MDBS_TERM_HREG_STS_RESERVED_0x412:
                case MDBS_TERM_HREG_STS_RESERVED_0x413:
                case MDBS_TERM_HREG_STS_RESERVED_0x414:
                case MDBS_TERM_HREG_STS_RESERVED_0x415:
                case MDBS_TERM_HREG_STS_RESERVED_0x416:
                case MDBS_TERM_HREG_STS_RESERVED_0x417:
                case MDBS_TERM_HREG_STS_RESERVED_0x418:
                case MDBS_TERM_HREG_STS_RESERVED_0x419:
                case MDBS_TERM_HREG_STS_RESERVED_0x41A:
                case MDBS_TERM_HREG_STS_RESERVED_0x41B:
                case MDBS_TERM_HREG_STS_RESERVED_0x41C:
                case MDBS_TERM_HREG_STS_RESERVED_0x41D:
                case MDBS_TERM_HREG_STS_RESERVED_0x41E:
                case MDBS_TERM_HREG_STS_RESERVED_0x41F:
                case MDBS_TERM_HREG_STS_RESERVED_0x420:
                case MDBS_TERM_HREG_STS_RESERVED_0x421:
                case MDBS_TERM_HREG_STS_RESERVED_0x422:
                case MDBS_TERM_HREG_STS_RESERVED_0x423:
                case MDBS_TERM_HREG_STS_RESERVED_0x424:
                case MDBS_TERM_HREG_STS_RESERVED_0x425:
                case MDBS_TERM_HREG_STS_RESERVED_0x426:
                case MDBS_TERM_HREG_STS_RESERVED_0x427:
                case MDBS_TERM_HREG_STS_RESERVED_0x428:
                case MDBS_TERM_HREG_STS_RESERVED_0x429:
                case MDBS_TERM_HREG_STS_RESERVED_0x42A:
                case MDBS_TERM_HREG_STS_RESERVED_0x42B:
                case MDBS_TERM_HREG_STS_RESERVED_0x42C:
                case MDBS_TERM_HREG_STS_RESERVED_0x42D:
                case MDBS_TERM_HREG_STS_RESERVED_0x42E:
                case MDBS_TERM_HREG_STS_RESERVED_0x42F:
                case MDBS_TERM_HREG_STS_RESERVED_0x430:
                case MDBS_TERM_HREG_STS_RESERVED_0x431:
                case MDBS_TERM_HREG_STS_RESERVED_0x432:
                case MDBS_TERM_HREG_STS_RESERVED_0x433:
                case MDBS_TERM_HREG_STS_RESERVED_0x434:
                case MDBS_TERM_HREG_STS_RESERVED_0x435:
                case MDBS_TERM_HREG_STS_RESERVED_0x436:
                case MDBS_TERM_HREG_STS_RESERVED_0x437:
                case MDBS_TERM_HREG_STS_RESERVED_0x438:
                case MDBS_TERM_HREG_STS_RESERVED_0x439:
                case MDBS_TERM_HREG_STS_RESERVED_0x43A:
                case MDBS_TERM_HREG_STS_RESERVED_0x43B:
                case MDBS_TERM_HREG_STS_RESERVED_0x43C:
                case MDBS_TERM_HREG_STS_RESERVED_0x43D:
                case MDBS_TERM_HREG_STS_RESERVED_0x43E:
                case MDBS_TERM_HREG_STS_RESERVED_0x43F:
                        *data   = 0;
                        break;


                case MDBS_TERM_HREG_SENS_OXGN_PPM_HI:
                        *data   = dev.sens->meas.ppm.integral >> 16;
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_PPM_LO:
                        *data   = dev.sens->meas.ppm.integral & 0xFFFF;
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_DIGC_HI:
                        *data   = dev.sens->meas.digc.integral >> 16;
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_DIGC_LO:
                        *data   = dev.sens->meas.digc.integral & 0xFFFF;
                        break;

                case MDBS_TERM_HREG_SENS_PRES_HPA_HI:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_PRES_HPA_LO:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_RESERVED_26:
                case MDBS_TERM_HREG_RESERVED_27:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_RAW_HI:
                        *data   = dev.sens->meas.raw >> 16;
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_RAW_LO:
                        *data   = dev.sens->meas.raw & 0xFFFF;
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_RAW_HI:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_RAW_LO:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_PRES_RAW_HI:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_PRES_RAW_LO:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_SLOPE_RAW:
                        *data   = dev.sens->meas.slope;
                        break;

                case MDBS_TERM_HREG_SENS_OFST_RAW:
                        //*data   = dev.ibus->sens.value_4;
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_HI:
                        *data   = dev.sens->cal.zero.timestamp.u16[1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_LO:
                        *data   = dev.sens->cal.zero.timestamp.u16[0];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_HI:
                        *data   = dev.sens->cal.zero.ppm.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_LO:
                        *data   = dev.sens->cal.zero.ppm.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_HI:
                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_LO:
                case MDBS_TERM_HREG_RESERVED_36:
                case MDBS_TERM_HREG_RESERVED_37:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_HI:
                        *data   = dev.sens->cal.span.timestamp.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_LO:
                        *data   = dev.sens->cal.span.timestamp.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_HI:
                        *data   = dev.sens->cal.span.ppm.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_LO:
                        *data   = dev.sens->cal.span.ppm.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_RAW_HI:
                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_RAW_LO:
                case MDBS_TERM_HREG_RESERVED_3E:
                case MDBS_TERM_HREG_RESERVED_3F:
                        *data   = 0;
                        break;


                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}


mdbs_err_t
mdbs_hreg_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;


        switch( idx )
        {
                case MDBS_TERM_HREG_DEVICE_ID_HI:
                case MDBS_TERM_HREG_DEVICE_ID_LO:
                case MDBS_TERM_HREG_HARDWARE_ID_HI:
                case MDBS_TERM_HREG_HARDWARE_ID_LO:
                case MDBS_TERM_HREG_FIRMWARE_ID_HI:
                case MDBS_TERM_HREG_FIRMWARE_ID_MI:
                case MDBS_TERM_HREG_FIRMWARE_ID_LO:
                case MDBS_TERM_HREG_RESERVED_07:
                        //do nothing, read-only register
                        break;

                case MDBS_TERM_HREG_SERIAL_NUM_00:
                case MDBS_TERM_HREG_SERIAL_NUM_01:
                case MDBS_TERM_HREG_SERIAL_NUM_02:
                case MDBS_TERM_HREG_SERIAL_NUM_03:
                case MDBS_TERM_HREG_SERIAL_NUM_04:
                case MDBS_TERM_HREG_SERIAL_NUM_05:
                case MDBS_TERM_HREG_SERIAL_NUM_06:
                case MDBS_TERM_HREG_SERIAL_NUM_07:
                        //do nothing, read-only register
                        break;

                case MDBS_TERM_HREG_ERR_CODE_LAST:
                case MDBS_TERM_HREG_STARTS_COUNTER:
                case MDBS_TERM_HREG_RESERVED_12:
                case MDBS_TERM_HREG_RESERVED_13:
                case MDBS_TERM_HREG_RESERVED_14:
                case MDBS_TERM_HREG_RESERVED_15:
                case MDBS_TERM_HREG_RESERVED_16:
                case MDBS_TERM_HREG_RESERVED_17:
                case MDBS_TERM_HREG_MCU_TEMP_DIGC:
                case MDBS_TERM_HREG_MCU_VDDA_mV:
                case MDBS_TERM_HREG_RAW2PPM_FV_HI:
                case MDBS_TERM_HREG_RAW2PPM_FV_LO:
                case MDBS_TERM_HREG_RAW2PPM_FT_HI:
                case MDBS_TERM_HREG_RAW2PPM_FT_LO:
                case MDBS_TERM_HREG_RAW2PPM_FP_HI:
                case MDBS_TERM_HREG_RAW2PPM_FP_LO:
                        //do nothing, read-only registers
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_PPM_HI:
                case MDBS_TERM_HREG_SENS_OXGN_PPM_LO:
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_HI:
                case MDBS_TERM_HREG_SENS_TEMP_DIGC_LO:
                case MDBS_TERM_HREG_SENS_PRES_HPA_HI:
                case MDBS_TERM_HREG_SENS_PRES_HPA_LO:
                case MDBS_TERM_HREG_RESERVED_26:
                case MDBS_TERM_HREG_RESERVED_27:
                case MDBS_TERM_HREG_SENS_OXGN_RAW_HI:
                case MDBS_TERM_HREG_SENS_OXGN_RAW_LO:
                case MDBS_TERM_HREG_SENS_TEMP_RAW_HI:
                case MDBS_TERM_HREG_SENS_TEMP_RAW_LO:
                case MDBS_TERM_HREG_SENS_PRES_RAW_HI:
                case MDBS_TERM_HREG_SENS_PRES_RAW_LO:
                case MDBS_TERM_HREG_SENS_SLOPE_RAW:
                case MDBS_TERM_HREG_SENS_OFST_RAW:
                        //do nothing, read-only registers
                        break;


                case MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_HI:
                        dev.sens->cal.zero.timestamp.u16[1]  = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_LO:
                        dev.sens->cal.zero.timestamp.u16[ 0]  = *data;
                        task_hmi_cal_update( 0 );
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_HI:
                        dev.sens->cal.zero.ppm.u16[ 1]    = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_LO:
                        dev.sens->cal.zero.ppm.u16[ 0]    = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_HI:
                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_LO:
                case MDBS_TERM_HREG_RESERVED_36:
                case MDBS_TERM_HREG_RESERVED_37:
                        //do nothing, read-only registers
                        break;


                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_HI:
                        dev.sens->cal.span.timestamp.u16[ 1] = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_LO:
                        dev.sens->cal.span.timestamp.u16[ 0] = *data;
                        task_hmi_cal_update( 1 );
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_HI:
                        dev.sens->cal.span.ppm.u16[ 1]  = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_LO:
                        dev.sens->cal.span.ppm.u16[ 0] = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_RAW_HI:
                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_RAW_LO:
                case MDBS_TERM_HREG_RESERVED_3E:
                case MDBS_TERM_HREG_RESERVED_3F:
                        //do nothing, read-only registers
                        break;

                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}
