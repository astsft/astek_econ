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
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  100, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,               102, 140, 66,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               168, 140, 66,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,               234, 140, 66,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,               300, 140, 66,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,               366, 140, 66,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,               432, 140, 66,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT7,               498, 140, 66,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT8,               564, 140, 66,  40, 0, 0x0, 0 },    
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_POINT,          227, 140, 10,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_PPM_POINT,      491, 140, 10,  40, 0, 0x0, 0 },       
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_UNITS,          650, 140, 120, 40, 0, 0x0, 0 },    

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          102,  60, 66, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          168,  60, 66, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          234,  60, 66, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          300,  60, 66, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL5,          366,  60, 66, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL6,          432,  60, 66, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL7,          498,  60, 66, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL8,          564,  60, 66, 200, 0, 0x0, 0 },    
                                                                
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  100, 1024, 500, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       32,   25,  960, 350, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,               130, 175,  85,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               215, 175,  85,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,               299, 175,  85,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,               384, 175,  85,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,               468, 175,  85,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,               553, 175,  85,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT7,               637, 175,  85,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT8,               722, 175,  85,  50, 0, 0x0, 0 },    
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_POINT,          290, 175,   12,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_PPM_POINT,      628, 175,   12,  50, 0, 0x0, 0 },           
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_UNITS,          832, 175,  84,  50, 0, 0x0, 0 }, 

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          130,  75, 85, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          215,  75, 85, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          299,  75, 85, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          384,  75, 85, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL5,          468,  75, 85, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL6,          553,  75, 85, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL7,          637,  75, 85, 250, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL8,          722,  75, 85, 250, 0, 0x0, 0 },       

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        512, 400, 512, 100, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};
#endif

