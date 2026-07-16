/*
 * graphics.h
 *
 * Thin LCD helper for the hardware bring-up test. Just enough to push a
 * full image (and, optionally, a solid color) to the ILI9341 display.
 *
 * The full graphics engine (objects, sprites, partial frame buffer, text,
 * progress bars) was removed to keep this baseline minimal. It comes back
 * when you build the real UI.
 */

#ifndef INCLUDE_GRAPHICS_H_
#define INCLUDE_GRAPHICS_H_


// ------ Standard C libraries -----

#include <stdint.h>


// ------ System level modules ---------

#include "LCD.h"   // low-level ILI9341 driver + uGUI library (UG_COLOR, UG_FillFrame, ...)



// --- Display geometry (ILI9341, landscape) ---

#define DISPLAY_SIZE_X   320
#define DISPLAY_SIZE_Y   240



// ---------------- Public function prototypes ----------------

// Draw a raw RGB565 image array into a window at (x,y) of size (size_x, size_y).
void GFX_display_image_array(uint16_t *img_array, int16_t x, int16_t y, uint16_t size_x, uint16_t size_y);

// Fill the whole screen with one solid uGUI color (e.g. C_RED, C_GREEN, C_BLACK).
// Handy for a first "is the panel alive?" test before trusting image data.
void GFX_fill_screen(UG_COLOR color);


#endif /* INCLUDE_GRAPHICS_H_ */
