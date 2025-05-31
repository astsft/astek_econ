/**
  * @file    emwin_set_defaults.c
  * @brief   emWin Set Defaults
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "DIALOG.h"
#include "gui_user.h"
#include "font\font.h"


/* Colors */
static const GUI_COLOR _aColorDisabled[] = {0x808080, 0x101010};
static const GUI_COLOR _aColorEnabled[]  = {GUI_WHITE, GUI_BLACK};

/* Palettes */
static const GUI_LOGPALETTE _PalCheckDisabled = {
  2,	/* number of entries */
  1, 	/* transparency flag */
  _aColorDisabled
};

static const GUI_LOGPALETTE _PalCheckEnabled = {
  2,	/* number of entries */
  1, 	/* transparency flag */
  _aColorEnabled
};


/* Pixel data */
static const unsigned char _acXLChecked[] = {
  ________, ________,
  ________, ________,
  ________, ________,
  ________, ____XX__,
  ________, ___XXX__,
  ________, __XXXX__,
  ________, _XXXX___,
  ________, XXXX____,
  __XX___X, XXX_____,
  __XXX_XX, XX______,
  __XXXXXX, X_______,
  ___XXXXX, ________,
  ____XXX_, ________,
  ________, ________,
  ________, ________,
};

/* Bitmaps */
const GUI_BITMAP _abmXL[2] = {
  { 16, 16, 2, 1, _acXLChecked, &_PalCheckDisabled },
  { 16, 16, 2, 1, _acXLChecked, &_PalCheckEnabled },
};


