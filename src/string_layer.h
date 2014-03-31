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

    typedef enum {
        GTextOrientationPortrait,
        GTextOrientationLandscape,
    } GTextOrientation;

#ifdef __STRING_LAYER_BUILD__
    typedef Layer StringLayer;
    typedef struct StringLayerData {
        const char *text;
        GFont font;
        GColor text_color : 2;
        GColor background_color : 2;
        GTextOverflowMode overflow_mode : 2;
        GTextAlignment text_alignment : 2;
        GTextOrientation text_orientation : 2;
    } StringLayerData;
#else
    struct StringLayer;
    typedef struct StringLayer StringLayer;
#endif /* __STRING_LAYER_BUILD__ */

/*
** string_layer.c
*/

    StringLayer *string_layer_create(GRect frame);
    void string_layer_destroy(StringLayer *string_layer);
    Layer *string_layer_get_layer(StringLayer *string_layer);
    GTextOrientation string_layer_get_text_orientation(StringLayer *string_layer);
    const char *string_layer_get_text(StringLayer *string_layer);
    void string_layer_set_text(StringLayer *string_layer, const char *text);
    void string_layer_set_text_alignment(StringLayer *string_layer,
                                         GTextAlignment text_alignment);
    void string_layer_set_text_color(StringLayer *string_layer,
                                     GColor text_color);
    void string_layer_set_text_orientation(StringLayer *string_layer,
                                           GTextOrientation text_orientation);

#endif /* STRING_LAYER_H_ */
