#ifndef SEEN_DIRECTIONALS_H
#define SEEN_DIRECTIONALS_H

enum DR_direction { NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST, FULL_CYCLE };
enum DR_orientation { FRONT, FRONTRIGHT, RIGHT, BACKRIGHT, BACK, BACKLEFT, LEFT, FRONTLEFT };

struct DR_position
{
    int x;
    int y;
};

struct DR_position DR_new_position ( int x, int y );

struct DR_position DR_get_adj ( struct DR_position this_pos, enum DR_direction dir );
enum DR_direction DR_get_rel ( enum DR_direction this_dir, enum DR_orientation relative_orientation );

#endif
