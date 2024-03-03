/**
  * @file    lcdconf.c
  * @brief   implements the configuration for the GUI library
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "LCDConf.h"
#include "GUI_Private.h"
#include "config.h"
#include "main.h"
#include "stm32f7xx_hal.h"
#include "stm32.h"


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  LCD_LayerPropTypedef            layer_prop[ GUI_NUM_LAYERS ];

static const LCD_API_COLOR_CONV * apColorConvAPI[] =
{
        COLOR_CONVERSION_0,

        #if GUI_NUM_LAYERS > 1
        COLOR_CONVERSION_1,
        #endif
};



/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
/**
  * @brief  Return Pixel format for a given layer
  * @param  LayerIndex : Layer Index
  * @retval Status ( 0 : 0k , 1: error)
  */
static
inline
U32
LCD_LL_GetPixelformat(                          U32             LayerIndex )
{
    return( PIXEL_FORMAT );
}


/**
  * @brief  Return Pixel format for a given layer
  * @param  LayerIndex : Layer Index
  * @retval Status ( 0 : 0k , 1: error)
  */
static
void
DMA2D_CopyBuffer(                       U32             LayerIndex,
                                        void *          pSrc,
                                        void *          pDst,
                                        U32             xSize,
                                        U32             ySize,
                                        U32             OffLineSrc,
                                        U32             OffLineDst )
{
        U32     PixelFormat;


        PixelFormat     = LCD_LL_GetPixelformat( LayerIndex );
        DMA2D->CR       = 0x00000000UL | (1 << 9);

        /* Set up pointers */
        DMA2D->FGMAR    = (U32)pSrc;
        DMA2D->OMAR     = (U32)pDst;
        DMA2D->FGOR     = OffLineSrc;
        DMA2D->OOR      = OffLineDst;

        /* Set up pixel format */
        DMA2D->FGPFCCR = PixelFormat;

        /*  Set up size */
        DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;
        DMA2D->CR     |= DMA2D_CR_START;

        /* Wait until transfer is done */
        while( DMA2D->CR & DMA2D_CR_START )
        {
        }
}


/**
  * @brief  Fill Buffer
  * @param  LayerIndex : Layer Index
  * @param  pDst:        pointer to destination
  * @param  xSize:       X size
  * @param  ySize:       Y size
  * @param  OffLine:     offset after each line
  * @param  ColorIndex:  color to be used.
  * @retval None.
  */
static
void
DMA2D_FillBuffer(                               U32             LayerIndex,
                                                void *          pDst,
                                                U32             xSize,
                                                U32             ySize,
                                                U32             OffLine,
                                                U32             ColorIndex )
{
        U32     PixelFormat;


        PixelFormat     = LCD_LL_GetPixelformat( LayerIndex );

        /* Set up mode */
        DMA2D->CR       = 0x00030000UL | (1 << 9);
        DMA2D->OCOLR    = ColorIndex;

        /* Set up pointers */
        DMA2D->OMAR     = (U32) pDst;

        /* Set up offsets */
        DMA2D->OOR      = OffLine;

        /* Set up pixel format */
        DMA2D->OPFCCR   = PixelFormat;

        /*  Set up size */
        DMA2D->NLR      = (U32) (xSize << 16) | (U16) ySize;

        DMA2D->CR       |= DMA2D_CR_START;

        /* Wait until transfer is done */
        while( DMA2D->CR & DMA2D_CR_START );
}


/**
  * @brief  Get buffer size
  * @param  LayerIndex : Layer Index
  * @retval None.
  */
static
U32
GetBufferSize(                                  U32             LayerIndex )
{
        U32     BufferSize;

        BufferSize      = layer_prop[LayerIndex].xSize * layer_prop[LayerIndex].ySize * layer_prop[LayerIndex].BytesPerPixel;

        return( BufferSize );
}


/**
  * @brief  Customized copy buffer
  * @param  LayerIndex : Layer Index
  * @param  IndexSrc:    index source
  * @param  IndexDst:    index destination
  * @retval None.
  */
