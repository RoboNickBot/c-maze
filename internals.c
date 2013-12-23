#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "internals.h"

struct position new_position ( int x, int y );
struct position copy_position ( struct position other );
struct player new_player ( struct position start_pos );
struct mazetile new_mazetile ( enum tiletype type, int x, int y );

struct mazegame new_mazegame ( int mapsize, enum tiletype **tiles, struct position start_pos, struct position goal_pos )
{
    int x;
    int y;
    struct mazegame game;
    
    game.player = new_player ( start_pos );
    game.goal_position = copy_position ( goal_pos );
    
    /* I need to malloc a block of mapsize pointers, and then malloc a block of mapsize mazetiles at each of those pointers
     * 
     * All of this then goes in game.tiles
     */
    game.tiles = malloc ( sizeof ( struct mazetile * ) * mapsize );
    for ( x = 0; x < mapsize; x++ )
    {
        game.tiles[x] = malloc ( sizeof ( struct mazetile ) * mapsize );
        for ( y = 0; y < mapsize; y++ )
        {
            game.tiles[x][y] = new_mazetile ( tiles[x][y], x, y );
        }
    }

    return game;
}

struct player new_player ( struct position start_pos )
{
    struct player player;
    player.p = copy_position ( start_pos );
    player.d = SOUTH;
    
    return player;
}

struct mazetile new_mazetile ( enum tiletype type, int x, int y )
{
    struct mazetile mazetile;
    mazetile.t = type;
    mazetile.p = new_position ( x, y );
    mazetile.light = 0;

    return mazetile;
}

struct position copy_position ( struct position other )
{
    struct position position;
    position.x = other.x;
    position.y = other.y;

    return position;
}
struct position new_position ( int x, int y )
{
    struct position position;
    position.x = x;
    position.y = y;
    
    return position;
}
bool equal_pos ( struct position a, struct position b )
{
    return ( a.x == b.x && a.y == b.y );
}

void update_game ( struct mazegame *g, enum command player_move )
{
    switch ( player_move )
    {
        case MOVE_N:
            g->player.p.y--;
            break;
        case MOVE_S:
            g->player.p.y++;
            break;
        case MOVE_W:
            g->player.p.x--;
            break;
        case MOVE_E:
            g->player.p.x++;
            printf ( "I moved!" );
            break;
        default:
            break;
    }
}
