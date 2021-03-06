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

#ifndef SEEN_IOSTUFF_H
#define SEEN_IOSTUFF_H

#include "internals.h"

struct game_display;

enum displaytype { MAINGAME, MAZEDEBUG };

struct game_display *init_display ( int mapsize, enum displaytype type );
void update_display ( struct game_display *display, struct mazegame* game );
void destroy_display ( struct game_display *display );
void get_command ( enum command *command );

void display_maze ( struct game_display *display, struct maze *maze );

#endif
