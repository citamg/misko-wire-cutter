/*
 * game.h
 *
 * Minimal application entry point.
 *
 * The name Game() is kept on purpose so your existing main.c superloop
 * (which calls Game()) still works without any changes.
 */

#ifndef INCLUDE_GAME_H_
#define INCLUDE_GAME_H_

#include "ugui_drawing.h"
#include "motor.h"


typedef enum { DEFAULT_MENU, MANUAL_MENU, PRESET_MENU } MENU_states_t;

typedef enum { STARTING_MENU_DEFAULT, STARTING_MENU_PRESET, STARTING_MENU_MANUAL, MANUAL_FUNC, PRESET_FUNC} STARTING_MENU_states_t;

typedef enum { MANUAL_MENU_DEFAULT, MANUAL_MENU_STRIP_LENGTH, MANUAL_MENU_WIRE_LENGTH, MANUAL_MENU_WIRE_WIDTH, MANUAL_MENU_QUANTITY,
			   MANUAL_MENU_START_BTN, MANUAL_MENU_BACK_BTN } MANUAL_MENU_states_t;

typedef enum { FOCUS_STATE, EDITING_STATE } MANUAL_MENU_t;

// ---------------- Public function prototypes ----------------

void MainMenu(void);
MENU_states_t DefaultMenu(void);
MENU_states_t ManualMenu(void);
MENU_states_t PresetMenu(void);

MANUAL_MENU_t EditingValue(MANUAL_VALUES_t* manual_val, MANUAL_ITEM_t focus);
#endif /* INCLUDE_GAME_H_ */
