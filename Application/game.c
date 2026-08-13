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
				drawManualMenu(focus, &manual_val);      //full redraw
		}

		JOY_scan_button();
		if (JOY_get_pressed_button() == JOY_BTN_FIRE) {
			if (focus == ITEM_BACK) {
				entered = 0;                			//repaints the menu
				return DEFAULT_MENU;
			}
			if(focus != ITEM_START && focus != ITEM_BACK){
				entered = 0;
				state = EDITING_STATE;
			}
			if(focus == ITEM_START){
				entered = 0;
				drawWaitingScreen();
				CuttingSequence(manual_val.strip_length, manual_val.wire_length, manual_val.quantity);
				return MANUAL_MENU;
			}
		}
		break;
    case EDITING_STATE:

    	switch(focus){
    		case ITEM_STRIP:
			state = EditingValue(&manual_val, ITEM_STRIP);
			break;

    		case ITEM_WIRE_LEN:
			state = EditingValue(&manual_val, ITEM_WIRE_LEN);
			break;

    		case ITEM_WIRE_W:
			state = EditingValue(&manual_val, ITEM_WIRE_W);
			break;

    		case ITEM_QTY:
			state = EditingValue(&manual_val, ITEM_QTY);
			break;
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


MANUAL_MENU_t EditingValue(MANUAL_VALUES_t* manual_val, MANUAL_ITEM_t focus){
	uint16_t* temp = NULL;
	joystick_buttons_enum_t pressed_key = JOY_BTN_NONE;
	uint16_t value_to_change = 0;
	char* btn_text = "Empty";
	uint8_t mul;
//swtich case to determine what value to change
	switch(focus){
	case ITEM_STRIP:
		value_to_change = manual_val->strip_length;
		temp = &manual_val->strip_length;
		btn_text = "Stripping length";
		mul = 0;
		break;
	case ITEM_WIRE_LEN:
		value_to_change = manual_val->wire_length;
		temp = &manual_val->wire_length;
		btn_text = "Wire length";
		mul = 1;
		break;
	case ITEM_WIRE_W:
		value_to_change = manual_val->wire_width;
		temp = &manual_val->wire_width;
		btn_text = "Wire width";
		mul = 2;
		break;
	case ITEM_QTY:
		value_to_change = manual_val->quantity;
		temp = &manual_val->quantity;
		btn_text = "Quantity";
		mul = 3;
		break;
	}


	uint16_t old = value_to_change;

	while(pressed_key != JOY_BTN_FIRE){
		static int16_t speed_up = 500;
		static uint8_t count = 0;
		char buf[16];
		uint16_t new = old;




		if(JOY_get_axis_position(Y) > 70){
			if(count > 10 ) new += 10;
			else	 new++;
			count++;
			speed_up -= 25 * count;
			if(speed_up < 0) speed_up = 100;				//lock fastest possible move speed
		}
		if(JOY_get_axis_position(Y) < 30){
			if(count > 10 ) new -= 10;
			else	 new--;
			count++;
			speed_up -= 25 * count;
			if(speed_up < 0) speed_up = 100;				//lock fastest possible move speed
		}
		if(JOY_get_axis_position(Y) > 30 && JOY_get_axis_position(Y) < 70){
			count = 0;
			speed_up = 500;
		}


		if(new > 1000) 	new = 0;
		if(new < 0) new = 1000;
		if(new != old){
			if(focus == ITEM_STRIP || focus == ITEM_WIRE_LEN) sprintf(buf, "%4d mm", new);	//transforms strip length and puts in buf to display it as string
			if(focus == ITEM_WIRE_W) sprintf(buf, "%4d mm^2", new);
			if(focus == ITEM_QTY) sprintf(buf, "%4d ", new);
			drawButton(20, 40 + mul * 40, 300, 70 + mul * 40, btn_text, buf, LEFT_AL, SMALL_FONT, 1);
			HAL_Delay(speed_up);
			}
		JOY_scan_button();
		pressed_key = JOY_get_pressed_button();

		old = new;

		*temp = new;
	}

	return FOCUS_STATE;
}














