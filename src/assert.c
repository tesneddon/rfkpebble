/*
**++
**  MODULE DESCRIPTION:
**
**      This module contains an assert function for Pebble.  It writes
**  a message to the log feed, using APP_LOG and a level of
**  APP_LOG_LEVEL_ERROR.  It also closes the application and displays a
**  window showing the assertion message.
**
**  AUTHOR:             Tim Sneddon
**
**  Copyright (C) 2014 Endless Software Solutions.
**
**  All rights reserved.
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions
**  are met:
**
**      * Redistributions of source code must retain the above
**        copyright notice, this list of conditions and the following
**        disclaimer.
**      * Redistributions in binary form must reproduce the above
**        copyright notice, this list of conditions and the following
**        disclaimer in the documentation and/or other materials provided
**        with the distribution.
**      * Neither the name of the copyright owner nor the names of any
**        other contributors may be used to endorse or promote products
**        derived from this software without specific prior written
**        permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
**  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
**  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
**  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
**  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
**  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
**  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
**  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
**  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
**  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  CREATION DATE:      08-APR-2014
**
**  MODIFICATION HISTORY:
**
**      08-APR-2014 V1.0    Sneddon  Initial coding.
**      15-MAY-2014 V1.1    Sneddon  Change license.
**--
*/
#define __BUILD_ASSERT__
#include "assert.h"

/*
** Forward declarations
*/

    void __assert(char *file, int line, const char *function, char *test);
    static void click_config(void *context);
    static void click_handler(ClickRecognizerRef recognizer, void *ctx);
    static void unload(Window *window);

/*
** Own storage
*/

    static TextLayer *header_layer = 0, *footer_layer = 0, *message_layer = 0;

void __assert(char *file,
              int line,
              const char *function,
              char *test) {

    static char message[132], header[2+APP_NAME_BYTES+1];
    static const char *footer = "Press Any Button...";

    extern PebbleAppInfo __pbl_app_info;
    Window *window = 0;
    Layer *window_layer;

    /*
    ** Generate the assertion message.
    */
    snprintf(header, sizeof(header)-1, "<%s>", __pbl_app_info.name);
    snprintf(message, sizeof(message)-1, "%s:%d:%s: Assertion `%s` failed.",
             file, line, function, test);

    /*
    ** Dump it to the log feed.
    */
    APP_LOG(APP_LOG_LEVEL_ERROR, "%s %s", header, message);

    /*
    ** Now, implement our own version of the 'App Crashed!' window.
    */
    window_stack_pop_all(false);
    window = window_create();
    if (window != 0) {
        window_layer = window_get_root_layer(window);
        header_layer = text_layer_create(GRect(0, 0, 144, 22));
        message_layer = text_layer_create(GRect(0, 22, 144, 128));
        footer_layer = text_layer_create(GRect(0, 135, 144, 18));
        if ((header_layer != 0)
            && (message_layer != 0)
            && (footer_layer != 0)) {

            /*
            ** Setup the header.
            */
            text_layer_set_text(header_layer, header);
            text_layer_set_overflow_mode(header_layer,
                                         GTextOverflowModeTrailingEllipsis);
            text_layer_set_font(header_layer,
                              fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
            layer_add_child(window_layer, text_layer_get_layer(header_layer));

            /*
            ** Setup the message.
            */
            text_layer_set_text(message_layer, message);
            text_layer_set_overflow_mode(message_layer,
                                         GTextOverflowModeTrailingEllipsis);
            text_layer_set_font(header_layer,
                              fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
            layer_add_child(window_layer, text_layer_get_layer(message_layer));

            /*
            ** Setup the footer.
            */
            text_layer_set_text(footer_layer, footer);
            layer_add_child(window_layer, text_layer_get_layer(footer_layer));
        }
        window_set_window_handlers(window, (WindowHandlers) {
            .unload = unload,
        });
        window_set_click_config_provider(window, click_config);
        window_stack_push(window, false);
    }
}

static void click_config(void *ctx) {
    window_single_click_subscribe(BUTTON_ID_BACK, click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, click_handler);
}

static void click_handler(ClickRecognizerRef recognizer,
                          void *ctx) {
    window_stack_pop_all(false);
}

static void unload(Window *window) {

    Layer *window_layer = window_get_root_layer(window);

    layer_remove_child_layers(window_layer);

    text_layer_destroy(header_layer);
    text_layer_destroy(message_layer);
    text_layer_destroy(footer_layer);

    window_destroy(window);
}
