/**
  * @file    scr_calibrate_point.c
  * @brief   Screen Calibrate Point
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "scr\scr.h"
#include "dev\dev.h"
#include "main.h"


/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
extern  dev_t           dev;


/*******************************************************************************
* LOCAL TYPES
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
* LOCAL VARIABLES
*******************************************************************************/
static const GUI_WIDGET_CREATE_INFO dialog_info[] =
{
    { WINDOW_CreateIndirect,    "", 0,                            0,  80, 800, 400, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_LABEL,      25,  20, 225,  50, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_MEAS_VALUE,      25,  70, 225,  70, 0, 0x0, 0 },
    //{ TEXT_CreateIndirect,      "", GUI_ID_TEXT_SLOPE_LABEL,     25, 180, 225,  50, 0, 0x0, 0 },
    //{ TEXT_CreateIndirect,      "", GUI_ID_TEXT_SLOPE_VALUE,     25, 230, 225,  70, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_BACKPLANE,      275,  20, 500, 280, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_POINT,          525, 140,  10,  40, 0, 0x0, 0 },

    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS0,         368, 140,  80,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS1,         448, 140,  80,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS2,         528, 140,  80,  40, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS3,         608, 140,  80,  40, 0, 0x0, 0 },
//    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS4,         688, 140,  80,  40, 0, 0x0, 0 },
//    { TEXT_CreateIndirect,      "", GUI_ID_TEXT_FOCUS5,         688, 140,  80,  40, 0, 0x0, 0 },

    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL0,          368,  50,  80, 220, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL1,          448,  50,  80, 220, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL2,          528,  50,  80, 220, 0, 0x0, 0 },
    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL3,          608,  50,  80, 220, 0, 0x0, 0 },
//    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL4,          688,  50,  80, 220, 0, 0x0, 0 },
//    { LISTWHEEL_CreateIndirect, "", GUI_ID_LISTWHEEL5,          688,  50,  80, 220, 0, 0x0, 0 },

    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_CANCEL,         0, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_ENTER,        400, 320, 400,  80, 0, 0x0, 0 },
    { BUTTON_CreateIndirect,    "", GUI_ID_BUTTON_DUMMY,         -1,  -1,   1,   1, 0, 0x0, 0 },
};

static  const   char *          list[]  = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", };
static  const   size_t          list_countof    = sizeof( list ) / sizeof( char * );
static  listwheel_idx_t         listwheel_idx;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
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

        //case LISTWHEEL_IDX_4:
        //    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL4 );
        //    LISTWHEEL_SetPos( hItem, (val / 10    ) % 10 );
        //    LISTWHEEL_SetSel( hItem, (val / 10    ) % 10 );
        //    break;
        //
        //case LISTWHEEL_IDX_5:
        //    hItem   = WM_GetDialogItem( hWin, GUI_ID_LISTWHEEL5 );
        //    LISTWHEEL_SetPos( hItem, (val / 1     ) % 10 );
        //    LISTWHEEL_SetSel( hItem, (val / 1     ) % 10 );
        //    break;

        default:
            break;
    }
}

static
void
listwheel_set_focus(            const   WM_HWIN                 hWin,
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
//            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_1:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_2:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_SEL   );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

        case LISTWHEEL_IDX_3:
            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_SEL   );
//            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
            listwheel_idx   = idx;
            break;

