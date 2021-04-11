/* -------------------------------------------------------------------------------- */
/* -- µGUI - Generic GUI module (C)Achim Döbler, 2015                            -- */
/* -------------------------------------------------------------------------------- */
// µGUI is a generic GUI module for embedded systems.
// This is a free software that is open for education, research and commercial
// developments under license policy of following terms.
//
//  Copyright (C) 2015, Achim Döbler, all rights reserved.
//  URL: http://www.embeddedlightning.com/
//
// * The µGUI module is a free software and there is NO WARRANTY.
// * No restriction on use. You can use, modify and redistribute it for
//   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
// * Redistributions of source code must retain the above copyright notice.
//
/* -------------------------------------------------------------------------------- */
#ifndef __UGUI_H
#define __UGUI_H

//#include "system.h"
#include "ugui_config.h"
#include <defines.h>

/* -------------------------------------------------------------------------------- */
/* -- µGUI FONTS                                                                 -- */
/* -- Source: http://www.mikrocontroller.net/user/show/benedikt                  -- */
/* -------------------------------------------------------------------------------- */
typedef enum
{
	FONT_TYPE_1BPP,
	FONT_TYPE_8BPP
} FONT_TYPE;

typedef struct
{
   UINT8* p;
   FONT_TYPE font_type;
   INT16 char_width;
   INT16 char_height;
   UINT16 start_char;
   UINT16 end_char;
   UINT8  *widths;
} UG_FONT;

#ifdef USE_FONT_4X6
   extern const UG_FONT FONT_4X6;
#endif
#ifdef USE_FONT_5X8
   extern const UG_FONT FONT_5X8;
#endif
#ifdef USE_FONT_5X12
   extern const UG_FONT FONT_5X12;
#endif
#ifdef USE_FONT_6X8
   extern const UG_FONT FONT_6X8;
#endif
#ifdef USE_FONT_6X10
   extern const UG_FONT FONT_6X10;
#endif
#ifdef USE_FONT_7X12
   extern const UG_FONT FONT_7X12;
#endif
#ifdef USE_FONT_8X8
   extern const UG_FONT FONT_8X8;
#endif
#ifdef USE_FONT_8X12
   extern const UG_FONT FONT_8X12;
#endif
#ifdef USE_FONT_8X12_CYRILLIC
   extern const UG_FONT FONT_8X12;
#endif
#ifdef USE_FONT_8X14
   extern const UG_FONT FONT_8X14;
#endif
#ifdef USE_FONT_10X16
   extern const UG_FONT FONT_10X16;
#endif
#ifdef USE_FONT_12X16
   extern const UG_FONT FONT_12X16;
#endif
#ifdef USE_FONT_12X20
   extern const UG_FONT FONT_12X20;
#endif
#ifdef USE_FONT_16X26
   extern const UG_FONT FONT_16X26;
#endif
#ifdef USE_FONT_22X36
   extern const UG_FONT FONT_22X36;
#endif
#ifdef USE_FONT_24X40
   extern const UG_FONT FONT_24X40;
#endif
#ifdef USE_FONT_32X53
   extern const UG_FONT FONT_32X53;
#endif

/* -------------------------------------------------------------------------------- */
/* -- TYPEDEFS                                                                   -- */
/* -------------------------------------------------------------------------------- */
typedef struct S_OBJECT                               UG_OBJECT;
typedef struct S_WINDOW                               UG_WINDOW;
typedef INT8                                         UG_RESULT;
typedef UINT16                                        UG_COLOR;
/* -------------------------------------------------------------------------------- */
/* -- DEFINES                                                                    -- */
/* -------------------------------------------------------------------------------- */
#ifndef NULL
   #define NULL ((void*) 0)
#endif

/* Alignments */
#define ALIGN_H_LEFT                                  (1<<0)
#define ALIGN_H_CENTER                                (1<<1)
#define ALIGN_H_RIGHT                                 (1<<2)
#define ALIGN_V_TOP                                   (1<<3)
#define ALIGN_V_CENTER                                (1<<4)
#define ALIGN_V_BOTTOM                                (1<<5)
#define ALIGN_BOTTOM_RIGHT                            (ALIGN_V_BOTTOM|ALIGN_H_RIGHT)
#define ALIGN_BOTTOM_CENTER                           (ALIGN_V_BOTTOM|ALIGN_H_CENTER)
#define ALIGN_BOTTOM_LEFT                             (ALIGN_V_BOTTOM|ALIGN_H_LEFT)
#define ALIGN_CENTER_RIGHT                            (ALIGN_V_CENTER|ALIGN_H_RIGHT)
#define ALIGN_CENTER                                  (ALIGN_V_CENTER|ALIGN_H_CENTER)
#define ALIGN_CENTER_LEFT                             (ALIGN_V_CENTER|ALIGN_H_LEFT)
#define ALIGN_TOP_RIGHT                               (ALIGN_V_TOP|ALIGN_H_RIGHT)
#define ALIGN_TOP_CENTER                              (ALIGN_V_TOP|ALIGN_H_CENTER)
#define ALIGN_TOP_LEFT                                (ALIGN_V_TOP|ALIGN_H_LEFT)

