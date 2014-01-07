#ifndef SEEN_INTERNALS_H
#define SEEN_INTERNALS_H

#include <stdbool.h>

enum orientation { STRAIGHT, LEFT, RIGHT };
enum command { QUIT, MOVE_N, MOVE_S, MOVE_E, MOVE_W, NONE };
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
    float light;
    struct position p;
};
struct maze
{
    struct mazetile *tiles;
    struct position start_position;
    struct position goal_position;
    int size;
};
struct mazegame
{
    struct player player;
    struct maze *maze;
};


struct mazegame new_mazegame ( struct maze *maze );
struct position new_position ( int x, int y );
struct mazetile new_mazetile ( enum tiletype type, int x, int y );
struct maze* new_maze_pointer ( int size, struct mazetile *tiles, struct position start_position, struct position goal_position );
void update_game ( struct mazegame *g, enum command player_move );
bool equal_pos ( struct position a, struct position b );
void destroy_mazegame ();

#endif
