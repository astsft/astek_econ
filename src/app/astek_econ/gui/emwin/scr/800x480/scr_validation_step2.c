/**
  * @file   scr_setup.c
  * @brief  Screen Setup
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"

extern  dev_t           dev;
static uint32_t waiting_time = 0;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480 
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                           0,  80, 800, 400, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        25,  20,  750,  280, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                 50,  32,  300,   32, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 50,  128, 350,   64, 0, 0x0, 0 },    
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_LABEL,       400,  64,  350,  64, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,       400,  192, 350,  64, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,                 50,  96,  500,   32, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,                 50,  128, 500,   32, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,                 50,  160, 500,   32, 0, 0x0, 0 },   
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,                 50,  192, 500,   32, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,                 50,  256, 500,   32, 0, 0x0, 0 },     

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT7,                 550,  96,  200,   32, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT8,                 550,  128, 200,   32, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT9,                 550,  160, 200,   32, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT10,                550,  192, 200,   32, 0, 0x0, 0 },       
    
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,          0, 320, 400,  80, 0, 0x0, 0  },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NEXT,          400, 320, 400,  80, 0, 0x0, 0  },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,          -1,  -1,   1,   1, 0, 0x0, 0  },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                             0,  100, 1024, 500, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        32,   25, 960,  350, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                 64,  40,  384,   40, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 64,  160, 448,   80, 0, 0x0, 0 },    
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_LABEL,       512,  80,  448,  80, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,       512,  240, 448,  80, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,                 64,  120,  640,   40, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,                 64,  160,  640,   40, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,                 64,  200,  640,   40, 0, 0x0, 0 },   
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,                 64,  240,  640,   40, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,                 64,  320,  640,   40, 0, 0x0, 0 },     

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT7,                 704,  120, 256,   40, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT8,                 704,  160, 256,   40, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT9,                 704,  200, 256,   40, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT10,                704,  240, 256,   40, 0, 0x0, 0 },       
    

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 400, 512,  100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NEXT,        512, 400, 512,  100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#endif


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN         hItem;
    char            str[ 64];
    waiting_time = dev.validation.timings.transition_time + dev.validation.timings.measure_time;
    
    int32_t     ppb         = dev.sens->meas.ppb.i32; 
    if (ppb < 0) ppb = 0;

    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    WM_BringToBottom( hItem );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );    
    if (dev.cfg.lang)      
      TEXT_SetText(           hItem, "ШАГ 2 ИЗ 3"  ); 
    else
      TEXT_SetText(           hItem, "STEP 2 FROM 3" ); 
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );    
    if (dev.cfg.lang)   
      snprintf(str, sizeof(str), "ЖДИТЕ %d с", waiting_time); 
    else
      snprintf(str, sizeof(str), "WAIT %d s", waiting_time);
    TEXT_SetText(           hItem, str );   
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );    

    if( abs(ppb) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "99.99 %%"  );
    }
    else if (ppb < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( ppb % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( ppb % (1000 * 10000)) / 100000 );
    
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL ), str );   
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );     
    snprintf( str, sizeof(str), "% 3i.%01u °C", dev.sens->meas.digc.integral, dev.sens->meas.digc.fractional );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    TEXT_SetText( hItem, str );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    if (dev.cfg.lang)   
      TEXT_SetText(hItem, "УСТАНОВЛЕННОЕ ЗНАЧЕНИЕ"); 
    else
      TEXT_SetText(hItem, "SET VALUE"); 
    WM_BringToBottom( hItem );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT3 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    if (dev.cfg.lang)   
      TEXT_SetText(hItem, "УСТАНОВЛЕННОЕ ОТКЛОНЕНИЕ"); 
    else
      TEXT_SetText(hItem, "SET DEVIATION"); 
    WM_BringToBottom( hItem );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    if (dev.cfg.lang)   
      TEXT_SetText(hItem, "ИЗМЕРЕННОЕ ЗНАЧЕНИЕ"); 
    else
      TEXT_SetText(hItem, "MEASURE VALUE"); 
    WM_BringToBottom( hItem );   
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT5 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    if (dev.cfg.lang)   
      TEXT_SetText(hItem, "РАССЧИТАННОЕ ОТКЛОНЕНИЕ"); 
    else
      TEXT_SetText(hItem, "CALCULATED DEVIATION"); 
    WM_BringToBottom( hItem );      
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    if (dev.cfg.lang)   
      TEXT_SetText(hItem, "РЕЗУЛЬТАТ ВАЛИДАЦИИ"); 
    else
      TEXT_SetText(hItem, "VALIDATION RESULT"); 
    WM_BringToBottom( hItem );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT7 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    WM_BringToBottom( hItem );     
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT8 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    WM_BringToBottom( hItem ); 

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT9 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    WM_BringToBottom( hItem );   
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT10 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    WM_BringToBottom( hItem );       

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
    BUTTON_SetFocusable(    hItem, 0 );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NEXT );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_NEXT )        );
    BUTTON_SetFocusable(    hItem, 0 );
    
    WM_DisableWindow(hItem);
}

static
void
scr_update_measure_data(                             WM_HWIN         hWin)
{
  WM_HWIN         hItem;  
  char            str[ 64];
  int32_t     ppb         = dev.sens->meas.ppb.i32;   
  if (ppb < 0) ppb = 0;  
  
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );    
    if (dev.cfg.lang)   
      snprintf(str, sizeof(str), "ЖДИТЕ %d с", waiting_time); 
    else
      snprintf(str, sizeof(str), "WAIT %d s", waiting_time);
    TEXT_SetText(           hItem, str );    
    

    if( abs(ppb) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "99.99 %%"  );
    }
    else if (ppb < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( ppb % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( ppb % (1000 * 10000)) / 100000 );    
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL ), str ); 
    
    snprintf( str, sizeof(str), "% 3i.%01u °C", dev.sens->meas.digc.integral, dev.sens->meas.digc.fractional );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    TEXT_SetText( hItem, str );       
}

static
void
scr_show_validation_results(                             WM_HWIN         hWin)
{
    WM_HWIN         hItem;
    char            str[ 64];
    int32_t     ppb = 0; 
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    WM_BringToBottom( hItem );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL );
    WM_BringToBottom( hItem );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_PRES_LABEL );
    WM_BringToBottom( hItem );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    WM_BringToBottom( hItem );  
    
  
    ppb = dev.validation.value.ppb;    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT7 );

    if( abs(ppb) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "99.99 %%"  );
    }
    else if (ppb < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( ppb % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( ppb % (1000 * 10000)) / 100000 );
    
    TEXT_SetText(hItem, str );

    WM_BringToTop( hItem );
    
    ppb = dev.validation.deviation.ppb;
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT8 );
    
    if( abs(ppb) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "99.99 %%"  );
    }
    else if (ppb < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( ppb % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( ppb % (1000 * 10000)) / 100000 );
    
    TEXT_SetText(hItem, str );
   
    WM_BringToTop( hItem );  
    
    ppb = dev.validation.measure_value;   
    if (ppb < 0) ppb = 0;
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT9 );

    if( abs(ppb) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "99.99 %%"  );
    }
    else if (ppb < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( ppb % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( ppb % (1000 * 10000)) / 100000 );
    
    TEXT_SetText(hItem, str );

    WM_BringToTop( hItem );
    
    ppb = dev.validation.measure_deviation;
    if (ppb < 0) ppb = 0;
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT10 );

    if( abs(ppb) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "99.99 %%"  );
    }
    else if (ppb < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( ppb % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( ppb % (1000 * 10000)) / 100000 );
    
    TEXT_SetText(hItem, str );
 
    WM_BringToTop( hItem );    
             
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
    WM_BringToTop( hItem );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT3 ); 
    WM_BringToTop( hItem );   
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT4 ); 
    WM_BringToTop( hItem );   
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT5 );
    WM_BringToTop( hItem );      
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );
    
    if (dev.validation.measure_deviation <= dev.validation.deviation.ppb)
    {
      dev.validation.result = VALIDATION_SUCCESS;
      send_cmd_for_validation_passed();
      if (dev.cfg.lang)      
        TEXT_SetText(           hItem, "ВАЛИДАЦИЯ ПРОЙДЕНА"  ); 
      else
        TEXT_SetText(           hItem, "VAlIDATION PASSED" );         
    }
    else
    {
      dev.validation.result = VALIDATION_FAIL;
      send_cmd_for_validation_error();
      if (dev.cfg.lang)      
        TEXT_SetText(           hItem, "ОШИБКА ВАЛИДАЦИИ"  ); 
      else
        TEXT_SetText(           hItem, "VAlIDATION FAILED" );               
    }
          
    WM_BringToTop( hItem );   

}

static void calc_validation_results(void)
{
  int64_t amount_value = 0; 
  int32_t min_value = 0;
  int32_t max_value = 0;
  
  max_value = dev.validation.measure_buf[0];
  for (int j = 1; j < dev.validation.measure_num; j++)
  {
  	if (max_value < dev.validation.measure_buf[j])
  	{
  		max_value = dev.validation.measure_buf[j];
  	}
  }

  min_value = dev.validation.measure_buf[0];
  for (int j = 1; j < dev.validation.measure_num; j++)
  {
  	if (min_value < dev.validation.measure_buf[j])
  	{
  	   min_value = dev.validation.measure_buf[j];
  	}
  }  
  
  for (int j = 0; j < dev.validation.measure_num; j++)
  {
  	amount_value += dev.validation.measure_buf[j];
  }
  
  amount_value -= (max_value + min_value);  
  
  if (dev.validation.measure_num < 3)
    dev.validation.measure_num = 3;
  
  dev.validation.measure_value = amount_value / (dev.validation.measure_num - 2);
  
  if (dev.validation.measure_value > dev.validation.value.ppb)
    dev.validation.measure_deviation = dev.validation.measure_value - dev.validation.value.ppb;
  else
    dev.validation.measure_deviation = dev.validation.value.ppb - dev.validation.measure_value;
  
}

static
void
scr_update(                             WM_HWIN         hWin)
{
  WM_HWIN         hItem;
  static int i = 0;
  int32_t     ppb         = dev.sens->meas.ppb.i32;     
  if (ppb < 0) ppb = 0;   
  
  if (waiting_time == 0)
  {  
    // Add final measure value      
    //dev.validation.measure_buf[i++] = ppb;
    dev.validation.measure_num = i;
    
    calc_validation_results();
    scr_show_validation_results(hWin);
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NEXT );    
    WM_EnableWindow(hItem);   
    
    waiting_time = 0xFFFFFFFF;
    i = 0;
  }
  else if (waiting_time <= dev.validation.timings.measure_time)
  {    
    dev.validation.measure_buf[i++] = ppb;    
    waiting_time--;     
    scr_update_measure_data(hWin);
  }
  else if (waiting_time == 0xFFFFFFFF)
  {
    if (dev.validation.start == VALIDATION_REMOTE_START || dev.validation.start == VALIDATION_STOP)
    {
       GUI_SendKeyMsg( GUI_KEY_RIGHT, 0x00 );
    }
  }
  else
  {
    waiting_time--;     
    scr_update_measure_data(hWin); 
    if (dev.validation.start == VALIDATION_STOP)
    {
      GUI_SendKeyMsg( GUI_KEY_ESCAPE, 0x00 );
    }
  }
}

static void cancel_pressed (void)
{
  //dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
  //dev.validation.state = FINISH;
  //dev.state.process_status = PROCESS_MEASURE;
  //dev.validation.start = VALIDATION_STOP;
  dev.validation.result = VALIDATION_CANCELED;
  //send_cmd_for_validation_stop();
  //scr_switch( SCR_IDX_VALIDATION, GUI_ID_BUTTON_START);   
  scr_switch( SCR_IDX_VALIDATION_STEP3, GUI_ID_BUTTON_DUMMY);     
}

static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    WM_HWIN         hItem;
  
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_DOWN:
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_LEFT:                  
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:                    
                    if (WM_IsEnabled(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_NEXT )))
                      scr_switch( SCR_IDX_VALIDATION_STEP3, GUI_ID_BUTTON_DUMMY ); 
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    cancel_pressed();
                    beep_play( BEEP_SHRT );
                    break;
                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON_NEXT:
                        scr_switch( SCR_IDX_VALIDATION_STEP3, GUI_ID_BUTTON_DUMMY ); 
                        beep_play( BEEP_SHRT );
                        break;                        
                    case GUI_ID_BUTTON_CANCEL:
                        cancel_pressed();
                        beep_play( BEEP_SHRT );
                        break;
                    default:
                        beep_play( BEEP_TYPE_ERROR );
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_CreateTimer( pMsg->hWin, 0, 1000, 0 );
            break;
            
        case WM_TIMER:
            scr_update( pMsg->hWin);
            hItem   = pMsg->Data.v;
            WM_RestartTimer( hItem, 0 );
            break;             

        default:
            WM_DefaultProc(pMsg);
    }
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief
  * @param  None
  * @retval None
  */
WM_HWIN
scr_validation_step2( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
