#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "internals.h"
#include "IOStuff.h"
#include "mapper.h"

void print_info ( struct mazegame game );

int main ()
{
    struct mazegame game;
    struct game_display *display;

    char *map_file_name = "mazeFile1.txt";

    enum tiletype *map;
    struct position start_pos;
    struct position goal_pos;

    int x;
    int mapsize = 30;
    int game_running = 1;
    enum command command;
    enum tiletype test;

    map = read_map ( mapsize, map_file_name, &start_pos, &goal_pos );

    printf ( "escaped the read_map\n" );
    
    game = new_mazegame ( mapsize, map, start_pos, goal_pos );

    printf ( "initializing display" );
    display = init_display ( mapsize );

    while ( game_running )
    {
        update_display ( display, &game );
        get_command ( &command );
        if ( command == QUIT )
        {
            game_running = 0;
        }
        else
        {
            update_game ( &game, command );
        }
    }

    destroy_display ( display );
    destroy_mazegame ( game );

    return 0;

}

void print_info ( struct mazegame game )
{
    printf ( "player at %d, %d with direction %d\n", game.player.p.x, game.player.p.y, game.player.d );
}
