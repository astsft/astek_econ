/**
  * @file    scr_setup_measure_cl420_value.c
  * @brief   Screen Setup Range Value
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"


/*******************************************************************************
* PRIVATE TYPES
*******************************************************************************/
typedef enum    listwheel_idx_e
{
    LISTWHEEL_IDX_0         = 0,
    LISTWHEEL_IDX_1,
    LISTWHEEL_IDX_2,
    LISTWHEEL_IDX_3,
//    LISTWHEEL_IDX_4,
//    LISTWHEEL_IDX_5,
    LISTWHEEL_IDX_LAST,
} listwheel_idx_t;

/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
extern  dev_t           dev;

/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO     dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,       25,  20, 750, 280, 0, 0x0, 0 },
    
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,          50, 140,  100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         150, 140,  100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,         250, 140,  100,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,         350, 140,  100,  40, 0, 0x0, 0 },
//    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS4,         450, 140,  100,  40, 0, 0x0, 0 },
//    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS5,         550, 140,  100,  40, 0, 0x0, 0 },    

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,           50,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          150,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          250,  60, 100, 200, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          350,  60, 100, 200, 0, 0x0, 0 },
//    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          450,  60, 100, 200, 0, 0x0, 0 },
//    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL5,          550,  60, 100, 200, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT0,               245, 140,  10, 40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_UNITS,          650, 100, 120,  80, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  const   char *          symb[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };

static  uint32_t                ppm;
static  listwheel_idx_t         listwheel_idx;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
void
init_listwheel(                                         WM_HWIN         hWin,
                                                const   char **         list,
                                                const   int             list_countof )
{
        const   int     y_size          = WM_GetWindowSizeY( hWin );
        const   size_t  line_height     = 40;


        LISTWHEEL_SetFont(      hWin,  &GUI_FontTahoma40                        );
        LISTWHEEL_SetBkColor(   hWin,  LISTWHEEL_CI_UNSEL,     GUI_BLACK        );
        LISTWHEEL_SetBkColor(   hWin,  LISTWHEEL_CI_SEL,       GUI_BLACK        );
        LISTWHEEL_SetLBorder(   hWin,  20                                       );
        LISTWHEEL_SetLineHeight( hWin, line_height                              );
        LISTWHEEL_SetRBorder(   hWin,  20                                       );
        LISTWHEEL_SetTextAlign( hWin,  GUI_TA_HCENTER | GUI_TA_VCENTER          );
        LISTWHEEL_SetTextColor( hWin,  LISTWHEEL_CI_UNSEL,     GUI_GRAY         );
        LISTWHEEL_SetTextColor( hWin,  LISTWHEEL_CI_SEL,       GUI_WHITE         );

        for( int i = 0; i < list_countof; i++ )
        {
                LISTWHEEL_AddString( hWin, list[i] );
        }

        LISTWHEEL_SetSnapPosition( hWin, (y_size/2 - line_height/2) );
}

static
void
listwheel_setup(                const   WM_HWIN                 hWin,
                                const   listwheel_idx_t         idx,
                                const   int32_t                 val )
{
    WM_HWIN     hItem;

    switch( idx )
    {
        case LISTWHEEL_IDX_0:
            hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );
            LISTWHEEL_SetPos( hItem, (val / 100000) % 10 );
            LISTWHEEL_SetSel( hItem, (val / 100000) % 10 );
            break;

        case LISTWHEEL_IDX_1:
            hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );
            LISTWHEEL_SetPos( hItem, (val / 10000 ) % 10 );
            LISTWHEEL_SetSel( hItem, (val / 10000 ) % 10 );
            break;

        case LISTWHEEL_IDX_2:
            hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );
            LISTWHEEL_SetPos( hItem, (val / 1000  ) % 10 );
            LISTWHEEL_SetSel( hItem, (val / 1000  ) % 10 );
            break;

        case LISTWHEEL_IDX_3:
            hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );
            LISTWHEEL_SetPos( hItem, (val / 100   ) % 10 );
            LISTWHEEL_SetSel( hItem, (val / 100   ) % 10 );
            break;

//        case LISTWHEEL_IDX_4:
//            hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
//            LISTWHEEL_SetPos( hItem, (val / 10    ) % 10 );
//            LISTWHEEL_SetSel( hItem, (val / 10    ) % 10 );
//            break;
//
//        case LISTWHEEL_IDX_5:
//            hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );
//            LISTWHEEL_SetPos( hItem, (val / 1     ) % 10 );
//            LISTWHEEL_SetSel( hItem, (val / 1     ) % 10 );
//            break;

        default:
            break;
    }
}

static
int
listwheel_increment(            const   WM_HWIN                 hWin,
                                        listwheel_idx_t         idx )
{
    WM_HWIN     hItem;
    size_t      list_countof    = sizeof( symb ) / sizeof( char * );
    int         val;


    switch( idx )
    {
        case LISTWHEEL_IDX_0:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );  break;
        case LISTWHEEL_IDX_1:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );  break;
        case LISTWHEEL_IDX_2:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );  break;
        case LISTWHEEL_IDX_3:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );  break;