//        case LISTWHEEL_IDX_4:
//            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_SEL   );
//            TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            listwheel_idx   = idx;
//            break;
//
//        case LISTWHEEL_IDX_5:
//            TEXT_SetBkColor( hText0, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText1, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText2, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText3, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText4, CONFIG_UI_COLOR_WHEEL_UNSEL );
//            TEXT_SetBkColor( hText5, CONFIG_UI_COLOR_WHEEL_SEL   );
//            listwheel_idx   = idx;
//            break;

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
int
listwheel_increment(            const   WM_HWIN                 hWin,
                                        listwheel_idx_t         idx )
{
    WM_HWIN     hItem;
    size_t      list_countof    = sizeof( list ) / sizeof( char * );
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
    size_t      list_countof    = sizeof( list ) / sizeof( char * );


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
void
init_dialog(                            WM_HWIN                 hWin )
{
    WM_HWIN                 hItem;

    ////////////////////////////////////////////////////////
    // MIDDLE AREA, LEFT SIDE
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_LABEL );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_BACKGROUND               );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE ) );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_MEAS_VALUE );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_BACKGROUND               );
    TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );

    //hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_LABEL );
    //TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    //TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_BOTTOM         );
    //TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_BACKGROUND               );
    //TEXT_SetTextColor(      hItem, GUI_LIGHTGRAY                            );
    //TEXT_SetText(           hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_SLOPE   )  );
    ////TEXT_SetText( hItem, "V/sec\0" );
    //
    //hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_SLOPE_VALUE );
    //TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    //TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    //TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_BACKGROUND               );
    //TEXT_SetTextColor(      hItem, GUI_LIGHTGREEN                           );

    ////////////////////////////////////////////////////////
    // MIDDLE AREA, RIGHT SIDE
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_BACKPLANE );
    TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_BACKGROUND               );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_TEXT_POINT );
    TEXT_SetFont(           hItem, &GUI_FontTahoma40                        );
    TEXT_SetTextAlign(      hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER        );
    TEXT_SetBkColor(        hItem, CONFIG_UI_COLOR_WHEEL_UNSEL              );
    TEXT_SetTextColor(      hItem, GUI_WHITE                                );
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

    ////////////////////////////////////////////////////////
    // FOOTER AREA
    ////////////////////////////////////////////////////////
    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_DUMMY );
    BUTTON_SetFocusable(    hItem, 1 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_CANCEL );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_CANCEL )       );
    BUTTON_SetFocusable(    hItem, 0 );

    hItem   = WM_GetDialogItem( hWin, GUI_ID_BUTTON_ENTER );
    BUTTON_SetText(         hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ENTER )        );
    BUTTON_SetFocusable(    hItem, 0 );
}


