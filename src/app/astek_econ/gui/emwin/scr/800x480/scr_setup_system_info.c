/**
  * @file    scr_setup_system_info.c
  * @brief   User Interface - Screen Setup Service Diagnostics Info
  * @author  nkp144
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "version.h"

extern  dev_t           dev;


/*******************************************************************************
* PRIVATE VARIBLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "",     0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",     GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "DEVICE: ",     GUI_ID_TEXT0,                50,  40, 125,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_DEVICE_ID,      175,  40, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "SN: ",         GUI_ID_TEXT1,               275,  40,  50,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_DEVICE_SERNUM,  335,  40, 150,  40, 0, 0x0, 0 },   
    { TEXT_CreateIndirect,      "TAG: ",        GUI_ID_TEXT10,              485,  40,  60,  40, 0, 0x0, 0 },       
    { BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TAG,          545,  40, 205,  40, 0, 0x0, 0 },            
    { TEXT_CreateIndirect,      "HW: ",         GUI_ID_TEXT2,                50,  80, 125,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_DEVICE_HW,      175,  80, 150,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "UID: ",        GUI_ID_TEXT3,               275,  80,  60,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_DEVICE_UID,     335,  80, 350,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "FW: ",         GUI_ID_TEXT4,                50, 120, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_DEVICE_FW,     175, 120, 475,  40, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "SENSOR: ",     GUI_ID_TEXT5,                50, 160, 125,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_SENSOR_ID,      175, 160, 150,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "HW: ",         GUI_ID_TEXT7,                50,  200, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_SENSOR_HW,      175,  200, 150,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "UID: ",        GUI_ID_TEXT8,               275,  200,  50,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_SENSOR_UID,     335,  200, 350,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "FW: ",         GUI_ID_TEXT9,                50, 240, 100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "",             GUI_ID_TEXT_SENSOR_FW,      175, 240, 475,  40, 0, 0x0, 0 },    
         
    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON_BACK,           0, 320, 800,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "",     GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    init_dialog(                                    WM_HWIN         hWin )
{
    WM_HWIN         hItem;
    char            str[64];

    // MIDDLE AREA
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, GUI_BLACK                                );
    
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT0 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT1 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT2 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT3 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT4 ) );
    
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT5 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT7 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT8 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT9 ) );
    scr_init_text_label( WM_GetDialogItem( hWin, GUI_ID_TEXT10 ) );
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVICE_ID );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
    snprintf(   str, sizeof( str ), "%s", "ECON");    
    TEXT_SetText(           hItem, str);
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVICE_SERNUM );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
    snprintf(   str, sizeof( str ), "%lu", dev.info.sn);
    TEXT_SetText(           hItem, str);
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_TAG );
    BUTTON_SetText(         hItem, dev.info.tag->c_tag);
    BUTTON_SetFocusable(    hItem, 1                                        );   
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVICE_HW  );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );    
    TEXT_SetText( hItem, dev.info.hardware_str);
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVICE_FW   );    
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );        
    snprintf(str, sizeof( str ), "%s_%s", built_date_time, firmware_commit);
    TEXT_SetText( hItem, str);    
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_DEVICE_UID    );    
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );            
    snprintf(   str,
                sizeof( str ),
                "%08X %08X %08X",
                *dev.info.uid_u32[ 2],
                *dev.info.uid_u32[ 1],
                *dev.info.uid_u32[ 0]
                  );
    TEXT_SetText( hItem, str );    
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_SENSOR_ID );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
    snprintf(   str, sizeof( str ), "%s", "ECON");    
    TEXT_SetText(           hItem, str);

    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_SENSOR_HW  );
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );    
    snprintf(   str, sizeof( str ), "%s", "ECON");    
    TEXT_SetText( hItem, str);
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_SENSOR_FW   );    
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
    snprintf(str, sizeof( str ), "%s", "ECON");
    TEXT_SetText( hItem, str);    
    
    hItem = WM_GetDialogItem( hWin, GUI_ID_TEXT_SENSOR_UID    );    
    TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
    TEXT_SetTextAlign(      hItem, GUI_TA_LEFT | TEXT_CF_VCENTER            );
    TEXT_SetBkColor(        hItem, GUI_TRANSPARENT                          );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );   
    
    TEXT_SetText( hItem, "" );      

    ////////////////////////////////////////////////////////////////////////////
    // FOOTER AREA
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1                                        );    
    
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_BACK );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_BACK )         );
    BUTTON_SetFocusable(    hItem, 0                                        );
}


/**
  * @brief
  * @param  None
  * @retval None
  */
static
void
dialog_callback(                                        WM_MESSAGE *    pMsg )
{
    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_ESCAPE:
                    beep_play( BEEP_SHRT );
                    scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_INFO );
                    break;

                case GUI_KEY_UP:
                case GUI_KEY_DOWN:
                case GUI_KEY_LEFT:
                case GUI_KEY_RIGHT:
                    GUI_StoreKeyMsg( GUI_KEY_TAB, 1 );
                    beep_play( BEEP_SHRT );
                    break;
                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
                {
                    case GUI_ID_BUTTON_TAG:
                        scr_switch( SCR_IDX_SETUP_SYSTEM_INFO_TAG, GUI_ID_BUTTON_DUMMY );
                        beep_play( BEEP_SHRT );
                        break;                  
                  
                    case GUI_ID_BUTTON_BACK:
                        scr_switch( SCR_IDX_SETUP_SYSTEM, GUI_ID_BUTTON_INFO );
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
scr_setup_system_info( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
