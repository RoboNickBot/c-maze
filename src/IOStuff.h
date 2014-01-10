#ifndef SEEN_IOSTUFF_H
#define SEEN_IOSTUFF_H

#include "internals.h"

struct game_display;

enum displaytype { MAINGAME, MAZEDEBUG };

struct game_display *init_display ( int mapsize, enum displaytype type );
void update_display ( struct game_display *display, struct mazegame* game );
void destroy_display ( struct game_display *display );
void get_command ( enum command *command );

void display_maze ( struct game_display *display, struct maze *maze );

#endif