/* Default IDs */
#define OBJ_ID_0                                      0
#define OBJ_ID_1                                      1
#define OBJ_ID_2                                      2
#define OBJ_ID_3                                      3
#define OBJ_ID_4                                      4
#define OBJ_ID_5                                      5
#define OBJ_ID_6                                      6
#define OBJ_ID_7                                      7
#define OBJ_ID_8                                      8
#define OBJ_ID_9                                      9
#define OBJ_ID_10                                     10
#define OBJ_ID_11                                     11
#define OBJ_ID_12                                     12
#define OBJ_ID_13                                     13
#define OBJ_ID_14                                     14
#define OBJ_ID_15                                     15
#define OBJ_ID_16                                     16
#define OBJ_ID_17                                     17
#define OBJ_ID_18                                     18
#define OBJ_ID_19                                     19

/* -------------------------------------------------------------------------------- */
/* -- FUNCTION RESULTS                                                           -- */
/* -------------------------------------------------------------------------------- */
#define UG_RESULT_FAIL                               -1
#define UG_RESULT_OK                                  0

/* -------------------------------------------------------------------------------- */
/* -- UNIVERSAL STRUCTURES                                                       -- */
/* -------------------------------------------------------------------------------- */
/* Area structure */
typedef struct
{
   INT16 xs;
   INT16 ys;
   INT16 xe;
   INT16 ye;
} UG_AREA;

/* Text structure */
typedef struct
{
   char* str;
   const UG_FONT* font;
   UG_AREA a;
   UG_COLOR fc;
   UG_COLOR bc;
   UINT8 align;
   INT16 h_space;
   INT16 v_space;
} UG_TEXT;

/* -------------------------------------------------------------------------------- */
/* -- BITMAP                                                                     -- */
/* -------------------------------------------------------------------------------- */
typedef struct
{
   void* p;
   UINT16 width;
   UINT16 height;
   UINT8 bpp;
   UINT8 colors;
} UG_BMP;

#define BMP_BPP_1                                     (1<<0)
#define BMP_BPP_2                                     (1<<1)
#define BMP_BPP_4                                     (1<<2)
#define BMP_BPP_8                                     (1<<3)
#define BMP_BPP_16                                    (1<<4)
#define BMP_BPP_32                                    (1<<5)
#define BMP_RGB888                                    (1<<0)
#define BMP_RGB565                                    (1<<1)
#define BMP_RGB555                                    (1<<2)

/* -------------------------------------------------------------------------------- */
/* -- MESSAGE                                                                    -- */
/* -------------------------------------------------------------------------------- */
/* Message structure */
typedef struct
{
   UINT8 type;
   UINT8 id;
   UINT8 sub_id;
   UINT8 event;
   void* src;
} UG_MESSAGE;

/* Message types */
#define MSG_TYPE_NONE                                 0
#define MSG_TYPE_WINDOW                               1
#define MSG_TYPE_OBJECT                               2

/* -------------------------------------------------------------------------------- */
/* -- TOUCH                                                                      -- */
/* -------------------------------------------------------------------------------- */
/* Touch structure */
typedef struct
{
   UINT8 state;
   INT16 xp;
   INT16 yp;
} UG_TOUCH;


#define TOUCH_STATE_PRESSED                           1
#define TOUCH_STATE_RELEASED                          0

/* -------------------------------------------------------------------------------- */
/* -- OBJECTS                                                                    -- */
/* -------------------------------------------------------------------------------- */
/* Object structure */
struct S_OBJECT
{
   UINT8 state;                              /* object state                               */
   UINT8 touch_state;                        /* object touch state                         */
   void (*update) (UG_WINDOW*,UG_OBJECT*);   /* pointer to object-specific update function */
   UG_AREA a_abs;                            /* absolute area of the object                */
   UG_AREA a_rel;                            /* relative area of the object                */
   UINT8 type;                               /* object type                                */
   UINT8 id;                                 /* object ID                                  */
   UINT8 event;                              /* object-specific events                     */
   void* data;                               /* pointer to object-specific data            */
};

/* Currently supported objects */
#define OBJ_TYPE_NONE                                 0
#define OBJ_TYPE_BUTTON                               1
#define OBJ_TYPE_TEXTBOX                              2
#define OBJ_TYPE_IMAGE                                3
#define OBJ_TYPE_CHECKBOX                             4

/* Standard object events */
#define OBJ_EVENT_NONE                                0
#define OBJ_EVENT_CLICKED                             1
#ifdef USE_PRERENDER_EVENT
#define OBJ_EVENT_PRERENDER                           2
#endif
#ifdef USE_POSTRENDER_EVENT
#define OBJ_EVENT_POSTRENDER                          3
#endif
#define OBJ_EVENT_PRESSED                             4
#define OBJ_EVENT_RELEASED                            5


/* Object states */
#define OBJ_STATE_FREE                                (1<<0)
#define OBJ_STATE_VALID                               (1<<1)
#define OBJ_STATE_BUSY                                (1<<2)
#define OBJ_STATE_VISIBLE                             (1<<3)
#define OBJ_STATE_ENABLE                              (1<<4)
#define OBJ_STATE_UPDATE                              (1<<5)
#define OBJ_STATE_REDRAW                              (1<<6)
#define OBJ_STATE_TOUCH_ENABLE                        (1<<7)
#define OBJ_STATE_INIT                                (OBJ_STATE_FREE | OBJ_STATE_VALID)

