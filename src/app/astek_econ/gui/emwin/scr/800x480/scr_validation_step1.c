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
    { WINDOW_CreateIndirect,    "", 0,                           0,  80, 800, 400, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        25,  20,  750,  280, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                 50,  30,  300,   50, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 50,  120, 700,   60, 0, 0x0, 0 },       
    
    
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,          0, 320, 400,  80, 0, 0x0, 0  },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NEXT,          400, 320, 400,  80, 0, 0x0, 0  },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,          -1,  -1,   1,   1, 0, 0x0, 0  },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                             0,  100, 1024,  500, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,        32,   25,  960,  350, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                 64,   38,  384,   62, 0, 0x0, 0 }, 
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                 64,  150,  896,   75, 0, 0x0, 0 },     
    

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 400, 512,  100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_NEXT,         512, 400, 512,  100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1,  0, 0x0, 0 },
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
      TEXT_SetText(           hItem, "ШАГ 1 ИЗ 3"  ); 
    else
      TEXT_SetText(           hItem, "STEP 1 FROM 3" ); 
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER          );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                 );    
    if (dev.cfg.lang)      
      TEXT_SetText(           hItem, "ПОДАЙТЕ ПРОБУ И НАЖМИТЕ ДАЛЕЕ"  ); 
    else
      TEXT_SetText(           hItem, "APPLY THE PROBE AND CLICK NEXT" );     

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
}

static void cancel_pressed (void)
{
  //dev.cloop->cloop_state = CLOOP_NORMAL_WORK;
  //dev.validation.state = FINISH;
  //dev.state.process_status = PROCESS_MEASURE;
  dev.validation.result = VALIDATION_CANCELED;
  //send_cmd_for_validation_stop();
  //scr_switch( SCR_IDX_VALIDATION, GUI_ID_BUTTON_START);   
  scr_switch( SCR_IDX_VALIDATION_STEP3, GUI_ID_BUTTON_DUMMY);     
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
                    scr_switch( SCR_IDX_VALIDATION_STEP2, GUI_ID_BUTTON_DUMMY); 
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
                        scr_switch( SCR_IDX_VALIDATION_STEP2, GUI_ID_BUTTON_DUMMY);                      
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
scr_validation_step1( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
