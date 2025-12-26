/**
  * @file    scr_setup_service_entry.c
  * @brief   Screen Setup Calibrate
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"

extern  dev_t           dev;


/*******************************************************************************
* PRIVATE TYPES
*******************************************************************************/
typedef enum    listwheel_idx_e
{
        LISTWHEEL_IDX_0         = 0,
        LISTWHEEL_IDX_1,
        LISTWHEEL_IDX_2,
        LISTWHEEL_IDX_3,
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,         200, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         300, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,         400, 140, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,         500, 140, 100,  40, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          200,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          300,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          400,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          500,  60, 100, 200, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_UNITS,          650, 100, 120,  80, 0, 0x0, 0 },  
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                50, 260, 725,  32, 0, 0x0, 0 }, 
    

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0, 100,1024, 500, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       32,  25, 960, 350, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,         256, 175, 128,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         384, 175, 128,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,         512, 175, 128,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,         640, 175, 128,  50, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          256,  75, 128, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          384,  75, 128, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          512,  75, 128, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          640,  75, 128, 250, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_UNITS,          832, 125, 153, 100, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                50, 300, 600,  40, 0, 0x0, 0 },     

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        512, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#endif

//static  const   char *  list[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };
static const char* list[] = {
    "9", "8", "7", "6", "5", "4", "3", "2", "1", "0"
};
static  const   size_t  list_countof    = sizeof( list ) / sizeof( char * );

static  listwheel_idx_t listwheel_idx;
static  WM_HWIN         hWheel;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
    const   WM_HWIN     hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS0 );
    const   WM_HWIN     hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS1 );
    const   WM_HWIN     hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS2 );
    const   WM_HWIN     hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS3 );

    WM_BringToBottom(  WM_GetDialogItem( hWin, GUI_ID_TEXT0 ) ); 

    switch( idx )
    {
        case LISTWHEEL_IDX_0:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_2:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_3:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
            hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
            listwheel_idx   = idx;
            break;

        default:
            break;
    }
}

static
void
listwheel_swtch_left(                   const   WM_HWIN         hWin )
{
    if( listwheel_idx > 0 )
    {
        listwheel_idx--;
    }
    else
    {
        listwheel_idx   = (listwheel_idx_t) (LISTWHEEL_IDX_LAST - 1);
    }

    listwheel_set_focus( hWin, listwheel_idx );
}


static
void
listwheel_swtch_rght(                   const   WM_HWIN         hWin )
{
    if( listwheel_idx < (LISTWHEEL_IDX_LAST - 1) )
    {
        listwheel_idx++;
    }
    else
    {
        listwheel_idx   = LISTWHEEL_IDX_0;
    }

    listwheel_set_focus( hWin, listwheel_idx );
}

static
void
init_listwheel(                                         WM_HWIN         hWin )
{
        hWheel  = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
}


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
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_UNITS );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
    if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
    {
      TEXT_SetText( hItem, "RAW" );
    }
    else
    {
      if (dev.cfg.lang)
      {
              TEXT_SetText( hItem, "с"  );
      }
      else
      {
              TEXT_SetText( hItem, "s"  );
      }    
    }
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_RED                                 );
    if (dev.cfg.lang)   
      TEXT_SetText( hItem, "СТАБИЛИЗАЦИЯ >= ИЗМЕРЕНИЕ");
    else
      TEXT_SetText( hItem, "STABLE >= MEASURE");  
    WM_BringToBottom( hItem );  

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
    BUTTON_SetFocusable(    hItem, 0                                        );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
    BUTTON_SetFocusable(    hItem, 0                                        );
}

static void check_measure_time (WM_HWIN         hWin)
{
  uint32_t sec = 0;;  
  uint8_t idx;
  
  sec += 1000 * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 )));
  sec += 100  * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 )));
  sec += 10   * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 )));
  sec += 1    * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 )));    
  if (sec > 600)
  {
    sec = 600;
    
    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), (list_countof - 1) );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), (list_countof - 1) );    
    
    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), (list_countof - 1) -  6 );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), (list_countof - 1) - 6 );        
    
    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), (list_countof - 1) );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), (list_countof - 1) );            
    
    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), (list_countof - 1) );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), (list_countof - 1) );                
  }
  else if (sec < 12)
  {
    sec = 12;

    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), (list_countof - 1) );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0), (list_countof - 1) );    
    
    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), (list_countof - 1) );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1), (list_countof - 1) );        
    
    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), (list_countof - 1) - 1 );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2), (list_countof - 1) - 1 );            
    
    LISTWHEEL_SetPos(  WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), (list_countof - 1) - 2 );
    LISTWHEEL_SetSel( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3), (list_countof - 1) - 2 );    
    
  }
  
}

