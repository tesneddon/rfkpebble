/*
**++
**  MODULE DESCRIPTION:
**
**      This module contains the guts of the game.
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
**  CREATION DATE:      11-APR-2014
**
**  MODIFICATION HISTORY:
**
**      11-APR-2014 V1.0    Sneddon  Initial coding.
**--
*/
#include "rfkpebble.h"
#define BOGUS_MAX 20        /* Max. bogus characters */
#define SCREEN_MAX (13*9)   /* Mono font at 18pt give a 13 x 9 grid */
#define ROBOT_CHAR '#'      /* Robot is always the hash character */

/*
** Forward declarations
*/

    Context *game_init(void);
    static void unload(Window *w);
    void go(Context *ctx);
    static int randxy(Context *game);
    static char randchr(Context *game, int max);

/*
** Game context.
*/

    typedef struct context_ {
        bool paused;
        GFont font;
        TextLayer *layer;
        Window *window;
        char *bogus[BOGUS_MAX];
        char screen[SCREEN_MAX+1];
    } Context;

    // setup screen aray
        // screen[0][0] = " ";
        // screen[0][3] = "@";
        // screen[9][6] = "#";

    // setup object array
        // object[0] = "@This is a message..."
        // object[1] = "#Another message..."


Context *game_init(void) {

    Context *game = 0;
    Layer *window_layer;

    game = malloc(sizeof(Context));
    if (game == 0) return 0;

    memset(game, 0, sizeof(Context));
    srand(time(0));

    game->window = window_create();
    if (game->window != 0) {
        window_set_user_data(game->window, game);
        window_set_fullscreen(game->window, true);
        window_set_window_handlers(game->window, (WindowHandlers) {
            .unload = unload,
        });
        window_layer = window_get_root_layer(game->window);

        game->font = fonts_load_custom_font(
                              resource_get_handle(RESOURCE_ID_FREE_MONO_18));

        game->layer = text_layer_create(layer_get_frame(window_layer));
        if (game->layer != 0) {
            text_layer_set_overflow_mode(game->layer,
                                         GTextOverflowModeWordWrap);
            text_layer_set_font(game->layer, game->font);
            text_layer_set_text(game->layer, game->screen);

            layer_add_child(window_layer, text_layer_get_layer(game->layer));
        }
    }

    return game;
}

static void unload(Window *w) {
    Context *game = window_get_user_data(w);

    if (game->layer != 0) text_layer_destroy(game->layer);
    if (game->font != 0) fonts_unload_custom_font(game->font);
    free(game);

    window_destroy(w);
}

void go(Context *game) {

    if (game->paused) {
        game->paused = false;
    } else {
        /*
        ** Initialize the screen.
        */
        memset(game->screen, ' ', SCREEN_MAX);
        game->screen[SCREEN_MAX] = '\0';

        /*
        ** Initialize the robot...
        */
        game->screen[randxy(game)] = ROBOT_CHAR;

        /*
        ** ...the kitten...
        */
        game->screen[randxy(game)] = randchr(game, 0);

        /*
        ** ...and all the bogus characters.
        */
        for (int i = 0; i < BOGUS_MAX; i++) {
            char c = randchr(game, i), *msg;
            int pos = randxy(game);

            msg = message_get();
            game->bogus[i] = malloc(1 + strlen(msg) + 1); /* c + msg + '\0' */
            assert(game->bogus[i] != 0);

            game->bogus[i][0] = c;
            strcpy(&game->bogus[i][1], msg);

            game->screen[pos] = c;
        }

        // do the stuff to queue input/etc
    }

    window_stack_push(game->window, true);
}

static int randxy(Context *game) {
    int i;

    /*
    ** Find a free position on the screen.  However, make sure we are not
    ** blocked in by other characters.
    */
    do {
        i = rand() % SCREEN_MAX;
    } while ((game->screen[i] | game->screen[i-1] | game->screen[i+1]) != ' ');

    return i;
}

static char randchr(Context *game,
                    int max) {
    int i;
    char c = '\0';

    /*
    ** Find a free character, making sure that we have not already
    ** used it before.
    */
    while (c == '\0') {
        c = (char) (rand() % ('~' - '!')) + '!';

        for (i = 0; i < max; i++) {
            if (game->bogus[i][0] == c) break;
        }
        if (i < max) c = '\0';
    }

    return c;
}