static
void
CUSTOM_CopyBuffer(                              int             LayerIndex,
                                                int             IndexSrc,
                                                int             IndexDst )
{
        U32     BufferSize, AddrSrc, AddrDst;


        BufferSize      = GetBufferSize(LayerIndex);
        AddrSrc         = layer_prop[LayerIndex].address + BufferSize * IndexSrc;
        AddrDst         = layer_prop[LayerIndex].address + BufferSize * IndexDst;
        DMA2D_CopyBuffer(       LayerIndex,
                       (void *) AddrSrc,
                       (void *) AddrDst,
                                layer_prop[LayerIndex].xSize,
                                layer_prop[LayerIndex].ySize,
                                0,
                                0 );

        layer_prop[LayerIndex].buffer_index     = IndexDst;
}


/**
  * @brief  Copy rectangle
  * @param  idx:        Layer Index
  * @param  x0:         X0 position
  * @param  y0:         Y0 position
  * @param  x1:         X1 position
  * @param  y1:         Y1 position
  * @param  xSize:      X size
  * @param  ySize:      Y size
  * @retval None
  */
static
void
CUSTOM_CopyRect(                                int             idx,
                                                int             x0,
                                                int             y0,
                                                int             x1,
                                                int             y1,
                                                int             xSize,
                                                int             ySize )
{
        U32     src, dst;

        src     = layer_prop[idx].address + (y0 * layer_prop[idx].xSize + x0) * layer_prop[idx].BytesPerPixel;
        dst     = layer_prop[idx].address + (y1 * layer_prop[idx].xSize + x1) * layer_prop[idx].BytesPerPixel;
        DMA2D_CopyBuffer(idx, (void *)src, (void *)dst, xSize, ySize, layer_prop[idx].xSize - xSize, layer_prop[idx].xSize - xSize);
}


/**
  * @brief  Fill rectangle
  * @param  LayerIndex : Layer Index
  * @param  x0:          X0 position
  * @param  y0:          Y0 position
  * @param  x1:          X1 position
  * @param  y1:          Y1 position
  * @param  PixelIndex:  Pixel index.
  * @retval None.
  */