/* Object touch states */
#define OBJ_TOUCH_STATE_CHANGED                       (1<<0)
#define OBJ_TOUCH_STATE_PRESSED_ON_OBJECT             (1<<1)
#define OBJ_TOUCH_STATE_PRESSED_OUTSIDE_OBJECT        (1<<2)
#define OBJ_TOUCH_STATE_RELEASED_ON_OBJECT            (1<<3)
#define OBJ_TOUCH_STATE_RELEASED_OUTSIDE_OBJECT       (1<<4)
#define OBJ_TOUCH_STATE_IS_PRESSED_ON_OBJECT          (1<<5)
#define OBJ_TOUCH_STATE_IS_PRESSED                    (1<<6)
#define OBJ_TOUCH_STATE_CLICK_ON_OBJECT               (1<<7)
#define OBJ_TOUCH_STATE_INIT                          0

/* -------------------------------------------------------------------------------- */
/* -- WINDOW                                                                     -- */
/* -------------------------------------------------------------------------------- */
/* Title structure */
typedef struct
{
   char* str;
   const UG_FONT* font;
   INT8 h_space;
   INT8 v_space;
   UINT8 align;
   UG_COLOR fc;
   UG_COLOR bc;
   UG_COLOR ifc;
   UG_COLOR ibc;
   UINT8 height;
} UG_TITLE;

/* Window structure */
struct S_WINDOW
{
   UINT8 objcnt;
   UG_OBJECT* objlst;
   UINT8 state;
   UG_COLOR fc;
   UG_COLOR bc;
   INT16 xs;
   INT16 ys;
   INT16 xe;
   INT16 ye;
   UINT8 style;
   UG_TITLE title;
   void (*cb)( UG_MESSAGE* );
};

/* Window states */
#define WND_STATE_FREE                                (1<<0)
#define WND_STATE_VALID                               (1<<1)
#define WND_STATE_BUSY                                (1<<2)
#define WND_STATE_VISIBLE                             (1<<3)
#define WND_STATE_ENABLE                              (1<<4)
#define WND_STATE_UPDATE                              (1<<5)
#define WND_STATE_REDRAW_TITLE                        (1<<6)

/* Window styles */
#define WND_STYLE_2D                                  (0<<0)
#define WND_STYLE_3D                                  (1<<0)
#define WND_STYLE_HIDE_TITLE                          (0<<1)
#define WND_STYLE_SHOW_TITLE                          (1<<1)

/* -------------------------------------------------------------------------------- */
/* -- BUTTON OBJECT                                                              -- */
/* -------------------------------------------------------------------------------- */
/* Button structure */
typedef struct
{
   UINT8 state;
   UINT8 style;
   UG_COLOR fc;
   UG_COLOR bc;
   UG_COLOR afc;
   UG_COLOR abc;
   const UG_FONT* font;
   UINT8 align;
   INT8 h_space;
   INT8 v_space;
   char* str;
}UG_BUTTON;

/* Default button IDs */
#define BTN_ID_0                                      OBJ_ID_0
#define BTN_ID_1                                      OBJ_ID_1
#define BTN_ID_2                                      OBJ_ID_2
#define BTN_ID_3                                      OBJ_ID_3
#define BTN_ID_4                                      OBJ_ID_4
#define BTN_ID_5                                      OBJ_ID_5
#define BTN_ID_6                                      OBJ_ID_6
#define BTN_ID_7                                      OBJ_ID_7
#define BTN_ID_8                                      OBJ_ID_8
#define BTN_ID_9                                      OBJ_ID_9
#define BTN_ID_10                                     OBJ_ID_10
#define BTN_ID_11                                     OBJ_ID_11
#define BTN_ID_12                                     OBJ_ID_12
#define BTN_ID_13                                     OBJ_ID_13
#define BTN_ID_14                                     OBJ_ID_14
#define BTN_ID_15                                     OBJ_ID_15
#define BTN_ID_16                                     OBJ_ID_16
#define BTN_ID_17                                     OBJ_ID_17
#define BTN_ID_18                                     OBJ_ID_18
#define BTN_ID_19                                     OBJ_ID_19

/* Button states */
#define BTN_STATE_RELEASED                            (0<<0)
#define BTN_STATE_PRESSED                             (1<<0)
#define BTN_STATE_ALWAYS_REDRAW                       (1<<1)

/* Button style */
#define BTN_STYLE_2D                                  (0<<0)
#define BTN_STYLE_3D                                  (1<<0)
#define BTN_STYLE_TOGGLE_COLORS                       (1<<1)
#define BTN_STYLE_USE_ALTERNATE_COLORS                (1<<2)
#define BTN_STYLE_NO_BORDERS                          (1<<3)
#define BTN_STYLE_NO_FILL                             (1<<4)

/* Button events */
#define BTN_EVENT_CLICKED                             OBJ_EVENT_CLICKED

/* -------------------------------------------------------------------------------- */
/* -- CHECKBOX OBJECT                                                            -- */
/* -------------------------------------------------------------------------------- */
/* Checkbox structure */
typedef struct
{
   UINT8 state;
   UINT8 style;
   UG_COLOR fc;
   UG_COLOR bc;
   UG_COLOR afc;
   UG_COLOR abc;
   const UG_FONT* font;
   UINT8 align;
   INT8 h_space;
   INT8 v_space;
   char* str;
   UINT8 checked;
}UG_CHECKBOX;

