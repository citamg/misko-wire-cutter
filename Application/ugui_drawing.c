/*
 * ugui_drawing.c
 *
 *  Created on: 19 Jul 2026
 *      Author: matic
 */

#include "ugui_drawing.h"
#include "graphics.h"   // pulls in LCD.h -> ugui.h (UG_* calls, colors, fonts)
#include "joystick.h"
#include <string.h>      // strlen(), for centering text


// Draw one menu button. Filled when focused, outline-only when not.
static void draw_button(int16_t x, int16_t y, int16_t w, int16_t h,
                        const char *label, uint8_t focused)
{
    UG_COLOR fill = focused ? C_STEEL_BLUE : C_BLACK;
    UG_COLOR edge = focused ? C_WHITE      : C_STEEL_BLUE;

    UG_FillFrame(x, y, x + w, y + h, fill);      // button body
    UG_DrawFrame(x, y, x + w, y + h, edge);      // button border

    // center the label (FONT_12X16 -> each char is 12 px wide, 16 tall)
    int16_t tw = 12 * (int16_t)strlen(label);
    UG_FontSelect((UG_FONT*)&FONT_12X16);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(fill);                        // match body so no odd box behind text
    UG_PutString(x + (w - tw) / 2, y + (h - 16) / 2, label);
}


// Draw the whole startup screen. focus: 0 = Manual, 1 = Preset.
static void draw_startup_screen(uint8_t focus)
{
    UG_FillScreen(C_BLACK);                       // background

    // ---- title bar ----
    UG_FillFrame(0, 0, 319, 34, C_STEEL_BLUE);
    UG_FontSelect((UG_FONT*)&FONT_12X16);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_STEEL_BLUE);
    UG_PutString(90, 9, "WIRE CUTTER");

    // ---- two choice buttons ----
    draw_button(20,  70, 125, 110, "MANUAL", focus == 0);
    draw_button(175, 70, 125, 110, "PRESET", focus == 1);

    // ---- footer hint ----
    UG_FontSelect((UG_FONT*)&FONT_7X12);
    UG_SetForecolor(C_SILVER);
    UG_SetBackcolor(C_BLACK);
    UG_PutString(60, 215, "MOVE STICK   -   PRESS TO SELECT");
}
