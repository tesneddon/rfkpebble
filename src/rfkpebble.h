/*
**++
**  MODULE DESCRIPTION:
**
**      Main include for robotfindskitten.
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
#ifndef RFKPEBBLE_H_
#define RFKPEBBLE_H_
#include <pebble.h>
#include "assert.h"

/*
** Useful macros...
*/
#define abs(i) ((i) < 0 ? -(i) : (i))

/*
** Our data types.
*/

    struct context_;
    typedef struct context_ Context;

/* game.c */

    Context *game_init(void);
    void go(Context *ctx);

/* input.c */

    void input_init(void);
    void input_deinit(void);

/* message.c */

    void message_init(Window *window);
    char *message_get(void);

/* robot.c */

    void robot_init(Window *window);
    void robot_deinit(void);

#endif /* RFKPEBBLE_H_ */