/* Default checkbox IDs */
#define CHB_ID_0                                      OBJ_ID_0
#define CHB_ID_1                                      OBJ_ID_1
#define CHB_ID_2                                      OBJ_ID_2
#define CHB_ID_3                                      OBJ_ID_3
#define CHB_ID_4                                      OBJ_ID_4
#define CHB_ID_5                                      OBJ_ID_5
#define CHB_ID_6                                      OBJ_ID_6
#define CHB_ID_7                                      OBJ_ID_7
#define CHB_ID_8                                      OBJ_ID_8
#define CHB_ID_9                                      OBJ_ID_9
#define CHB_ID_10                                     OBJ_ID_10
#define CHB_ID_11                                     OBJ_ID_11
#define CHB_ID_12                                     OBJ_ID_12
#define CHB_ID_13                                     OBJ_ID_13
#define CHB_ID_14                                     OBJ_ID_14
#define CHB_ID_15                                     OBJ_ID_15
#define CHB_ID_16                                     OBJ_ID_16
#define CHB_ID_17                                     OBJ_ID_17
#define CHB_ID_18                                     OBJ_ID_18
#define CHB_ID_19                                     OBJ_ID_19

/* Checkbox states */
#define CHB_STATE_RELEASED                            (0<<0)
#define CHB_STATE_PRESSED                             (1<<0)
#define CHB_STATE_ALWAYS_REDRAW                       (1<<1)

/* Checkbox style */
#define CHB_STYLE_2D                                  (0<<0)
#define CHB_STYLE_3D                                  (1<<0)
#define CHB_STYLE_TOGGLE_COLORS                       (1<<1)
#define CHB_STYLE_USE_ALTERNATE_COLORS                (1<<2)
#define CHB_STYLE_NO_BORDERS                          (1<<3)
#define CHB_STYLE_NO_FILL                             (1<<4)

/* Checkbox events */
#define CHB_EVENT_CLICKED                             OBJ_EVENT_CLICKED


/* -------------------------------------------------------------------------------- */
/* -- TEXTBOX OBJECT                                                             -- */
/* -------------------------------------------------------------------------------- */
/* Textbox structure */
typedef struct
{
   char* str;
   const UG_FONT* font;
   UINT8 style;
   UG_COLOR fc;
   UG_COLOR bc;
   UINT8 align;
   INT8 h_space;
   INT8 v_space;
} UG_TEXTBOX;

/* Default textbox IDs */
#define TXB_ID_0                                      OBJ_ID_0
#define TXB_ID_1                                      OBJ_ID_1
#define TXB_ID_2                                      OBJ_ID_2
#define TXB_ID_3                                      OBJ_ID_3
#define TXB_ID_4                                      OBJ_ID_4
#define TXB_ID_5                                      OBJ_ID_5
#define TXB_ID_6                                      OBJ_ID_6
#define TXB_ID_7                                      OBJ_ID_7
#define TXB_ID_8                                      OBJ_ID_8
#define TXB_ID_9                                      OBJ_ID_9
#define TXB_ID_10                                     OBJ_ID_10
#define TXB_ID_11                                     OBJ_ID_11
#define TXB_ID_12                                     OBJ_ID_12
#define TXB_ID_13                                     OBJ_ID_13
#define TXB_ID_14                                     OBJ_ID_14
#define TXB_ID_15                                     OBJ_ID_15
#define TXB_ID_16                                     OBJ_ID_16
#define TXB_ID_17                                     OBJ_ID_17
#define TXB_ID_18                                     OBJ_ID_18
#define TXB_ID_19                                     OBJ_ID_19

/* -------------------------------------------------------------------------------- */
/* -- IMAGE OBJECT                                                               -- */
/* -------------------------------------------------------------------------------- */
/* Image structure */
typedef struct
{
   void* img;
   UINT8 type;
} UG_IMAGE;

/* Default image IDs */
#define IMG_ID_0                                      OBJ_ID_0
#define IMG_ID_1                                      OBJ_ID_1
#define IMG_ID_2                                      OBJ_ID_2
#define IMG_ID_3                                      OBJ_ID_3
#define IMG_ID_4                                      OBJ_ID_4
#define IMG_ID_5                                      OBJ_ID_5
#define IMG_ID_6                                      OBJ_ID_6
#define IMG_ID_7                                      OBJ_ID_7
#define IMG_ID_8                                      OBJ_ID_8
#define IMG_ID_9                                      OBJ_ID_9
#define IMG_ID_10                                     OBJ_ID_10
#define IMG_ID_11                                     OBJ_ID_11
#define IMG_ID_12                                     OBJ_ID_12
#define IMG_ID_13                                     OBJ_ID_13
#define IMG_ID_14                                     OBJ_ID_14
#define IMG_ID_15                                     OBJ_ID_15
#define IMG_ID_16                                     OBJ_ID_16
#define IMG_ID_17                                     OBJ_ID_17
#define IMG_ID_18                                     OBJ_ID_18
#define IMG_ID_19                                     OBJ_ID_19

/* Image types */
#define IMG_TYPE_BMP                                  (1<<0)

/* -------------------------------------------------------------------------------- */
/* -- µGUI CORE STRUCTURE                                                        -- */
/* -------------------------------------------------------------------------------- */
typedef struct
{
   INT16 x_dim;
   INT16 y_dim;
   UG_TOUCH touch;
   UG_WINDOW* next_window;
   UG_WINDOW* active_window;
   UG_WINDOW* last_window;
   struct
   {
      INT16 x_pos;
      INT16 y_pos;
      INT16 x_start;
      INT16 y_start;
      INT16 x_end;
      INT16 y_end;
      UG_COLOR fore_color;
      UG_COLOR back_color;
   } console;
   UG_FONT font;
   INT8 char_h_space;
   INT8 char_v_space;
   UG_COLOR fore_color;
   UG_COLOR back_color;
   UG_COLOR desktop_color;
   UINT8 state;
} UG_GUI;