//        case LISTWHEEL_IDX_4:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );  break;
//        case LISTWHEEL_IDX_5:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );  break;
        default:
            return( 0 );
    }

    val     = LISTWHEEL_GetPos( hItem );

    if( ++val >= list_countof )
    {
        val       = 0;
    }

    LISTWHEEL_SetPos( hItem, val );
    LISTWHEEL_SetSel( hItem, val );
    return( val );
}

static
int
listwheel_decrement(            const   WM_HWIN                 hWin,
                                        listwheel_idx_t         idx )
{
    WM_HWIN     hItem;
    int         val;
    size_t      list_countof    = sizeof( symb ) / sizeof( char * );


    switch( idx )
    {
        case LISTWHEEL_IDX_0:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 );  break;
        case LISTWHEEL_IDX_1:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 );  break;
        case LISTWHEEL_IDX_2:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 );  break;
        case LISTWHEEL_IDX_3:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 );  break;
//        case LISTWHEEL_IDX_4:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );  break;
//        case LISTWHEEL_IDX_5:   hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );  break;
        default:
                return( 0 );
    }

    val     = LISTWHEEL_GetPos( hItem );

    if( val > 0 )
    {
        val--;
    }
    else
    {
        val       = list_countof-1;
    }

    LISTWHEEL_SetPos( hItem, val );
    LISTWHEEL_SetSel( hItem, val );
    return( val );
}

static
void
listwheel_set_focus(                    const   WM_HWIN                 hWin,
                                                listwheel_idx_t         idx )
{
    const   WM_HWIN     hText0  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS0 );
    const   WM_HWIN     hText1  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS1 );
    const   WM_HWIN     hText2  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS2 );
    const   WM_HWIN     hText3  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS3 );
//    const   WM_HWIN     hText4  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS4 );
//    const   WM_HWIN     hText5  = WM_GetDialogItem( hWin, GUI_ID_TEXT_FOCUS5 );


        switch( idx )
        {
                case LISTWHEEL_IDX_0:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_1:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_2:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        listwheel_idx   = idx;
                        break;

                case LISTWHEEL_IDX_3:
                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
                        listwheel_idx   = idx;
                        break;

//                case LISTWHEEL_IDX_4:
//                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL   );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
//                        listwheel_idx   = idx;
//                        break;
//
//                case LISTWHEEL_IDX_5:
//                        TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//                        TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_SEL   );
//                        hWheel          = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );
//                        listwheel_idx   = idx;
//                        break;

                default:
                        break;
        }
}

static
void
listwheel_swtch_left(   const   WM_HWIN                 hWin )
{
    if( listwheel_idx > 0 )
    {
        listwheel_idx--;
    }
    else
    {
        listwheel_idx  = (listwheel_idx_t) (LISTWHEEL_IDX_LAST - 1);
    }

    listwheel_set_focus( hWin, listwheel_idx );
}

static
void
listwheel_swtch_rght(   const   WM_HWIN                 hWin )
{
    if( listwheel_idx < (LISTWHEEL_IDX_LAST - 1) )
    {
        listwheel_idx++;
    }
    else
    {
        listwheel_idx  = LISTWHEEL_IDX_0;
    }

    listwheel_set_focus( hWin, listwheel_idx );
}

static
uint32_t
listwheel_assemble(             const   WM_HWIN                 hWin )
{
    int     val = 0;

    val += 100000 * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL0 ) );
    val += 10000  * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL1 ) );
    val += 1000   * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL2 ) );
    val += 100    * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL3 ) );
//    val += 10     * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 ) );
//    val += 1      * LISTWHEEL_GetPos( WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 ) );

    return( val );
}

static
void    init_dialog(                                    WM_HWIN         hWin )
{
        WM_HWIN                 hItem;
        //dev_range_units_t       range_units     = term_range_units_get( sys.ibus );
        cl420_units_t   range_units = dev_cl420_get_units( &dev.cl420 );

        WINDOW_SetBkColor( hWin, GUI_GRAY_2F );

        ////////////////////////////////////////////////////////
        // MIDDLE AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
        TEXT_SetFont(           hItem, &GUI_FontTahoma30                        );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT0 );
        TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
        TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | GUI_TA_VCENTER         );
        TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_WHEEL_UNSEL              );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
        TEXT_SetText(           hItem, "."  );
        WM_BringToTop( hItem );
//        if( range_units == CL420_UNITS_PERCENT )
//        {
//                TEXT_SetText(           hItem, "."  );
//                WM_BringToTop( hItem );
//        }
//        else
//        {
//                WM_BringToBottom( hItem );
//        }

        hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_UNITS );
        TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
        TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
        TEXT_SetBkColor(        hItem, GUI_BLACK                                );
        TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );
        TEXT_SetText(           hItem, "%"  );
        //if(         range_units == CL420_UNITS_PPM )
        //{
        //        TEXT_SetText(           hItem, "PPM"  );
        //}
        //else if(    range_units == CL420_UNITS_PERCENT )
        //{
        //        TEXT_SetText(           hItem, "%"  );
        //}
        //else
        //{
        //        TEXT_SetText(           hItem, "---"  );
        //}

        ////////////////////////////////////////////////////////
        // FOOTER AREA
        ////////////////////////////////////////////////////////
        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
        BUTTON_SetFocusable(    hItem, 1                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL ) );
        BUTTON_SetFocusable(    hItem, 0                                        );

        hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
        BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER ) );
        BUTTON_SetFocusable(    hItem, 0                                        );
}


