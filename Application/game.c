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
#include "game.h"


static uint8_t exit_value = 0;


MENU_states_t ManualMenu(void){
	exit_value = 1;
	static MANUAL_MENU_states_t state = MANUAL_MENU_DEFAULT;

	// Draw the screen only when the state changes, not on every loop pass.
	static uint8_t needs_redraw = 1;

	joystick_buttons_enum_t pressed_key;

	switch(state){

		case MANUAL_MENU_DEFAULT:

			if (needs_redraw) {
				drawManualMenu(N_F, N_F, N_F, N_F, N_F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(Y) < 30){
				state = MANUAL_MENU_STRIP_LENGTH;
				needs_redraw = 1;
			}

			drawStrippingLengthString();

			break;

		case MANUAL_MENU_STRIP_LENGTH:

			if(needs_redraw){
				drawManualMenu(N_F, N_F, F, N_F, N_F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(Y) < 30){
				state = MANUAL_MENU_WIRE_LENGTH;
				needs_redraw = 1;
			}

			break;

		case MANUAL_MENU_WIRE_LENGTH:

			if(needs_redraw){
				drawManualMenu(N_F, N_F, N_F, F, N_F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(Y) < 30){
				state = MANUAL_MENU_WIRE_WIDTH;
				needs_redraw = 1;
			}
			if(JOY_get_axis_position(Y) > 70){
				state = MANUAL_MENU_STRIP_LENGTH;
				needs_redraw = 1;
			}

			break;

		case MANUAL_MENU_WIRE_WIDTH:

			if(needs_redraw){
				drawManualMenu(N_F, N_F, N_F, N_F, F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(Y) < 30){
				state = MANUAL_MENU_QUANTITY;
				needs_redraw = 1;
			}
			if(JOY_get_axis_position(Y) > 70){
				state = MANUAL_MENU_WIRE_LENGTH;
				needs_redraw = 1;
			}

			break;

		case MANUAL_MENU_QUANTITY:

			if(needs_redraw){
				drawManualMenu(N_F, N_F, N_F, N_F, N_F, F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(Y) < 30){
				state = MANUAL_MENU_START_BTN;
				needs_redraw = 1;
			}
			if(JOY_get_axis_position(Y) > 70){
				state = MANUAL_MENU_WIRE_WIDTH;
				needs_redraw = 1;
			}

			break;

		case MANUAL_MENU_START_BTN:

			if(needs_redraw){
				drawManualMenu(N_F, F, N_F, N_F, N_F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(X) < 30){
				state = MANUAL_MENU_BACK_BTN;
				needs_redraw = 1;
			}
			if(JOY_get_axis_position(Y) > 70){
				state = MANUAL_MENU_QUANTITY;
				needs_redraw = 1;
			}

			break;

		case MANUAL_MENU_BACK_BTN:

			if(needs_redraw){
				drawManualMenu(F, N_F, N_F, N_F, N_F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(X) > 70){
				state = MANUAL_MENU_START_BTN;
				needs_redraw = 1;
			}
			if(JOY_get_axis_position(Y) > 70){
				state = MANUAL_MENU_QUANTITY;
				needs_redraw = 1;
			}

			JOY_scan_button();
			pressed_key = JOY_get_pressed_button();
			if(pressed_key == JOY_BTN_FIRE){
				state = MANUAL_MENU_DEFAULT;
				needs_redraw = 1;
				return DEFAULT_MENU;
			}

			break;
	}
	return MANUAL_MENU;
}




MENU_states_t DefaultMenu(void)
{


	static STARTING_MENU_states_t state = STARTING_MENU_DEFAULT;

	uint8_t exit_value = 0;

	// Draw the screen only when the state changes, not on every loop pass.
	static uint8_t needs_redraw = 1;

	joystick_buttons_enum_t pressed_key;

	switch (state)
	{
		case STARTING_MENU_DEFAULT:

			if (needs_redraw){
				drawStartingMenu(N_F, N_F);
				needs_redraw = 0;
			}

			if (JOY_get_axis_position(X) > 70){
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

			if (needs_redraw){
				drawStartingMenu(N_F, F);
				needs_redraw = 0;
			}

			if (JOY_get_axis_position(X) < 30){
				state = STARTING_MENU_MANUAL;
				needs_redraw = 1;
			}
			JOY_scan_button();
			pressed_key = JOY_get_pressed_button();

			//TO DO - PresetMenu

			break;


		case STARTING_MENU_MANUAL:

			if(needs_redraw){
				drawStartingMenu(F, N_F);
				needs_redraw = 0;
			}
			if(JOY_get_axis_position(X) > 70){
				state = STARTING_MENU_PRESET;
				needs_redraw = 1;
			}
			JOY_scan_button();
			pressed_key = JOY_get_pressed_button();
			if(pressed_key == JOY_BTN_FIRE){
				state = MANUAL_FUNC;
				needs_redraw = 0;

			}
			break;

		case MANUAL_FUNC:

			needs_redraw = 1;
			state = MANUAL_MENU_DEFAULT;
			return MANUAL_MENU;

			break;

		case PRESET_FUNC:

			//TO DO

			break;

	}
	return DEFAULT_MENU;
}



void MainMenu(){

	static MENU_states_t state = DEFAULT_MENU;

	MENU_states_t exit_value;

	switch(state){

		case DEFAULT_MENU:

			exit_value = DefaultMenu();

			if(exit_value == DEFAULT_MENU)
				state = DEFAULT_MENU;

			if(exit_value == MANUAL_MENU)
				state = MANUAL_MENU;

			if(exit_value == PRESET_MENU)
				state = PRESET_MENU;

		break;

		case MANUAL_MENU:

			exit_value = ManualMenu();

			if(exit_value == DEFAULT_MENU)
				state = DEFAULT_MENU;

		break;

		case PRESET_MENU:

			//TO DO

		break;

	}


}