static
void
CUSTOM_FillRect(                                int             LayerIndex,
                                                int             x0,
                                                int             y0,
                                                int             x1,
                                                int             y1,
                                                U32             PixelIndex )
{
        U32     BufferSize, AddrDst;
        int     xSize, ySize;


        if( GUI_GetDrawMode() == GUI_DM_XOR )
        {
                LCD_SetDevFunc( LayerIndex, LCD_DEVFUNC_FILLRECT, NULL );
                LCD_FillRect( x0, y0, x1, y1 );
                LCD_SetDevFunc( LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_FillRect );
        }
        else
        {
                xSize           = x1 - x0 + 1;
                ySize           = y1 - y0 + 1;
                BufferSize      = GetBufferSize(LayerIndex);
		AddrDst         = layer_prop[LayerIndex].address +
                                        BufferSize * layer_prop[LayerIndex].buffer_index +
                                        (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;

                DMA2D_FillBuffer(       LayerIndex,
                               (void *) AddrDst,
                                        xSize,
                                        ySize,
                                        layer_prop[LayerIndex].xSize - xSize, PixelIndex);
	}
}


/**
  * @brief  Draw indirect color bitmap
  * @param  pSrc: pointer to the source
  * @param  pDst: pointer to the destination
  * @param  OffSrc: offset source
  * @param  OffDst: offset destination
  * @param  PixelFormatDst: pixel format for destination
  * @param  xSize: X size
  * @param  ySize: Y size
  * @retval None
  */
static
void
CUSTOM_DrawBitmap32bpp(                         int             LayerIndex,
                                                int             x,
                                                int             y,
                                                U8 const *      p,
                                                int             xSize,
                                                int             ySize,
                                                int             BytesPerLine )
{
        U32     BufferSize, AddrDst;
        int     OffLineSrc, OffLineDst;


        BufferSize      = GetBufferSize( LayerIndex );
        AddrDst         = layer_prop[LayerIndex].address + BufferSize * layer_prop[LayerIndex].buffer_index + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
        OffLineSrc      = (BytesPerLine / 4) - xSize;
        OffLineDst      = layer_prop[LayerIndex].xSize - xSize;
        DMA2D_CopyBuffer( LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}


/*******************************************************************************
                       LTDC and DMA2D BSP Routines
*******************************************************************************/
/**
  * @brief LTDC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
/**
  * @brief  Initialize the LCD Controller.
  * @param  LayerIndex : layer Index.
  * @retval None
  */
static
void
LCD_LL_LayerInit(                       U32     LayerIndex )
{
    stm32_ltdc_config_layer( LayerIndex );
}


/**
  * @brief  Line Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the specified LTDC.
  * @retval None
  */
void
HAL_LTDC_LineEventCallback(             LTDC_HandleTypeDef *    hltdc )
{
        U32 Addr;
        U32 layer;

        for (layer = 0; layer < GUI_NUM_LAYERS; layer++)
        {
                if (layer_prop[layer].pending_buffer >= 0)
                {
                        /* Calculate address of buffer to be used  as visible frame buffer */
                        Addr    = layer_prop[layer].address + \
                                        layer_prop[layer].xSize * layer_prop[layer].ySize * layer_prop[layer].pending_buffer * layer_prop[layer].BytesPerPixel;

                        __HAL_LTDC_LAYER( hltdc, layer )->CFBAR = Addr;
                        __HAL_LTDC_RELOAD_CONFIG( hltdc );

                        /* Notify STemWin that buffer is used */
                        GUI_MULTIBUF_ConfirmEx( layer, layer_prop[layer].pending_buffer );

                        /* Clear pending buffer flag of layer */
                        layer_prop[ layer ].pending_buffer      = -1;
                }
        }

        HAL_LTDC_ProgramLineEvent( hltdc, 0 );
}


/*******************************************************************************
                          Display configuration
*******************************************************************************/
/**
  * @brief  Called during the initialization process in order to set up the
  *          display driver configuration
  * @param  None
  * @retval None
  */
void
LCD_X_Config( void )
{
        U32     i;


        stm32_ltdc_init();

        /* At first initialize use of multiple buffers on demand */
        #if ( NUM_BUFFERS > 1 )
        for( i = 0; i < GUI_NUM_LAYERS; i++ )
        {
                GUI_MULTIBUF_ConfigEx( i, NUM_BUFFERS );
        }
        #endif

        /* Set display driver and color conversion for 1st layer */
        GUI_DEVICE_CreateAndLink( DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0 );

        /* Set size of 1st layer */
        if( LCD_GetSwapXYEx(0) )
        {
                LCD_SetSizeEx(  0, YSIZE_PHYS, XSIZE_PHYS );
                LCD_SetVSizeEx( 0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS );
        }
        else
        {
                LCD_SetSizeEx(  0, XSIZE_PHYS, YSIZE_PHYS );
                LCD_SetVSizeEx( 0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS );
        }

        #if (GUI_NUM_LAYERS > 1)
        /* Set display driver and color conversion for 2nd layer */
        GUI_DEVICE_CreateAndLink( DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1 );

        /* Set size of 2nd layer */
        if( LCD_GetSwapXYEx(1) )
        {
                LCD_SetSizeEx(  1, YSIZE_PHYS, XSIZE_PHYS );
                LCD_SetVSizeEx( 1, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS );
        }
        else
        {
                LCD_SetSizeEx(  1, XSIZE_PHYS, YSIZE_PHYS );
                LCD_SetVSizeEx( 1, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS );
        }
        #endif

        /*Initialize GUI Layer structure */
        layer_prop[0].address   = LCD_LAYER0_FRAME_BUFFER;

        #if (GUI_NUM_LAYERS > 1)
        layer_prop[1].address = LCD_LAYER1_FRAME_BUFFER;
        #endif

        /* Setting up VRam address and custom functions for CopyBuffer-, CopyRect- and FillRect operations */
        for( i = 0; i < GUI_NUM_LAYERS; i++ )
        {
                layer_prop[i].pColorConvAPI     = (LCD_API_COLOR_CONV *) apColorConvAPI[ i ];
                layer_prop[i].pending_buffer    = -1;

                /* Set VRAM address */
                LCD_SetVRAMAddrEx( i, (void *)(layer_prop[i].address) );

                /* Remember color depth for further operations */
                layer_prop[i].BytesPerPixel     = LCD_GetBitsPerPixelEx(i) >> 3;

                /* Set custom functions for several operations */
                LCD_SetDevFunc( i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))CUSTOM_CopyBuffer);
                LCD_SetDevFunc( i, LCD_DEVFUNC_COPYRECT,   (void(*)(void))CUSTOM_CopyRect);
                LCD_SetDevFunc( i, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_FillRect);

                /* Set up drawing routine for 32bpp bitmap using DMA2D */
                if( LCD_LL_GetPixelformat(i) == LTDC_PIXEL_FORMAT_ARGB8888 )
                {
                        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))CUSTOM_DrawBitmap32bpp);     /* Set up drawing routine for 32bpp bitmap using DMA2D. Makes only sense with ARGB8888 */
                }
        }
}


