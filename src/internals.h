#ifndef SEEN_INTERNALS_H
#define SEEN_INTERNALS_H

#include "directionals.h"

enum command { QUIT, MOVE_N, MOVE_S, MOVE_E, MOVE_W, NONE };
enum tiletype { WALL, SPACE };

struct player
{
    struct DR_position p;
    enum DR_direction d;
};
struct mazetile
{
    enum tiletype t;
    float light;
    struct DR_position p;
};
struct maze
{
    struct mazetile *tiles;
    struct DR_position start_position;
    struct DR_position goal_position;
    int size;
};
struct mazegame
{
    struct player player;
    struct maze *maze;
};


enum tiletype get_tiletype ( struct mazetile *tiles, int size, struct DR_position pos );
struct mazegame new_mazegame ( struct maze *maze );
struct mazetile new_mazetile ( enum tiletype type, int x, int y );
struct maze* new_maze_pointer ( int size, struct mazetile *tiles, struct DR_position start_position, struct DR_position goal_position );
void update_game ( struct mazegame *g, enum command player_move );
void destroy_mazegame ();

#endif
