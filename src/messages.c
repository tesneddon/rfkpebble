/*
**++
**  MODULE DESCRIPTION:
**
**      Retrieve random message for store.  As there are so many messages
**  we store a small collection on the Pebble (in case the phone cannot be
**  reached) and store the rest in the Pebble-JS framework and use
**  app_message to retrieve them.
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
#include "messages.h"
#define KEY_ID_MESSAGE 0

/*
** Forward declarations
*/

void message_init(Window *window) ;
    static void sync_error(DictionaryResult dict_status,
                       AppMessageResult app_msg_status,
                       void *context) ;
    static void sync_changed(const uint32_t key,
                         const Tuple *new,
                         const Tuple *old,
                         void *context) ;
    char *message_get(void);

/*
** Own storage
*/

    static AppSync sync;
    static uint8_t sync_buffer[124];
    static const Tuplet dummy[] = { {
            .type = TUPLE_CSTRING,
            .key = KEY_ID_MESSAGE,
            .cstring = {
                .data = "",
                .length = 0,
            },
        },
    };


void message_init(Window *window) {
    int status;
//    Layer *window_layer = window_get_root_layer(window);

    status = app_message_open(app_message_inbox_size_maximum(),
                              app_message_outbox_size_maximum());
APP_LOG(APP_LOG_LEVEL_INFO, "app_message_open=%d;ok=%d", status,APP_MSG_OK);
    if (status == APP_MSG_OK) {
        app_sync_init(&sync, sync_buffer, sizeof(sync_buffer),
                      dummy, ARRAY_LENGTH(dummy), sync_changed,
                      sync_error, 0);
    }
}

static void sync_error(DictionaryResult dict_status,
                       AppMessageResult app_msg_status,
                       void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "failed to retrieve a message;[%s]",
            messages[45]);
}

static void sync_changed(const uint32_t key,
                         const Tuple *new,
                         const Tuple *old,
                         void *context) {
    APP_LOG(APP_LOG_LEVEL_INFO, "got a message back....");
}

char *message_get(void) {

    static char msg[132];
    int status;

    strcpy(msg, "testing...");


    status = app_sync_set(&sync, dummy, ARRAY_LENGTH(dummy));
    APP_LOG(APP_LOG_LEVEL_INFO, "status =%d", status);

    return msg;
}
