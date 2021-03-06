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
#include <stdbool.h>

#include "internals.h"
#include "directionals.h"
#include "IOStuff.h"
#include "mapper.h"
#include "maze_gen.h"

void print_info ( struct mazegame game );

int main ()
{
    struct mazegame game;
    struct game_display *display;

    /* char *map_file_name = "mazes/mazeFile1.txt"; */

    struct maze *maze;

    int game_running = 1;
    enum command command;

    srand ( time ( NULL ) );

    /* maze = read_map ( map_file_name ); */
    maze = generate_maze ( 20, 70, 50, 30 );

    printf ( "escaped the read_map\n" );
    
    game = new_mazegame ( maze );

    printf ( "initializing display" );
    display = init_display ( game.maze->size, MAINGAME );

    while ( game_running )
    {
        update_display ( display, &game );
        get_command ( &command );
        if ( command == QUIT || DR_equal_pos ( game.player.p, game.maze->goal_position ) || game.player.battery < -5 ) {
          game_running = 0;

        } else if ( RUN == command ) {
          update_game( &game, command );
          update_display( display, &game );
          update_game( &game, command );
          update_display( display, &game );
          update_game( &game, command );

        } else {
          update_game ( &game, command );
        }
    }

    destroy_display ( display );
    destroy_mazegame ( &game );

    return 0;

}

void print_info ( struct mazegame game )
{
    printf ( "player at %d, %d with direction %d\n", game.player.p.x, game.player.p.y, game.player.d );
}
