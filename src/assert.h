/*
**++
**  MODULE DESCRIPTION:
**
**      This module provides an assert-like function for the Pebble.
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
**  CREATION DATE:      08-APR-2014
**
**  MODIFICATION HISTORY:
**
**      08-APR-2014 V1.0    Sneddon  Initial coding.
**--
*/
#ifndef ASSERT_H_
#define ASSERT_H_
#include <pebble.h>
#ifdef __BUILD_ASSERT__
# include <pebble_app_info.h>
#endif /* __BUILD_ASSERT__ */

/*
** assert.c
*/

    void __assert(char *file, int line, const char *function, char *test);

#define assert(test) \
do { \
    if (!(test)) { \
        __assert(__FILE__, __LINE__, __FUNCTION__, #test); \
    } \
} while (0)

#endif /* ASSERT_H_ */
