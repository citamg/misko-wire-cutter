/*
 * ugui_drawing.h
 *
 *  Created on: 19 Jul 2026
 *      Author: matic
 */

#ifndef INCLUDE_UGUI_DRAWING_H_
#define INCLUDE_UGUI_DRAWING_H_

#include <stdint.h>



typedef enum { FOCUSED, NOT_FOCUSED } FOCUSED_states_t;


void drawStartingMenu(FOCUSED_states_t manual_focus, FOCUSED_states_t preset_focus);

#endif /* INCLUDE_UGUI_DRAWING_H_ */
