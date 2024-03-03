/**
  * @file    mt48lc4m32b2.h
  * @brief   MICRON MT48LC16M16A2 - 256Mb SDRAM - 4 Meg x 16 x 4 banks
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "mt48lc4m32b2.h"


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
int
mt48lc4m32b2_init(                      SDRAM_HandleTypeDef *   hsdram )
{
        volatile        uint32_t                        tmpmrd  = 0;
                        FMC_SDRAM_CommandTypeDef        cmd;


        cmd.CommandMode                 = FMC_SDRAM_CMD_CLK_ENABLE;
        cmd.CommandTarget               = FMC_SDRAM_CMD_TARGET_BANK1;
        cmd.AutoRefreshNumber           = 1;
        cmd.ModeRegisterDefinition      = 0;
        HAL_SDRAM_SendCommand( hsdram, &cmd, SDRAM_TIMEOUT );

        HAL_Delay( 1 ); 

	cmd.CommandMode                 = FMC_SDRAM_CMD_PALL;
        cmd.CommandTarget               = FMC_SDRAM_CMD_TARGET_BANK1;
        cmd.AutoRefreshNumber           = 1;
        cmd.ModeRegisterDefinition      = 0;
        HAL_SDRAM_SendCommand( hsdram, &cmd, SDRAM_TIMEOUT );

	cmd.CommandMode                 = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
        cmd.CommandTarget               = FMC_SDRAM_CMD_TARGET_BANK1;
        cmd.AutoRefreshNumber           = 8;
        cmd.ModeRegisterDefinition      = 0;
        HAL_SDRAM_SendCommand( hsdram, &cmd, SDRAM_TIMEOUT );

	tmpmrd                          = (uint32_t)  ( SDRAM_MODEREG_BURST_LENGTH_1          |
                                                        SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                                                        SDRAM_MODEREG_CAS_LATENCY_2           |
                                                        SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                                                        SDRAM_MODEREG_WRITEBURST_MODE_SINGLE );

        cmd.CommandMode                 = FMC_SDRAM_CMD_LOAD_MODE;
        cmd.CommandTarget               = FMC_SDRAM_CMD_TARGET_BANK1;
        cmd.AutoRefreshNumber           = 1;
        cmd.ModeRegisterDefinition      = tmpmrd;
        HAL_SDRAM_SendCommand( hsdram, &cmd, SDRAM_TIMEOUT ); 

        /* Step 8: Set the refresh rate counter */
        /* (15.62 us x Freq) – 20 */
        /* Set the device refresh counter */
	//hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1)); 

        HAL_SDRAM_ProgramRefreshRate( hsdram, REFRESH_COUNT );	

        return( 0 );
}
