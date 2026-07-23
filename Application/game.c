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

typedef enum { FOCUS_STATE, EDITING_STATE } MANUAL_MENU_t;

MENU_states_t ManualMenu(void)
{
	static MANUAL_MENU_t state;
    static uint8_t entered = 0;
    static MANUAL_ITEM_t focus = ITEM_STRIP;
    static MANUAL_VALUES_t manual_val = {5, 250, 10, 10 };

    if (!entered) {
        entered = 1;
        JOY_flush();
        drawManualMenu(focus, &manual_val);          // full paint on entry
    }


    int8_t step = JOY_get_axis_step(Y);

    switch(state){

    case FOCUS_STATE:
		if (step != 0) {
			MANUAL_ITEM_t old = focus;

			int8_t f = (int8_t)focus - step;

			if (f > ITEM_COUNT - 1)
				f = ITEM_COUNT - 1;
			if (f < ITEM_STRIP)
				f = ITEM_STRIP;

			focus = (MANUAL_ITEM_t)f;

			if (focus != old)
				drawManualMenu(focus, &manual_val);      // still a full redraw, that's fine for now
		}

		JOY_scan_button();
		if (JOY_get_pressed_button() == JOY_BTN_FIRE) {
			if (focus == ITEM_BACK) {
				entered = 0;                // so it repaints next time you come back
				return DEFAULT_MENU;
			}
			if(focus != ITEM_START && focus != ITEM_BACK){
				entered = 0;
				state = EDITING_STATE;
			}
		}
		break;
    case EDITING_STATE:

    	if(focus == ITEM_STRIP){
    		static int16_t speed_up = 500;
    		static uint8_t count = 0;
    		char buf[16];
    		uint16_t old = manual_val.strip_length;

    		if(JOY_get_axis_position(Y) > 70){
    			if(count > 10 ) manual_val.strip_length += 10;
    			else	 manual_val.strip_length++;
    			count++;
    			speed_up -= 25*count;
    			if(speed_up < 0) speed_up = 100;
    		}
			if(JOY_get_axis_position(Y) < 30){
				manual_val.strip_length--;
				count = 0;
				speed_up = 500;
			}
			if(JOY_get_axis_position(Y) > 30 && JOY_get_axis_position(Y) < 70){
				count = 0;
				speed_up = 500;
			}
			if(manual_val.strip_length > 1000) 	manual_val.strip_length = 1000;
    		if(manual_val.strip_length != old){
			sprintf(buf, "%4d mm", manual_val.strip_length);
			drawButton(20, 40, 300, 70, "Stripping length", buf, LEFT_AL, SMALL_FONT, focus == ITEM_STRIP);
			HAL_Delay(speed_up);
    		}

    	}



    	break;
	}
    return MANUAL_MENU;
}




MENU_states_t DefaultMenu(void)
{


	static STARTING_MENU_states_t state = STARTING_MENU_DEFAULT;

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















