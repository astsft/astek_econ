/**
  * @file   scr_setup.c
  * @brief  Screen Setup
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"

extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480 
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                   0,  80, 800, 400, 0, 0x0, 0 },    
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_PURGE_TIME,             100,  25, 300,  50, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEASURE_TIME,           100,  98, 300,  50, 0, 0x0, 0 },        
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_STABLE_TIME,            100, 171, 300,  50, 0, 0x0, 0 },        
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_RETURN_TIME,            100, 244, 300,  50, 0, 0x0, 0 },            
         
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_PURGE_TIME,           450,  25, 250,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MEASURE_TIME,         450,  98, 250,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_STABLE_TIME,          450, 171, 250,  50, 0, 0x0, 0 },         
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RETURN_TIME,          450, 244, 250,  50, 0, 0x0, 0 },             
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,                 0, 320, 800,  80, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                   0, 100,  1024, 500, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_PURGE_TIME,             128,   31,  384,  62, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEASURE_TIME,           128,  123,  384,  62, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_STABLE_TIME,            128,  214,  384,  62, 0, 0x0, 0 },        
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_RETURN_TIME,            128,  305,  384,  62, 0, 0x0, 0 },        
                                                                        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_PURGE_TIME,           576,   31,  384,  62, 0, 0x0, 0 },  
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MEASURE_TIME,         576,  123,  384,  62, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_STABLE_TIME,          576,  214,  384,  62, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RETURN_TIME,          576,  305,  384,  62, 0, 0x0, 0 },         
    
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,                 0, 400, 1024,  100, 0, 0x0, 0 },
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

    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_PURGE_TIME );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    if (dev.cfg.lang)
      snprintf(str, sizeof(str), "%s, с", l10n_str_get( dev.cfg.lang, L10N_STR_ID_PURGE ));       
    else
      snprintf(str, sizeof(str), "%s, s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_PURGE )); 
    TEXT_SetText(           hItem,  str );          
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEASURE_TIME );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    if (dev.cfg.lang)
      snprintf(str, sizeof(str), "%s, с", l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE ));       
    else
      snprintf(str, sizeof(str), "%s, s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE )); 
    TEXT_SetText(           hItem,  str );          
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_STABLE_TIME );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    if (dev.cfg.lang)
      snprintf(str, sizeof(str), "%s, с", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SLOPE_TIME ));       
    else
      snprintf(str, sizeof(str), "%s, s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SLOPE_TIME )); 
    TEXT_SetText(           hItem,  str );      
          
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_RETURN_TIME );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    if (dev.cfg.lang)
      snprintf(str, sizeof(str), "%s, с", l10n_str_get( dev.cfg.lang, L10N_STR_ID_RETURN_TIME ));       
    else
      snprintf(str, sizeof(str), "%s, s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_RETURN_TIME )); 
    TEXT_SetText(           hItem,  str );       
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_PURGE_TIME );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    snprintf( str, sizeof(str), "%u", dev.calibration.timings.transition_time);   
    BUTTON_SetText(         hItem, str );      
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_MEASURE_TIME );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    snprintf( str, sizeof(str), "%u", dev.calibration.timings.measure_time);
    BUTTON_SetText(         hItem, str );     
        
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_STABLE_TIME );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    snprintf( str, sizeof(str), "%u", dev.calibration.timings.stable_slope_time);
    BUTTON_SetText(         hItem, str );     
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RETURN_TIME );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    snprintf( str, sizeof(str), "%u", dev.calibration.timings.return_time);
    BUTTON_SetText(         hItem, str );      

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK )         );
    BUTTON_SetFocusable(    hItem, 0                                        );
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
                    GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );                 
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );              
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    scr_switch( SCR_IDX_CALIBRATE_AUTO, GUI_ID_BUTTON_CYCLE_TIME );
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
                    case GUI_ID_BUTTON_PURGE_TIME:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_TRANSITION_TIME;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_TRANSITION_TIME, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_BUTTON_MEASURE_TIME:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_MEASURE_TIME;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_MEASURE_TIME, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_BUTTON_RETURN_TIME:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_RETURN_TIME;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_RETURN_TIME, GUI_ID_BUTTON_DUMMY);                      
                        beep_play( BEEP_SHRT );
                        break;   
                        
                    case GUI_ID_BUTTON_STABLE_TIME:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_SLOPE_TIME;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_SLOPE_TIME, GUI_ID_BUTTON_DUMMY);                      
                        beep_play( BEEP_SHRT );
                        break;                         
                        
                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_CALIBRATE_AUTO, GUI_ID_BUTTON_CYCLE_TIME);
                        break;
                        
                    default:
                        beep_play( BEEP_TYPE_ERROR );
                        break;
                }
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
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
scr_calibrate_auto_cycle_time( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
