/**
  * @file    mdbs_dev.c
  * @brief   MODBUS Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "mdbs.h"
#include "mdbs_dev_term.h"
#include "main.h"
#include "dev.h"


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
                        //*data   = dev.cfg->starts_count;
                        *data   = dev.log.starts_counter;
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
                        *data   = dev.ibus->sens.mcu_temp_cels;
                        break;

                case MDBS_TERM_HREG_MCU_VDDA_mV:
                        *data   = dev.ibus->sens.mcu_vdda_mv;
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FV_HI:
                        *data   = dev.ibus->sens.raw2ppm_fv.u16[ 1];
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FV_LO:
                        *data   = dev.ibus->sens.raw2ppm_fv.u16[ 0];
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FT_HI:
                        *data   = dev.ibus->sens.raw2ppm_ft.u16[ 1];
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FT_LO:
                        *data   = dev.ibus->sens.raw2ppm_ft.u16[ 0];
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FP_HI:
                        *data   = dev.ibus->sens.raw2ppm_fp.u16[ 1];
                        break;

                case MDBS_TERM_HREG_RAW2PPM_FP_LO:
                        *data   = dev.ibus->sens.raw2ppm_fp.u16[ 0];
                        break;


                case MDBS_TERM_HREG_SENS_OXGN_PPM_HI:
                        *data   = dev.ibus->sens.oxgn_ppm.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_PPM_LO:
                        *data   = dev.ibus->sens.oxgn_ppm.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_DIGC_HI:
                        *data   = dev.ibus->sens.temp_digc.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_DIGC_LO:
                        *data   = dev.ibus->sens.temp_digc.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_PRES_HPA_HI:
                        *data   = dev.ibus->sens.pres_hpa.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_PRES_HPA_LO:
                        *data   = dev.ibus->sens.pres_hpa.u16[ 0];
                        break;

                case MDBS_TERM_HREG_RESERVED_26:
                case MDBS_TERM_HREG_RESERVED_27:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_RAW_HI:
                        *data   = dev.ibus->sens.oxgn_raw.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_OXGN_RAW_LO:
                        *data   = dev.ibus->sens.oxgn_raw.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_RAW_HI:
                        *data   = dev.ibus->sens.temp_raw.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TEMP_RAW_LO:
                        *data   = dev.ibus->sens.temp_raw.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_PRES_RAW_HI:
                        *data   = dev.ibus->sens.pres_raw.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_PRES_RAW_LO:
                        *data   = dev.ibus->sens.pres_raw.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_SLOPE_RAW:
                        *data   = dev.ibus->sens.slope;
                        break;

                case MDBS_TERM_HREG_SENS_OFST_RAW:
                        //*data   = dev.ibus->sens.value_4;
                        *data   = 0;
                        break;


                case MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_HI:
                        *data   = dev.ibus->sens.trim[ 0].timestamp.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_LO:
                        *data   = dev.ibus->sens.trim[ 0].timestamp.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_HI:
                        *data   = dev.ibus->sens.trim[ 0].ppm.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_LO:
                        *data   = dev.ibus->sens.trim[ 0].ppm.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_HI:
                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_LO:
                case MDBS_TERM_HREG_RESERVED_36:
                case MDBS_TERM_HREG_RESERVED_37:
                        *data   = 0;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_HI:
                        *data   = dev.ibus->sens.trim[ 1].timestamp.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_LO:
                        *data   = dev.ibus->sens.trim[ 1].timestamp.u16[ 0];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_HI:
                        *data   = dev.ibus->sens.trim[ 1].ppm.u16[ 1];
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_LO:
                        *data   = dev.ibus->sens.trim[ 1].ppm.u16[ 0];
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
                        dev.ibus->sens.trim[ 0].timestamp.u16[1] = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_TIMESTMP_LO:
                        dev.ibus->sens.trim[ 0].timestamp.u16[0] = *data;
                        task_ibus_trim_update( 0 );
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_HI:
                        dev.ibus->sens.trim[ 0].ppm.u16[ 1]      = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_PPM_LO:
                        dev.ibus->sens.trim[ 0].ppm.u16[ 0]      = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_HI:
                case MDBS_TERM_HREG_SENS_TRIM_P0_OXGN_RAW_LO:
                case MDBS_TERM_HREG_RESERVED_36:
                case MDBS_TERM_HREG_RESERVED_37:
                        //do nothing, read-only registers
                        break;


                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_HI:
                        dev.ibus->sens.trim[ 1].timestamp.u16[1] = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_TIMESTMP_LO:
                        dev.ibus->sens.trim[ 1].timestamp.u16[0] = *data;
                        task_ibus_trim_update( 1 );
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_HI:
                        dev.ibus->sens.trim[ 1].ppm.u16[ 1]      = *data;
                        break;

                case MDBS_TERM_HREG_SENS_TRIM_P1_OXGN_PPM_LO:
                        dev.ibus->sens.trim[ 1].ppm.u16[ 0]      = *data;
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