static
void
dialog_callback(                                        WM_MESSAGE *    pMsg )
{
    uint32_t sec = 0;
    int             idx;
    scr_idx_t       scr_idx;
    int button = 0;

    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_LEFT:
                    listwheel_swtch_left( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_RIGHT:
                    listwheel_swtch_rght( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_UP:
                    beep_play( BEEP_SHRT );
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( idx > 0 )
                    {
                        idx--;
                    }
                    else
                    {
                        idx     = LISTWHEEL_GetNumItems( hWheel ) - 1;
                    }
                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    WM_BringToBottom(  WM_GetDialogItem(  pMsg->hWin, GUI_ID_TEXT0 ) ); 
                    break;

                case GUI_KEY_DOWN:
                    beep_play( BEEP_SHRT );
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
                    }
                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    WM_BringToBottom(  WM_GetDialogItem(  pMsg->hWin, GUI_ID_TEXT0 ) ); 
                    break;

                case GUI_KEY_ESCAPE:
                    if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME || dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_TRANSITION_TIME)
                    {
                      button = GUI_ID_BUTTON_TRANSITION_TIME;                      
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME || dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_MEASURE_TIME)
                    {
                      button = GUI_ID_BUTTON_MEASURE_TIME;
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_RETURN_TIME || dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_RETURN_TIME)
                    {
                      button = GUI_ID_BUTTON_RETURN_TIME;
                    }         
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
                    {
                      button = GUI_ID_BUTTON_SLOPE;
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE_TIME)
                    {
                      button = GUI_ID_BUTTON_STABLE_TIME;
                    }
                      
                    if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME || dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME || dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_RETURN_TIME)
                        scr_idx = SCR_IDX_VALIDATION_SETUP;
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
                        scr_idx = SCR_IDX_CALIBRATION_SETUP;
                    else
                        scr_idx = SCR_IDX_CALIBRATION_AUTO_CYCLE_TIME;
                    
                    scr_switch( scr_idx, button );
                    beep_play( BEEP_SHRT );
                    break;

                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
                {
                    case GUI_ID_LISTWHEEL0:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );                     
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_LISTWHEEL1:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_1 );
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_LISTWHEEL2:                      
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_2 );                     
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_LISTWHEEL3:
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_3 );                    
                        beep_play( BEEP_SHRT );
                        break;
                    default:
                        break;
                }
            }

            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId(pMsg->hWinSrc) )
                {
                    case GUI_ID_BUTTON_CANCEL:
                        if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME || dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_TRANSITION_TIME)
                        {
                          button = GUI_ID_BUTTON_TRANSITION_TIME;
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME || dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_MEASURE_TIME)
                        {
                          button = GUI_ID_BUTTON_MEASURE_TIME;
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_RETURN_TIME || dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_RETURN_TIME)
                        {
                          button = GUI_ID_BUTTON_RETURN_TIME;
                        }        
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
                        {
                          button = GUI_ID_BUTTON_SLOPE;
                        }        
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE_TIME)
                        {
                          button = GUI_ID_BUTTON_STABLE_TIME;
                        }
                        if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME || dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME || dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_RETURN_TIME)
                            scr_idx = SCR_IDX_VALIDATION_SETUP;
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
                            scr_idx = SCR_IDX_CALIBRATION_SETUP;                        
                        else
                            scr_idx = SCR_IDX_CALIBRATION_AUTO_CYCLE_TIME;                        
                        scr_switch( scr_idx, button );
                        beep_play( BEEP_SHRT );
                        break;

                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
                        beep_play( BEEP_SHRT );
                        sec  = 0;
                        sec += 1000 * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 )));
                        sec += 100  * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 )));
                        sec += 10   * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 )));
                        sec += 1    * ((list_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 )));
                        if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME)
                        {
                          dev.validation.timings.transition_time = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_VALIDATION_TRANSITION_TIME, dev.validation.timings.transition_time); 
                          button = GUI_ID_BUTTON_TRANSITION_TIME;
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME)
                        {
                          dev.validation.timings.measure_time = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_VALIDATION_MEASURE_TIME, dev.validation.timings.measure_time);
                          button = GUI_ID_BUTTON_MEASURE_TIME;
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_RETURN_TIME)
                        {
                          dev.validation.timings.return_time = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_VALIDATION_RETURN_TIME, dev.validation.timings.return_time);
                          button = GUI_ID_BUTTON_RETURN_TIME;
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_TRANSITION_TIME)
                        {
                          dev.calibration.timings.transition_time = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_TRANSITION_TIME, dev.calibration.timings.transition_time); 
                          button = GUI_ID_BUTTON_TRANSITION_TIME;                          
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_MEASURE_TIME)
                        {
                          dev.calibration.timings.measure_time = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_MEASURE_TIME, dev.calibration.timings.measure_time);
                          button = GUI_ID_BUTTON_MEASURE_TIME;                          
                        }                        
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_RETURN_TIME)
                        {
                          dev.calibration.timings.return_time = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_RETURN_TIME, dev.calibration.timings.return_time);
                          button = GUI_ID_BUTTON_RETURN_TIME;                          
                        }       
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
                        {
                          dev.calibration.max_slope = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_SLOPE, dev.calibration.max_slope);
                          button = GUI_ID_BUTTON_SLOPE;
                        }    
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE_TIME)
                        {
                          if (sec >= dev.calibration.timings.measure_time)
                          {
                            WM_BringToTop( WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT0 ) ); 
                            break;
                          }
                          dev.calibration.timings.stable_slope_time = sec;
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_SLOPE_TIME, dev.calibration.timings.stable_slope_time);                          
                          button = GUI_ID_BUTTON_STABLE_TIME;
                        }                        
                        
                        if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME || dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME || dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_RETURN_TIME)
                            scr_idx = SCR_IDX_VALIDATION_SETUP;
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
                            scr_idx = SCR_IDX_CALIBRATION_SETUP;
                        else
                            scr_idx = SCR_IDX_CALIBRATION_AUTO_CYCLE_TIME;
                    
                        scr_switch( scr_idx, button );
                        
                        break;

                    default:
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
          
            if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME)
            {
              sec = dev.validation.timings.transition_time;
            }
            else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME)
            {
              sec = dev.validation.timings.measure_time;
            }
            else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_RETURN_TIME)
            {
              sec = dev.validation.timings.return_time;
            }         
            else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_TRANSITION_TIME)
            {
              sec = dev.calibration.timings.transition_time;
            }
            else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_MEASURE_TIME)
            {
              sec = dev.calibration.timings.measure_time;              
            }
            else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_RETURN_TIME)
            {
              sec = dev.calibration.timings.return_time;              
            }
            else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SLOPE)
            {
              sec = dev.calibration.max_slope;              
            }           
            else if (SCR_IDX_CALIBRATION_SETUP_SLOPE_TIME)
            {
              sec = dev.calibration.timings.stable_slope_time; 
            }
            
            init_dialog( pMsg->hWin );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, list, list_countof );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 50, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 50, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 50, list, list_countof );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 50, list, list_countof );            
#endif            
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ), (list_countof - 1) - (sec / 1000));
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ), (list_countof - 1) - (sec / 1000));
            
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ),  (list_countof - 1) - (sec / 100) % 10);
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ),  (list_countof - 1) - (sec / 100) % 10);            
            
            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ), (list_countof - 1) - (sec / 10) % 10);
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ), (list_countof - 1) - (sec / 10) % 10);            

            LISTWHEEL_SetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ), (list_countof - 1) - (sec / 1) % 10);
            LISTWHEEL_SetSel( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ), (list_countof - 1) - (sec / 1) % 10);            
            
            listwheel_idx   = LISTWHEEL_IDX_0;
            init_listwheel( pMsg->hWin );
            listwheel_set_focus( pMsg->hWin, listwheel_idx );
            break;

        default:
            WM_DefaultProc(pMsg);
            break;
    }
    
    if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_MEASURE_TIME)
    {                
     check_measure_time(pMsg->hWin);
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
scr_validation_setup_time( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
