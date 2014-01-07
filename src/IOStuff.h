#ifndef SEEN_IOSTUFF_H
#define SEEN_IOSTUFF_H

#include "internals.h"

struct game_display;

struct game_display *init_display ( int mapsize );
void update_display ( struct game_display *display, struct mazegame* game );
void destroy_display ( struct game_display *display );
void get_command ( enum command *command );

#endif
