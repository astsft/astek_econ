/**
  * @file       stm32_ltdc.h
  * @brief      STM32 LTDC controller routines
  * @author     Igor T. <research.tahoe@gmail.com>
  */


#include "stm32.h"
#include "LCDConf.h"
#include "config.h"


/*******************************************************************************
* PRIVATE CONSTATNTS
*******************************************************************************/
#define LCD_DITHB_Pin           GPIO_PIN_12
#define LCD_DITHB_GPIO_Port     GPIOC

#define LCD_U_D_Pin             GPIO_PIN_2
#define LCD_U_D_GPIO_Port       GPIOD

#define LCD_L_R_Pin             GPIO_PIN_4
#define LCD_L_R_GPIO_Port       GPIOD

#define LCD_MODE_Pin            GPIO_PIN_4
#define LCD_MODE_GPIO_Port      GPIOE

#define FMC_NBL0_Pin            GPIO_PIN_0
#define FMC_NBL0_GPIO_Port      GPIOE
#define FMC_NBL1_Pin            GPIO_PIN_1
#define FMC_NBL1_GPIO_Port      GPIOE


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  DMA2D_HandleTypeDef             hdma2d;

/*******************************************************************************
*
*******************************************************************************/
        LTDC_HandleTypeDef              hltdc   = {0};


/*******************************************************************************
* LOCAL FUNCTIONS
*******************************************************************************/
static
void
init_io( void )
{
        GPIO_InitTypeDef        gpio    = {0};


        gpio.Pin        = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF14_LTDC;
        HAL_GPIO_Init( GPIOA, &gpio );

        gpio.Pin        = GPIO_PIN_0 | GPIO_PIN_1;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF9_LTDC;
        HAL_GPIO_Init( GPIOB, &gpio );

        gpio.Pin        = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF14_LTDC;
        HAL_GPIO_Init( GPIOB, &gpio );

        gpio.Pin        = GPIO_PIN_6 | GPIO_PIN_7;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF14_LTDC;
        HAL_GPIO_Init( GPIOC, &gpio );

        gpio.Pin        = GPIO_PIN_3;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF14_LTDC;
        HAL_GPIO_Init( GPIOD, &gpio );

        gpio.Pin        = GPIO_PIN_10;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF14_LTDC;
        HAL_GPIO_Init( GPIOF, &gpio );

        gpio.Pin        = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF14_LTDC;
        HAL_GPIO_Init( GPIOG, &gpio );

        gpio.Pin        = GPIO_PIN_10 | GPIO_PIN_12;
        gpio.Mode       = GPIO_MODE_AF_PP;
        gpio.Pull       = GPIO_NOPULL;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        gpio.Alternate  = GPIO_AF9_LTDC;
        HAL_GPIO_Init( GPIOG, &gpio );


        //LCD PANEL GPIO
        gpio.Pin        = LCD_DITHB_Pin;
        gpio.Mode       = GPIO_MODE_OUTPUT_PP;
        gpio.Pull       = GPIO_PULLDOWN;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init( GPIOC, &gpio );

        gpio.Pin        = LCD_U_D_Pin | LCD_L_R_Pin;
        gpio.Mode       = GPIO_MODE_OUTPUT_PP;
        gpio.Pull       = GPIO_PULLDOWN;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init( GPIOD, &gpio );

        /*Configure GPIO pin : LCD_MODE_Pin */
        gpio.Pin        = LCD_MODE_Pin;
        gpio.Mode       = GPIO_MODE_OUTPUT_PP;
        gpio.Pull       = GPIO_PULLUP;
        gpio.Speed      = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init( GPIOE, &gpio );

        //HAL_GPIO_WritePin( GPIOE, LCD_DITHB_Pin | LCD_U_D_Pin | LCD_L_R_Pin, GPIO_PIN_RESET );
	//HAL_GPIO_WritePin( GPIOE, LCD_L_R_Pin, GPIO_PIN_SET );

        HAL_GPIO_WritePin( GPIOE, LCD_MODE_Pin, GPIO_PIN_SET );
        //HAL_GPIO_WritePin( GPIOE, LCD_MODE_Pin, GPIO_PIN_RESET );

    HAL_GPIO_WritePin( GPIOD, LCD_U_D_Pin, GPIO_PIN_SET );
    HAL_GPIO_WritePin( GPIOD, LCD_L_R_Pin, GPIO_PIN_RESET );
    //HAL_GPIO_WritePin( GPIOD, LCD_U_D_Pin, GPIO_PIN_RESET );
    //HAL_GPIO_WritePin( GPIOD, LCD_L_R_Pin, GPIO_PIN_SET );

        #if 0
        HAL_GPIO_WritePin( GPIOI, GPIO_PIN_12, GPIO_PIN_SET );  //Assert display enable LCD_DISP pin
        HAL_GPIO_WritePin( GPIOK, GPIO_PIN_3, GPIO_PIN_SET );   //Assert backlight LCD_BL_CTRL pin
	#endif
}


