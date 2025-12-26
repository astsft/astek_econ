/**
  * @file   scr_setup.c
  * @brief  Screen Setup
  * @author Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480 
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                   0,  80, 800, 400, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_VALUE,                  100,  10,  300,  50, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DEVIATION,              100,  72,  300,  50, 0, 0x0, 0 },     
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_VALUE,                450,  10,  250,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DEVIATION,            450,  72,  250,  50, 0, 0x0, 0 },  
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TRANSITION_TIME,        100, 134, 300,  50, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEASURE_TIME,           100, 196, 300,  50, 0, 0x0, 0 },        
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_RETURN_TIME,            100, 258, 300,  50, 0, 0x0, 0 },        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_TRANSITION_TIME,      450, 134, 250,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MEASURE_TIME,         450, 196, 250,  50, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RETURN_TIME,          450, 258, 250,  50, 0, 0x0, 0 },         
    
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_BACK,                 0, 320, 800,  80, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                                    0, 100,1024, 500, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_VALUE,                  128,  12, 384,  62, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_DEVIATION,              128,  90, 384,  62, 0, 0x0, 0 },     
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_VALUE,                576,  12, 320,  62, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DEVIATION,            576,  90, 320,  62, 0, 0x0, 0 },  
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_TRANSITION_TIME,        128, 167, 384,  62, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEASURE_TIME,           128, 245, 384,  62, 0, 0x0, 0 },        
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_RETURN_TIME,            128, 322, 384,  62, 0, 0x0, 0 },        
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_TRANSITION_TIME,      576, 167, 320,  62, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MEASURE_TIME,         576, 245, 320,  62, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RETURN_TIME,          576, 322, 320,  62, 0, 0x0, 0 },         
    
    
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
    int32_t         ppb      = dev.validation.value.ppb;
    int32_t         dev_ppb  = dev.validation.deviation.ppb;
    char            str[ 64];

    ////////////////////////////////////////////////////////
    // MIDDLE AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_VALUE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );

    snprintf(str, sizeof(str), "%s, %", l10n_str_get( dev.cfg.lang, L10N_STR_ID_VALUE )); 
    TEXT_SetText(           hItem, str );      
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVIATION );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );    
    
    snprintf(str, sizeof(str), "%s, %", l10n_str_get( dev.cfg.lang, L10N_STR_ID_DEVIATION )); 
    TEXT_SetText(           hItem, str ); 

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_VALUE );
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
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_TRANSITION_TIME );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    if (dev.cfg.lang)
      snprintf(str, sizeof(str), "%s, с", l10n_str_get( dev.cfg.lang, L10N_STR_ID_TRANSITION_TIME ));       
    else
      snprintf(str, sizeof(str), "%s, s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_TRANSITION_TIME )); 
    TEXT_SetText(           hItem,  str );          
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEASURE_TIME );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );
    if (dev.cfg.lang)
      snprintf(str, sizeof(str), "%s, с", l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE_TIME ));       
    else
      snprintf(str, sizeof(str), "%s, s", l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE_TIME )); 
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
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_TRANSITION_TIME );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    snprintf( str, sizeof(str), "%u", dev.validation.timings.transition_time);   
    BUTTON_SetText(         hItem, str );      
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_MEASURE_TIME );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    snprintf( str, sizeof(str), "%u", dev.validation.timings.measure_time);
    BUTTON_SetText(         hItem, str );     
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RETURN_TIME );
    BUTTON_SetFont(         hItem, &GUI_FontTahoma30                        );
    BUTTON_SetTextAlign(    hItem, GUI_TA_HCENTER | GUI_TA_VCENTER          );
    BUTTON_SetFocusable(    hItem, 1                                        );
    snprintf( str, sizeof(str), "%u", dev.validation.timings.return_time);
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
                    scr_switch( SCR_IDX_VALIDATION, GUI_ID_BUTTON_DUMMY );
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
                    case GUI_ID_BUTTON_VALUE:
                        dev.gui.scr_idx = SCR_IDX_VALIDATION_SETUP_VALUE;
                        scr_switch( SCR_IDX_VALIDATION_SETUP_VALUE, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_DEVIATION:
                        dev.gui.scr_idx = SCR_IDX_VALIDATION_SETUP_DEVIATION;
                        scr_switch( SCR_IDX_VALIDATION_SETUP_DEVIATION, GUI_ID_BUTTON_DUMMY);                      
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_TRANSITION_TIME:
                        dev.gui.scr_idx = SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME;
                        scr_switch( SCR_IDX_VALIDATION_SETUP_TRANSITION_TIME, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_MEASURE_TIME:
                        dev.gui.scr_idx = SCR_IDX_VALIDATION_SETUP_MEASURE_TIME;
                        scr_switch( SCR_IDX_VALIDATION_SETUP_MEASURE_TIME, GUI_ID_BUTTON_DUMMY);
                        beep_play( BEEP_SHRT );
                        break;
                    case GUI_ID_BUTTON_RETURN_TIME:
                        dev.gui.scr_idx = SCR_IDX_VALIDATION_SETUP_RETURN_TIME;
                        scr_switch( SCR_IDX_VALIDATION_SETUP_RETURN_TIME, GUI_ID_BUTTON_DUMMY);                      
                        beep_play( BEEP_SHRT );
                        break;                        
                    case GUI_ID_BUTTON_BACK:
                        beep_play( BEEP_SHRT );
                        scr_switch( SCR_IDX_VALIDATION, GUI_ID_BUTTON_SETUP);
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
scr_validation_setup( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
