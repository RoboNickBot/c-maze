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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "maze_gen.h"
#include "internals.h"
#include "mapper.h"
#include "image_key.h"
#include "IOStuff.h"

int main ( int argc, char *argv[] )
{
    struct maze *maze;
    struct game_display *display;
    int c = 1;
    enum command com;

    int maze_size = 20;
    int maze_twisty = 70;
    int maze_swirly = 50;
    int maze_branchy = 30;

    if ( argc != 1 && argc != 5 )
    {
        printf ( "Syntax: %s [size] [twisty] [swirly] [branchy]\n", argv[0] );
        printf ( "  if no options are specified, defaults will be %d %d %d %d\n", maze_size, maze_twisty, maze_swirly, maze_branchy );
    }
    else
    {
        if ( argc == 5 )
        {
            maze_size = atoi ( argv[1] );
            maze_twisty = atoi ( argv[2] );
            maze_swirly = atoi ( argv[3] );
            maze_branchy = atoi ( argv[4] );
        }

        srand ( time ( NULL ) );
        maze = generate_maze ( maze_size, maze_twisty, maze_swirly, maze_branchy );
    
        write_map ( maze, "mazes/gentest3.txt" );
    
        display = init_display ( maze->size, MAZEDEBUG );
    
        display_maze ( display, maze );
    
        while ( c )
        {
            get_command ( &com );
            if ( com == QUIT )
            {
                c = 0;
            }
        }
    
        destroy_display ( display );
    
    
        destroy_maze ( maze );
        free ( maze );
    }
    
    return 0;
}
