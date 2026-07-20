/*
 * ugui_drawing.c
 *
 *  Created on: 19 Jul 2026
 *      Author: matic
 */

#include "ugui_drawing.h"
#include "graphics.h"
#include "joystick.h"
#include <string.h>

#define FOCUSED_FRAME_THICKNESS 5




void drawButton(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const char* label, uint8_t focused){

	UG_FillFrame(x1, y1, x2, y2, C_BLACK);

	if(focused == FOCUSED){
		for(uint8_t i = 0; i < 5; i++){
			UG_DrawFrame(x1 + i, y1 + i, x2 - i, y2 - i, C_WHITE);
		}
	}
	else
		UG_DrawFrame(x1, y1, x2, y2, C_WHITE);

	uint16_t text_width = 12 * (uint16_t)strlen(label);
	uint16_t text_x_cord = (x1 + (x2 - text_width)) / 2;
	uint16_t text_y_cord = (y1 + (y2 - 16)) / 2; //16 because of font 12x16

	UG_FontSelect((UG_FONT*) &FONT_12X16);
	UG_SetForecolor(C_WHITE);
	UG_SetBackcolor(C_BLACK);
	UG_PutString(text_x_cord, text_y_cord, label);



}


void drawStartingMenu(FOCUSED_states_t manual_focus, FOCUSED_states_t preset_focus){

	UG_FillFrame(0, 0, 320, 34, C_BLACK);
	UG_DrawFrame(0, 0, 320, 34, C_WHITE);

	UG_FontSelect((UG_FONT*)&FONT_12X16);
	UG_SetForecolor(C_WHITE);
	UG_SetBackcolor(C_BLACK);
	UG_PutString(94 , 9, "WIRE CUTTER");




	drawButton(20, 70, 145, 190, "MANUAL", manual_focus);
	drawButton(175, 70, 300, 190, "PRESET", preset_focus);


}



