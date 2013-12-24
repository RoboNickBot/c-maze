#include <stdlib.h>
#include <stdio.h>

#include "internals.h"
#include "IOStuff.h"

void print_info ( struct mazegame game );

int main ()
{
    struct mazegame game;
    struct game_display *display;
    enum tiletype **map;
    struct position start_pos;
    struct position goal_pos;
    int x;
    int mapsize = 4;
    
    start_pos = new_position ( 1, 1 );
    goal_pos = new_position ( 2, 1 );

    map = malloc ( sizeof ( enum tiletype * ) * mapsize );
    for ( x = 0; x < mapsize; x++ )
    {
        map[x] = malloc ( sizeof ( enum tiletype ) * mapsize );
    }

    map[0][0] = WALL;
    map[0][1] = WALL;
    map[0][2] = WALL;
    map[0][3] = WALL;
    map[1][0] = WALL;
    map[1][1] = SPACE;
    map[1][2] = SPACE;
    map[1][3] = WALL;
    map[2][0] = WALL;
    map[2][1] = SPACE;
    map[2][2] = SPACE;
    map[2][3] = WALL;
    map[3][0] = WALL;
    map[3][1] = WALL;
    map[3][2] = WALL;
    map[3][3] = WALL;

    game = new_mazegame ( mapsize, map, start_pos, goal_pos );

    display = init_display ( mapsize );

    printf ( "test\n" );
    update_display ( display, &game );
    print_info ( game );

    update_game ( &game, MOVE_E );
    update_display ( display, &game );
    print_info ( game );

    update_game ( &game, MOVE_E );
    update_display ( display, &game );
    print_info ( game );

    update_game ( &game, MOVE_N );
    update_display ( display, &game );
    print_info ( game );

    update_game ( &game, MOVE_N );
    update_display ( display, &game );
    print_info ( game );

    destroy_display ( display );

    return 0;

}

void print_info ( struct mazegame game )
{
    printf ( "player at %d, %d with direction %d\n", game.player.p.x, game.player.p.y, game.player.d );
}
