/**
  * @file    scr_setup_service_network_mac.c
  * @brief   setup mac address
  * @author  nkp144
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

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,                37, 140, 55,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT1,                92, 140, 55,  40, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT2,               159, 140, 55,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT3,               214, 140, 55,  40, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT4,               281, 140, 55,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT5,               336, 140, 55,  40, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT6,               403, 140, 55,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT7,               458, 140, 55,  40, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT8,               525, 140, 55,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT9,               580, 140, 55,  40, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT10,              647, 140, 55,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT11,              702, 140, 55,  40, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "-", GUI_ID_TEXT12,             147, 140, 12,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "-", GUI_ID_TEXT13,             269, 140, 12,  40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "-", GUI_ID_TEXT14,             391, 140, 12,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "-", GUI_ID_TEXT15,             513, 140, 12,  40, 0, 0x0, 0 },    
    { TEXT_CreateIndirect,      "-", GUI_ID_TEXT16,             635, 140, 12,  40, 0, 0x0, 0 },
        
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,           37,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,           92,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          159,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          214,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          281,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL5,          336,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL6,          403,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL7,          458,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL8,          525,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL9,          580,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL10,         647,  60, 55, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL11,         702,  60, 55, 200, 0, 0x0, 0 },
                                                                
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
        LISTWHEEL_IDX_5,
        LISTWHEEL_IDX_6,
        LISTWHEEL_IDX_7,
        LISTWHEEL_IDX_8,
        LISTWHEEL_IDX_9,
        LISTWHEEL_IDX_10,
        LISTWHEEL_IDX_11,              
        LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

static  listwheel_idx_t         listwheel_idx;
static  WM_HWIN                 hWheel;
static  const   char *          symb[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

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
        const   WM_HWIN         hText5  = WM_GetDialogItem( hWin, GUI_ID_TEXT5 );
        const   WM_HWIN         hText6  = WM_GetDialogItem( hWin, GUI_ID_TEXT6 );
        const   WM_HWIN         hText7  = WM_GetDialogItem( hWin, GUI_ID_TEXT7 );
        const   WM_HWIN         hText8  = WM_GetDialogItem( hWin, GUI_ID_TEXT8 );
        const   WM_HWIN         hText9  = WM_GetDialogItem( hWin, GUI_ID_TEXT9 );
        const   WM_HWIN         hText10  = WM_GetDialogItem( hWin, GUI_ID_TEXT10 );
        const   WM_HWIN         hText11  = WM_GetDialogItem( hWin, GUI_ID_TEXT11 );

        switch( idx )
        {
                case LISTWHEEL_IDX_0:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                        
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_1:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_2:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_3:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_4:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_5:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_6:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL6 );
                        listwheel_idx   = idx;
                        break;
                        
                case LISTWHEEL_IDX_7:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_SEL);
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL7 );
                        listwheel_idx   = idx;
                        break;      
                        
                case LISTWHEEL_IDX_8:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_SEL);
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL8 );
                        listwheel_idx   = idx;
                        break;         
                        
                case LISTWHEEL_IDX_9:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_SEL);
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL9 );
                        listwheel_idx   = idx;
                        break;         
                        
                case LISTWHEEL_IDX_10:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL);
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_SEL );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_UNSEL );                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL10 );
                        listwheel_idx   = idx;
                        break;    

                case LISTWHEEL_IDX_11:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText6, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText7, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText8, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText9, CONFIG_UI_COLOR_WHEEL_UNSEL);
                        TEXT_SetBkColor( hText10, CONFIG_UI_COLOR_WHEEL_UNSEL  );
                        TEXT_SetBkColor( hText11, CONFIG_UI_COLOR_WHEEL_SEL);                         
                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL11 );
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
    uint8_t         mac_byte = 0x00;

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
                    scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK, GUI_ID_BUTTON_NETWORK_MAC);
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
                    
                case GUI_ID_LISTWHEEL9:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_9 );
                        beep_play( BEEP_SHRT );
                    }
                    break;                                        

                case GUI_ID_LISTWHEEL10:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_10 );
                        beep_play( BEEP_SHRT );
                    }
                    break;                                        
                    
                case GUI_ID_LISTWHEEL11:
                    if( pMsg->Data.v == WM_NOTIFICATION_CLICKED )
                    {
                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_11 );
                        beep_play( BEEP_SHRT );
                    }
                    break;                                        
                    
                case GUI_ID_BUTTON_CANCEL:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK, GUI_ID_BUTTON_NETWORK_MAC);
                        beep_play( BEEP_SHRT );
                    }
                    break;

                case GUI_ID_BUTTON_ENTER:
                case GUI_ID_BUTTON_DUMMY:
                    if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
                    {                                            
                        for (int i = 0; i < 6; i++)
                        {
                          dev.net->mac.u8[i] = LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 + (i*2) ) );
                          dev.net->mac.u8[i] <<= 4;
                          dev.net->mac.u8[i] |= LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 + (i*2) + 1 ) );
                        }
                        send_cmd_for_nvm_write_param(NVM_REG_NET_MAC_PART1, dev.net->mac.u32[0]);
                        send_cmd_for_nvm_write_param(NVM_REG_NET_MAC_PART2, dev.net->mac.u32[1]);                        
                        send_cmd_to_net_task(OS_USER_TAG_ETH_CHANGE_PARAM);
                        scr_switch( SCR_IDX_SETUP_SERVICE_NETWORK, GUI_ID_BUTTON_NETWORK_MAC);
                        beep_play( BEEP_TYPE_CONFIRM );
                    }
                    break;

                default:
                    break;
            }
            break;

        case WM_INIT_DIALOG:
            init_dialog( pMsg->hWin );
                       
            for (int i = 0; i < LISTWHEEL_IDX_LAST; i++)
            {
              mac_byte = dev.net->mac.u8[i/2];              
            
              hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 + i);
#if LCD_SPEC_XSIZE == 800 && LCD_SPEC_YSIZE == 480            
              gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0 + i, 60, symb, sizeof( symb ) / sizeof( char * ) );
#elif LCD_SPEC_XSIZE == 1024 && LCD_SPEC_YSIZE == 600 
              gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0 + i, 75, symb, sizeof( symb ) / sizeof( char * ) );            
#endif           
              if (i % 2)
                idx = mac_byte & 0x0F;
              else
                idx = mac_byte >> 4;                            
              LISTWHEEL_SetPos( hItem, idx );
              LISTWHEEL_SetSel( hItem, idx );
            }              

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
scr_setup_service_network_mac( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
