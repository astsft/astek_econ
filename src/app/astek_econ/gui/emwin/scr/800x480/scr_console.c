/**
  * @file    scr_console.c
  * @brief   Screen Console
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;

#define         MAX_ROW_COUNT   100

static  uint32_t        priveous_index = 0;

/*******************************************************************************
* PRIVATE VARIBLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0, 80, 800, 400, 0, 0x0, 0 },

    { LISTVIEW_CreateIndirect,   0, GUI_ID_LISTVIEW0,             0,   0, 800, 320, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,               0, 320, 800,  80, 0, 0x0, 0 },
};

static char data_str[11];
static char time_str[6];
static char event_type_str[2];
static char event_message_str[80];

static char *  list_row_data[1][4] = 
{
  data_str,
  time_str,
  event_type_str,
  event_message_str
};

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
add_event_string (char * event_string, log_source_t source, uint8_t event)
{
  char event_str[80] = {0x00};
  
    switch(source)
    {
    case LOG_SOURCE_SENSOR:
      switch (event)
      {
      case LOG_SENSOR_EVENT_LINK_OK:
         snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_LINK_OK ));
         break;        
        
      case LOG_SENSOR_EVENT_LINK_ERR:
         snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_LINK_ERROR ));
         break;
         
      //case LOG_SENSOR_EVENT_VALUE_TOO_HIGH_WAR:
      //   snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_VALUE_TOO_HIGH_WAR));
      //   break;
      //   
      //case LOG_SENSOR_EVENT_VALUE_TOO_LOW_WAR:
      //   snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_VALUE_TOO_LOW_WAR));
      //   break;         
      //   
      case LOG_SENSOR_EVENT_VALUE_OK:
         snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_VALUE_OK));
         break;         
         
      case LOG_SENSOR_EVENT_ZERO_CALIBRATION:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_ZERO_CALIBRATION));        
        break;        
        
      case LOG_SENSOR_EVENT_SPAN_CALIBRATION:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_SPAN_CALIBRATION));        
        break;        
        
      case LOG_SENSOR_EVENT_CALIBRATION_RESTORE:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_RESTORE_CALIBRATION));        
        break;        

      case LOG_SENSOR_EVENT_ZERO_FACTORY_UPDATE:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_ZERO_FACTORY_UPDATE));        
        break;
          
      case LOG_SENSOR_EVENT_ZERO_FACTORY_RESTORE:  
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_ZERO_FACTORY_RESTORE));                
        break;
        
      case LOG_SENSOR_EVENT_SPAN_FACTORY_UPDATE:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_SPAN_FACTORY_UPDATE));               
        break;
        
      case LOG_SENSOR_EVENT_ZERO_CALIBRATION_RESTORE:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_RESTORE_ZERO_CALIBRATION));        
        break;
        
      case LOG_SENSOR_EVENT_SPAN_CALIBRATION_RESTORE:  
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SENSOR_RESTORE_SPAN_CALIBRATION)); 
        break;
                 
      default:
         snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SENSOR ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_ERROR ));        
         break;          
      }
      break;
      
    case LOG_SOURCE_CLOOP:
      switch (event)
      {
      case LOG_CLOOP_EVENT_LINK_OK:
          snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_LINK_OK ));        
          break;        
        
      case LOG_CLOOP_EVENT_LINK_ERR:
          snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_LINK_ERROR ));        
          break;
          
      case LOG_CLOOP_EVENT_CLOOP_CALIBRATION_4MA:
          snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_CALIBRATION_4MA ));        
          break;

      case LOG_CLOOP_EVENT_CLOOP_CALIBRATION_20MA:
          snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_CALIBRATION_20MA));        
          break;          
          
      default:
         snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_CLOOP ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_ERROR ));        
         break;
      }
      break;
      
    case LOG_SOURCE_EXT_RELAY:
      switch (event)
      {
      case LOG_EXT_RELAY_EVENT_LINK_OK:
          snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_MDB_RELAY ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_LINK_OK ));        
          break;        
        
      case LOG_EXT_RELAY_EVENT_LINK_ERR:
          snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_MDB_RELAY ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_LINK_ERROR ));        
          break;
          
      default:
         snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_MDB_RELAY ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_ERROR ));        
         break;
      }
      break;      
            
    case LOG_SOURCE_SYSTEM:
      switch (event)
      {
      case LOG_SYSTEM_EVENT_START:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_SYSTEM_STARTUP ));        
        break;
        
      case LOG_SYSTEM_EVENT_FACTORY_RESET:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_FACTORY_RESET ));        
        break;  
        
      case LOG_SYSTEM_EVENT_VALIDATION_START:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_VALIDATION_START ));        
        break;
        
      case LOG_SYSTEM_EVENT_VALIDATION_STOP:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_VALIDATION_STOP ));        
        break;

      case LOG_SYSTEM_EVENT_VALIDATION_ERROR:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_VALIDATION_ERROR ));        
        break;        
        
      case LOG_SYSTEM_EVENT_VALIDATION_PASSED:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_VALIDATION_PASSED ));        
        break;        
        
      case LOG_SYSTEM_EVENT_CALIBRATION_ZERO_START:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_CALIBRATION_ZERO_START ));        
        break; 

      case LOG_SYSTEM_EVENT_CALIBRATION_SPAN_START:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_CALIBRATION_SPAN_START ));        
        break;

      case LOG_SYSTEM_EVENT_CALIBRATION_PASSED:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_CALIBRATION_PASSED ));        
        break; 
        
      case LOG_SYSTEM_EVENT_CALIBRATION_STOP:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_CALIBRATION_STOP ));        
        break;       
        
      case LOG_SYSTEM_EVENT_CALIBRATION_ERROR:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_EVENT_CALIBRATION_ERROR ));        
        break;          
                
      default:
        snprintf(event_str, 80, "%s: %s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.cfg.lang, L10N_STR_ID_ERROR ));        
        break;
      }      
      break;      
    }   
  
  
  snprintf(event_string, 80, "%s", event_str);
}

static
void
complete_listview (WM_HWIN         hItem)
{
  int             read_data_result = 0;
  log_data_t      log_data;
  uint32_t        last_log_index = 0;
  struct tm *     ts;
  time_t          time;
  
  last_log_index = (dev.log.head - 1) % LOG_INDEX_MAX; 
  priveous_index = last_log_index;
  
  for (int i = 0; i < MAX_ROW_COUNT; i++)
  {
    read_data_result = log_read_event(&log_data, last_log_index);
    if (read_data_result != 0) break;
    
    time = log_data.timestamp;
    ts = localtime(&time);
    snprintf(data_str, 11, "%04d/%02d/%02d\0", ts->tm_year + 1900, ts->tm_mon,  ts->tm_mday);
    snprintf( time_str, 6, "%02d:%02d\0", ts->tm_hour, ts->tm_min);
    
    if (log_data.event_type == LOG_TYPE_WARNING)
      snprintf(event_type_str, 2, "!");
    else 
      snprintf(event_type_str, 2, "X");
    
    add_event_string(event_message_str, log_data.source, log_data.event);
    //snprintf(event_message_str, 80, "%s: %s", l10n_str_get( dev.conf.language, L10N_STR_ID_SYSTEM ), l10n_str_get( dev.conf.language, L10N_STR_ID_EVENT_SYSTEM_STARTUP )); 
    
    LISTVIEW_AddRow( hItem, list_row_data[0] );
    
    last_log_index = (last_log_index - 1) % LOG_INDEX_MAX; 
  } 
}


static
void
init_dialog(                                            WM_HWIN         hWin )
{
        WM_HWIN         hItem;

        //WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTVIEW0 );
        LISTVIEW_SetFont(               hItem, &GUI_FontTahoma30 );
        //LISTVIEW_SetTextAlign(          hItem,   0, GUI_TA_HCENTER | GUI_TA_VCENTER );
        //LISTVIEW_SetTextAlign(          hItem,   1, GUI_TA_HCENTER | GUI_TA_VCENTER );
        //LISTVIEW_SetTextAlign(          hItem,   2, GUI_TA_LEFT    | GUI_TA_VCENTER );
        LISTVIEW_SetHeaderHeight(       hItem,   0      );
        LISTVIEW_SetGridVis(            hItem,   1      );
        LISTVIEW_SetRowHeight(          hItem,  30      );
        LISTVIEW_SetAutoScrollV (       hItem,  1       );
        LISTVIEW_SetAutoScrollH (       hItem,  0       );       
        LISTVIEW_SetBkColor(            hItem, LISTVIEW_CI_UNSEL,       GUI_BLACK );
        LISTVIEW_SetBkColor(            hItem, LISTVIEW_CI_SEL,         GUI_BLACK );
        LISTVIEW_SetBkColor(            hItem, LISTVIEW_CI_SELFOCUS,    GUI_BLACK );
        LISTVIEW_SetBkColor(            hItem, LISTVIEW_CI_DISABLED,    GUI_BLACK );
        LISTVIEW_SetTextColor(          hItem, LISTVIEW_CI_UNSEL,       GUI_GREEN );
        LISTVIEW_SetTextColor(          hItem, LISTVIEW_CI_SEL,         GUI_GREEN );
        LISTVIEW_SetTextColor(          hItem, LISTVIEW_CI_SELFOCUS,    GUI_GREEN );
        LISTVIEW_SetTextColor(          hItem, LISTVIEW_CI_DISABLED,    GUI_GREEN );
        //LISTVIEW_SetLBorder( hItem, 10);
        //LISTVIEW_SetRBorder( hItem, 10);
        //LISTVIEW_SetColumnWidth(hItem, 0, 50);
        SCROLLBAR_SetDefaultWidth(20);
        

        LISTVIEW_AddColumn(             hItem,  140, "DATE",    GUI_TA_HCENTER | GUI_TA_VCENTER );
        LISTVIEW_AddColumn(             hItem,   80, "TIME",    GUI_TA_HCENTER | GUI_TA_VCENTER );
        LISTVIEW_AddColumn(             hItem,   20, "TYPE",    GUI_TA_HCENTER | GUI_TA_VCENTER );
        LISTVIEW_AddColumn(             hItem,  535, "MESSAGE", GUI_TA_LEFT    | GUI_TA_VCENTER );
        
        complete_listview (hItem);
                
        ////////////////////////////////////////////////////////
        // FOOTER AREA
        ////////////////////////////////////////////////////////
        //hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LEFT );
        //BUTTON_SetFocusable(    hItem, 0                                        );
        //BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CALIBRATE_AL ) );
        //
        //hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RIGHT );
        //BUTTON_SetFocusable(    hItem, 0                                        );
        //BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SETUP_AR )     );
        //
        //hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
        //BUTTON_SetFocusable(    hItem, 1                                        );
        //WM_SetFocus( hItem );
        
        ////////////////////////////////////////////////////////////////////////////
        // FOOTER AREA
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK )         );
        BUTTON_SetFocusable(    hItem, 0                                        );        
}

static
void
update( WM_HWIN         hWin )
{
  WM_HWIN         hItem;
  uint32_t        last_log_index = 0;
  uint32_t        raw_to_add = 0;
  struct tm *     ts;
  time_t          time;  
  log_data_t      log_data;
  int             read_data_result = 0;
  
  hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTVIEW0 );
  
  last_log_index = (dev.log.head - 1) % LOG_INDEX_MAX; 
  
  if (last_log_index != priveous_index )        // Add row
  {
    raw_to_add = (last_log_index > priveous_index) ? (last_log_index - priveous_index) : (LOG_INDEX_MAX - priveous_index + last_log_index);
    priveous_index = last_log_index;
    
    for (int i = 0; i < raw_to_add; i++)
    {
      read_data_result = log_read_event(&log_data, last_log_index);
      if (read_data_result != 0) break;      
      
      time = log_data.timestamp;
      ts = localtime(&time);
      snprintf(data_str, 11, "%04d/%02d/%02d\0", ts->tm_year + 1900, ts->tm_mon,  ts->tm_mday);
      snprintf( time_str, 6, "%02d:%02d\0", ts->tm_hour, ts->tm_min);
      
      if (log_data.event_type == LOG_TYPE_WARNING)
        snprintf(event_type_str, 2, "!");
      else 
        snprintf(event_type_str, 2, "X");
      
      add_event_string(event_message_str, log_data.source, log_data.event);
      
      LISTVIEW_InsertRow( hItem, 0, list_row_data[0] );
      LISTVIEW_DeleteRow( hItem, MAX_ROW_COUNT );
      
      last_log_index = (last_log_index - 1) % LOG_INDEX_MAX;      
    }
    
    
  }
  
  
}

static
void
dialog_callback(                        WM_MESSAGE *            pMsg )
{
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    //scr_switch( UI_DSPL_SCR_IDX_CALIBRATE, GUI_ID_BUTTON_CALIBRATE_HI );
                    //scr_switch(     SCR_IDX_CALIBRATE, GUI_ID_BUTTON_LAUNCH );
                    beep_play( BEEP_SHRT );
                    break;
               
                case GUI_KEY_RIGHT:
                    //scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_MEASURE );
                    beep_play( BEEP_SHRT );
                    break;
                  
                case GUI_KEY_UP:
                    beep_play( BEEP_SHRT );
                    break;      
                    
                case GUI_KEY_DOWN:
                    beep_play( BEEP_SHRT );                  
                    break;      
                    
                case GUI_KEY_ESCAPE:
                    beep_play( BEEP_SHRT ); 
                    scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
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
                    case GUI_ID_BUTTON_LEFT:
                        beep_play( BEEP_SHRT );
                        //scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_LAUNCH );
                        break;

                    case GUI_ID_BUTTON_RIGHT:
                        beep_play( BEEP_SHRT );
                        //scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_MEASURE );
                        break;
                        
                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_SETUP, GUI_ID_BUTTON_SERVICE );
                        break;                        

                    default:
                            break;
                }
            }
            break;
            
        case WM_TIMER:
            WM_RestartTimer( pMsg->Data.v, 0 );
            update( pMsg->hWin );
            break;            

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            WM_SetFocus( WM_GetDialogItem( pMsg->hWin, GUI_ID_BUTTON_DUMMY ) );
            WM_CreateTimer( pMsg->hWin, 0, 2000, 0 );
            break;

        default:
            WM_DefaultProc(pMsg);
            break;
    }
}


/*******************************************************************************
* GLOBAL FUNCTIONS
*******************************************************************************/
/**
  * @brief
  * @param  None
  * @retval None
  */
WM_HWIN
scr_console( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
