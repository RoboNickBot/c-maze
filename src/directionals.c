#include "directionals.h"

struct DR_position DR_new_position ( int x, int y )
{
    struct DR_position pos;
    pos.x = x;
    pos.y = y;
    
    return pos;
}
int DR_equal_pos ( struct DR_position a, struct DR_position b )
{
    return ( a.x == b.x && a.y == b.y );
}

struct DR_position DR_get_adj ( struct DR_position this_pos, enum DR_direction dir )
{
    struct DR_position result;

    switch ( dir )
    {
        case NORTH:
            result = DR_new_position ( this_pos.x, this_pos.y-1 );
            break;
        case NORTHEAST:
            result = DR_new_position ( this_pos.x+1, this_pos.y-1 );
            break;
        case NORTHWEST:
            result = DR_new_position ( this_pos.x-1, this_pos.y-1 );
            break;
        case SOUTH:
            result = DR_new_position ( this_pos.x, this_pos.y+1 );
            break;
        case SOUTHEAST:
            result = DR_new_position ( this_pos.x+1, this_pos.y+1 );
            break;
        case SOUTHWEST:
            result = DR_new_position ( this_pos.x-1, this_pos.y+1 );
            break;
        case EAST:
            result = DR_new_position ( this_pos.x+1, this_pos.y );
            break;
        case WEST:
            result = DR_new_position ( this_pos.x-1, this_pos.y );
            break;
        default:
            result = this_pos;
            break;
    }

    return result;
}

enum DR_direction DR_get_rel ( enum DR_direction this_dir, enum DR_orientation relative_orientation )
{
    enum DR_direction result;
    result = this_dir + relative_orientation;

    if ( result >= FULL_CYCLE )
    {
        result -= FULL_CYCLE;
    }

    return result;
}