static
void
dialog_callback(                                WM_MESSAGE *            pMsg )
{
    dev_range_idx_t         range_idx;

    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    beep_play( BEEP_SHRT );
                    listwheel_increment( pMsg->hWin, listwheel_idx );
                    break;
                case GUI_KEY_DOWN:
                    listwheel_decrement( pMsg->hWin, listwheel_idx );
                    beep_play( BEEP_SHRT );
                    break;                    
                case GUI_KEY_LEFT:
                    listwheel_swtch_left( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_RIGHT:
                    listwheel_swtch_rght( pMsg->hWin );
                    beep_play( BEEP_SHRT );
                    break;
                case GUI_KEY_ESCAPE:
                    range_idx   = dev_cl420_get_range_idx( &dev.cl420 );
                    beep_play( BEEP_SHRT );
                    if(         range_idx == DEV_RANGE_IDX_R3 )
                    {
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON3 );
                    }
                    else if(    range_idx == DEV_RANGE_IDX_R2 )
                    {
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON2 );
                    }
                    else
                    {
                        scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON1 );
                    }
                    break;
                default:
                    beep_play( BEEP_TYPE_ERROR );
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if (pMsg->Data.v == WM_NOTIFICATION_CLICKED)
            {
              switch( WM_GetId(pMsg->hWinSrc) )
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
          
                    //case GUI_ID_LISTWHEEL4:
                    //    listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_4 );
                    //    beep_play( BEEP_SHRT );   
                    //    break;
                    //
                    //case GUI_ID_LISTWHEEL5:
                    //    listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_5 );
                    //    beep_play( BEEP_SHRT );   
                    //    break;  
                        
                    default:
                        break;
                }
            }
          
            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId(pMsg->hWinSrc) )
                {                              
                    case GUI_ID_BUTTON_ENTER:
                    case GUI_ID_BUTTON_DUMMY:
                        beep_play( BEEP_SHRT );
                        ppm     =  100000 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL0 ) );
                        ppm     +=  10000 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL1 ) );
                        ppm     +=   1000 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL2 ) );
                        ppm     +=    100 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL3 ) );
                        //ppm     +=     10 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL4 ) );
                        //ppm     +=      1 * LISTWHEEL_GetPos( WM_GetDialogItem( pMsg->hWin, GUI_ID_LISTWHEEL5 ) );
                        dev_cl420_set_range( &dev.cl420, ppm );
                        range_idx   = dev_cl420_get_range_idx( &dev.cl420 );
                        if(         range_idx == DEV_RANGE_IDX_R3 )
                        {
                            scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON3 );
                        }
                        else if(    range_idx == DEV_RANGE_IDX_R2 )
                        {
                            scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON2 );
                        }
                        else
                        {
                            scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON1 );
                        }
                        break;
                        
                    case GUI_ID_BUTTON_CANCEL:
                        range_idx   = dev_cl420_get_range_idx( &dev.cl420 );
                        beep_play( BEEP_SHRT );
                        if(         range_idx == DEV_RANGE_IDX_R3 )
                        {
                            scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON3 );
                        }
                        else if(    range_idx == DEV_RANGE_IDX_R2 )
                        {
                            scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON2 );
                        }
                        else
                        {
                            scr_switch( SCR_IDX_SETUP_MEASURE_CL420, GUI_ID_BUTTON1 );
                        }
                        break;
                    
                      default:
                        //beep_play( BEEP_TYPE_ERROR );
                        break;
                }
            }
            break;
            
        case WM_INIT_DIALOG:
            ppm = dev_cl420_get_range( &dev.cl420 );

            init_dialog( pMsg->hWin );
                        
            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, symb, sizeof( symb ) / sizeof( char * ) );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_0, ppm );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, symb, sizeof( symb ) / sizeof( char * ) );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_1, ppm );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, symb, sizeof( symb ) / sizeof( char * ) );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_2, ppm );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, symb, sizeof( symb ) / sizeof( char * ) );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_3, ppm );

            //gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 40, symb, sizeof( symb ) / sizeof( char * ) );
            //listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_4, ppm );
            //
            //gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL5, 40, symb, sizeof( symb ) / sizeof( char * ) );
            //listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_5, ppm );

            listwheel_idx   = LISTWHEEL_IDX_0;
            listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );
            
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
scr_setup_measure_cl420_value( void )
{
        return  GUI_CreateDialogBox(    dialog_info,
                                        GUI_COUNTOF( dialog_info ),
                                        &dialog_callback,
                                        WM_HBKWIN,
                                        0,
                                        0 );
}