#define UG_SATUS_WAIT_FOR_UPDATE                      (1<<0)

/* -------------------------------------------------------------------------------- */
/* -- µGUI COLORS                                                                -- */
/* -- Source: http://www.rapidtables.com/web/color/RGB_Color.htm                 -- */
/* -------------------------------------------------------------------------------- */
#ifdef USE_COLOR_RGB565
#define C_MAROON                       0x8000
#define C_DARK_RED                     0x8800
#define C_BROWN                        0xA145
#define C_FIREBRICK                    0xB104
#define C_CRIMSON                      0xD8A7
#define C_RED                          0xF800
#define C_TOMATO                       0xFB09
#define C_CORAL                        0xFBEA
#define C_INDIAN_RED                   0xCAEB
#define C_LIGHT_CORAL                  0xEC10
#define C_DARK_SALMON                  0xE4AF
#define C_SALMON                       0xF40E
#define C_LIGHT_SALMON                 0xFD0F
#define C_ORANGE_RED                   0xFA20
#define C_DARK_ORANGE                  0xFC60
#define C_ORANGE                       0xFD20
#define C_GOLD                         0xFEA0
#define C_DARK_GOLDEN_ROD              0xB421
#define C_GOLDEN_ROD                   0xDD24
#define C_PALE_GOLDEN_ROD              0xEF35
#define C_DARK_KHAKI                   0xBDAD
#define C_KHAKI                        0xEF31
#define C_OLIVE                        0x8400
#define C_YELLOW                       0xFFE0
#define C_YELLOW_GREEN                 0x9E66
#define C_DARK_OLIVE_GREEN             0x5346
#define C_OLIVE_DRAB                   0x6C64
#define C_LAWN_GREEN                   0x7FC0
#define C_CHART_REUSE                  0x7FE0
#define C_GREEN_YELLOW                 0xAFE6
#define C_DARK_GREEN                   0x0320
#define C_GREEN                        0x07E0
#define C_FOREST_GREEN                 0x2444
#define C_LIME                         0x07E0
#define C_LIME_GREEN                   0x3666
#define C_LIGHT_GREEN                  0x9772
#define C_PALE_GREEN                   0x97D2
#define C_DARK_SEA_GREEN               0x8DD1
#define C_MEDIUM_SPRING_GREEN          0x07D3
#define C_SPRING_GREEN                 0x07EF
#define C_SEA_GREEN                    0x344B
#define C_MEDIUM_AQUA_MARINE           0x6675
#define C_MEDIUM_SEA_GREEN             0x3D8E
#define C_LIGHT_SEA_GREEN              0x2595
#define C_DARK_SLATE_GRAY              0x328A
#define C_TEAL                         0x0410
#define C_DARK_CYAN                    0x0451
#define C_AQUA                         0x07FF
#define C_CYAN                         0x07FF
#define C_LIGHT_CYAN                   0xDFFF
#define C_DARK_TURQUOISE               0x0679
#define C_TURQUOISE                    0x46F9
#define C_MEDIUM_TURQUOISE             0x4E99
#define C_PALE_TURQUOISE               0xAF7D
#define C_AQUA_MARINE                  0x7FFA
#define C_POWDER_BLUE                  0xAEFC
#define C_CADET_BLUE                   0x64F3
#define C_STEEL_BLUE                   0x4C16
#define C_CORN_FLOWER_BLUE             0x64BD
#define C_DEEP_SKY_BLUE                0x05FF
#define C_DODGER_BLUE                  0x249F
#define C_LIGHT_BLUE                   0xAEBC
#define C_SKY_BLUE                     0x867D
#define C_LIGHT_SKY_BLUE               0x867E
#define C_MIDNIGHT_BLUE                0x18CE
#define C_NAVY                         0x0010
#define C_DARK_BLUE                    0x0011
#define C_MEDIUM_BLUE                  0x0019
#define C_BLUE                         0x001F
#define C_ROYAL_BLUE                   0x435B
#define C_BLUE_VIOLET                  0x897B
#define C_INDIGO                       0x4810
#define C_DARK_SLATE_BLUE              0x49F1
#define C_SLATE_BLUE                   0x6AD9
#define C_MEDIUM_SLATE_BLUE            0x7B5D
#define C_MEDIUM_PURPLE                0x939B
#define C_DARK_MAGENTA                 0x8811
#define C_DARK_VIOLET                  0x901A
#define C_DARK_ORCHID                  0x9999
#define C_MEDIUM_ORCHID                0xBABA
#define C_PURPLE                       0x8010
#define C_THISTLE                      0xD5FA
#define C_PLUM                         0xDD1B
#define C_VIOLET                       0xEC1D
#define C_MAGENTA                      0xF81F
#define C_ORCHID                       0xDB9A
#define C_MEDIUM_VIOLET_RED            0xC0B0
#define C_PALE_VIOLET_RED              0xDB92
#define C_DEEP_PINK                    0xF8B2
#define C_HOT_PINK                     0xFB56
#define C_LIGHT_PINK                   0xFDB7
#define C_PINK                         0xFDF9
#define C_ANTIQUE_WHITE                0xF75A
#define C_BEIGE                        0xF7BB
#define C_BISQUE                       0xFF18
#define C_BLANCHED_ALMOND              0xFF59
#define C_WHEAT                        0xF6F6
#define C_CORN_SILK                    0xFFBB
#define C_LEMON_CHIFFON                0xFFD9
#define C_LIGHT_GOLDEN_ROD_YELLOW      0xF7DA
#define C_LIGHT_YELLOW                 0xFFFB
#define C_SADDLE_BROWN                 0x8A22
#define C_SIENNA                       0x9A85
#define C_CHOCOLATE                    0xD344
#define C_PERU                         0xCC28
#define C_SANDY_BROWN                  0xF52C
#define C_BURLY_WOOD                   0xDDB0
#define C_TAN                          0xD591
#define C_ROSY_BROWN                   0xBC71
#define C_MOCCASIN                     0xFF16
#define C_NAVAJO_WHITE                 0xFEF5
#define C_PEACH_PUFF                   0xFED6
#define C_MISTY_ROSE                   0xFF1B
#define C_LAVENDER_BLUSH               0xFF7E
#define C_LINEN                        0xF77C
#define C_OLD_LACE                     0xFFBC
#define C_PAPAYA_WHIP                  0xFF7A
#define C_SEA_SHELL                    0xFFBD
#define C_MINT_CREAM                   0xF7FE
#define C_SLATE_GRAY                   0x7412
#define C_LIGHT_SLATE_GRAY             0x7453
#define C_LIGHT_STEEL_BLUE             0xAE1B
#define C_LAVENDER                     0xE73E
#define C_FLORAL_WHITE                 0xFFDD
#define C_ALICE_BLUE                   0xEFBF
#define C_GHOST_WHITE                  0xF7BF
#define C_HONEYDEW                     0xEFFD
#define C_IVORY                        0xFFFD
#define C_AZURE                        0xEFFF
#define C_SNOW                         0xFFDE
#define C_BLACK                        0x0000
#define C_DIM_GRAY                     0x6B4D
#define C_GRAY                         0x8410
#define C_DARK_GRAY                    0xAD55
#define C_SILVER                       0xBDF7
#define C_LIGHT_GRAY                   0xD69A
#define C_GAINSBORO                    0xDEDB
#define C_WHITE_SMOKE                  0xF7BE
#define C_WHITE                        0xFFFF
#endif


