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
#define KEY_ID_MESSAGE 0

/*
** Forward declarations
*/

    void message_init(void);
    void message_deinit(void);
    static void sync_error(DictionaryResult dict_status,
                           AppMessageResult app_msg_status, void *context);
    static void sync_changed(const uint32_t key, const Tuple *new,
                             const Tuple *old, void *context);

/*
** Own storage
*/

    static bool message_initialised = false;
    static AppSync sync;
    static uint8_t sync_buffer[124];

void message_init(void) {
    int status;
    Tuplet initial[] = {
        TupletCString(KEY_ID_MESSAGE, ""),
    };

    if (message_initialised) return;

    status = app_message_open(app_message_inbox_size_maximum(),
                              app_message_outbox_size_maximum());
    if (status == APP_MSG_OK) {
        app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial,
                      ARRAY_LENGTH(initial), sync_changed, sync_error, 0);
        message_initialised = true;
    }
}

void message_deinit(void) {
    if (!message_initialised) return;

    app_sync_deinit(&sync);
}

static void sync_error(DictionaryResult dict_status,
                       AppMessageResult app_msg_status,
                       void *context) {
}

static void sync_changed(const uint32_t key,
                         const Tuple *new,
                         const Tuple *old,
                         void *context) {
}
