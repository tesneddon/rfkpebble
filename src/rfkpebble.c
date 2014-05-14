/*
**++
**  MODULE DESCRIPTION:
**
**      Main robotfindskitten entry point.
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
#define MENU_SIZE 3

/*
** Forward declarations
*/

    static void init(void);
    static void deinit(void);
    static void play(int index, void *ctx);
    static void about(int index, void *ctx);
    static void about_click_config(void *ctx);
    static void about_click_handler(ClickRecognizerRef recognizer,
                                    void *context);
    int main(void);

/*
** Own storage
*/

    static Context *game = 0;
    static Window *about_window = 0, *window = 0;
    static Layer *window_layer = 0;
    static TextLayer *about_text_layer = 0;
    static SimpleMenuLayer *menu_layer = 0;
    static SimpleMenuItem menu[MENU_SIZE];
    static SimpleMenuSection menu_sections[] = { {
        .title = "Robot Finds Kitten",
        .num_items = MENU_SIZE,
        .items = menu,
    } };

static void init(void) {
    int i = 0;

    window = window_create();
    if (window == 0) {
        // error...
        return;
    }
    window_set_fullscreen(window, true);
    window_layer = window_get_root_layer(window);
    window_stack_push(window, true);

    /*
    ** Build the main menu.
    */
    menu[i++] = (SimpleMenuItem) {
        .title = "Play",
        .callback = play,
        // .icon = play_icon,
    };
    menu[i++] = (SimpleMenuItem) {
        .title = "High Score",
        // .callback = highscore,
        // .icon = highscore_icon,
    };
    menu[i++] = (SimpleMenuItem) {
        .title = "About",
        .callback = about,
        // .icon = about_icon,
    };
    assert(i == MENU_SIZE);

    /*
    ** Load the menu on the phone and away we go...!
    */
    menu_layer = simple_menu_layer_create(layer_get_frame(window_layer),
                                          window, menu_sections, 1, 0);
    if (menu_layer != 0) {
        layer_add_child(window_layer,
                        simple_menu_layer_get_layer(menu_layer));
    }
}

static void deinit(void) {
    if (about_window != 0) {
        if (about_text_layer != 0) text_layer_destroy(about_text_layer);
        window_destroy(about_window);
    }

    if (window != 0) {
        if (menu_layer != 0) simple_menu_layer_destroy(menu_layer);
        window_destroy(window);
    }
}

static void play(int index,
                 void *ctx) {

    message_init(0);

    if (game == 0) game = game_init();
    assert(game != 0);

    go(game);
}

static void about(int index,
                  void *ctx) {

    if (about_window == 0) {
        about_window = window_create();
        if (about_window != 0) {
            window_set_click_config_provider(about_window, about_click_config);
            about_text_layer = text_layer_create(layer_get_frame(window_layer));
            if (about_text_layer != 0) {
                //
            }
        }
    }

    if (about_window != 0) {
        window_stack_push(about_window, true);
    }
}

static void about_click_config(void *ctx) {
    window_single_click_subscribe(BUTTON_ID_BACK, about_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, about_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, about_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, about_click_handler);
}

static void about_click_handler(ClickRecognizerRef recognizer,
                                void *context) {
    window_stack_remove(about_window, true);
}

int main(void) {

    init();
    app_event_loop();
    deinit();
}