/* -------------------------------------------------------------------------------- */
/* -- PROTOTYPES                                                                 -- */
/* -------------------------------------------------------------------------------- */
/* Classic functions */
INT16 UG_Init( UG_GUI* g, INT16 x, INT16 y );
INT16 UG_SelectGUI( UG_GUI* g );
void UG_FontSelect( const UG_FONT* font );
void UG_FillScreen( UG_COLOR c );
void UG_FillFrame( INT16 x1, INT16 y1, INT16 x2, INT16 y2, UG_COLOR c );
void UG_FillRoundFrame( INT16 x1, INT16 y1, INT16 x2, INT16 y2, INT16 r, UG_COLOR c );
void UG_DrawMesh( INT16 x1, INT16 y1, INT16 x2, INT16 y2, UG_COLOR c );
void UG_DrawFrame( INT16 x1, INT16 y1, INT16 x2, INT16 y2, UG_COLOR c );
void UG_DrawRoundFrame( INT16 x1, INT16 y1, INT16 x2, INT16 y2, INT16 r, UG_COLOR c );
void UG_DrawPixel( INT16 x0, INT16 y0, UG_COLOR c );
void UG_DrawCircle( INT16 x0, INT16 y0, INT16 r, UG_COLOR c );
void UG_FillCircle( INT16 x0, INT16 y0, INT16 r, UG_COLOR c );
void UG_DrawArc( INT16 x0, INT16 y0, INT16 r, UINT8 s, UG_COLOR c );
void UG_DrawLine( INT16 x1, INT16 y1, INT16 x2, INT16 y2, UG_COLOR c );
void UG_PutString( INT16 x, INT16 y, char* str );
void UG_PutChar( char chr, INT16 x, INT16 y, UG_COLOR fc, UG_COLOR bc );
void UG_ConsolePutString( char* str );
void UG_ConsoleSetArea( INT16 xs, INT16 ys, INT16 xe, INT16 ye );
void UG_ConsoleSetForecolor( UG_COLOR c );
void UG_ConsoleSetBackcolor( UG_COLOR c );
void UG_SetForecolor( UG_COLOR c );
void UG_SetBackcolor( UG_COLOR c );
INT16 UG_GetXDim( void );
INT16 UG_GetYDim( void );
void UG_FontSetHSpace( UINT16 s );
void UG_FontSetVSpace( UINT16 s );

/* Miscellaneous functions */
void UG_WaitForUpdate( void );
void UG_Update( void );
void UG_DrawBMP( INT16 xp, INT16 yp, UG_BMP* bmp );
void UG_TouchUpdate( INT16 xp, INT16 yp, UINT8 state );