/**
  * @brief  This function is called by the display driver for several purposes.
  *         To support the according task the routine needs to be adapted to
  *         the display controller. Please note that the commands marked with
  *         'optional' are not cogently required and should only be adapted if
  *         the display controller supports these features
  * @param  LayerIndex: Index of layer to be configured
  * @param  Cmd       :Please refer to the details in the switch statement below
  * @param  pData     :Pointer to a LCD_X_DATA structure
  * @retval Status (-1 : Error,  0 : Ok)
  */
int
LCD_X_DisplayDriver(                            unsigned        LayerIndex,
                                                unsigned        Cmd,
                                                void *          pData )
{
        int     r       = 0;
        U32     addr;
        int     xPos, yPos;
        U32     Color;


        switch( Cmd )
        {
                case LCD_X_INITCONTROLLER:
                        LCD_LL_LayerInit( LayerIndex );
                        break;

                case LCD_X_SETORG:
                        addr = layer_prop[LayerIndex].address + ((LCD_X_SETORG_INFO *)pData)->yPos * layer_prop[LayerIndex].xSize * layer_prop[LayerIndex].BytesPerPixel;
                        stm32_ltdc_set_address( addr, LayerIndex );
                        break;

                case LCD_X_SHOWBUFFER:
                        layer_prop[LayerIndex].pending_buffer = ((LCD_X_SHOWBUFFER_INFO *)pData)->Index;
                        break;

                case LCD_X_SETLUTENTRY:
                        stm32_ltdc_config_lut( (uint32_t *) &(((LCD_X_SETLUTENTRY_INFO *)pData)->Color), 1, LayerIndex );
                        break;

                case LCD_X_ON:
                        stm32_ltdc_enable();
                        break;

                case LCD_X_OFF:
                        stm32_ltdc_disable();
                        break;

                case LCD_X_SETVIS:
                        if(((LCD_X_SETVIS_INFO *)pData)->OnOff  == ENABLE )
                        {
                            stm32_ltdc_layer_enable( LayerIndex );
                        }
                        else
                        {
                            stm32_ltdc_layer_disable( LayerIndex );
                        }

                        stm32_ltdc_reload_config();
                        break;

                case LCD_X_SETPOS:
                        stm32_ltdc_set_window_pos(  ((LCD_X_SETPOS_INFO *)pData)->xPos,
                                                    ((LCD_X_SETPOS_INFO *)pData)->yPos,
                                                    LayerIndex );
                        break;

                case LCD_X_SETSIZE:
                        GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
                        layer_prop[LayerIndex].xSize = ((LCD_X_SETSIZE_INFO *)pData)->xSize;
                        layer_prop[LayerIndex].ySize = ((LCD_X_SETSIZE_INFO *)pData)->ySize;
                        stm32_ltdc_set_window_pos( xPos, yPos, LayerIndex);
                        break;

                case LCD_X_SETALPHA:
                        stm32_ltdc_set_alpha( ((LCD_X_SETALPHA_INFO *)pData)->Alpha, LayerIndex );
                        break;

                case LCD_X_SETCHROMAMODE:
                        if(((LCD_X_SETCHROMAMODE_INFO *)pData)->ChromaMode != 0)
                        {
                            stm32_ltdc_enable_color_keying( LayerIndex );
                        }
                        else
                        {
                            stm32_ltdc_disable_color_keying( LayerIndex );
                        }
                        break;

                case LCD_X_SETCHROMA:
                        Color   = ((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0xFF0000) >> 16) |\
                                  (((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x00FF00) |\
                                  ((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x0000FF) << 16);
                        stm32_ltdc_config_color_keying( Color, LayerIndex );
                        break;

                default:
                        r = -1;
        }

        return r;
}
