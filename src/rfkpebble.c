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

/*
** Forward declarations
*/

    static void init(void);
    static void deinit(void);
    int main(void);

/*
** Own storage
*/

    static Window *window = 0;
    static Layer *window_layer = 0;

static void init(void) {
    window = window_create();
    if (window == 0) {
        // error...
        return;
    }
    window_set_fullscreen(window, true);
    window_layer = window_get_root_layer(window);
    window_stack_push(window, true);

    robot_init(window);
}

static void deinit(void) {
    if (window != 0) {
        window_destroy(window);
    }
}

int main(void) {

    init();
    app_event_loop();
    deinit();
}