/* Window functions */
UG_RESULT UG_WindowCreate( UG_WINDOW* wnd, UG_OBJECT* objlst, UINT8 objcnt, void (*cb)( UG_MESSAGE* ) );
UG_RESULT UG_WindowDelete( UG_WINDOW* wnd );
UG_RESULT UG_WindowShow( UG_WINDOW* wnd );
UG_RESULT UG_WindowHide( UG_WINDOW* wnd );
UG_RESULT UG_WindowResize( UG_WINDOW* wnd, INT16 xs, INT16 ys, INT16 xe, INT16 ye );
UG_RESULT UG_WindowAlert( UG_WINDOW* wnd );
UG_RESULT UG_WindowSetForeColor( UG_WINDOW* wnd, UG_COLOR fc );
UG_RESULT UG_WindowSetBackColor( UG_WINDOW* wnd, UG_COLOR bc );
UG_RESULT UG_WindowSetTitleTextColor( UG_WINDOW* wnd, UG_COLOR c );
UG_RESULT UG_WindowSetTitleColor( UG_WINDOW* wnd, UG_COLOR c );
UG_RESULT UG_WindowSetTitleInactiveTextColor( UG_WINDOW* wnd, UG_COLOR c );
UG_RESULT UG_WindowSetTitleInactiveColor( UG_WINDOW* wnd, UG_COLOR c );
UG_RESULT UG_WindowSetTitleText( UG_WINDOW* wnd, char* str );
UG_RESULT UG_WindowSetTitleTextFont( UG_WINDOW* wnd, const UG_FONT* font );
UG_RESULT UG_WindowSetTitleTextHSpace( UG_WINDOW* wnd, INT8 hs );
UG_RESULT UG_WindowSetTitleTextVSpace( UG_WINDOW* wnd, INT8 vs );
UG_RESULT UG_WindowSetTitleTextAlignment( UG_WINDOW* wnd, UINT8 align );
UG_RESULT UG_WindowSetTitleHeight( UG_WINDOW* wnd, UINT8 height );
UG_RESULT UG_WindowSetXStart( UG_WINDOW* wnd, INT16 xs );
UG_RESULT UG_WindowSetYStart( UG_WINDOW* wnd, INT16 ys );
UG_RESULT UG_WindowSetXEnd( UG_WINDOW* wnd, INT16 xe );
UG_RESULT UG_WindowSetYEnd( UG_WINDOW* wnd, INT16 ye );
UG_RESULT UG_WindowSetStyle( UG_WINDOW* wnd, UINT8 style );
UG_COLOR UG_WindowGetForeColor( UG_WINDOW* wnd );
UG_COLOR UG_WindowGetBackColor( UG_WINDOW* wnd );
UG_COLOR UG_WindowGetTitleTextColor( UG_WINDOW* wnd );
UG_COLOR UG_WindowGetTitleColor( UG_WINDOW* wnd );
UG_COLOR UG_WindowGetTitleInactiveTextColor( UG_WINDOW* wnd );
UG_COLOR UG_WindowGetTitleInactiveColor( UG_WINDOW* wnd );
char* UG_WindowGetTitleText( UG_WINDOW* wnd );
UG_FONT* UG_WindowGetTitleTextFont( UG_WINDOW* wnd );
INT8 UG_WindowGetTitleTextHSpace( UG_WINDOW* wnd );
INT8 UG_WindowGetTitleTextVSpace( UG_WINDOW* wnd );
UINT8 UG_WindowGetTitleTextAlignment( UG_WINDOW* wnd );
UINT8 UG_WindowGetTitleHeight( UG_WINDOW* wnd );
INT16 UG_WindowGetXStart( UG_WINDOW* wnd );
INT16 UG_WindowGetYStart( UG_WINDOW* wnd );
INT16 UG_WindowGetXEnd( UG_WINDOW* wnd );
INT16 UG_WindowGetYEnd( UG_WINDOW* wnd );
UINT8 UG_WindowGetStyle( UG_WINDOW* wnd );
UG_RESULT UG_WindowGetArea( UG_WINDOW* wnd, UG_AREA* a );
INT16 UG_WindowGetInnerWidth( UG_WINDOW* wnd );
INT16 UG_WindowGetOuterWidth( UG_WINDOW* wnd );
INT16 UG_WindowGetInnerHeight( UG_WINDOW* wnd );
INT16 UG_WindowGetOuterHeight( UG_WINDOW* wnd );

