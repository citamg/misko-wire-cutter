/*
 * game.c
 *
 * Startup menu for the wire-cutter UI.
 *
 * Menu() is called from the main() superloop (see Core/Src/main.c). It is a
 * state machine: each call does a little work and returns immediately, so the
 * loop never blocks. State is kept in a `static` variable so it survives
 * between calls.
 */


// --- Application level modules ---

#include "graphics.h"   // GFX_display_image_array(), DISPLAY_SIZE_* (pulls in LCD.h)
#include "joystick.h"   // JOY_get_axis_position()


typedef enum { STARTING_MENU_DEFAULT, STARTING_MENU_PRESET, STARTING_MENU_MANUAL } STARTING_MENU_states_t;

typedef enum { MANUAL_MENU_DEFAULT, MANUAL_MENU_STRIP_LENGTH, MANUAL_MENU_WIRE_LENGTH, MANUAL_MENU_WIRE_WIDTH, MANUAL_MENU_QUANTITY,
			   MANUAL_MENU_START_BTN, MANUAL_MENU_BACK_BTN } MANUAL_MENU_states_t;



void ManualMenu(void){

	JOY_flush();
	static MANUAL_MENU_states_t state = MANUAL_MENU_DEFAULT;

	// Draw the screen only when the state changes, not on every loop pass.
	static uint8_t needs_redraw = 1;

	switch(state){

		case MANUAL_MENU_DEFAULT:

			if (needs_redraw) {
				GFX_display_image_array((uint16_t*)manual_screen_default, 0, 0, 320, 240);
				needs_redraw = 0;
			}
	}
}


void Menu(void)
{
	joystick_buttons_enum_t pressed_key;

	static STARTING_MENU_states_t state = STARTING_MENU_DEFAULT;

	// Draw the screen only when the state changes, not on every loop pass.
	static uint8_t needs_redraw = 1;

	switch (state)
	{
		case STARTING_MENU_DEFAULT:

			if (needs_redraw)
			{
				GFX_display_image_array((uint16_t*) default_screen, 0, 0, 320, 240);
				needs_redraw = 0;
			}

			if (JOY_get_axis_position(X) > 70)
			{
				state = STARTING_MENU_PRESET;
				needs_redraw = 1;
			}
			else if(JOY_get_axis_position(X) < 30){
				state = STARTING_MENU_MANUAL;
				needs_redraw = 1;
			}
			else{
				state = STARTING_MENU_DEFAULT;
				needs_redraw = 0;
			}
			break;


		case STARTING_MENU_PRESET:

			if (needs_redraw)
			{
				GFX_display_image_array((uint16_t*) default_screen_preset, 0, 0, 320, 240);
				needs_redraw = 0;
			}

			if (JOY_get_axis_position(X) < 30)
			{
				state = STARTING_MENU_MANUAL;
				needs_redraw = 1;
			}
			JOY_scan_button();
			pressed_key = JOY_get_pressed_button();

			break;


		case STARTING_MENU_MANUAL:

			if(needs_redraw){
				GFX_display_image_array((uint16_t*) default_screen_manual, 0, 0, 320, 240);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(X) > 70){
				state = STARTING_MENU_PRESET;
				needs_redraw = 1;
			}
			JOY_scan_button();
			pressed_key = JOY_get_pressed_button();
			if(pressed_key == JOY_BTN_FIRE){
				ManualMenu();
			}
			break;


		default:

			// Safety net for an undefined state.
			state = STARTING_MENU_DEFAULT;
			needs_redraw = 1;

			break;
	}
}
























