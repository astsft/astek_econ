/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


#ifndef LCDCONF_H
#define LCDCONF_H


#include <stdint.h>
#include "GUI.h"
#include "GUIDRV_Lin.h"


/*******************************************************************************
*
*******************************************************************************/
#undef  LCD_SWAP_XY
#define LCD_SWAP_XY             1

#undef  LCD_MIRROR_Y
#define LCD_MIRROR_Y            1

#undef  LCD_SWAP_RB
#define LCD_SWAP_RB             1

#define XSIZE_PHYS              LCD_SPEC_XSIZE
#define YSIZE_PHYS              LCD_SPEC_YSIZE

#define NUM_BUFFERS             2
#define NUM_VSCREENS            1

#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS          1

#define BK_COLOR                GUI_BLACK



#define _CM_ARGB8888 1
#define _CM_RGB888   2
#define _CM_RGB565   3
#define _CM_ARGB1555 4
#define _CM_ARGB4444 5
#define _CM_L8       6
#define _CM_AL44     7
#define _CM_AL88     8

//#define COLOR_MODE_0          _CM_ARGB8888
//#define COLOR_MODE_0          _CM_RGB888
#define COLOR_MODE_0            _CM_RGB565
//#define COLOR_MODE_0            _CM_ARGB1555
//#define COLOR_MODE_0            _CM_ARGB4444


#if   (COLOR_MODE_0 == _CM_ARGB8888)
    #define COLOR_CONVERSION_0          GUICC_M8888I
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_32
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_ARGB8888
    #define DMA_COLOR_MODE              DMA2D_ARGB8888
#elif (COLOR_MODE_0 == _CM_RGB888)
    #define COLOR_CONVERSION_0          GUICC_M888
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_24
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_RGB888
    #define DMA_COLOR_MODE              DMA2D_RGB888
#elif (COLOR_MODE_0 == _CM_RGB565)
    #define COLOR_CONVERSION_0          GUICC_M565
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_16
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_RGB565
    #define DMA_COLOR_MODE              DMA2D_RGB565
#elif (COLOR_MODE_0 == _CM_ARGB1555)
    #define COLOR_CONVERSION_0          GUICC_M1555I
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_16
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_ARGB1555
    #define DMA_COLOR_MODE              DMA2D_ARGB1555
#elif (COLOR_MODE_0 == _CM_ARGB4444)
    #define COLOR_CONVERSION_0          GUICC_M4444I
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_16
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_ARGB4444
    #define DMA_COLOR_MODE              DMA2D_ARGB4444
/*
#elif (COLOR_MODE_0 == _CM_L8)
    #define COLOR_CONVERSION_0          GUICC_8666
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_8
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_L8
#elif (COLOR_MODE_0 == _CM_AL44)
    #define COLOR_CONVERSION_0          GUICC_1616I
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_8
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_AL44
#elif (COLOR_MODE_0 == _CM_AL88)
    #define COLOR_CONVERSION_0          GUICC_88666I
    #define DISPLAY_DRIVER_0            GUIDRV_LIN_16
    #define PIXEL_FORMAT                LTDC_PIXEL_FORMAT_AL88
*/
#else
    #error Illegal color mode 0!
#endif


#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS          1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif


/* From SDRAM */
//#define LCD_LAYER0_FRAME_BUFFER         ((int)0xC0000000)
//#define LCD_LAYER0_FRAME_BUFFER         ((uint32_t) 0xC0200000)
//#define LCD_LAYER1_FRAME_BUFFER         (LCD_LAYER0_FRAME_BUFFER + XSIZE_PHYS * YSIZE_PHYS * sizeof(uint32_t) * NUM_VSCREENS * NUM_BUFFERS)
//#define LCD_LAYER1_FRAME_BUFFER         ((int)0xC0200000)
//#define LCD_LAYER1_FRAME_BUFFER         ((int)0xC0400000)

//#define LCD_LAYER0_FRAME_BUFFER         ((int)0xC0200000)
//#define LCD_LAYER0_FRAME_BUFFER         ((int)0xC0400000)
#define LCD_LAYER0_FRAME_BUFFER         ((int)0xC0800000)


#define SWAP(a,b)       {int16_t t=a;a=b;b=t;}


/*******************************************************************************
*
*******************************************************************************/
typedef struct
{
                int32_t                 address;
       volatile int32_t                 pending_buffer;
                int32_t                 buffer_index;
                int32_t                 xSize;
                int32_t                 ySize;
                int32_t                 BytesPerPixel;
                LCD_API_COLOR_CONV *    pColorConvAPI;
} LCD_LayerPropTypedef;

/*******************************************************************************
*
*******************************************************************************/
void TFT_FillScreen565(uint32_t color);


void TFT_FillScreen(uint32_t color);


void TFT_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t color);

void TFT_FillRectangle(uint16_t x1, uint16_t y1,
        uint16_t x2, uint16_t y2, uint32_t color);


#endif  //LCDCONF_H
