/*
**++
**  MODULE DESCRIPTION:
**
**      All routines related to Robot control and display.
**
**  AUTHOR:             Tim Sneddon
**
**  Copyright (C) 2014 Endless Software Solutions.
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
**  CREATION DATE:      26-MAR-2014
**
**  MODIFICATION HISTORY:
**
**      26-MAR-2014 V1.0    Sneddon  Initial coding.
**--
*/
#include "rfkpebble.h"

/*
** Forward declarations
*/

    void robot_init(Window *window);
    void robot_deinit(void);
    static void click_config_provider(void *context);
    static void down_handler(ClickRecognizerRef recognizer, void *context);
    static void up_handler(ClickRecognizerRef recognizer, void *context);

void robot_init(Window *window) {
    window_set_click_config_provider(window, click_config_provider);
}

void robot_deinit(void) {
    // disable click provider...
}

/*
** Register our interest in these clicks...
*/
static void click_config_provider(void *context) {
    static bool select_state = false;

    window_raw_click_subscribe(BUTTON_ID_DOWN, down_handler, 0, &select_state);
    window_raw_click_subscribe(BUTTON_ID_UP, down_handler, 0, &select_state);
    window_raw_click_subscribe(BUTTON_ID_SELECT, down_handler, up_handler,
                               &select_state);
}

static void down_handler(ClickRecognizerRef recognizer,
                         void *context) {
    ButtonId button;
    bool *select_state = context;

    button = click_recognizer_get_button_id(recognizer);
    switch (button) {
    case BUTTON_ID_DOWN:
        if (*select_state) {
            APP_LOG(APP_LOG_LEVEL_INFO, "Down");
        } else {
            APP_LOG(APP_LOG_LEVEL_INFO, "Left");
        }
        break;

    case BUTTON_ID_SELECT:
        *select_state = true;
        APP_LOG(APP_LOG_LEVEL_INFO, "Select");
        break;

    case BUTTON_ID_UP:
        if (*select_state) {
            APP_LOG(APP_LOG_LEVEL_INFO, "Up");
        } else {
            APP_LOG(APP_LOG_LEVEL_INFO, "Right");
        }
        break;

    default:
        break;
    }
}

static void up_handler(ClickRecognizerRef recognizer,
                       void *context) {
    ButtonId button;
    bool *select_state = context;

    button = click_recognizer_get_button_id(recognizer);
    switch (button) {
    case BUTTON_ID_SELECT:
        *select_state = false;
        APP_LOG(APP_LOG_LEVEL_INFO, "Select Relase");
        break;

    default:
        break;
    }
}