/* Button functions */
UG_RESULT UG_ButtonCreate( UG_WINDOW* wnd, UG_BUTTON* btn, UINT8 id, INT16 xs, INT16 ys, INT16 xe, INT16 ye );
UG_RESULT UG_ButtonDelete( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_ButtonShow( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_ButtonHide( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_ButtonSetForeColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR fc );
UG_RESULT UG_ButtonSetBackColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR bc );
UG_RESULT UG_ButtonSetAlternateForeColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR afc );
UG_RESULT UG_ButtonSetAlternateBackColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR abc );
UG_RESULT UG_ButtonSetText( UG_WINDOW* wnd, UINT8 id, char* str );
UG_RESULT UG_ButtonSetFont( UG_WINDOW* wnd, UINT8 id, const UG_FONT* font );
UG_RESULT UG_ButtonSetStyle( UG_WINDOW* wnd, UINT8 id, UINT8 style );
UG_RESULT UG_ButtonSetHSpace( UG_WINDOW* wnd, UINT8 id, INT8 hs );
UG_RESULT UG_ButtonSetVSpace( UG_WINDOW* wnd, UINT8 id, INT8 vs );
UG_RESULT UG_ButtonSetAlignment( UG_WINDOW* wnd, UINT8 id, UINT8 align );
UG_COLOR UG_ButtonGetForeColor( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_ButtonGetBackColor( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_ButtonGetAlternateForeColor( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_ButtonGetAlternateBackColor( UG_WINDOW* wnd, UINT8 id );
char* UG_ButtonGetText( UG_WINDOW* wnd, UINT8 id );
UG_FONT* UG_ButtonGetFont( UG_WINDOW* wnd, UINT8 id );
UINT8 UG_ButtonGetStyle( UG_WINDOW* wnd, UINT8 id );
INT8 UG_ButtonGetHSpace( UG_WINDOW* wnd, UINT8 id );
INT8 UG_ButtonGetVSpace( UG_WINDOW* wnd, UINT8 id );
UINT8 UG_ButtonGetAlignment( UG_WINDOW* wnd, UINT8 id );

/* Checkbox functions */
UG_RESULT UG_CheckboxCreate( UG_WINDOW* wnd, UG_CHECKBOX* btn, UINT8 id, INT16 xs, INT16 ys, INT16 xe, INT16 ye );
UG_RESULT UG_CheckboxDelete( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_CheckboxShow( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_CheckboxHide( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_CheckboxSetCheched( UG_WINDOW* wnd, UINT8 id, UINT8 ch );
UG_RESULT UG_CheckboxSetForeColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR fc );
UG_RESULT UG_CheckboxSetBackColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR bc );
UG_RESULT UG_CheckboxSetAlternateForeColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR afc );
UG_RESULT UG_CheckboxSetAlternateBackColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR abc );
UG_RESULT UG_CheckboxSetText( UG_WINDOW* wnd, UINT8 id, char* str );
UG_RESULT UG_CheckboxSetFont( UG_WINDOW* wnd, UINT8 id, const UG_FONT* font );
UG_RESULT UG_CheckboxSetStyle( UG_WINDOW* wnd, UINT8 id, UINT8 style );
UG_RESULT UG_CheckboxSetHSpace( UG_WINDOW* wnd, UINT8 id, INT8 hs );
UG_RESULT UG_CheckboxSetVSpace( UG_WINDOW* wnd, UINT8 id, INT8 vs );
UG_RESULT UG_CheckboxSetAlignment( UG_WINDOW* wnd, UINT8 id, UINT8 align );
UINT8 UG_CheckboxGetChecked( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_CheckboxGetForeColor( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_CheckboxGetBackColor( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_CheckboxGetAlternateForeColor( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_CheckboxGetAlternateBackColor( UG_WINDOW* wnd, UINT8 id );
char* UG_CheckboxGetText( UG_WINDOW* wnd, UINT8 id );
UG_FONT* UG_CheckboxGetFont( UG_WINDOW* wnd, UINT8 id );
UINT8 UG_CheckboxGetStyle( UG_WINDOW* wnd, UINT8 id );
INT8 UG_CheckboxGetHSpace( UG_WINDOW* wnd, UINT8 id );
INT8 UG_CheckboxGetVSpace( UG_WINDOW* wnd, UINT8 id );
UINT8 UG_CheckboxGetAlignment( UG_WINDOW* wnd, UINT8 id );

/* Textbox functions */
UG_RESULT UG_TextboxCreate( UG_WINDOW* wnd, UG_TEXTBOX* txb, UINT8 id, INT16 xs, INT16 ys, INT16 xe, INT16 ye );
UG_RESULT UG_TextboxDelete( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_TextboxShow( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_TextboxHide( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_TextboxSetForeColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR fc );
UG_RESULT UG_TextboxSetBackColor( UG_WINDOW* wnd, UINT8 id, UG_COLOR bc );
UG_RESULT UG_TextboxSetText( UG_WINDOW* wnd, UINT8 id, char* str );
UG_RESULT UG_TextboxSetFont( UG_WINDOW* wnd, UINT8 id, const UG_FONT* font );
UG_RESULT UG_TextboxSetHSpace( UG_WINDOW* wnd, UINT8 id, INT8 hs );
UG_RESULT UG_TextboxSetVSpace( UG_WINDOW* wnd, UINT8 id, INT8 vs );
UG_RESULT UG_TextboxSetAlignment( UG_WINDOW* wnd, UINT8 id, UINT8 align );
UG_COLOR UG_TextboxGetForeColor( UG_WINDOW* wnd, UINT8 id );
UG_COLOR UG_TextboxGetBackColor( UG_WINDOW* wnd, UINT8 id );
char* UG_TextboxGetText( UG_WINDOW* wnd, UINT8 id );
UG_FONT* UG_TextboxGetFont( UG_WINDOW* wnd, UINT8 id );
INT8 UG_TextboxGetHSpace( UG_WINDOW* wnd, UINT8 id );
INT8 UG_TextboxGetVSpace( UG_WINDOW* wnd, UINT8 id );
UINT8 UG_TextboxGetAlignment( UG_WINDOW* wnd, UINT8 id );

/* Image functions */
UG_RESULT UG_ImageCreate( UG_WINDOW* wnd, UG_IMAGE* img, UINT8 id, INT16 xs, INT16 ys, INT16 xe, INT16 ye );
UG_RESULT UG_ImageDelete( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_ImageShow( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_ImageHide( UG_WINDOW* wnd, UINT8 id );
UG_RESULT UG_ImageSetBMP( UG_WINDOW* wnd, UINT8 id, const UG_BMP* bmp );



#endif
