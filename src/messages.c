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

    char *message_get(void);

char *message_get(void) {

    static char msg[132];

    strcpy(msg, "testing...");

    return msg;
}
