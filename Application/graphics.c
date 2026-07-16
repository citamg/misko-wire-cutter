/*
 * graphics.c
 *
 * Thin LCD helper for the hardware bring-up test.
 * See graphics.h for the rationale.
 */


#include "graphics.h"



// Send a raw RGB565 array straight to the display window.
// This is the exact, proven path the original engine used.
void GFX_display_image_array(uint16_t *img_array, int16_t x, int16_t y, uint16_t size_x, uint16_t size_y)
{
    ILI9341_SetDisplayWindow( x, y, size_x, size_y );
    ILI9341_SendData( (LCD_IO_Data_t *) img_array, size_x * size_y );
}



// Fill the whole panel with one solid color using the uGUI primitive.
void GFX_fill_screen(UG_COLOR color)
{
    UG_FillFrame( 0, 0, DISPLAY_SIZE_X - 1, DISPLAY_SIZE_Y - 1, color );
}
