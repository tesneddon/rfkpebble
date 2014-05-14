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
#define HALF_SECOND 500     /* Half a second, in milliseconds */
#define SCREEN_HEIGHT 9     /* Screen height in characters } Mono font at */
#define SCREEN_WIDTH 14     /* Screen width in characters  } 18 pt.       */
#define SCREEN_MAX (SCREEN_HEIGHT*SCREEN_WIDTH)
#define EMPTY ' '           /* Empty character */
#define ROBOT '#'           /* Robot is always the hash character */

/*
** Forward declarations
*/

    Context *game_init(void);
    static void unload(Window *w);
    void go(Context *ctx);
    static int randxy(Context *game);
    static char randchr(Context *game);
    static void calibrate(Context *game);
    static void calibration_complete(void *data);
    static void process_input(AccelData *data, uint32_t num_samples);

/*
** Game context.
*/

    typedef struct context_ {
        enum {
            IGNORE = 0,
            CALIBRATE,
            PAUSE,
            PLAY,
        } state;
        void *data;
        GFont bg_font, fg_font;
        TextLayer *bg_layer, *fg_layer;
        Window *window;
        int robot;
        uint64_t timestamp;
        int16_t min_x, min_y;
        int16_t max_x, max_y;
/*

    Add some max/min movements, so we calibrate over a small time, with
    a message on the screen...

    get a min/max x and min/max y

    also, use the timestamp to ignore samples that come in too quickly...
    don't fetch window, etc. if we are ignoring an event...

*/

        char *bogus[BOGUS_MAX];
        char bg_screen[SCREEN_MAX+1];
        char fg_screen[SCREEN_MAX+1];
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

        game->bg_font = fonts_load_custom_font(
                          resource_get_handle(RESOURCE_ID_FREE_MONO_18));
        game->fg_font = fonts_load_custom_font(
                          resource_get_handle(RESOURCE_ID_FREE_MONO_BOLD_18));

        game->bg_layer = text_layer_create(layer_get_frame(window_layer));
//        game->bg_layer = text_layer_create(GRect(0,0,143,168));
        if (game->bg_layer != 0) {
            text_layer_set_overflow_mode(game->bg_layer,
                                         GTextOverflowModeTrailingEllipsis);
            text_layer_set_font(game->bg_layer, game->bg_font);
            text_layer_set_text(game->bg_layer, &game->bg_screen[1]);
            text_layer_set_text_alignment(game->bg_layer, GTextAlignmentLeft);

            layer_add_child(window_layer,
                            text_layer_get_layer(game->bg_layer));

            game->fg_layer = text_layer_create(layer_get_frame(window_layer));
/*
** TODO: @tesneddon define game_rect using the dimensions below.  Use this
**                  for the fg and bg.  This means that there is an
**                  invisible column, ie. 12, 25, 38, .etc.  Or,
**                  (pos % 13) - 1.  Make sure that we don't store
**                  anything in it when we generate the random positions
**                  for all the characters.
*/
//            game->fg_layer = text_layer_create(GRect(0,0,143,168));
            if (game->fg_layer != 0) {
                text_layer_set_overflow_mode(game->fg_layer,
                                             GTextOverflowModeTrailingEllipsis);
                text_layer_set_font(game->fg_layer, game->fg_font);
                text_layer_set_text(game->fg_layer, &game->fg_screen[1]);
                text_layer_set_text_alignment(game->fg_layer, GTextAlignmentLeft);
                text_layer_set_background_color(game->fg_layer, GColorClear);
                layer_add_child(window_layer,
                                text_layer_get_layer(game->fg_layer));
            }
        }
    }

    return game;
}

static void unload(Window *w) {
    Context *game = window_get_user_data(w);

    accel_data_service_unsubscribe();

    if (game->bg_layer != 0) text_layer_destroy(game->bg_layer);
    if (game->bg_font != 0) fonts_unload_custom_font(game->bg_font);
    if (game->fg_layer != 0) text_layer_destroy(game->fg_layer);
    if (game->fg_font != 0) fonts_unload_custom_font(game->fg_font);
    free(game);

    window_destroy(w);
}

void go(Context *game) {

    // pause?
    {
        /*
        ** Initialize the screen.
        */
        memset(game->bg_screen, EMPTY, SCREEN_MAX);
        memset(game->fg_screen, EMPTY, SCREEN_MAX);
        game->bg_screen[SCREEN_MAX] = '\0';
        game->fg_screen[SCREEN_MAX] = '\0';

        /*
        ** Initialize the robot...
        */
        game->robot = randxy(game);
        game->fg_screen[game->robot] = ROBOT;

        /*
        ** ...the kitten...
        */
        game->bg_screen[randxy(game)] = randchr(game);

        /*
        ** ...and all the bogus characters.
        */
        for (int i = 0; i < BOGUS_MAX; i++) {
            char c = randchr(game), *msg;
            int pos = randxy(game);

            msg = message_get();
            game->bogus[i] = malloc(1 + strlen(msg) + 1); /* c + msg + '\0' */
            assert(game->bogus[i] != 0);

            game->bogus[i][0] = c;
            strcpy(&game->bogus[i][1], msg);

            game->bg_screen[pos] = c;
        }

        /*
        ** Setup controller input and away we go!
        */
        game->state = IGNORE;
        accel_data_service_subscribe(1, process_input);
        accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
    }

    window_stack_push(game->window, true);

    calibrate(game);
}

