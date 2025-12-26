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

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_REMOTELY,               100,  10, 300,  50, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_ZERO,                   100,  72, 300,  50, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SPAN,                   100, 134, 300,  50, 0, 0x0, 0 },         
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DEVIATION,              100, 196, 300,  50, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SLOPE,                  100, 258, 300,  50, 0, 0x0, 0 },     
        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_REMOTELY,             450,  10, 250, 50, 0, 0x0, 0 },   
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ZERO,                 450,  72, 250, 50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SPAN,                 450, 134, 250, 50, 0, 0x0, 0 }, 
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DEVIATION,            450, 196, 250, 50, 0, 0x0, 0 },      
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SLOPE,                450, 258, 250, 50, 0, 0x0, 0 },      
        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,                 0, 320, 800,  80, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                    0, 100, 1024, 500, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_REMOTELY,               128,  12, 384,  62, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_ZERO,                   128,  90, 384,  62, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SPAN,                   128, 167, 384,  62, 0, 0x0, 0 },  
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DEVIATION,              128, 245, 384,  62, 0, 0x0, 0 },     
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_SLOPE,                  128, 322, 384,  62, 0, 0x0, 0 },     
        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_REMOTELY,             576,  12, 320, 62, 0, 0x0, 0 },   
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ZERO,                 576,  90, 320, 62, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SPAN,                 576, 167, 320, 62, 0, 0x0, 0 },    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DEVIATION,            576, 245, 320, 62, 0, 0x0, 0 },     
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_SLOPE,                576, 322, 320, 62, 0, 0x0, 0 },      
    
    
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
    int32_t         ppb      = dev.calibration.zero.ppb;
    int32_t         dev_ppb  = dev.calibration.deviation.ppb;

    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_REMOTELY );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, dev.calibration.remote_calibration_enabled ? GUI_GREEN : GUI_RED );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, dev.calibration.remote_calibration_enabled ? L10N_STR_ID_ALLOWED : L10N_STR_ID_PROHIBITED ) );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_ZERO );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );

    snprintf(str, sizeof(str), "%s, %", l10n_str_get( dev.cfg.lang, L10N_STR_ID_ZERO )); 
    TEXT_SetText(           hItem, str );      
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SPAN );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );

    snprintf(str, sizeof(str), "%s, %", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SPAN )); 
    TEXT_SetText(           hItem, str );  
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVIATION );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );    

    snprintf(str, sizeof(str), "%s, %", l10n_str_get( dev.cfg.lang, L10N_STR_ID_DEVIATION )); 
    TEXT_SetText(           hItem, str );     
   
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE );
    snprintf(str, sizeof(str), "%s, RAW", l10n_str_get( dev.cfg.lang, L10N_STR_ID_SLOPE ));
    TEXT_SetText(           hItem, str );          
       
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SLOPE );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );    
    snprintf( str, sizeof(str), "%d", dev.calibration.max_slope); 
    BUTTON_SetText(         hItem, str );     
        
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_REMOTELY );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_REMOTELY ));
    BUTTON_SetFocusable(    hItem, 1                                        );  

    ppb      = dev.calibration.zero.ppb;
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ZERO );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );    

    snprintf( str, sizeof(str), "%2d.%02u", ppb / (1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 100000 );
    BUTTON_SetText(         hItem, str );     
    
    ppb      = dev.calibration.span.ppb;
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_SPAN );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );    

    snprintf( str, sizeof(str), "%2d.%02u", ppb / (1000 * 10000), ( abs(ppb) % (1000 * 10000)) / 100000 );
    BUTTON_SetText(         hItem, str );        
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DEVIATION );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );    

    snprintf( str, sizeof(str), "%2d.%04u", dev_ppb / (1000 * 10000), ( abs(dev_ppb) % (1000 * 10000)) / 1000 );
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
                    scr_switch( SCR_IDX_CALIBRATE_AUTO, GUI_ID_BUTTON_SETTINGS );
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
                    case GUI_ID_BUTTON_REMOTELY:
                        dev.calibration.remote_calibration_enabled = dev.calibration.remote_calibration_enabled ? false : true;
                        hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_REMOTELY );
                        TEXT_SetText( hItem, l10n_str_get( dev.cfg.lang, dev.calibration.remote_calibration_enabled ? L10N_STR_ID_ALLOWED : L10N_STR_ID_PROHIBITED ) );
                        TEXT_SetTextColor( hItem, dev.calibration.remote_calibration_enabled ? GUI_GREEN : GUI_RED );
                        send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_REMOTE_ENABLE, dev.calibration.remote_calibration_enabled);
                        beep_play( BEEP_SHRT );
                        break;
                        
                    case GUI_ID_BUTTON_ZERO:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_ZERO;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_ZERO, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );                      
                        break;
                        
                    case GUI_ID_BUTTON_SPAN:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_SPAN;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_SPAN, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );                       
                        break;   
                        
                    case GUI_ID_BUTTON_DEVIATION:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_DEVIATION;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_DEVIATION, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );                       
                        break;                           
                        
                    case GUI_ID_BUTTON_SLOPE:
                        dev.gui.scr_idx = SCR_IDX_CALIBRATION_SETUP_SLOPE;
                        scr_switch( SCR_IDX_CALIBRATION_SETUP_SLOPE, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );
                        break;                        
                        
                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_CALIBRATE_AUTO, GUI_ID_BUTTON_SETUP);
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
scr_calibration_setup( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