static
int
init_dma2d( void )
{
    hdma2d.Instance                 = DMA2D;
    hdma2d.Init.Mode                = DMA2D_R2M;
    hdma2d.Init.ColorMode           = DMA_COLOR_MODE;
    hdma2d.Init.OutputOffset        = 0x0;

    if( HAL_DMA2D_Init( &hdma2d ) != HAL_OK )
    {
        return( -1 );
    }

    return( 0 );
}


static
void
init_ltdc( void )
{
    HAL_LTDC_DeInit( &hltdc );


    /* Set LCD Timings */
	hltdc.Instance                  = LTDC;
    hltdc.Init.HSPolarity           = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity           = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity           = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity           = LTDC_PCPOLARITY_IPC;

   hltdc.Init.AccumulatedActiveW   = LCD_SPEC_HSYNC_ACTIVE_WIDTH;
   hltdc.Init.HorizontalSync       = LCD_SPEC_HSYNC_PULSE_WIDTH;
   hltdc.Init.AccumulatedHBP       = LCD_SPEC_HSYNC_BACK_PORCH;
   hltdc.Init.TotalWidth           = LCD_SPEC_HSYNC_PERIOD_TOTAL;

   hltdc.Init.AccumulatedActiveH   = LCD_SPEC_VSYNC_ACTIVE_WIDTH;
   hltdc.Init.VerticalSync         = LCD_SPEC_VSYNC_PULSE_WIDTH;
   hltdc.Init.AccumulatedVBP       = LCD_SPEC_VSYNC_BACK_PORCH;
   hltdc.Init.TotalHeigh           = LCD_SPEC_VSYNC_PERIOD_TOTAL;

   hltdc.Init.Backcolor.Blue       = 0;
   hltdc.Init.Backcolor.Green      = 0;
   hltdc.Init.Backcolor.Red        = 0;

   HAL_LTDC_Init( &hltdc );
   HAL_LTDC_ProgramLineEvent( &hltdc, 0 );

   HAL_LTDC_EnableDither( &hltdc );
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief  Initialize the LCD Controller.
  * @param  LayerIndex : layer Index.
  * @retval None
  */
void
stm32_ltdc_init( void )
{
    init_ltdc();
    init_dma2d();
    init_io();

    NVIC_SetPriority(       LTDC_IRQn,      STM32_NVIC_PRIO_LTDC        );
    NVIC_EnableIRQ(         LTDC_IRQn                                   );
    NVIC_SetPriority(       LTDC_ER_IRQn,   STM32_NVIC_PRIO_LTDC_ER     );
    NVIC_EnableIRQ(         LTDC_ER_IRQn                                );
}


/**
  * @brief LTDC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void
HAL_LTDC_MspDeInit(                     LTDC_HandleTypeDef *    hltdc )
{
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();
}

/**
  * @brief DMA2D MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void
HAL_DMA2D_MspDeInit(                    DMA2D_HandleTypeDef *   hdma2d )
{
    __HAL_RCC_DMA2D_FORCE_RESET();
    __HAL_RCC_DMA2D_RELEASE_RESET();
}

/**
  * @brief DMA2D MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void
HAL_DMA2D_MspInit(                      DMA2D_HandleTypeDef *   hdma2d )
{
    __HAL_RCC_DMA2D_CLK_ENABLE();
}




/**
  * @brief  Initialize the LCD Controller.
  * @param  LayerIndex : layer Index.
  * @retval None
  */
void
stm32_ltdc_set_address(         uint32_t        addr,
                                uint32_t        LayerIndex )
{
    HAL_LTDC_SetAddress( &hltdc, addr, LayerIndex );
}


void
stm32_ltdc_config_layer(        uint32_t        LayerIndex )
{
    LTDC_LayerCfgTypeDef    layer_cfg;


    if( LayerIndex < GUI_NUM_LAYERS )
    {
        /* Layer configuration */
        layer_cfg.WindowX0      = 0;
        layer_cfg.WindowX1      = XSIZE_PHYS;
        layer_cfg.WindowY0      = 0;
        layer_cfg.WindowY1      = YSIZE_PHYS;
        //layer_cfg.PixelFormat   = LCD_LL_GetPixelformat(LayerIndex);
        layer_cfg.PixelFormat   = PIXEL_FORMAT;

		if( LayerIndex == 0 )
        {
            layer_cfg.FBStartAdress = (uint32_t) LCD_LAYER0_FRAME_BUFFER; // ((uint32_t)0xC0000000);
        }
        else
        {
            layer_cfg.FBStartAdress = (uint32_t) (LCD_LAYER0_FRAME_BUFFER + XSIZE_PHYS*YSIZE_PHYS*2);
        }

        layer_cfg.Alpha                 = 255;
        layer_cfg.Alpha0                = 0;
        layer_cfg.Backcolor.Blue        = 0;
        layer_cfg.Backcolor.Green       = 0;
        layer_cfg.Backcolor.Red         = 0;
        layer_cfg.BlendingFactor1       = LTDC_BLENDING_FACTOR1_PAxCA;
        layer_cfg.BlendingFactor2       = LTDC_BLENDING_FACTOR2_PAxCA;
        layer_cfg.ImageWidth            = XSIZE_PHYS;
        layer_cfg.ImageHeight           = YSIZE_PHYS;
        HAL_LTDC_ConfigLayer( &hltdc, &layer_cfg, LayerIndex );

        /* Enable LUT on demand */
        if( LCD_GetBitsPerPixelEx( LayerIndex ) <= 8 )
        {
            /* Enable usage of LUT for all modes with <= 8bpp*/
            HAL_LTDC_EnableCLUT( &hltdc, LayerIndex );
        }
    }
}


void
stm32_ltdc_config_lut(          uint32_t *      clut,
                                uint32_t        clut_size,
                                uint32_t        LayerIdx )
{
    HAL_LTDC_ConfigCLUT( &hltdc, clut, clut_size, LayerIdx );
}


void
stm32_ltdc_enable( void )
{
    __HAL_LTDC_ENABLE( &hltdc );
}

void
stm32_ltdc_disable( void )
{
    __HAL_LTDC_DISABLE( &hltdc );
}

void
stm32_ltdc_layer_enable(        uint32_t        LayerIndex )
{
    __HAL_LTDC_LAYER_ENABLE( &hltdc, LayerIndex );
}

void
stm32_ltdc_layer_disable(       uint32_t        LayerIndex )
{
    __HAL_LTDC_LAYER_DISABLE( &hltdc, LayerIndex );
}

void
stm32_ltdc_reload_config( void )
{
    __HAL_LTDC_RELOAD_CONFIG(&hltdc);
}

void
stm32_ltdc_set_window_pos(      int             xPos,
                                int             yPos,
                                uint32_t        LayerIndex )
{
    HAL_LTDC_SetWindowPosition( &hltdc,
                                xPos,
                                yPos,
                                LayerIndex );
}

void
stm32_ltdc_set_alpha(           int             alpha,
                                uint32_t        LayerIndex )
{
    HAL_LTDC_SetAlpha( &hltdc, alpha, LayerIndex );
}

void
stm32_ltdc_enable_color_keying( uint32_t        LayerIndex )
{
    HAL_LTDC_EnableColorKeying( &hltdc, LayerIndex );
}

void
stm32_ltdc_disable_color_keying( uint32_t       LayerIndex )
{
    HAL_LTDC_DisableColorKeying( &hltdc, LayerIndex );
}

void
stm32_ltdc_config_color_keying( uint32_t        Color,
                                uint32_t        LayerIndex )
{
    HAL_LTDC_ConfigColorKeying( &hltdc, Color, LayerIndex );
}


void
stm32_ltdc_swap_x(              int             swap_x )
{
    if( swap_x )
    {
        HAL_GPIO_WritePin( GPIOD, LCD_U_D_Pin, GPIO_PIN_SET );
        //HAL_GPIO_WritePin( GPIOD, LCD_L_R_Pin, GPIO_PIN_RESET );
    }
    else
    {
        HAL_GPIO_WritePin( GPIOD, LCD_U_D_Pin, GPIO_PIN_RESET );
        //HAL_GPIO_WritePin( GPIOD, LCD_L_R_Pin, GPIO_PIN_SET );
    }
}

void    stm32_ltdc_swap_y(      int             swap_y )
{
    if( swap_y )
    {
        //HAL_GPIO_WritePin( GPIOD, LCD_U_D_Pin, GPIO_PIN_SET );
        HAL_GPIO_WritePin( GPIOD, LCD_L_R_Pin, GPIO_PIN_RESET );
    }
    else
    {
        //HAL_GPIO_WritePin( GPIOD, LCD_U_D_Pin, GPIO_PIN_RESET );
        HAL_GPIO_WritePin( GPIOD, LCD_L_R_Pin, GPIO_PIN_SET );
    }
}
