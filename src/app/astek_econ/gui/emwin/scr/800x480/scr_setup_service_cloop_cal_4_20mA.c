/**
  * @file    scr_setup_cloop_cal_4_20mA.c
  * @brief   Cloop calibrate
  * @author  Nikonov Konstantin
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "hw_cl420.h"

extern  dev_t           dev;

uint16_t raw_backup = 0;
WM_MESSAGE Message;
/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  const   GUI_WIDGET_CREATE_INFO  dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0, 80, 800, 400, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        25,  20, 750, 280, 0, 0x0, 0 },
  
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 25,  150, 375,  32, 0, 0x0, 0 }, 
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_COURSE_UP,      112, 70, 200,  64, 0, 0x0, 0 }, 
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_COURSE_DOWN,    112, 198, 200,  64, 0, 0x0, 0 }, 

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,                 400, 150, 375,  32, 0, 0x0, 0 }, 
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_FINE_DOWN,      487, 198, 200,  64, 0, 0x0, 0 }, 
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_FINE_UP,        487, 70, 200,  64, 0, 0x0, 0 }, 
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_dialog(                                            WM_HWIN         hWin )
{
    WM_HWIN         hItem;


    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_HCENTER | TEXT_CF_VCENTER         );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                     );
    TEXT_SetTextAlign(      hItem, GUI_TA_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                             );
    TEXT_SetTextColor(      hItem, GUI_WHITE                             );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_COARSE  )     );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_COURSE_UP );
    BUTTON_SetFont(           hItem, &GUI_FontTahoma40                     );
    BUTTON_SetText(         hItem, "+");
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_COURSE_DOWN );
    BUTTON_SetFont(           hItem, &GUI_FontTahoma40                     );
    BUTTON_SetText(         hItem, "-");
    BUTTON_SetFocusable(    hItem, 1                                        );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                     );
    TEXT_SetTextAlign(      hItem, GUI_TA_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, GUI_BLACK                             );
    TEXT_SetTextColor(      hItem, GUI_GRAY                             );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_FINE )     );
    //WM_DisableWindow(hItem);
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_FINE_UP );
    BUTTON_SetFont(           hItem, &GUI_FontTahoma40                     );
    BUTTON_SetText(         hItem, "+");
    BUTTON_SetFocusable(    hItem, 1                                        );
    WM_DisableWindow(hItem);
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_FINE_DOWN );
    BUTTON_SetFont(           hItem, &GUI_FontTahoma40                     );
    BUTTON_SetText(         hItem, "-");
    BUTTON_SetFocusable(    hItem, 1                                        );
    WM_DisableWindow(hItem);
    

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1                                        );
   
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
    BUTTON_SetFocusable(    hItem, 0                                        );
   
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
    BUTTON_SetFocusable(    hItem, 0                                        );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
}

static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{        
        int btn_id = 0;

        switch( pMsg->MsgId )
        {
                case WM_KEY:
                        switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
                        {
                                case GUI_KEY_UP:
                                        if (WM_IsEnabled(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_UP))) // COARSE      
                                        {
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                    if (dev.cloop->cal_ch1[0].raw.u16[0] < 0xFFF5) coarse_increase_4ma_raw(1);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(1);                            
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                    if (dev.cloop->cal_ch2[0].raw.u16[0] < 0xFFF5) coarse_increase_4ma_raw(2);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(2);
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    if (dev.cloop->cal_ch1[1].raw.u16[0] < 0xFFF5) coarse_increase_20ma_raw(1);
                                                    send_cmd_for_cloop_set_raw_20mA(1);
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    if (dev.cloop->cal_ch2[1].raw.u16[0] < 0xFFF5) coarse_increase_20ma_raw(2);
                                                    send_cmd_for_cloop_set_raw_20mA(2);
                                                    break;                                                    
                                                }
                                        }
                                        else    // FINE                                        
                                        {
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                    if (dev.cloop->cal_ch1[0].raw.u16[0] < 0xFFFF) fine_increase_4ma_raw(1);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(1);                            
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                    if (dev.cloop->cal_ch2[0].raw.u16[0] < 0xFFFF) fine_increase_4ma_raw(2);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(2);                            
                                                    break;                                                    
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    if (dev.cloop->cal_ch1[1].raw.u16[0] < 0xFFFF) fine_increase_20ma_raw(1);
                                                    send_cmd_for_cloop_set_raw_20mA(1);
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    if (dev.cloop->cal_ch2[1].raw.u16[0] < 0xFFFF) fine_increase_20ma_raw(2);
                                                    send_cmd_for_cloop_set_raw_20mA(2);
                                                    break;                                                    
                                                }
                                        }
                                        beep_play( BEEP_SHRT );
                                        break;

                                case GUI_KEY_DOWN:
                                        if (WM_IsEnabled(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_DOWN))) // COARSE                                                                                
                                        {
                                            switch (dev.gui.scr_idx)
                                            {
                                              case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                if (dev.cloop->cal_ch1[0].raw.u16[0] >= 10) coarse_decrease_4ma_raw(1);                                                    
                                                send_cmd_for_cloop_set_raw_4mA(1);                            
                                                break;
                                                
                                              case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                if (dev.cloop->cal_ch2[0].raw.u16[0] >= 10) coarse_decrease_4ma_raw(2);                                                    
                                                send_cmd_for_cloop_set_raw_4mA(2);                            
                                                break;                                                
                                                
                                              case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                if (dev.cloop->cal_ch1[1].raw.u16[0] >= 10) coarse_decrease_20ma_raw(1);
                                                send_cmd_for_cloop_set_raw_20mA(1);
                                                break;
                                                
                                              case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                if (dev.cloop->cal_ch2[1].raw.u16[0] >= 10) coarse_decrease_20ma_raw(2);
                                                send_cmd_for_cloop_set_raw_20mA(2);
                                                break;                                                
                                            }                                         
                                        }                                      
                                        else    // FINE         
                                        {
                                             switch (dev.gui.scr_idx)
                                             {
                                               case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                 if (dev.cloop->cal_ch1[0].raw.u16[0] >= 1) fine_decrease_4ma_raw(1);                                                    
                                                 send_cmd_for_cloop_set_raw_4mA(1);                            
                                                 break;
                                                 
                                               case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                 if (dev.cloop->cal_ch2[0].raw.u16[0] >= 1) fine_decrease_4ma_raw(2);                                                    
                                                 send_cmd_for_cloop_set_raw_4mA(2);                            
                                                 break;                                                 
                                                 
                                               case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                 if (dev.cloop->cal_ch1[1].raw.u16[0] >= 1) fine_decrease_20ma_raw(1);
                                                 send_cmd_for_cloop_set_raw_20mA(1);
                                                 break;

                                               case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                 if (dev.cloop->cal_ch2[1].raw.u16[0] >= 1) fine_decrease_20ma_raw(2);
                                                 send_cmd_for_cloop_set_raw_20mA(2);
                                                 break;                                                 
                                             }                                          
                                        }
                                        beep_play( BEEP_SHRT );
                                        break;

                                case GUI_KEY_ESCAPE:
                                        switch (dev.gui.scr_idx)
                                        {
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:                                                    
                                            dev.cloop->cal_ch1[0].raw.u16[0] = raw_backup;
                                            btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                            break;
                                            
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:                                                    
                                            dev.cloop->cal_ch2[0].raw.u16[0] = raw_backup;
                                            btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                            break;                                            
                                            
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                            dev.cloop->cal_ch1[1].raw.u16[0] = raw_backup;       
                                            btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                            break;
                                            
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                            dev.cloop->cal_ch2[1].raw.u16[0] = raw_backup;       
                                            btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                            break;                                            
                                        }                     
                                        dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
                                        dev.state.process_status = PROCESS_MEASURE;
                                        scr_switch( dev.gui.scr_idx, btn_id );
                                        beep_play( BEEP_SHRT );
                                        break;

                                case GUI_KEY_LEFT:
                                        // Left - enabled COARSE, disabled FINE
                                        // 
                                        //WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1));
                                        WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_DOWN));
                                        WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_UP));      
                                        TEXT_SetTextColor(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1), GUI_WHITE);
                                        
                                        //WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2));
                                        WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_DOWN));                                        
                                        WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_UP));                                        
                                        TEXT_SetTextColor(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2), GUI_GRAY);
                                        beep_play( BEEP_SHRT );
                                        break;

                                case GUI_KEY_RIGHT:
                                        // Right - enabled FINE, disabled COARSE
                                        //
                                        //WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2));
                                        WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_DOWN));
                                        WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_UP));
                                        TEXT_SetTextColor(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2), GUI_WHITE);
                                        
                                        //WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1));
                                        WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_DOWN));                                        
                                        WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_UP)); 
                                        TEXT_SetTextColor(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1), GUI_GRAY);
                                        beep_play( BEEP_SHRT );
                                        break;
                                        
                                 case GUI_KEY_ENTER:
                                        switch (dev.gui.scr_idx)
                                        {
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:                                                    
                                            send_cmd_for_cloop_4mA_write(1);          
                                            btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                            break;
                                            
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:                                                    
                                            send_cmd_for_cloop_4mA_write(2);          
                                            btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;                                            
                                            break;                                                                                                                              
                                            
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                            send_cmd_for_cloop_20mA_write(1);  
                                            btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;                                            
                                            break;
                                            
                                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                            send_cmd_for_cloop_20mA_write(2);  
                                            btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                            dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                            break;                                            
                                        }                     
                                        dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
                                        dev.state.process_status = PROCESS_MEASURE;
                                        scr_switch( dev.gui.scr_idx, btn_id );
                                        beep_play( BEEP_TYPE_CONFIRM );
                                        break;

                                default:
                                        break;
                        }
                        break;

                case WM_NOTIFY_PARENT:
                        if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                        {
                                int Id = WM_GetId(pMsg->hWinSrc);
                                switch( Id )
                                {
                                        case GUI_ID_TEXT1:
                                          // Left - enabled COARSE, disabled FINE
                                          // 
                                          //WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1));
                                          WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_DOWN));
                                          WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_UP));      
                                          TEXT_SetTextColor(      WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1), GUI_WHITE);
                                          
                                          //WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2));
                                          WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_DOWN));                                        
                                          WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_UP));                                        
                                          TEXT_SetTextColor(      WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2), GUI_GRAY);
                                          beep_play( BEEP_SHRT );                                         
                                          break;
                                          
                                        case GUI_ID_TEXT2:
                                          // Right - enabled FINE, disabled COARSE
                                          //
                                          //WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2));
                                          WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_DOWN));
                                          WM_EnableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_FINE_UP));
                                          TEXT_SetTextColor(      WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT2), GUI_WHITE);
                                          
                                          //WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1));
                                          WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_DOWN));                                        
                                          WM_DisableWindow(WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_COURSE_UP)); 
                                          TEXT_SetTextColor(      WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT1), GUI_GRAY);
                                          beep_play( BEEP_SHRT );                                          
                                          break;                                          
                                  
                                        case GUI_ID_BUTTON_COURSE_UP:
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                    if (dev.cloop->cal_ch1[0].raw.u16[0] < 0xFFF5) coarse_increase_4ma_raw(1);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(1);                            
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                    if (dev.cloop->cal_ch2[0].raw.u16[0] < 0xFFF5) coarse_increase_4ma_raw(2);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(2);                            
                                                    break;                                                    
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    if (dev.cloop->cal_ch1[1].raw.u16[0] < 0xFFF5) coarse_increase_20ma_raw(1);
                                                    send_cmd_for_cloop_set_raw_20mA(1);
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    if (dev.cloop->cal_ch2[1].raw.u16[0] < 0xFFF5) coarse_increase_20ma_raw(2);
                                                    send_cmd_for_cloop_set_raw_20mA(2);
                                                    break;                                                    
                                                }
                                                beep_play( BEEP_SHRT );
                                                break;

                                        case GUI_ID_BUTTON_COURSE_DOWN:
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                    if (dev.cloop->cal_ch1[0].raw.u16[0] >= 10) coarse_decrease_4ma_raw(1);                                                   
                                                    send_cmd_for_cloop_set_raw_4mA(1);                            
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                    if (dev.cloop->cal_ch2[0].raw.u16[0] >= 10) coarse_decrease_4ma_raw(2);                                                   
                                                    send_cmd_for_cloop_set_raw_4mA(2);                            
                                                    break;                                                    
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    if (dev.cloop->cal_ch1[1].raw.u16[0] >= 10) coarse_decrease_20ma_raw(1);
                                                    send_cmd_for_cloop_set_raw_20mA(1);
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    if (dev.cloop->cal_ch2[1].raw.u16[0] >= 10) coarse_decrease_20ma_raw(2);
                                                    send_cmd_for_cloop_set_raw_20mA(2);
                                                    break;                                                    
                                                }
                                                beep_play( BEEP_SHRT );
                                                break;
                                                
                                        case GUI_ID_BUTTON_FINE_UP:
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                    if (dev.cloop->cal_ch1[0].raw.u16[0] < 0xFFFF) fine_increase_4ma_raw(1);                                                 
                                                    send_cmd_for_cloop_set_raw_4mA(1);                            
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                    if (dev.cloop->cal_ch2[0].raw.u16[0] < 0xFFFF) fine_increase_4ma_raw(2);                                                 
                                                    send_cmd_for_cloop_set_raw_4mA(2);                            
                                                    break;                                                    
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    if (dev.cloop->cal_ch1[1].raw.u16[0] < 0xFFFF) fine_increase_20ma_raw(1);
                                                    send_cmd_for_cloop_set_raw_20mA(1);
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    if (dev.cloop->cal_ch2[1].raw.u16[0] < 0xFFFF) fine_increase_20ma_raw(2);
                                                    send_cmd_for_cloop_set_raw_20mA(2);
                                                    break;                                                    
                                                }
                                                beep_play( BEEP_SHRT );
                                                break;

                                        case GUI_ID_BUTTON_FINE_DOWN:
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                                                    if (dev.cloop->cal_ch1[0].raw.u16[0] >= 1) fine_decrease_4ma_raw(1);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(1);                            
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                                                    if (dev.cloop->cal_ch2[0].raw.u16[0] >= 1) fine_decrease_4ma_raw(2);                                                    
                                                    send_cmd_for_cloop_set_raw_4mA(2);                            
                                                    break;                                                    
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    if (dev.cloop->cal_ch1[1].raw.u16[0] >= 1) fine_decrease_20ma_raw(1);
                                                    send_cmd_for_cloop_set_raw_20mA(1);
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    if (dev.cloop->cal_ch2[1].raw.u16[0] >= 1) fine_decrease_20ma_raw(2);
                                                    send_cmd_for_cloop_set_raw_20mA(2);
                                                    break;                                                    
                                                }
                                                beep_play( BEEP_SHRT );
                                                break;                                                
                                                
                                        case GUI_ID_BUTTON_ENTER:
                                        case GUI_ID_BUTTON_DUMMY:     
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:                                                    
                                                    send_cmd_for_cloop_4mA_write(1);    
                                                    btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:                                                    
                                                    send_cmd_for_cloop_4mA_write(2);    
                                                    btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                                    break;                                                    
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    send_cmd_for_cloop_20mA_write(1); 
                                                    btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    send_cmd_for_cloop_20mA_write(2); 
                                                    btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                                    break;                                                    
                                                }                     
                                                dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
                                                dev.state.process_status = PROCESS_MEASURE;
                                                scr_switch( dev.gui.scr_idx, btn_id );
                                                beep_play( BEEP_TYPE_CONFIRM );
                                                break;

                                        case GUI_ID_BUTTON_CANCEL:
                                                switch (dev.gui.scr_idx)
                                                {
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:                                                    
                                                    dev.cloop->cal_ch1[0].raw.u16[0] = raw_backup;
                                                    btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:                                                    
                                                    dev.cloop->cal_ch2[0].raw.u16[0] = raw_backup;
                                                    btn_id = GUI_ID_BUTTON_CLOOP_4MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                                    break;                                                    
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                                                    dev.cloop->cal_ch1[1].raw.u16[0] = raw_backup; 
                                                    btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_1_CAL;
                                                    break;
                                                    
                                                  case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                                                    dev.cloop->cal_ch2[1].raw.u16[0] = raw_backup; 
                                                    btn_id = GUI_ID_BUTTON_CLOOP_20MA;
                                                    dev.gui.scr_idx = SCR_IDX_SETUP_SERVICE_CLOOP_CHANNEL_2_CAL;
                                                    break;                                                    
                                                }                     
                                                dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
                                                dev.state.process_status = PROCESS_MEASURE;
                                                scr_switch( dev.gui.scr_idx, btn_id );
                                                beep_play( BEEP_SHRT );
                                                break;                                                

                                        default:
                                                //beep_play( BEEP_TYPE_ERROR );
                                                break;
                                }
                        }
                        break;

                case WM_INIT_DIALOG:
                        init_dialog( pMsg->hWin );
                                                                        
                        dev.cloop->cloop_state = CLOOP_FREEZ;
                        dev.state.process_status = PROCESS_CLOOP_CALIBRATION;
                        
                        switch (dev.gui.scr_idx)
                        {
                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_4MA:
                            raw_backup = dev.cloop->cal_ch1[0].raw.u16[0];
                            send_cmd_for_cloop_set_raw_4mA(1);                            
                            break;
                            
                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_4MA:
                            raw_backup = dev.cloop->cal_ch2[0].raw.u16[0];
                            send_cmd_for_cloop_set_raw_4mA(2);                            
                            break;                            
                            
                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_1_20MA:
                            raw_backup = dev.cloop->cal_ch1[1].raw.u16[0];
                            send_cmd_for_cloop_set_raw_20mA(1);
                            break;
                            
                          case   SCR_IDX_SETUP_SERVICE_CLOOP_CAL_CHANNEL_2_20MA:
                            raw_backup = dev.cloop->cal_ch2[1].raw.u16[0];
                            send_cmd_for_cloop_set_raw_20mA(2);
                            break;                            
                        }
                                              
                        //WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_SYSTEM ) );
                        break;

                default:
                        WM_DefaultProc(pMsg);
                        break;
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
scr_setup_service_cloop_cal_4_20mA( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}