static
void
dialog_callback(                        WM_MESSAGE *            pMsg )
{
    static  int         blink;
            WM_HWIN     hItem;
            int         ppm;
            char        str[32];
            int32_t     i32;
            econ_cal_point_t *  point;

    switch( dev.gui.cal_idx )
    {
        //case 2:
        //    ppm     = (dev.sens->cal_factory.zero.ppm_int_hi << 16) | (dev.sens->cal_factory.zero.ppm_int_lo & 0xFFFF);
        //    point   = &(dev.sens->cal_factory.zero);
        //    break;
        case 1:
            ppm     = dev.sens->cal.span.ppm.i32;
            point   = &(dev.sens->cal.span);
            break;
        case 0:
        default:
            ppm     = dev.sens->cal.zero.ppm.i32;
            point   = &(dev.sens->cal.zero);
            break;
    }

    switch( pMsg->MsgId )
    {
        case WM_KEY:
            switch( ((WM_KEY_INFO*) (pMsg->Data.p) )->Key )
            {
                case GUI_KEY_UP:
                    listwheel_increment( pMsg->hWin, listwheel_idx );
                    beep_play( BEEP_SHRT );
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
                    switch( dev.gui.cal_idx )
                    {
                        case 2:     scr_switch( SCR_IDX_SETUP_SERVICE_DIAGNOSTICS, GUI_ID_BUTTON_FACTORY_ZERO );    break;
                        case 1:     scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_SPAN );    break;
                        default:    scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_ZERO );    break;
                    }
                    beep_play( BEEP_SHRT );
                    break;
                default:
                    break;
            }
            break;

        case WM_NOTIFY_PARENT:
            if(         pMsg->Data.v == WM_NOTIFICATION_CLICKED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
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
//                    case GUI_ID_LISTWHEEL4:
//                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_4 );
//                        beep_play( BEEP_SHRT );
//                        break;
//                    case GUI_ID_LISTWHEEL5:
//                        listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_5 );
//                        beep_play( BEEP_SHRT );
//                        break;                                                
                    case GUI_ID_BUTTON_DUMMY:
                    case GUI_ID_BUTTON_ENTER:
                        i32                 = listwheel_assemble( pMsg->hWin );
                        point->ppm.i32   = i32; 
                        i32                 = dev.mcu->rtc.get_timestamp();
                        point->timestamp.i32 = i32;
                        task_hmi_cal_update( dev.gui.cal_idx );

                        switch( dev.gui.cal_idx )
                        {
                            case 2:     scr_switch( SCR_IDX_SETUP_SERVICE_DIAGNOSTICS, GUI_ID_BUTTON_FACTORY_ZERO );    break;
                            case 1:     scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_SPAN );    break;
                            default:    scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_ZERO );    break;
                        }

                        beep_play( BEEP_TYPE_CONFIRM );
                        break;
                    default:
                        break;
                }
            }

            if( pMsg->Data.v == WM_NOTIFICATION_RELEASED )
            {
                switch( WM_GetId( pMsg->hWinSrc ) )
                {
                    case GUI_ID_BUTTON_CANCEL:
                        switch( dev.gui.cal_idx )
                        {
                            case 2:     scr_switch( SCR_IDX_SETUP_SERVICE_DIAGNOSTICS, GUI_ID_BUTTON_FACTORY_ZERO );    break;
                            case 1:     scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_SPAN );    break;
                            default:    scr_switch( SCR_IDX_CALIBRATE, GUI_ID_BUTTON_ZERO );    break;
                        }
                        beep_play( BEEP_SHRT );
                        break;
                    default:
                        break;
                }
            }
            break;

        case WM_TIMER:
            blink++;
            ppm = dev.sens->meas.ppm.integral;
            //ppb = (dev.sens->meas.ppb_hi << 16) | (dev.sens->meas.ppb_lo & 0xFFFF);        

            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_MEAS_LABEL );
            TEXT_SetText( hItem, (blink & 1) ? "" : l10n_str_get( dev.cfg.lang, L10N_STR_ID_MEASURE ) );

            //hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SLOPE_LABEL );
            //TEXT_SetText( hItem, (blink & 1) ? "" : l10n_str_get( dev.cfg.lang, L10N_STR_ID_SLOPE ) );


            hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_MEAS_VALUE );
            switch( dev.cfg.display_mode )
            {
                //case DEV_DSPL_MODE_PPM:
                //    snprintf( str, sizeof(str), "% 8d PPM", ppm );
                //    break;
                case DEV_DSPL_MODE_PERCENTS:
                default:
                    snprintf( str, sizeof(str), "% 4d.%02u %%", ppm / 10000, ( abs(ppm) % 10000) / 100 );
                    break;
            }
            TEXT_SetText( hItem, str );

            //hItem   = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SLOPE_VALUE );
            //snprintf( str, sizeof(str), "%8d\0", dev.sens->meas.slope );
            //TEXT_SetText( hItem, str );

            hItem   = pMsg->Data.v; //pMsg->Data.v contains a handle the expired timer only if the message WM_TIMER is currently processed
            WM_RestartTimer( hItem, 0 );
            break;

        case WM_INIT_DIALOG:
            hItem           = WM_GetDialogItem( pMsg->hWin, GUI_ID_TEXT_SBAR_HEADER );
            TEXT_SetText( hItem, l10n_str_get( dev.cfg.lang, L10N_STR_ID_ZERO ) );

            listwheel_idx   = LISTWHEEL_IDX_0;
            init_dialog( pMsg->hWin );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL0, 40, list, list_countof );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_0, ppm );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL1, 40, list, list_countof );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_1, ppm );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL2, 40, list, list_countof );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_2, ppm );

            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL3, 40, list, list_countof );
            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_3, ppm );

//            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL4, 40, list, list_countof );
//            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_4, ppm );
//
//            gui_init_listwheel( pMsg->hWin, GUI_ID_LISTWHEEL5, 40, list, list_countof );
//            listwheel_setup( pMsg->hWin, LISTWHEEL_IDX_5, ppm );

            listwheel_idx   = LISTWHEEL_IDX_0;
            listwheel_set_focus( pMsg->hWin, LISTWHEEL_IDX_0 );

            WM_CreateTimer( pMsg->hWin, 0, 500, 0 );
            break;

        default:
            WM_DefaultProc( pMsg );
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
scr_calibrate_point( void )
{
    return  GUI_CreateDialogBox(    dialog_info,
                                    GUI_COUNTOF( dialog_info ),
                                    &dialog_callback,
                                    WM_HBKWIN,
                                    0,
                                    0 );
}
