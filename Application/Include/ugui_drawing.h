/*
 * ugui_drawing.h
 *
 *  Created on: 19 Jul 2026
 *      Author: matic
 */

#ifndef INCLUDE_UGUI_DRAWING_H_
#define INCLUDE_UGUI_DRAWING_H_

#include <stdint.h>

static void draw_button(int16_t x, int16_t y, int16_t w, int16_t h, const char *label, uint8_t focused);
static void draw_startup_screen(uint8_t focus);


#endif /* INCLUDE_UGUI_DRAWING_H_ */
