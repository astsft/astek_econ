/**
  * @file   scr_setup.c
  * @brief  Screen Setup
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "main.h"

extern  dev_t           dev;
static uint32_t waiting_time = 0;
static uint32_t slope_ready = 0;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480 
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                           0,  80, 800, 400, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        25,  20,  750,  280, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                 50,  32,  600,   32, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 50,  128, 350,   64, 0, 0x0, 0 },    

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_LABEL,       500,   96, 250,  32, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,       500,  160, 250,  32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SLOPE_LABEL,      500,  192, 250,  32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DEVIATION_LABEL,  500,  224, 250,  32, 0, 0x0, 0 },        
            
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,                 50,  64,  500,   32, 0, 0x0, 0 },  
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,                 300, 192, 250,   32, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,                 300, 224, 250,   32, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,                 50,  128, 700,   32, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT7,                 50,  160, 700,   32, 0, 0x0, 0 },    
     
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,          0, 320, 400,  80, 0, 0x0, 0  },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NEXT,          400, 320, 400,  80, 0, 0x0, 0  },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,          -1,  -1,   1,   1, 0, 0x0, 0  },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                             0,  100, 1024, 500, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        32,   25,   960,  350, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                 64,   40,   768,   40, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 64,  160,   448,   80, 0, 0x0, 0 },    
                                                                             
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_LABEL,       640,  120,  320,  40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,       640,  200,  320,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SLOPE_LABEL,      640,  240,  320,  40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DEVIATION_LABEL,  640,  280,  320,  40, 0, 0x0, 0 },            
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,                 64,    80,  640,   40, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,                 384,  240,  320,   40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,                 384,  280,  320,   40, 0, 0x0, 0 },         
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,                 64,   160,  904,   40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT7,                 64,   200,  904,   40, 0, 0x0, 0 },      
    
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
    uint32_t slope_ready = 0;
    waiting_time = dev.calibration.timings.transition_time + dev.calibration.timings.measure_time;
    
    int32_t     ppb         = dev.sens->meas.ppb.i32; 
    if (ppb < 0) ppb = 0;
    
    int32_t     measure_deviation = 0;
    if (dev.state.process_status == PROCESS_CALIBRATION_ZERO)    
        measure_deviation  = ppb - dev.calibration.zero.ppb;
    else
        measure_deviation  = ppb - dev.calibration.span.ppb;      

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
    {
      if (dev.state.process_status == PROCESS_CALIBRATION_ZERO)
      {
          ppb      = dev.calibration.zero.ppb;
          snprintf( str, sizeof(str), "ШАГ 2 ИЗ 3, КАЛИБРОВКА НИЗ: %2d.%02u %%", ppb / (1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 100000);        
                        
      }
      else
      {
          ppb      = dev.calibration.span.ppb;
          snprintf( str, sizeof(str), "ШАГ 2 ИЗ 3, КАЛИБРОВКА ВЕРХ: %2d.%02u %%", ppb / (1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 100000);          
      }       
    }
    else
    {
      if (dev.state.process_status == PROCESS_CALIBRATION_ZERO)
      {
          ppb      = dev.calibration.zero.ppb;
          snprintf( str, sizeof(str), "STEP 2 FROM 3, ZERO CALIBRATION: %2d.%02u %%", ppb / (1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 100000);                   
      }
      else
      {
          ppb      = dev.calibration.span.ppb;
          snprintf( str, sizeof(str), "STEP 2 FROM 3, SPAN CALIBRATION: %2d.%02u %%", ppb / (1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 100000);
          
      }       
    }    
    TEXT_SetText(           hItem, str  );
    
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
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );    

    if( abs(ppb) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "100.00 %%"  );
    }
    else if (ppb < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 100000 );
    
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL ), str );
        
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );     
    snprintf( str, sizeof(str), "% 3i.%01u °C", dev.sens->meas.digc.integral, dev.sens->meas.digc.fractional );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    TEXT_SetText( hItem, str );  
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );     
    snprintf( str, sizeof(str), "%d RAW", dev.sens->meas.slope); 
    TEXT_SetText( hItem, str );  
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVIATION_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );    

    if( abs(measure_deviation) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "100.00 %%"  );
    }
    else if (measure_deviation < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", measure_deviation /(1000 * 10000), ( abs(measure_deviation) % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", measure_deviation / (1000 * 10000), ( abs(measure_deviation) % (1000 * 10000)) / 100000 );
    
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVIATION_LABEL ), str );
   
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT3 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );    
    if (dev.cfg.lang)   
      TEXT_SetText( hItem, "НАКЛОН:");
    else
      TEXT_SetText( hItem, "SLOPE:");
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_RIGHT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );    
    if (dev.cfg.lang)   
      TEXT_SetText( hItem, "ОТКЛОНЕНИЕ:");
    else
      TEXT_SetText( hItem, "DEVIATION:");    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    if (dev.cfg.lang)   
      TEXT_SetText(hItem, "РЕЗУЛЬТАТ КАЛИБРОВКИ"); 
    else
      TEXT_SetText(hItem, "CALIBRATION RESULT"); 
    WM_BringToBottom( hItem );  
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT7 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER           );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );  
    if (dev.cfg.lang)   
      TEXT_SetText(hItem, "ОШИБКА ОТКЛОНЕНИЯ"); 
    else
      TEXT_SetText(hItem, "DEVIATION ERROR"); 
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
  static uint32_t slope_timer = 0;
  int32_t     ppb = dev.sens->meas.ppb.i32;   
  
  int32_t     measure_deviation = 0;
  if (dev.state.process_status == PROCESS_CALIBRATION_ZERO)    
      measure_deviation  = ppb - dev.calibration.zero.ppb;
  else
      measure_deviation  = ppb - dev.calibration.span.ppb;
  
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );    
    if (dev.cfg.lang)   
      snprintf(str, sizeof(str), "ЖДИТЕ %d с", waiting_time); 
    else
      snprintf(str, sizeof(str), "WAIT %d s", waiting_time);
    TEXT_SetText(           hItem, str );    
    
    if (abs(ppb) < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb /(1000 * 10000), ( ppb % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", ppb / (1000 * 10000), ( ppb % (1000 * 10000)) / 100000 );
    
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL ), str );    
    
    snprintf( str, sizeof(str), "% 3i.%01u °C", dev.sens->meas.digc.integral, dev.sens->meas.digc.fractional );
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    TEXT_SetText( hItem, str );   

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_LABEL ); 
    snprintf( str, sizeof(str), "%d RAW", dev.sens->meas.slope); 
    TEXT_SetText( hItem, str );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVIATION_LABEL ); 
   
    if( abs(measure_deviation) >= 1000000000ul )
    {
      snprintf( str, sizeof(str), "100.00 %%"  );
    }
    else if (measure_deviation < 100000)
    {
      snprintf( str, sizeof(str), "% 2d.%02u %%", measure_deviation /(1000 * 10000), ( abs(measure_deviation) % (1000 * 10000)) / 1000 );
    }
    else
      snprintf( str, sizeof(str), "% 2d.%02u %%", measure_deviation / (1000 * 10000), ( abs(measure_deviation) % (1000 * 10000)) / 100000 );
    
    TEXT_SetText( WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVIATION_LABEL ), str );
    
    if (abs(dev.sens->meas.slope) <= dev.calibration.max_slope)
    {
      slope_timer++;
      if (slope_timer > dev.calibration.timings.stable_slope_time)
        slope_ready = 1;
    }
    else
    {
      slope_timer = 0;
      slope_ready = 0;
    }
}

static
void
scr_show_calibration_results(                             WM_HWIN         hWin)
{
    WM_HWIN         hItem;
    char            str[ 64];
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    WM_BringToBottom( hItem );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL );
    WM_BringToBottom( hItem );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_PRES_LABEL );
    WM_BringToBottom( hItem );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TEMP_LABEL );
    WM_BringToBottom( hItem );  
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_LABEL );
    WM_BringToBottom( hItem ); 

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVIATION_LABEL );
    WM_BringToBottom( hItem );     
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT3 );
    WM_BringToBottom( hItem );   

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );
    WM_BringToBottom( hItem );  
    
    int32_t     measure_deviation = 0;
    int32_t     ppb         = dev.sens->meas.ppb.i32;
    if (dev.state.process_status == PROCESS_CALIBRATION_ZERO)    
        measure_deviation  = ppb - dev.calibration.zero.ppb;
    else
        measure_deviation  = ppb - dev.calibration.span.ppb;
    
    
    dev.calibration.result = CALIBRATION_SUCCESS;     
    
    if (abs(measure_deviation) > dev.calibration.deviation.ppb)
    {
        dev.calibration.result = CALIBRATION_FAIL;
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT7 );
        TEXT_SetTextColor(      hItem, GUI_RED                               );  
        if (dev.cfg.lang)   
          TEXT_SetText(hItem, "ОШИБКА ОТКЛОНЕНИЯ"); 
        else
          TEXT_SetText(hItem, "DEVIATION ERROR"); 
        WM_BringToTop( hItem );          
    }
    
    if (!slope_ready)  
    {
       dev.calibration.result = CALIBRATION_FAIL;  
       hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT7 );
       TEXT_SetTextColor(      hItem, GUI_RED                               );  
       if (dev.cfg.lang)   
         TEXT_SetText(hItem, "ОШИБКА СТАБИЛИЗАЦИИ"); 
       else
         TEXT_SetText(hItem, "STABILIZATION ERROR"); 
       WM_BringToTop( hItem );         
    }
    
    if (dev.calibration.result == CALIBRATION_SUCCESS )
    {    
       send_cmd_for_calibration_passed();
       
       uint32_t timestamp = 0;
       if (dev.state.process_status == PROCESS_CALIBRATION_ZERO)
       {
         dev.sens->cal.zero.ppm.i32   = dev.calibration.zero.ppb / 1000;

         timestamp = dev.mcu->rtc.get_timestamp();
         dev.sens->cal.zero.timestamp.i32 = dev.mcu->rtc.get_timestamp();  
         task_hmi_cal_update( 0 );
       }
       else
       {
         dev.sens->cal.span.ppm.i32   = dev.calibration.span.ppb / 1000;
         dev.sens->cal.span.timestamp.i32 = dev.mcu->rtc.get_timestamp();    
         task_hmi_cal_update( 1 );
       }
       
       hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );
       if (dev.cfg.lang)      
         TEXT_SetText(           hItem, "УСПЕШНАЯ КАЛИБРОВКА"  ); 
       else
         TEXT_SetText(           hItem, "CALIBRATION PASSED" );         
    }
    else
    {
      send_cmd_for_calibration_error();
       hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );
       if (dev.cfg.lang)      
         TEXT_SetText(           hItem, "ОШИБКА КАЛИБРОВКИ"  ); 
       else
         TEXT_SetText(           hItem, "CALIBRATION FAILED" );       
       TEXT_SetTextColor(      hItem, GUI_RED        );
    }
          
    WM_BringToTop( hItem );  
    
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
    scr_show_calibration_results(hWin);
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_NEXT );    
    WM_EnableWindow(hItem);   
    
    waiting_time = 0xFFFFFFFF;
  }
  else if (waiting_time <= dev.validation.timings.measure_time)
  {      
    waiting_time--;     
    scr_update_measure_data(hWin);
  }
  else if (waiting_time == 0xFFFFFFFF)
  {
    if (dev.calibration.start == CALIBRATION_REMOTE_START || dev.calibration.start == CALIBRATION_STOP)
    {
       GUI_SendKeyMsg( GUI_KEY_RIGHT, 0x00 );
    }
  }
  else
  {
    waiting_time--;     
    scr_update_measure_data(hWin); 
    if (dev.calibration.start == CALIBRATION_STOP)
    {
      GUI_SendKeyMsg( GUI_KEY_ESCAPE, 0x00 );
    }
  }
}

static void cancel_pressed (void)
{
  dev.calibration.result = CALIBRATION_CANCELED;
  scr_switch( SCR_IDX_CALIBRATION_STEP3, GUI_ID_BUTTON_DUMMY);     
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
                      scr_switch( SCR_IDX_CALIBRATION_STEP3, GUI_ID_BUTTON_DUMMY ); 
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
                        scr_switch( SCR_IDX_CALIBRATION_STEP3, GUI_ID_BUTTON_DUMMY ); 
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
scr_calibration_step2( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
