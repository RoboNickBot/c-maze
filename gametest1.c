#include <stdlib.h>
#include <stdio.h>

#include "internals.h"

void print_info ( struct mazegame game );

int main ()
{
    struct mazegame game;
    enum tiletype **map;
    struct position start_pos;
    struct position goal_pos;
    int x;
    
    start_pos = new_position ( 0, 0 );
    goal_pos = new_position ( 2, 1 );

    map = malloc ( sizeof ( enum tiletype * ) * 3 );
    for ( x = 0; x < 3; x++ )
    {
        map[x] = malloc ( sizeof ( enum tiletype ) * 3 );
    }

    map[0][0] = WALL;
    map[0][1] = WALL;
    map[0][2] = WALL;
    map[1][0] = WALL;
    map[1][1] = SPACE;
    map[1][2] = WALL;
    map[2][0] = WALL;
    map[2][1] = WALL;
    map[2][2] = WALL;

    game = new_mazegame ( 3, map, start_pos, goal_pos );

    print_info ( game );

    update_game ( &game, MOVE_E );

    print_info ( game );

    return 0;

}

void print_info ( struct mazegame game )
{
    printf ( "player at %d, %d\n", game.player.p.x, game.player.p.y );
}
