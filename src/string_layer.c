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
#define __STRING_LAYER_BUILD__
#include "string_layer.h"

/*
** Forward Declarations
*/

    StringLayer *string_layer_create();
    void string_layer_destroy(StringLayer *string_layer);
    Layer *string_layer_get_layer(StringLayer *string_layer);
    const char *string_layer_get_text(StringLayer *string_layer);
    GTextOrientation string_layer_get_text_orientation(
        StringLayer *string_layer);
    void string_layer_set_text(StringLayer *string_layer, const char *text);
    void string_layer_set_text_alignment(StringLayer *string_layer,
                                         GTextAlignment text_alignment);
    void string_layer_set_text_color(StringLayer *string_layer,
                                     GColor text_color);
    void string_layer_set_text_orientation(StringLayer *string_layer,
                                           GTextOrientation text_orientation);

StringLayer *string_layer_create(GRect frame) {
    StringLayer *string_layer;

    string_layer = malloc(sizeof(StringLayer));
    if (string_layer != 0) {
        string_layer->layer = layer_create(frame);
        if (string_layer->layer != 0) {
            string_layer->text_color = GColorBlack;
            string_layer->background_color = GColorWhite;
            string_layer->text_alignment = GTextAlignmentLeft;
            string_layer->text_orientation = GTextOrientationPortrait;
            string_layer->font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);

            // set the update proc...
        } else {
            free(string_layer);
            string_layer = 0;
        }
    }

    return string_layer;
}

void string_layer_destroy(StringLayer *string_layer) {
    if (string_layer == 0) return;

    layer_destroy(string_layer->layer);
    free(string_layer);
}

Layer *string_layer_get_layer(StringLayer *string_layer) {
    if (string_layer == 0) return 0;
    return string_layer->layer;
}

const char *string_layer_get_text(StringLayer *string_layer) {
    if (string_layer == 0) return 0;
    return string_layer->text;
}

GTextOrientation string_layer_get_text_orientation(StringLayer *string_layer) {
    if (string_layer == 0) return 0;
    return string_layer->text_orientation;
}

void string_layer_set_text(StringLayer *string_layer,
                           const char *text) {
    if (string_layer == 0) return;
    string_layer->text = text;
}

void string_layer_set_text_alignment(StringLayer *string_layer,
                                     GTextAlignment text_alignment) {
    if (string_layer == 0) return;
    string_layer->text_alignment = text_alignment;
}

void string_layer_set_text_color(StringLayer *string_layer,
                                 GColor text_color) {
    if (string_layer == 0) return;
    string_layer->text_color = text_color;
}

void string_layer_set_text_orientation(StringLayer *string_layer,
                                       GTextOrientation text_orientation) {
    if (string_layer == 0) return;
    string_layer->text_orientation = text_orientation;
}

