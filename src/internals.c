#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "internals.h"

struct position new_position ( int x, int y );
struct position copy_position ( struct position other );
struct player new_player ( struct position start_pos );
struct mazetile new_mazetile ( enum tiletype type, int x, int y );
void destroy_maze ( struct maze *maze );

struct mazegame new_mazegame ( struct maze *maze )
{
    struct mazegame game;
    
    game.player = new_player ( maze->start_position );
    game.maze = maze;
    
    return game;
}

void destroy_mazegame ( struct mazegame *game )
{
    destroy_maze ( game->maze );
    free ( game->maze );
}
void destroy_maze ( struct maze *maze )
{
    free ( maze->tiles );
}

struct player new_player ( struct position start_pos )
{
    struct player player;
    player.p = copy_position ( start_pos );
    player.d = SOUTH;
    
    return player;
}
struct maze* new_maze_pointer ( int size, struct mazetile *tiles, struct position start_position, struct position goal_position )
{
    struct maze *maze;
    maze = malloc ( sizeof ( struct maze ) );
    maze->size = size;
    maze->tiles = tiles;
    maze->start_position = start_position;
    maze->goal_position = goal_position;

    return maze;
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

void reset_light ( struct mazegame *game )
{
    int x;
    int y;
    int s = game->maze->size;
    for ( x = 0; x < s; x++ )
    {
        for ( y = 0; y < s; y++ )
        {
            game->maze->tiles[ x * s + y ].light = 0;
        }
    }
}

void recurse_light ( float value, struct maze *maze, enum direction d, enum orientation o, struct position p )
{
    /*printf ( "starting recurse light with %f\n", value );*/
    float divisor = 2.0;
    float side_divisor = 3.0;
    int s = maze->size;
    maze->tiles[p.x * s + p.y].light += value;
    if ( value >= 1.0 && maze->tiles[p.x * s + p.y].t == SPACE )
    {
        switch ( o )
        {
            case STRAIGHT:
                switch ( d )
                {
                    case NORTH:
                        recurse_light ( value / divisor, maze, d, STRAIGHT, new_position ( p.x, p.y-1 ));
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x-1, p.y-1 ));
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x+1, p.y-1 ));
                        break;
                    case SOUTH:
                        recurse_light ( value / divisor, maze, d, STRAIGHT, new_position ( p.x, p.y+1 ));
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x-1, p.y+1 ));
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x+1, p.y+1 ));
                        break;
                    case EAST:
                        recurse_light ( value / divisor, maze, d, STRAIGHT, new_position ( p.x+1, p.y ));
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x+1, p.y-1 ));
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x+1, p.y+1 ));
                        break;
                    case WEST:
                        recurse_light ( value / divisor, maze, d, STRAIGHT, new_position ( p.x-1, p.y ));
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x-1, p.y-1 ));
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x-1, p.y+1 ));
                        break;
                    default:
                        break;
                }
                break;
            case LEFT:
                switch ( d )
                {
                    case NORTH:
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x-1, p.y-1 ));
                        break;
                    case SOUTH:
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x+1, p.y+1 ));
                        break;
                    case EAST:
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x+1, p.y-1 ));
                        break;
                    case WEST:
                        recurse_light ( value / side_divisor, maze, d, LEFT, new_position ( p.x-1, p.y+1 ));
                        break;
                    default:
                        break;
                }
                break;
            case RIGHT:
                switch ( d )
                {
                    case NORTH:
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x+1, p.y-1 ));
                        break;
                    case SOUTH:
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x-1, p.y+1 ));
                        break;
                    case EAST:
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x+1, p.y+1 ));
                        break;
                    case WEST:
                        recurse_light ( value / side_divisor, maze, d, RIGHT, new_position ( p.x-1, p.y-1 ));
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}


void update_game ( struct mazegame *g, enum command player_move )
{
    int s = g->maze->size;
    switch ( player_move )
    {
        case MOVE_N:
            if ( g->player.d == NORTH )
            {
                if ( g->maze->tiles[g->player.p.x * s + g->player.p.y - 1].t == SPACE )
                {
                    g->player.p.y--;
                }
            }
            else
            {
                g->player.d = NORTH;
            }
            break;
        case MOVE_S:
            if ( g->player.d == SOUTH )
            {
                if ( g->maze->tiles[g->player.p.x * s + g->player.p.y + 1].t == SPACE )
                {
                    g->player.p.y++;
                }
            }
            else
            {
                g->player.d = SOUTH;
            }
            break;
        case MOVE_W:
            if ( g->player.d == WEST )
            {
                if ( g->maze->tiles[ ( g->player.p.x - 1 ) * s + g->player.p.y].t == SPACE )
                {
                    g->player.p.x--;
                }
            }
            else
            {
                g->player.d = WEST;
            }
            break;
        case MOVE_E:
            if ( g->player.d == EAST )
            {
                if ( g->maze->tiles[ ( g->player.p.x + 1 ) * s + g->player.p.y].t == SPACE )
                {
                    g->player.p.x++;
                }
            }
            else
            {
                g->player.d = EAST;
            }
            break;
        default:
            break;
    }

    reset_light ( g );
    recurse_light ( 10, g->maze, g->player.d, STRAIGHT, g->player.p );
}