typedef enum    listwheel_idx_e
{
        LISTWHEEL_IDX_1 = 0,
        LISTWHEEL_IDX_2,
        LISTWHEEL_IDX_3,
        LISTWHEEL_IDX_4,
        LISTWHEEL_IDX_5,
        LISTWHEEL_IDX_6,
        LISTWHEEL_IDX_7,
        LISTWHEEL_IDX_8,        
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

static  listwheel_idx_t         listwheel_idx;
static  WM_HWIN                 hWheel;
//static  const   char *          symb[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };

static const char* symb[] = {
    "9", "8", "7", "6", "5", "4", "3", "2", "1", "0"
};
static  const   size_t          symb_countof    = sizeof( symb ) / sizeof( char * );

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
        TEXT_SetText(           hItem, "."  );
        WM_BringToTop( hItem );
        
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_PPM_POINT );
        TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
        TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
        TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_WHEEL_UNSEL              );
        TEXT_SetTextColor(      hItem, GUI_WHITE                                );
        WM_BringToBottom( hItem );

        
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_UNITS );
        TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
        TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );            
        TEXT_SetText(           hItem, "%"  );

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
        const   WM_HWIN         hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT1 );
        const   WM_HWIN         hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT2 );
        const   WM_HWIN         hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT3 );
        const   WM_HWIN         hText4  = WM_GetDialogItem( hWin, GUI_ID_TEXT4 );
        const   WM_HWIN         hText5  = WM_GetDialogItem( hWin, GUI_ID_TEXT5 );
        const   WM_HWIN         hText6  = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );
        const   WM_HWIN         hText7  = WM_GetDialogItem( hWin, GUI_ID_TEXT7 );
        const   WM_HWIN         hText8  = WM_GetDialogItem( hWin, GUI_ID_TEXT8 );


        switch( idx )
        {
                case LISTWHEEL_IDX_1:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );                        
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_2:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL  );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );                                                
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_3:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL   );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );                                                
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_4:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );                                                
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_5:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL   );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );                                                
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_6:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL  );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );                                                
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL6 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_7:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );                                                
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL7 );
                        listwheel_idx   = idx;
                        break;
                        
                case LISTWHEEL_IDX_8:
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_SEL );                                                
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL8 );
                        listwheel_idx   = idx;
                        break;                                                                                           

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
                listwheel_idx   = LISTWHEEL_IDX_1;
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
    int             level;
    int button = 0;
    scr_idx_t       scr_idx;

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

                case GUI_KEY_DOWN:
                    idx     = LISTWHEEL_GetPos( hWheel );
                    if( ++idx >= LISTWHEEL_GetNumItems( hWheel ) )
                    {
                        idx     = 0;
                    }

                    LISTWHEEL_SetPos( hWheel, idx );
                    LISTWHEEL_SetSel( hWheel, idx );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_KEY_ESCAPE:
                    if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_VALUE)
                    {
                      button = GUI_ID_BUTTON_VALUE;   
                      scr_idx = SCR_IDX_VALIDATION_SETUP;
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_DEVIATION)
                    {
                      button = GUI_ID_BUTTON_DEVIATION;
                      scr_idx = SCR_IDX_VALIDATION_SETUP;
                    }                         
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_ZERO)
                    {
                      button = GUI_ID_BUTTON_ZERO;
                      scr_idx = SCR_IDX_CALIBRATION_SETUP;
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SPAN)
                    {
                      button = GUI_ID_BUTTON_SPAN;
                      scr_idx = SCR_IDX_CALIBRATION_SETUP;                      
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_DEVIATION)
                    {
                      button = GUI_ID_BUTTON_DEVIATION;
                      scr_idx = SCR_IDX_CALIBRATION_SETUP;                      
                    }                    
                    
                    scr_switch( scr_idx, button );
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

                case GUI_ID_LISTWHEEL5:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_5 );
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_LISTWHEEL6:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_6 );
                        beep_play( BEEP_SHRT );
                    }
                    break;
                    
                case GUI_ID_LISTWHEEL7:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_7 );
                        beep_play( BEEP_SHRT );
                    }
                    break;                    
                    
                case GUI_ID_LISTWHEEL8:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_8 );
                        beep_play( BEEP_SHRT );
                    }
                    break;                                        

                case GUI_ID_BUTTON_CANCEL:
                    if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_VALUE)
                    {
                      button = GUI_ID_BUTTON_VALUE;   
                      scr_idx = SCR_IDX_VALIDATION_SETUP;
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_DEVIATION)
                    {
                      button = GUI_ID_BUTTON_DEVIATION;
                      scr_idx = SCR_IDX_VALIDATION_SETUP;
                    }                         
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_ZERO)
                    {
                      button = GUI_ID_BUTTON_ZERO;
                      scr_idx = SCR_IDX_CALIBRATION_SETUP;
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SPAN)
                    {
                      button = GUI_ID_BUTTON_SPAN;
                      scr_idx = SCR_IDX_CALIBRATION_SETUP;                      
                    }
                    else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_DEVIATION)
                    {
                      button = GUI_ID_BUTTON_DEVIATION;
                      scr_idx = SCR_IDX_CALIBRATION_SETUP;                      
                    }                    
                    
                    scr_switch( scr_idx, button );
                    beep_play( BEEP_SHRT );
                    break;

                case GUI_ID_BUTTON_ENTER:
                case GUI_ID_BUTTON_DUMMY:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        level  = 0;
                        level  += 100000000 * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 )));
                        level  += 10000000  * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 )));
                        level  += 1000000   * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 )));
                        level  += 100000    * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 )));
                        level  += 10000     * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL5 )));
                        level  += 1000      * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL6 )));                                                                          
                        level  += 100     * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL7 )));
                        level  += 10      * ((symb_countof - 1) - LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL8 )));                                                                          
                        
                        if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_VALUE)
                        {
                          dev.validation.value.ppb = level;
                          button = GUI_ID_BUTTON_VALUE;
                          scr_idx = SCR_IDX_VALIDATION_SETUP;
                          send_cmd_for_nvm_write_param(NVM_REG_VALIDATION_VALUE, dev.validation.value.ppb);                                                                              
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_DEVIATION)
                        {
                          dev.validation.deviation.ppb = level; 
                          button = GUI_ID_BUTTON_DEVIATION;
                          scr_idx = SCR_IDX_VALIDATION_SETUP;
                          send_cmd_for_nvm_write_param(NVM_REG_VALIDATION_DEVIATION, dev.validation.deviation.ppb);
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_ZERO)
                        {
                          dev.calibration.zero.ppb = level;
                          button = GUI_ID_BUTTON_ZERO;
                          scr_idx = SCR_IDX_CALIBRATION_SETUP;
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_ZERO, dev.calibration.zero.ppb);
                        }
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SPAN)
                        {
                          dev.calibration.span.ppb = level;
                          button = GUI_ID_BUTTON_SPAN;
                          scr_idx = SCR_IDX_CALIBRATION_SETUP;  
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_SPAN, dev.calibration.span.ppb);
                        }   
                        else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_DEVIATION)
                        {
                          dev.calibration.deviation.ppb = level;
                          button = GUI_ID_BUTTON_DEVIATION;
                          scr_idx = SCR_IDX_CALIBRATION_SETUP;  
                          send_cmd_for_nvm_write_param(NVM_REG_CALIBRATION_DEVIATION, dev.calibration.deviation.ppb);                          
                        }
                                                                                             
                        scr_switch( scr_idx, button );
                        beep_play( BEEP_TYPE_CONFIRM );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
            
            if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_VALUE)
            {
              level = dev.validation.value.ppb;
            }
            else if (dev.gui.scr_idx == SCR_IDX_VALIDATION_SETUP_DEVIATION)
            {
              level = dev.validation.deviation.ppb;
            }
            else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_ZERO)
            {
              level = dev.calibration.zero.ppb;
            }
            else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_SPAN)
            {
              level = dev.calibration.span.ppb;
            }
            else if (dev.gui.scr_idx == SCR_IDX_CALIBRATION_SETUP_DEVIATION)
            {
              level = dev.calibration.deviation.ppb;
            }
              
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600  
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 50, symb, sizeof( symb ) / sizeof( char * ) );            
#endif 
            idx     = (symb_countof - 1) - (level / 100000000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600   
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 50, symb, sizeof( symb ) / sizeof( char * ) );            
#endif             
            idx     = (symb_countof - 1) - (level /  10000000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 50, symb, sizeof( symb ) / sizeof( char * ) );
#endif             
            idx     = (symb_countof - 1) - (level /   1000000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 50, symb, sizeof( symb ) / sizeof( char * ) );
#endif 
            idx     = (symb_countof - 1) - (level /    100000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL5 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL5, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL5, 50, symb, sizeof( symb ) / sizeof( char * ) );
#endif             
            idx     = (symb_countof - 1) - (level /     10000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL6 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL6, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600  
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL6, 50, symb, sizeof( symb ) / sizeof( char * ) );
#endif             
            idx     = (symb_countof - 1) - (level /       1000) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );
            
            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL7 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL7, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL7, 50, symb, sizeof( symb ) / sizeof( char * ) );
#endif             
            idx     = (symb_countof - 1) - (level /     100) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL8 );
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL8, 40, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL8, 50, symb, sizeof( symb ) / sizeof( char * ) );
#endif 
            idx     = (symb_countof - 1) - (level /       10) % 10;
            LISTWHEEL_SetPos( hItem, idx );
            LISTWHEEL_SetSel( hItem, idx );            


            listwheel_idx   = LISTWHEEL_IDX_1;
            listwheel_set_focus( pMsg->hWin, listwheel_idx );
            hWheel  = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 );
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
scr_validation_setup_value( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
