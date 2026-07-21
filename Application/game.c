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
#include "ugui_drawing.h"

typedef enum { STARTING_MENU_DEFAULT, STARTING_MENU_PRESET, STARTING_MENU_MANUAL } STARTING_MENU_states_t;

typedef enum { MANUAL_MENU_DEFAULT, MANUAL_MENU_STRIP_LENGTH, MANUAL_MENU_WIRE_LENGTH, MANUAL_MENU_WIRE_WIDTH, MANUAL_MENU_QUANTITY,
			   MANUAL_MENU_START_BTN, MANUAL_MENU_BACK_BTN } MANUAL_MENU_states_t;

static uint8_t exit_value = 0;


uint8_t ManualMenu(void){
	exit_value = 1;
	JOY_flush();
	static MANUAL_MENU_states_t state = MANUAL_MENU_DEFAULT;

	// Draw the screen only when the state changes, not on every loop pass.
	static uint8_t needs_redraw = 1;

	switch(state){

		case MANUAL_MENU_DEFAULT:

			if (needs_redraw) {
				drawManualMenu(F, N_F, N_F, N_F, N_F, N_F);
				needs_redraw = 0;
			}
			volatile uint8_t x = JOY_get_axis_position(Y);
			if(x < 30){
				state = MANUAL_MENU_STRIP_LENGTH;
				needs_redraw = 1;
			}

			break;

		case MANUAL_MENU_STRIP_LENGTH:

			if(needs_redraw){
				drawManualMenu(N_F, N_F, F, N_F, N_F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(Y) < 30){
					state = MANUAL_MENU_DEFAULT;
					needs_redraw = 1;
			}
			if(JOY_get_axis_position(Y) < 30){
					state = MANUAL_MENU_WIRE_LENGTH;
					needs_redraw = 1;
			}
			break;

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
				drawStartingMenu(N_F, N_F);
				needs_redraw = 0;
			}

			if (JOY_get_axis_position(X) > 70 && exit_value == 0)
			{
				state = STARTING_MENU_PRESET;
				needs_redraw = 1;
			}
			else if(JOY_get_axis_position(X) < 30 && exit_value == 0){
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
				drawStartingMenu(N_F, F);
				needs_redraw = 0;
			}

			if (JOY_get_axis_position(X) < 30 && exit_value == 0)
			{
				state = STARTING_MENU_MANUAL;
				needs_redraw = 1;
			}
			JOY_scan_button();
			pressed_key = JOY_get_pressed_button();

			break;


		case STARTING_MENU_MANUAL:

			if(needs_redraw){
				drawStartingMenu(F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(X) > 70 && exit_value == 0){
				state = STARTING_MENU_PRESET;
				needs_redraw = 1;
			}
			JOY_scan_button();
			pressed_key = JOY_get_pressed_button();
			if(pressed_key == JOY_BTN_FIRE){
				needs_redraw = 0;
				ManualMenu();
			}
			break;


		/*default:

			// Safety net for an undefined state.
			state = STARTING_MENU_DEFAULT;
			needs_redraw = 1;

			break;*/
	}
}



