static
void
gui_set_defaults_standart( void )
{
    BUTTON_SKINFLEX_PROPS           button;
    RADIO_SKINFLEX_PROPS            radio;
    CHECKBOX_SKINFLEX_PROPS         chkbx;
    PROGBAR_SKINFLEX_PROPS          progbar;


    // BUTTON
    BUTTON_GetSkinFlexProps(&button, BUTTON_SKINFLEX_PI_FOCUSED );
    button.aColorFrame[ 0]  = GUI_BLACK;
    button.aColorFrame[ 1]  = GUI_BLACK;
    button.aColorFrame[ 2]  = GUI_BLACK;
    button.aColorUpper[ 0]  = GUI_GRAY_27;
    button.aColorUpper[ 1]  = GUI_BLACK;
    button.aColorLower[ 0]  = GUI_BLACK;
    button.aColorLower[ 1]  = GUI_BLACK;
    button.Radius           = 4;
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_PRESSED );
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_ENABLED );
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_DISABLED );
    button.aColorFrame[ 0]  = GUI_GRAY;
    button.aColorFrame[ 1]  = GUI_WHITE;
    button.aColorUpper[ 0]  = GUI_GRAY_2F;
    button.aColorUpper[ 1]  = GUI_GRAY_17;
    button.aColorLower[ 0]  = GUI_GRAY_17;
    button.aColorLower[ 1]  = GUI_BLACK;
    BUTTON_SetSkinFlexProps(&button, BUTTON_SKINFLEX_PI_FOCUSED );

    BUTTON_SetDefaultTextColor(     GUI_GRAY,       BUTTON_CI_DISABLED      );
    BUTTON_SetDefaultTextColor(     GUI_WHITE,       BUTTON_CI_PRESSED       );
    BUTTON_SetDefaultTextColor(     GUI_WHITE,       BUTTON_CI_UNPRESSED     );

    BUTTON_SetDefaultTextAlign(     GUI_TA_HCENTER | GUI_TA_VCENTER         );
    BUTTON_SetDefaultFont( &GUI_FontTahoma30 );

    // CHECKBOX
    CHECKBOX_SetDefaultFont(  &GUI_FontTahoma30 );
    CHECKBOX_SetDefaultAlign( GUI_TA_HCENTER | GUI_TA_VCENTER );
    CHECKBOX_SetDefaultTextColor( GUI_WHITE );
    CHECKBOX_SetDefaultFocusColor( GUI_GRAY );

    CHECKBOX_SetDefaultImage( &_abmXL[0], CHECKBOX_BI_INACTIV_CHECKED       );
    CHECKBOX_SetDefaultImage( &_abmXL[1], CHECKBOX_BI_ACTIV_CHECKED         );

    CHECKBOX_GetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_ENABLED );
    chkbx.aColorFrame[ 0]   = GUI_GREEN;
    chkbx.aColorFrame[ 1]   = GUI_GREEN;
    chkbx.aColorFrame[ 2]   = GUI_GREEN;
    chkbx.aColorInner[ 0]   = GUI_GRAY;
    chkbx.aColorInner[ 1]   = GUI_GRAY;
    chkbx.ColorCheck        = GUI_GRAY_27;
    CHECKBOX_SetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_ENABLED );

    chkbx.aColorFrame[ 0]   = GUI_RED;
    chkbx.aColorFrame[ 1]   = GUI_RED;
    chkbx.aColorFrame[ 2]   = GUI_RED;
    CHECKBOX_SetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_DISABLED );

    LISTVIEW_SetDefaultFont( &GUI_FontTahoma30 );
    LISTVIEW_SetDefaultGridColor( GUI_DARKGRAY );
    LISTVIEW_SetDefaultBkColor(     LISTVIEW_CI_UNSEL,      GUI_BLACK       );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_UNSEL,      GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_SEL,        GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_SELFOCUS,   GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_DISABLED,   GUI_GRAY        );

    PROGBAR_GetSkinFlexProps(       &progbar, 0 );
    progbar.aColorLowerL[ 0]        = 0x0029D400;
    progbar.aColorLowerL[ 1]        = 0x0033E21C;
    progbar.aColorLowerR[ 0]        = GUI_GRAY_2F;
    progbar.aColorLowerR[ 1]        = GUI_GRAY_2F;
    progbar.aColorUpperR[ 0]        = GUI_DARKGRAY;
    progbar.aColorUpperR[ 1]        = GUI_DARKGRAY;
    progbar.ColorFrame              = GUI_GRAY;
    PROGBAR_SetSkinFlexProps(       &progbar, 0 );

    RADIO_GetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_PRESSED );
    radio.aColorButton[ 0]  = GUI_GRAY;             //Outer color of button frame.
    radio.aColorButton[ 1]  = GUI_GRAY;             //Middle color of button frame.
    radio.aColorButton[ 2]  = GUI_DARKGRAY;         //Inner color of button frame.
    radio.aColorButton[ 3]  = GUI_WHITE;         //Inner color of button.
    radio.ButtonSize        = 26;                   //Size of the button in pixels.
    RADIO_SetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_PRESSED );

    RADIO_GetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_UNPRESSED );
    radio.aColorButton[ 0]  = GUI_GRAY;             //Outer color of button frame.
    radio.aColorButton[ 1]  = GUI_GRAY;             //Middle color of button frame.
    radio.aColorButton[ 2]  = GUI_DARKGRAY;         //Inner color of button frame.
    radio.aColorButton[ 3]  = GUI_BLACK;            //Inner color of button.
    radio.ButtonSize        = 26;                   //Size of the button in pixels.
    RADIO_SetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_UNPRESSED );

    RADIO_SetDefaultTextColor( GUI_LIGHTGRAY );
    RADIO_SetDefaultFocusColor( GUI_WHITE );

    SCROLLBAR_SetDefaultColor(      GUI_LIGHTBLUE,      SCROLLBAR_CI_THUMB      );
    SCROLLBAR_SetDefaultSkinClassic();

    SPINBOX_SetDefaultSkinClassic();

    SWIPELIST_SetDefaultFont( SWIPELIST_FI_SEP_ITEM,    &GUI_FontTahoma40 );
    SWIPELIST_SetDefaultFont( SWIPELIST_FI_ITEM_HEADER, &GUI_FontTahoma30 );
    SWIPELIST_SetDefaultFont( SWIPELIST_FI_ITEM_TEXT,   &GUI_FontTahoma30 );
    SWIPELIST_SetDefaultTextColor( SWIPELIST_CI_ITEM_TEXT_UNSEL, GUI_LIGHTGRAY );
    SWIPELIST_SetDefaultTextColor( SWIPELIST_CI_ITEM_TEXT_SEL, GUI_LIGHTGRAY );
    SWIPELIST_SetDefaultBkColor( SWIPELIST_CI_BK_ITEM_UNSEL, GUI_BLACK );
    SWIPELIST_SetDefaultBkColor( SWIPELIST_CI_BK_ITEM_SEL, GUI_BLACK );

    TEXT_SetDefaultTextColor( GUI_LIGHTGRAY );
    TEXT_SetDefaultFont( &GUI_FontTahoma40 );

    WINDOW_SetDefaultBkColor( GUI_GRAY_2F );
}


