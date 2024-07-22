/**
  * @file    ui_dspl_scr_threshold_value.c
  * @brief   User Interface - Screen Threshold Point
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "DIALOG.h"
#include "scr\scr.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "main.h"



/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
extern  dev_t           dev;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  100, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                36, 140, 86,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,               208, 140, 86,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               294, 140, 86,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,               380, 140, 86,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,               466, 140, 86,  40, 0, 0x0, 0 },
//    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,               552, 140, 86,  40, 0, 0x0, 0 },
//    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,               552, 140, 86,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_POINT,          375, 140, 10,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_UNITS,          650, 140, 120, 40, 0, 0x0, 0 },    

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,           36, 100, 86, 120, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          208,  60, 86, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          294,  60, 86, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          380,  60, 86, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          466,  60, 86, 200, 0, 0x0, 0 },
//    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL5,          552,  60, 86, 200, 0, 0x0, 0 },
//    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL6,          552,  60, 86, 200, 0, 0x0, 0 },
                                                                
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

typedef enum    listwheel_idx_e
{
        LISTWHEEL_IDX_0         = 0,
        LISTWHEEL_IDX_1,
        LISTWHEEL_IDX_2,
        LISTWHEEL_IDX_3,
        LISTWHEEL_IDX_4,
//        LISTWHEEL_IDX_5,
//        LISTWHEEL_IDX_6,
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

static  listwheel_idx_t         listwheel_idx;
static  WM_HWIN                 hWheel;
static  const   char *          symb[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };
static  const   char *          sign[]  = { "<", ">", };

/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void    init_dialog(                                    WM_HWIN         hWin )
{
        WM_HWIN         hItem;


        WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_POINT );
        TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
        TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | GUI_TA_VCENTER         );
        TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_WHEEL_UNSEL              );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );      
        switch( dev.cfg.display_mode )
        {
            //case DEV_DSPL_MODE_PPM:
            //    WM_BringToBottom( hItem );
            //    break;
            case DEV_DSPL_MODE_PERCENTS:
            default:
                TEXT_SetText(           hItem, "."  );
                WM_BringToTop( hItem );
                break;
        }
        
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_UNITS );
        TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
        TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );            
        switch( dev.cfg.display_mode )
        {
            //case DEV_DSPL_MODE_PPM:
            //    TEXT_SetText(           hItem, "PPM"  );
            //    break;
            case DEV_DSPL_MODE_PERCENTS:
            default:
                TEXT_SetText(           hItem, "%"  );
                WM_BringToTop( hItem );
                break;
        }
 

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


static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
        const   WM_HWIN         hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
        const   WM_HWIN         hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
        const   WM_HWIN         hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
        const   WM_HWIN         hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT3 );
        const   WM_HWIN         hText4  = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );
//        const   WM_HWIN         hText5  = WM_GetDialogItem( hWin, GUI_ID_TEXT5 );
//        const   WM_HWIN         hText6  = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );


        switch( idx )
        {
                case LISTWHEEL_IDX_0:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_1:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_2:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_3:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_4:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL   );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
                        listwheel_idx   = idx;
                        break;

//                case LISTWHEEL_IDX_5:
//                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_SEL   );
//                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );
//                        listwheel_idx   = idx;
//                        break;
//
//                case LISTWHEEL_IDX_6:
//                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_SEL   );
//                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL6 );
//                        listwheel_idx   = idx;
//                        break;

                default:
                        break;
        }
}


static
void    listwheel_swtch_left(                   const   WM_HWIN         hWin )
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
void    listwheel_swtch_rght(                   const   WM_HWIN         hWin )
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


/**
  * @brief
  * @param  None
  * @retval None
  */
static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    WM_HWIN         hItem;
    int             idx;
    float           level;
    thld_type_e     thld_type;
    uint32_t        relay_config = 0;


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
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
                    }

                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_DOWN:
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
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_ESCAPE:
                    if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_THRESHOLD );
                    else scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_THRESHOLD );
                    beep_play( BEEP_SHRT );
                    break;

                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            switch( WM_GetId( pMsg->hWinSrc ) )
            {
                case GUI_ID_LISTWHEEL0:
                if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                {
                    listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );
                    beep_play( BEEP_SHRT );
                }
                break;

                case GUI_ID_LISTWHEEL1:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_1 );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_LISTWHEEL2:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_2 );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_LISTWHEEL3:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_3 );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_LISTWHEEL4:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_4 );
                        beep_play( BEEP_SHRT );
                    }
                    break;

//                case GUI_ID_LISTWHEEL5:
//                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
//                    {
//                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_5 );
//                        beep_play( BEEP_SHRT );
//                    }
//                    break;
//
//                case GUI_ID_LISTWHEEL6:
//                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
//                    {
//                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_6 );
//                        beep_play( BEEP_SHRT );
//                    }
//                    break;

                case GUI_ID_BUTTON_CANCEL:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_THRESHOLD );
                        else scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_THRESHOLD );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_ENTER:
                case GUI_ID_BUTTON_DUMMY:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        thld_type = (thld_type_e) LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                        level  = 0;
                        level  += 100000 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) );
                        level  += 10000  * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ) );
                        level  += 1000   * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ) );
                        level  += 100    * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 ) );