static int randxy(Context *game) {
    /*
    ** Find a free position on the screen.  However, make sure we are not
    ** blocked in by other characters.
    */
    for (;;) {
        int i = rand() % SCREEN_MAX;

        /*
        ** Regenerate numbers that land in the "invisible column" or
        ** are taken by the robot (if the foreground layer).
        */
        if ((i % SCREEN_WIDTH) == 0) continue;
        if (game->fg_screen[i] != EMPTY) continue;

        if ((game->bg_screen[i] | game->bg_screen[i-1]
            | game->bg_screen[i+1]) == EMPTY) {
            /*
            ** The number generated is not surrounded by other characters,
            ** so return it to the caller!
            */
            return i;
        }
    }

    return 0; /* We should never reach here */
}

static char randchr(Context *game) {
    int i;
    char c = '\0';

    /*
    ** Find a free character, making sure that we have not already
    ** used it before.
    */
    while (c == '\0') {
        c = (char) (rand() % ('~' - '!')) + '!';

        if (c == ROBOT) {
            c = '\0';
        } else {
            for (i = 0; i < SCREEN_MAX; i++) {
                if (game->bg_screen[i] == c) break;
            }
            if (i < SCREEN_MAX) c = '\0';
        }
    }

    return c;
}

static void calibrate(Context *game) {

    Layer *window_layer = window_get_root_layer(game->window);
    TextLayer *message_layer = 0;

    message_layer = text_layer_create(layer_get_frame(window_layer));
    if (message_layer != 0) {
        text_layer_set_text_alignment(message_layer,
                                      GTextAlignmentCenter);
        text_layer_set_text(message_layer, "\n\n\n\n\nCalibrarting...");
        layer_add_child(window_layer, text_layer_get_layer(message_layer));

        game->state = CALIBRATE;
        game->min_x = game->min_y = game->max_x = game->max_y = 0;
        game->data = message_layer;
        app_timer_register(1000, calibration_complete, game);
    }
}

static void calibration_complete(void *data) {
    Context *game = data;
    TextLayer *message_layer = (TextLayer *)game->data;

    layer_remove_from_parent(text_layer_get_layer(message_layer));
    text_layer_destroy(message_layer);
    game->state = PLAY;
}

static void process_input(AccelData *data,
                          uint32_t num_samples) {

    Window *window = window_stack_get_top_window();
    Context *game = window_get_user_data(window);

    if (game->state == CALIBRATE) {
        /*
        ** We read the accelerometer data to try determine a "normal"
        ** on which all other movement is based.
        */
        if (data->x < game->min_x) game->min_x = data->x;
        if (data->x > game->max_x) game->max_x = data->x;
        if (data->y < game->min_y) game->min_y = data->y;
        if (data->y > game->max_y) game->max_y = data->y;
    } else if (game->state == PLAY) {
        /*
        ** We only want to sample data every 200ms?
        */
        if (data->timestamp < game->timestamp) return;

        /*
        ** Test for movement along the X axis...
        */
        if (data->x < (game->min_x - 75)) {             /* LEFT */
            if (((game->robot - 1) % SCREEN_WIDTH) != 0) {
                if (game->bg_screen[game->robot-1] == EMPTY) {
                    game->fg_screen[game->robot--] = EMPTY;
                    game->fg_screen[game->robot] = ROBOT;
                    layer_mark_dirty(text_layer_get_layer(game->fg_layer));
                } else {
                    // we have a collision and have to do something...
                }
            }
        } else if (data->x > (game->max_x + 75)) {      /* RIGHT */
            if (((game->robot + 1) % SCREEN_WIDTH) != 0) {
                if (game->bg_screen[game->robot+1] == EMPTY) {
                    game->fg_screen[game->robot++] = EMPTY;
                    game->fg_screen[game->robot] = ROBOT;
                    layer_mark_dirty(text_layer_get_layer(game->fg_layer));
                } else {
                    // we have a collision and have to do something...
                }
            }
        } else {
            /*
            ** Test for movement along the Y axis...
            */
            if (data->y > (game->max_y + 5)) {             /* UP */
APP_LOG(APP_LOG_LEVEL_INFO, "UP");
                if ((game->robot - SCREEN_WIDTH) > 0) {
                    if (game->bg_screen[game->robot-SCREEN_WIDTH] == EMPTY) {
                        game->fg_screen[game->robot] = EMPTY;
                        game->robot -= SCREEN_WIDTH;
                        game->fg_screen[game->robot] = ROBOT;
                        layer_mark_dirty(text_layer_get_layer(game->fg_layer));
                    } else {
                        // we have a collision and have to do something
                    }
                }
            } else if (data->y < (game->min_y - 20)) {      /* DOWN */
APP_LOG(APP_LOG_LEVEL_INFO, "DOWN");
                if ((game->robot + SCREEN_WIDTH) < SCREEN_MAX) {
                    if (game->bg_screen[game->robot+SCREEN_WIDTH] == EMPTY) {
                        game->fg_screen[game->robot] = EMPTY;
                        game->robot += SCREEN_WIDTH;
                        game->fg_screen[game->robot] = ROBOT;
                        layer_mark_dirty(text_layer_get_layer(game->fg_layer));
                    } else {
                        // we have a collision and have to do something
                    }
                }
            }
        }

        game->timestamp = data->timestamp + 200;
    }
}

