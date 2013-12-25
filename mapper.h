#ifndef SEEN_MAPPER_H
#define SEEN_MAPPER_H

#include "internals.h"

enum tiletype*  read_map ( int mapsize, char *map_file_name, struct position *p_pos, struct position *g_pos );

#endif
