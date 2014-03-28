/*
**++
**  MODULE DESCRIPTION:
**
**      This module contains an improved TextLayer that allows
**  for both vertical and horizontal display.
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
**  CREATION DATE:      28-MAR-2014
**
**  MODIFICATION HISTORY:
**
**      28-MAR-2014 V1.0    Sneddon  Initial coding.
**--
*/
#ifndef STRING_LAYER_H_
#define STRING_LAYER_H_
#include <pebble.h>
#ifdef __STRING_LAYER_BUILD__
    struct StringLayer {
        // put our stuff in here...
    };
#else
    struct StringLayer;
#endif /* __STRING_LAYER_BUILD__ */

    typedef struct StringLayer StringLayer;

/*
** string_layer.c
*/

    string_layer_create
    string_layer_destroy
    string_layer_get_layer
    string_layer_get_text
    string_layer_set_font
    string_layer_set_overflow_mode
    string_layer_set_text
    string_layer_set_text_alignment
    string_layer_set_text_color

#endif /* STRING_LAYER_H_ */
