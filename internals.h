#ifndef SEEN_INTERNALS_H
#define SEEN_INTERNALS_H

#include <stdbool.h>

enum command { MOVE_N, MOVE_S, MOVE_E, MOVE_W, NONE };
enum direction { NORTH, SOUTH, EAST, WEST };
enum tiletype { WALL, SPACE };

struct position
{
    int x;
    int y;
};
struct player
{
    struct position p;
    enum direction d;
};
struct mazetile
{
    enum tiletype t;
    int light;
    struct position p;
};
struct mazegame
{
    struct player player;
    struct mazetile **tiles;
    struct position goal_position;
    int mapsize;
};


struct mazegame new_mazegame ( int mapsize, enum tiletype **tiles, struct position start_pos, struct position goal_pos );
struct position new_position ( int x, int y );
void update_game ( struct mazegame *g, enum command player_move );
bool equal_pos ( struct position a, struct position b );

#endif
