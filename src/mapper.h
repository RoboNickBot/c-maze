/*
 *  << c-maze, a simple generated maze crawler written in C >>
 *  Copyright (C) 2013 Nick Lewchenko
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SEEN_MAPPER_H
#define SEEN_MAPPER_H

#include "internals.h"

struct maze* read_map ( char *map_file_name );

void write_map ( struct maze *maze, char *map_file_name );

#endif