//                        level  += 10     * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL5 ) );
//                        level  += 1      * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL6 ) );                                                                          
                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM)
                        {
                          dev.ext_relay->relay[0].thld_type = thld_type;                          
                          dev.ext_relay->relay[0].ppm.ppm_f = level;                                 
                          dev.ext_relay->relay[0].hyst_ppm.ppm_f = (level * 5) / 100;
                          if (dev.ext_relay->relay[0].ppm.ppm_f == 0 && dev.ext_relay->relay[0].hyst_ppm.ppm_f == 0) dev.ext_relay->relay[0].hyst_ppm.ppm_f = 0.1;
                          convert_flt_to_int_fract(dev.ext_relay->relay[0].ppm.ppm_f, &dev.ext_relay->relay[0].ppm.integral.i32, &dev.ext_relay->relay[0].ppm.fractional.i32);                                                    
                          relay_config =  dev.ext_relay->relay[0].relay_mode << 16;
                          relay_config |= dev.ext_relay->relay[0].relay_state << 8;
                          relay_config |= dev.ext_relay->relay[0].thld_type;
                          send_cmd_for_nvm_write_param(NVM_REG_RELAY1_MODE_STATE_TYPE, relay_config);                           
                          send_cmd_for_nvm_write_param(NVM_REG_RELAY1_THRESHOLD, (uint32_t)dev.ext_relay->relay[0].ppm.ppm_f);
                        }
                        else
                        {
                          dev.ext_relay->relay[1].thld_type = thld_type;                          
                          dev.ext_relay->relay[1].ppm.ppm_f = level;                                 
                          dev.ext_relay->relay[1].hyst_ppm.ppm_f = (level * 5) / 100;
                          if (dev.ext_relay->relay[1].ppm.ppm_f == 0 && dev.ext_relay->relay[1].hyst_ppm.ppm_f == 0) dev.ext_relay->relay[1].hyst_ppm.ppm_f = 0.1;
                          convert_flt_to_int_fract(dev.ext_relay->relay[1].ppm.ppm_f, &dev.ext_relay->relay[1].ppm.integral.i32, &dev.ext_relay->relay[1].ppm.fractional.i32);                                                   
                          relay_config =  dev.ext_relay->relay[1].relay_mode << 16;
                          relay_config |= dev.ext_relay->relay[1].relay_state << 8;
                          relay_config |= dev.ext_relay->relay[1].thld_type;
                          send_cmd_for_nvm_write_param(NVM_REG_RELAY2_MODE_STATE_TYPE, relay_config); 
                          send_cmd_for_nvm_write_param(NVM_REG_RELAY2_THRESHOLD, (uint32_t)dev.ext_relay->relay[1].ppm.ppm_f);
                        }
                                                                       
                        if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM) scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM, GUI_ID_BUTTON_RELAY_THRESHOLD );
                        else scr_switch( SCR_IDX_SETUP_SERVICE_RELAY_2_PARAM, GUI_ID_BUTTON_RELAY_THRESHOLD );
                        beep_play( BEEP_TYPE_CONFIRM );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            
            if (dev.gui.scr_idx == SCR_IDX_SETUP_SERVICE_RELAY_1_PARAM)
            {
              level = dev.ext_relay->relay[0].ppm.ppm_f;
              thld_type = dev.ext_relay->relay[0].thld_type;
            }
            else
            {
              thld_type = dev.ext_relay->relay[1].thld_type;
              level = dev.ext_relay->relay[1].ppm.ppm_f;;           
            }

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, sign, sizeof( sign ) / sizeof( char * ) );
            idx     = thld_type;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, symb, sizeof( symb ) / sizeof( char * ) );
            idx     = ((int32_t)level / 100000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, symb, sizeof( symb ) / sizeof( char * ) );
            idx     = ((int32_t)level /  10000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, symb, sizeof( symb ) / sizeof( char * ) );
            idx     = ((int32_t)level /   1000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 );
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 40, symb, sizeof( symb ) / sizeof( char * ) );
            idx     = ((int32_t)level /    100) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

//            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL5 );
//            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL5, 40, symb, sizeof( symb ) / sizeof( char * ) );
//            idx     = ((int32_t)level /     10) % 10;
//            LISTWHEEL_SetPos( hItem, idx );
//            LISTWHEEL_SetSel( hItem, idx );
//
//            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL6 );
//            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL6, 40, symb, sizeof( symb ) / sizeof( char * ) );
//            idx     = (int32_t)(level /       1) % 10;
//            LISTWHEEL_SetPos( hItem, idx );
//            LISTWHEEL_SetSel( hItem, idx );

            //LISTWHEEL_SetBkColor(   hItem,  LISTWHEEL_CI_UNSEL, GUI_BLACK        );
            //LISTWHEEL_SetBkColor(   hItem,  LISTWHEEL_CI_SEL,   GUI_GRAY_2F      );

            listwheel_idx   = LISTWHEEL_IDX_0;
            listwheel_set_focus( pMsg->hWin, listwheel_idx );

            hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 );
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
scr_setup_service_relay_param_threshold( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
