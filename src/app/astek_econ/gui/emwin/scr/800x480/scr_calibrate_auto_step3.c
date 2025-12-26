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
static WM_MESSAGE *            pMsg_temp;

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
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_LABEL,       400,  64,  350,  64, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,       400,  192, 350,  64, 0, 0x0, 0 },      
        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,          0, 320, 800,  80, 0, 0x0, 0  },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,          -1,  -1,   1,   1, 0, 0x0, 0  },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                             0,  100, 1024, 500, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        32,   25,  960, 350, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                 64,   40,  768,  40, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 64,  160,  448,  80, 0, 0x0, 0 },    
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_LABEL,       512,  80,  448,  80, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TEMP_LABEL,       512,  240, 448,  80, 0, 0x0, 0 },    
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,          0, 400, 1024,  100, 0, 0x0, 0 },
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
    waiting_time = dev.calibration.timings.return_time;
    
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
      TEXT_SetText(           hItem, "ШАГ 3 ИЗ 3, ВОЗВРАТ К ИЗМЕРЕНИЮ"  ); 
    else
      TEXT_SetText(           hItem, "STEP 3 FROM 3, RETURN TO MEASURE" ); 
    
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
      snprintf( str, sizeof(str), "100.00 %%"  );
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

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
    BUTTON_SetFocusable(    hItem, 0 );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
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
      snprintf( str, sizeof(str), "100.00 %%"  );
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
scr_update(                             WM_HWIN         hWin)
{
  WM_HWIN         hItem;
  
  if (waiting_time == 0)
  {  
    scr_update_measure_data(hWin);    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    if (dev.cfg.lang)  
      TEXT_SetText(           hItem, "ГОТОВО");
    else
      TEXT_SetText(           hItem, "READY");
              
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );    
    WM_EnableWindow(hItem);    
    
    if (dev.calibration.start == CALIBRATION_STOP || dev.calibration.start == CALIBRATION_REMOTE_START)
    {

       memset(pMsg_temp, 0x00, sizeof(WM_MESSAGE));
       
       pMsg_temp->Data.v = WM_NOTIFICATION_RELEASED;
       pMsg_temp->hWinSrc =  WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );      
       pMsg_temp->MsgId = WM_NOTIFY_PARENT;
       pMsg_temp->hWin = hWin;
       WM_SendMessage( hWin, pMsg_temp );
    }    
  }
  else
  {
    waiting_time--;   
    scr_update_measure_data(hWin); 
  }
}

static void cancel_pressed (void)
{
 
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
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    //cancel_pressed();  
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
                    case GUI_ID_BUTTON_DUMMY:
                    case GUI_ID_BUTTON_ENTER: 
                        if (WM_IsEnabled(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_ENTER )))
                        {
                          send_cmd_for_calibration_stop();
                          dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
                          dev.calibration.state = CALIBRATION_FINISH;
                          dev.state.process_status = PROCESS_MEASURE;
                          dev.calibration.start = CALIBRATION_STOP;                          
                          scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY);                           
                        }
                        beep_play( BEEP_SHRT );
                        break;                        
                    case GUI_ID_BUTTON_CANCEL:
                        //cancel_pressed();
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
scr_calibration_step3( void )
{
    send_cmd_for_calibration_return_to_measure();
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
