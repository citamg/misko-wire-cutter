/*
 * ugui_drawing.h
 *
 *  Created on: 19 Jul 2026
 *      Author: matic
 */

#ifndef INCLUDE_UGUI_DRAWING_H_
#define INCLUDE_UGUI_DRAWING_H_

#include <stdint.h>
#include <stdio.h>


typedef enum { N_F, F } FOCUSED_states_t; //F => FOCUSED 		N_F => NOT FOCUSED
typedef enum { LEFT_AL, CENTER_AL } TEXT_ALLING_states_t;
typedef enum { SMALL_FONT, BIG_FONT } FONT_states_t;
typedef enum { 	ITEM_STRIP, ITEM_WIRE_LEN, ITEM_WIRE_W, ITEM_QTY,
				ITEM_BACK, ITEM_START, ITEM_COUNT, ITEM_NONE} MANUAL_ITEM_t;

typedef struct{
	uint16_t strip_length;
	uint16_t wire_length;
	uint16_t wire_width;
	uint16_t quantity;
}MANUAL_VALUES_t;


void drawButton(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const char* label,
				const char* value, TEXT_ALLING_states_t allign, FONT_states_t font, uint8_t focused);
void drawStartingMenu(FOCUSED_states_t manual_focus, FOCUSED_states_t preset_focus);
void drawManualMenu(MANUAL_ITEM_t focus, MANUAL_VALUES_t *values);

void drawWaitingScreen(void);


#endif /* INCLUDE_UGUI_DRAWING_H_ */