static
void
gui_set_defaults_neon( void )
{
    BUTTON_SKINFLEX_PROPS           button;
    RADIO_SKINFLEX_PROPS            radio;
    CHECKBOX_SKINFLEX_PROPS         chkbx;
    PROGBAR_SKINFLEX_PROPS          progbar;


    // BUTTON //////////////////////////////////////////////////////////////////
    BUTTON_GetSkinFlexProps(&button, BUTTON_SKINFLEX_PI_FOCUSED );
    button.aColorFrame[ 0]  = GUI_TRANSPARENT;   //GUI_BLACK;
    button.aColorFrame[ 1]  = GUI_TRANSPARENT;    //GUI_BLACK;
    button.aColorFrame[ 2]  = GUI_BLUE;      //GUI_BLACK;
    button.aColorUpper[ 0]  = GUI_LIGHTBLUE;
    button.aColorUpper[ 1]  = GUI_BLUE;
    button.aColorLower[ 0]  = GUI_BLUE;
    button.aColorLower[ 1]  = GUI_LIGHTBLUE;
    button.Radius           = 16;
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_PRESSED );
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_ENABLED );
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_DISABLED );
    button.aColorFrame[ 0]  = GUI_TRANSPARENT;
    button.aColorFrame[ 1]  = GUI_TRANSPARENT;
    button.aColorUpper[ 0]  = GUI_LIGHTBLUE;
    button.aColorUpper[ 1]  = GUI_BLUE;
    button.aColorLower[ 0]  = GUI_BLUE;
    button.aColorLower[ 1]  = GUI_LIGHTBLUE;
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_FOCUSED );
    BUTTON_SetDefaultTextColor(     GUI_GRAY,       BUTTON_CI_DISABLED      );
    BUTTON_SetDefaultTextColor(     GUI_BLACK,       BUTTON_CI_PRESSED       );
    BUTTON_SetDefaultTextColor(     GUI_BLACK,       BUTTON_CI_UNPRESSED     );

    BUTTON_SetDefaultTextAlign(     GUI_TA_HCENTER | GUI_TA_VCENTER         );
    BUTTON_SetDefaultFont( &GUI_FontTahoma30 );

    // CHECKBOX ////////////////////////////////////////////////////////////////
    CHECKBOX_SetDefaultFont(  &GUI_FontTahoma30 );
    CHECKBOX_SetDefaultAlign( GUI_TA_HCENTER | GUI_TA_VCENTER );
    CHECKBOX_SetDefaultTextColor( GUI_WHITE );
    CHECKBOX_SetDefaultFocusColor( GUI_GRAY );

    CHECKBOX_SetDefaultImage( &_abmXL[0], CHECKBOX_BI_INACTIV_CHECKED       );
    CHECKBOX_SetDefaultImage( &_abmXL[1], CHECKBOX_BI_ACTIV_CHECKED         );

    CHECKBOX_GetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_ENABLED );
    chkbx.aColorFrame[ 0]   = GUI_GREEN;
    chkbx.aColorFrame[ 1]   = GUI_GREEN;
    chkbx.aColorFrame[ 2]   = GUI_GREEN;
    chkbx.aColorInner[ 0]   = GUI_GRAY;
    chkbx.aColorInner[ 1]   = GUI_GRAY;
    chkbx.ColorCheck        = GUI_GRAY_27;
    CHECKBOX_SetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_ENABLED );

    chkbx.aColorFrame[ 0]   = GUI_RED;
    chkbx.aColorFrame[ 1]   = GUI_RED;
    chkbx.aColorFrame[ 2]   = GUI_RED;
    CHECKBOX_SetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_DISABLED );

    LISTVIEW_SetDefaultFont( &GUI_FontTahoma30 );
    LISTVIEW_SetDefaultGridColor( GUI_DARKGRAY );
    LISTVIEW_SetDefaultBkColor(     LISTVIEW_CI_UNSEL,      GUI_BLACK       );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_UNSEL,      GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_SEL,        GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_SELFOCUS,   GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_DISABLED,   GUI_GRAY        );

    PROGBAR_GetSkinFlexProps(       &progbar, 0 );
    progbar.aColorLowerL[ 0]        = 0x0029D400;
    progbar.aColorLowerL[ 1]        = 0x0033E21C;
    progbar.aColorLowerR[ 0]        = GUI_GRAY_2F;
    progbar.aColorLowerR[ 1]        = GUI_GRAY_2F;
    progbar.aColorUpperR[ 0]        = GUI_DARKGRAY;
    progbar.aColorUpperR[ 1]        = GUI_DARKGRAY;
    progbar.ColorFrame              = GUI_GRAY;
    PROGBAR_SetSkinFlexProps(       &progbar, 0 );

    RADIO_GetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_PRESSED );
    radio.aColorButton[ 0]  = GUI_GRAY;             //Outer color of button frame.
    radio.aColorButton[ 1]  = GUI_GRAY;             //Middle color of button frame.
    radio.aColorButton[ 2]  = GUI_DARKGRAY;         //Inner color of button frame.
    radio.aColorButton[ 3]  = GUI_WHITE;         //Inner color of button.
    radio.ButtonSize        = 26;                   //Size of the button in pixels.
    RADIO_SetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_PRESSED );

    RADIO_GetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_UNPRESSED );
    radio.aColorButton[ 0]  = GUI_GRAY;             //Outer color of button frame.
    radio.aColorButton[ 1]  = GUI_GRAY;             //Middle color of button frame.
    radio.aColorButton[ 2]  = GUI_DARKGRAY;         //Inner color of button frame.
    radio.aColorButton[ 3]  = GUI_BLACK;            //Inner color of button.
    radio.ButtonSize        = 26;                   //Size of the button in pixels.
    RADIO_SetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_UNPRESSED );

    RADIO_SetDefaultTextColor( GUI_LIGHTGRAY );
    RADIO_SetDefaultFocusColor( GUI_WHITE );

    SCROLLBAR_SetDefaultColor(      GUI_LIGHTBLUE,      SCROLLBAR_CI_THUMB      );
    SCROLLBAR_SetDefaultSkinClassic();

    SPINBOX_SetDefaultSkinClassic();

    SWIPELIST_SetDefaultFont( SWIPELIST_FI_SEP_ITEM,    &GUI_FontTahoma40 );
    SWIPELIST_SetDefaultFont( SWIPELIST_FI_ITEM_HEADER, &GUI_FontTahoma30 );
    SWIPELIST_SetDefaultFont( SWIPELIST_FI_ITEM_TEXT,   &GUI_FontTahoma30 );
    SWIPELIST_SetDefaultTextColor( SWIPELIST_CI_ITEM_TEXT_UNSEL, GUI_LIGHTGRAY );
    SWIPELIST_SetDefaultTextColor( SWIPELIST_CI_ITEM_TEXT_SEL, GUI_LIGHTGRAY );
    SWIPELIST_SetDefaultBkColor( SWIPELIST_CI_BK_ITEM_UNSEL, GUI_BLACK );
    SWIPELIST_SetDefaultBkColor( SWIPELIST_CI_BK_ITEM_SEL, GUI_BLACK );

    TEXT_SetDefaultTextColor( GUI_LIGHTGRAY );
    TEXT_SetDefaultFont( &GUI_FontTahoma40 );

    //WINDOW_SetDefaultBkColor( GUI_GRAY_2F );
    //WINDOW_SetDefaultBkColor( GUI_GRAY );
    WINDOW_SetDefaultBkColor( GUI_BLACK );
}


