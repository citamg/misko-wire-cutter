/*
 * ugui_drawing.h
 *
 *  Created on: 19 Jul 2026
 *      Author: matic
 */

#ifndef INCLUDE_UGUI_DRAWING_H_
#define INCLUDE_UGUI_DRAWING_H_

#include <stdint.h>



typedef enum { F, N_F } FOCUSED_states_t; //F => FOCUSED 		N_F => NOT FOCUSED
typedef enum { LEFT_AL, CENTER_AL } TEXT_ALLING_states_t;
typedef enum { SMALL_FONT, BIG_FONT } FONT_states_t;


void drawButton(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const char* label,
				TEXT_ALLING_states_t allign, FONT_states_t font, uint8_t focused);
void drawStartingMenu(FOCUSED_states_t manual_focus, FOCUSED_states_t preset_focus);
void drawManualMenu(FOCUSED_states_t back_focus, FOCUSED_states_t start_focus, FOCUSED_states_t strip_length_focus,
					FOCUSED_states_t wire_length_focus, FOCUSED_states_t wire_width_focus, FOCUSED_states_t quantity_focus);

void drawStrippingLengthString(void);


#endif /* INCLUDE_UGUI_DRAWING_H_ */
