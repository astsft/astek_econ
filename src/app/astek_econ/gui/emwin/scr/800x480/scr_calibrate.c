/**
  * @file   scr_setup_system.c
  * @brief  Screen Setup System
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
    { WINDOW_CreateIndirect,    "", 0,                             0, 80, 800, 400, 0, 0x0, 0 },

#if defined(USE_REMOTE_CALIBRATION)      
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_AUTO,      100,  64, 600,  64, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MANUAL,    100, 192, 600,  64, 0, 0x0, 0 },    
#else    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MANUAL,    100, 128, 600,  64, 0, 0x0, 0 },
#endif     
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LEFT,        0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RIGHT,     400, 320, 400,  80, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                         0, 100, 1024, 500, 0, 0x0, 0 },

#if defined(USE_REMOTE_CALIBRATION)     
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_AUTO,      128,  80,  768,  80, 0, 0x0, 0 },  
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MANUAL,    128, 240,  768,  80, 0, 0x0, 0 },    
#else
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_MANUAL,    128, 160,  768,  80, 0, 0x0, 0 },    
#endif    

    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_LEFT,         0, 400, 512,  100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_RIGHT,      512, 400, 512,  100, 0, 0x0, 0 },
};
#endif


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    init_dialog(                               WM_HWIN         hWin )
{
        WM_HWIN         hItem;

        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
#if defined(USE_REMOTE_CALIBRATION)         
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_AUTO );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_AUTO )     );
        BUTTON_SetFocusable(    hItem, 1                                        );
#endif  
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_MANUAL );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MANUAL )     );
        BUTTON_SetFocusable(    hItem, 1                                        );
       

        ////////////////////////////////////////////////////////
        // FOOTER AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_LEFT );
        BUTTON_SetFocusable(    hItem, 0 );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE_AL )   );
    
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_RIGHT );
        BUTTON_SetFocusable(    hItem, 0 );
#if defined(USE_VALIDATION)
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_VALIDATION_AR )   );
#else
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CONSOLE_AR )   );
#endif
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
                                        beep_play( BEEP_SHRT );
                                        GUI_StoreKeyMsg( GUI_KEY_BACKTAB, 1 );                                        
                                        break;

                                case GUI_KEY_DOWN:
                                        beep_play( BEEP_SHRT );
                                        GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                                        break;
                                        
                                case GUI_KEY_LEFT:
                                    scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                                    beep_play( BEEP_SHRT );
                                    break;                                        

                                case GUI_KEY_RIGHT:
#if defined(USE_VALIDATION)                  
                                scr_switch( SCR_IDX_VALIDATION, GUI_ID_BUTTON_MEASURE );
#else
                                scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_DUMMY );                    
#endif                    
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
                                int Id = WM_GetId(pMsg->hWinSrc);
                                switch( Id )
                                {
                                        case GUI_ID_BUTTON_AUTO:
                                                dev.gui.scr_idx    = SCR_IDX_CALIBRATE_AUTO;
                                                scr_switch( SCR_IDX_CALIBRATE_AUTO, GUI_ID_BUTTON_DUMMY );
                                                beep_play( BEEP_SHRT );
                                                break;

                                        case GUI_ID_BUTTON_MANUAL:
                                                dev.gui.scr_idx    = SCR_IDX_CALIBRATE_MANUAL;
                                                scr_switch( SCR_IDX_CALIBRATE_MANUAL, GUI_ID_BUTTON_DUMMY );
                                                beep_play( BEEP_SHRT );
                                                break;                                                
                                                                                                
                                        case GUI_ID_BUTTON_LEFT:
                                            scr_switch( SCR_IDX_MEASURE, GUI_ID_BUTTON_DUMMY );
                                            beep_play( BEEP_SHRT );                  
                                            break;
                                        
                                        case GUI_ID_BUTTON_RIGHT:
                    #if defined(USE_VALIDATION)                      
                                            scr_switch( SCR_IDX_VALIDATION, GUI_ID_BUTTON_MEASURE );
                    #else
                                            scr_switch( SCR_IDX_CONSOLE, GUI_ID_BUTTON_MEASURE );
                    #endif                        
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
scr_calibrate( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