static
void
gui_set_defaults_lite( void )
{
    BUTTON_SKINFLEX_PROPS           button;
    RADIO_SKINFLEX_PROPS            radio;
    CHECKBOX_SKINFLEX_PROPS         chkbx;
    PROGBAR_SKINFLEX_PROPS          progbar;


    // BUTTON //////////////////////////////////////////////////////////////////
    BUTTON_GetSkinFlexProps(&button, BUTTON_SKINFLEX_PI_FOCUSED );
    button.aColorFrame[ 0]  = GUI_BLUE;   //GUI_BLACK;
    button.aColorFrame[ 1]  = GUI_BLUE;    //GUI_BLACK;
    button.aColorFrame[ 2]  = GUI_TRANSPARENT;      //GUI_BLACK;
    button.aColorUpper[ 0]  = GUI_TRANSPARENT;
    button.aColorUpper[ 1]  = GUI_TRANSPARENT;
    button.aColorLower[ 0]  = GUI_TRANSPARENT;
    button.aColorLower[ 1]  = GUI_TRANSPARENT;
    button.Radius           = 16;
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_PRESSED );
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_ENABLED );
    button.aColorFrame[ 0]  = GUI_GRAY;
    button.aColorFrame[ 1]  = GUI_GRAY;
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_DISABLED );
    button.aColorFrame[ 0]  = GUI_WHITE;
    button.aColorFrame[ 1]  = GUI_WHITE;
    button.aColorUpper[ 0]  = GUI_TRANSPARENT;
    button.aColorUpper[ 1]  = GUI_TRANSPARENT;
    button.aColorLower[ 0]  = GUI_TRANSPARENT;
    button.aColorLower[ 1]  = GUI_TRANSPARENT;
    BUTTON_SetSkinFlexProps( &button, BUTTON_SKINFLEX_PI_FOCUSED );

    BUTTON_SetDefaultTextColor(     GUI_GRAY,       BUTTON_CI_DISABLED      );
    BUTTON_SetDefaultTextColor(     GUI_WHITE,      BUTTON_CI_PRESSED       );
    BUTTON_SetDefaultTextColor(     GUI_WHITE,       BUTTON_CI_UNPRESSED     );
    BUTTON_SetDefaultFocusColor( GUI_WHITE );

    //BUTTON_SetDefaultTextColor(     GUI_BLUE,       BUTTON_SKINFLEX_PI_PRESSED     );
    //BUTTON_SetDefaultTextColor(     GUI_GREEN,       BUTTON_SKINFLEX_PI_FOCUSED     );
    //BUTTON_SetDefaultTextColor(     GUI_RED,       BUTTON_SKINFLEX_PI_ENABLED     );
    //BUTTON_SetDefaultTextColor(     GUI_CYAN,       BUTTON_SKINFLEX_PI_DISABLED     );

    BUTTON_SetDefaultTextAlign(     GUI_TA_HCENTER | GUI_TA_VCENTER         );
    BUTTON_SetDefaultFont( &GUI_FontTahoma40 );

    // CHECKBOX ////////////////////////////////////////////////////////////////
    CHECKBOX_SetDefaultFont(  &GUI_FontTahoma30 );
    CHECKBOX_SetDefaultAlign( GUI_TA_HCENTER | GUI_TA_VCENTER );
    CHECKBOX_SetDefaultTextColor( GUI_WHITE );
    CHECKBOX_SetDefaultFocusColor( GUI_GRAY );

    CHECKBOX_SetDefaultImage( &_abmXL[0], CHECKBOX_BI_INACTIV_CHECKED       );
    CHECKBOX_SetDefaultImage( &_abmXL[1], CHECKBOX_BI_ACTIV_CHECKED         );

    CHECKBOX_GetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_ENABLED );
    chkbx.aColorFrame[ 0]   = GUI_GREEN;
    chkbx.aColorFrame[ 1]   = GUI_GREEN;
    chkbx.aColorFrame[ 2]   = GUI_GREEN;
    chkbx.aColorInner[ 0]   = GUI_GRAY;
    chkbx.aColorInner[ 1]   = GUI_GRAY;
    chkbx.ColorCheck        = GUI_GRAY_27;
    CHECKBOX_SetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_ENABLED );

    chkbx.aColorFrame[ 0]   = GUI_RED;
    chkbx.aColorFrame[ 1]   = GUI_RED;
    chkbx.aColorFrame[ 2]   = GUI_RED;
    CHECKBOX_SetSkinFlexProps( &chkbx, CHECKBOX_SKINFLEX_PI_DISABLED );

    LISTVIEW_SetDefaultFont( &GUI_FontTahoma30 );
    LISTVIEW_SetDefaultGridColor( GUI_DARKGRAY );
    LISTVIEW_SetDefaultBkColor(     LISTVIEW_CI_UNSEL,      GUI_BLACK       );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_UNSEL,      GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_SEL,        GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_SELFOCUS,   GUI_GRAY        );
    LISTVIEW_SetDefaultTextColor(   LISTVIEW_CI_DISABLED,   GUI_GRAY        );

    PROGBAR_GetSkinFlexProps(       &progbar, 0 );
    progbar.aColorLowerL[ 0]        = 0x0029D400;
    progbar.aColorLowerL[ 1]        = 0x0033E21C;
    progbar.aColorLowerR[ 0]        = GUI_GRAY_2F;
    progbar.aColorLowerR[ 1]        = GUI_GRAY_2F;
    progbar.aColorUpperR[ 0]        = GUI_DARKGRAY;
    progbar.aColorUpperR[ 1]        = GUI_DARKGRAY;
    progbar.ColorFrame              = GUI_GRAY;
    PROGBAR_SetSkinFlexProps(       &progbar, 0 );

    RADIO_GetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_PRESSED );
    radio.aColorButton[ 0]  = GUI_GRAY;             //Outer color of button frame.
    radio.aColorButton[ 1]  = GUI_GRAY;             //Middle color of button frame.
    radio.aColorButton[ 2]  = GUI_DARKGRAY;         //Inner color of button frame.
    radio.aColorButton[ 3]  = GUI_WHITE;         //Inner color of button.
    radio.ButtonSize        = 26;                   //Size of the button in pixels.
    RADIO_SetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_PRESSED );

    RADIO_GetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_UNPRESSED );
    radio.aColorButton[ 0]  = GUI_GRAY;             //Outer color of button frame.
    radio.aColorButton[ 1]  = GUI_GRAY;             //Middle color of button frame.
    radio.aColorButton[ 2]  = GUI_DARKGRAY;         //Inner color of button frame.
    radio.aColorButton[ 3]  = GUI_BLACK;            //Inner color of button.
    radio.ButtonSize        = 26;                   //Size of the button in pixels.
    RADIO_SetSkinFlexProps( &radio, RADIO_SKINFLEX_PI_UNPRESSED );

    RADIO_SetDefaultTextColor( GUI_LIGHTGRAY );
    RADIO_SetDefaultFocusColor( GUI_WHITE );

    SCROLLBAR_SetDefaultColor(      GUI_LIGHTBLUE,      SCROLLBAR_CI_THUMB      );
    SCROLLBAR_SetDefaultSkinClassic();

    SPINBOX_SetDefaultSkinClassic();

    SWIPELIST_SetDefaultFont( SWIPELIST_FI_SEP_ITEM,    &GUI_FontTahoma30 );
    SWIPELIST_SetDefaultFont( SWIPELIST_FI_ITEM_HEADER, &GUI_FontTahoma30 );
    SWIPELIST_SetDefaultFont( SWIPELIST_FI_ITEM_TEXT,   &GUI_FontTahoma30 );
    SWIPELIST_SetDefaultTextColor( SWIPELIST_CI_ITEM_TEXT_UNSEL, GUI_LIGHTGRAY );
    SWIPELIST_SetDefaultTextColor( SWIPELIST_CI_ITEM_TEXT_SEL, GUI_LIGHTGRAY );
    SWIPELIST_SetDefaultBkColor( SWIPELIST_CI_BK_ITEM_UNSEL, GUI_BLACK );
    SWIPELIST_SetDefaultBkColor( SWIPELIST_CI_BK_ITEM_SEL, GUI_BLACK );

    TEXT_SetDefaultTextColor( GUI_LIGHTGRAY );
    TEXT_SetDefaultFont( &GUI_FontTahoma40 );

    //WINDOW_SetDefaultBkColor( GUI_GRAY_2F );
    //WINDOW_SetDefaultBkColor( GUI_GRAY );
    WINDOW_SetDefaultBkColor( GUI_BLACK );
}


void
gui_set_defaults( void )
{
    gui_set_defaults_standart();
    //gui_set_defaults_neon();
    //gui_set_defaults_lite();
}