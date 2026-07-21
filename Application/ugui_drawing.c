/*
 * ugui_drawing.c
 *
 *  Created on: 19 Jul 2026
 *      Author: matic
 */
#include "lcd.h"
#include "ugui_drawing.h"
#include "graphics.h"
#include "joystick.h"
#include <string.h>





void drawButton(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const char* label,
				TEXT_ALLING_states_t allign, FONT_states_t font, uint8_t focused){


	uint8_t font_height;
	uint8_t font_width;

	if(font == SMALL_FONT){
		font_height = 12;
		font_width = 8;
		UG_FontSelect((UG_FONT*)&FONT_8X14);
	}
	else{
		font_height = 16;
		font_width = 12;
		UG_FontSelect((UG_FONT*)&FONT_12X16);
	}

	UG_FillFrame(x1, y1, x2, y2, C_BLACK);

	if(focused == F){
		for(uint8_t i = 0; i < 3; i++){
			UG_DrawFrame(x1 + i, y1 + i, x2 - i, y2 - i, C_WHITE);
		}
	}
	else
		UG_DrawFrame(x1, y1, x2, y2, C_WHITE);

	uint16_t text_width = font_width * (uint16_t)strlen(label);
	uint16_t text_x_cord = 0;
	uint16_t text_y_cord = 0;
	if(allign == CENTER_AL){
		text_x_cord = (x1 + (x2 - text_width)) / 2;
		text_y_cord = (y1 + (y2 - font_height)) / 2;
	}
	else{
		text_x_cord = x1 + 15;
		text_y_cord = (y1 + (y2 - font_height)) / 2; //16 because of font 12x16
	}


	UG_SetForecolor(C_WHITE);
	UG_SetBackcolor(C_BLACK);
	UG_PutString(text_x_cord, text_y_cord, label);



}


void drawStartingMenu(FOCUSED_states_t manual_focus, FOCUSED_states_t preset_focus){
	LCD_ClearScreen();
	UG_FillFrame(0, 0, 320, 34, C_BLACK);
	UG_DrawFrame(0, 0, 320, 34, C_WHITE);

	UG_FontSelect((UG_FONT*)&FONT_12X16);
	UG_SetForecolor(C_WHITE);
	UG_SetBackcolor(C_BLACK);
	UG_PutString(94 , 9, "WIRE CUTTER");




	drawButton(20, 70, 145, 190, "MANUAL", CENTER_AL, BIG_FONT, manual_focus);
	drawButton(175, 70, 300, 190, "PRESET", CENTER_AL, BIG_FONT, preset_focus);


}

void drawManualMenu(FOCUSED_states_t back_focus, FOCUSED_states_t start_focus, FOCUSED_states_t strip_length_focus, FOCUSED_states_t wire_length_focus, FOCUSED_states_t wire_width_focus, FOCUSED_states_t quantity_focus){
	LCD_ClearScreen();
	UG_FillFrame(0, 0, 320, 34, C_BLACK);
	UG_DrawFrame(0, 0, 320, 34, C_WHITE);

	UG_FontSelect((UG_FONT*)&FONT_12X16);
	UG_SetForecolor(C_WHITE);
	UG_SetBackcolor(C_BLACK);
	UG_PutString(94 , 9, "MANUAL SETUP");




	drawButton(20, 40, 300, 70, "Stripping length", LEFT_AL, SMALL_FONT, strip_length_focus);
	drawButton(20, 80, 300, 110, "Wire length", LEFT_AL, SMALL_FONT, wire_length_focus);
	drawButton(20, 120, 300, 150, "Wire width", LEFT_AL, SMALL_FONT, wire_width_focus);
	drawButton(20, 160, 300, 190, "Quantity", LEFT_AL, SMALL_FONT, quantity_focus);

	drawButton(20, 206, 150, 240, "BACK", CENTER_AL, BIG_FONT, back_focus);
	drawButton(170, 206, 300, 240, "START", CENTER_AL, BIG_FONT, start_focus);

}

