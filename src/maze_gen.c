#include <stdlib.h>
#include <stdio.h>

#include "maze_gen.h"
#include "internals.h"

struct probs
{
    double twisty; /* Probability of changing direction */
    double swirly; /* Probability of going right when changing direction (so non-swirly would be 0.5) */
    double branchy; /* Probability of branching */
};

struct path_head
{
    struct position p;
    enum direction d;
    double g; /* growth probability */
    int still_space;

    struct path_head *next;
};

struct path_head *new_path ( struct position p, enum direction d, double g )
{
    struct path_head *path;
    printf ( "empty path pointer created\n" );
    path = malloc ( sizeof ( struct path_head ) );
    printf ( "path_head allocated\n" );
    path->p = p;
    path->d = d;
    path->g = g;
    path->still_space = 1;
    path->next = NULL;
    printf ( "fields filled\n" );

    return path;
}

void add_path ( struct path_head *last, struct path_head *addition )
{
    if ( last->next == NULL )
    {
        last->next = addition;
    }
    else
    {
        add_path ( last->next, addition );
    }
}

void destroy_paths ( struct path_head *last )
{
    if ( last->next != NULL )
    {
        destroy_paths ( last->next );
        free ( last->next );
    }
}

struct position get_adj ( struct position pos, enum direction d )
{
    struct position adj;
    switch ( d )
    {
        case NORTH:
            adj = new_position ( pos.x, pos.y-1 );
            break;
        case SOUTH:
            adj = new_position ( pos.x, pos.y+1 );
        case EAST:
            adj = new_position ( pos.x+1, pos.y );
            break;
        case WEST:
            adj = new_position ( pos.x-1, pos.y );
            break;
        default:
            break;
    }

    return adj;
}
enum tiletype get_tiletype ( struct mazetile *tiles, int size, struct position pos )
{
    return tiles[pos.x * size + pos.y].t;
}
enum direction get_opposite ( enum direction d )
{
    enum direction o;
    switch ( d )
    {
        case NORTH:
            o = SOUTH;
            break;
        case SOUTH:
            o = NORTH;
            break;
        case EAST:
            o = WEST;
            break;
        case WEST:
            o = EAST;
            break;
        default:
            break;
    }
    return o;
}
enum direction get_side ( enum direction d, enum orientation s )
{
    enum direction o;
    switch ( s )
    {
        case LEFT:
    switch ( d )
    {
        case NORTH:
            o = WEST;
            break;
        case SOUTH:
            o = EAST;
            break;
        case EAST:
            o = NORTH;
            break;
        case WEST:
            o = SOUTH;
            break;
        default:
            break;
    }
            break;
        case RIGHT:
    switch ( d )
    {
        case NORTH:
            o = EAST;
            break;
        case SOUTH:
            o = WEST;
            break;
        case EAST:
            o = SOUTH;
            break;
        case WEST:
            o = NORTH;
            break;
        default:
            break;
    }
            break;
        default:
            o = d;
    }
    return o;
}

int check_space ( struct mazetile *tiles, int size, struct position pos, enum direction d )
{
    if ( pos.x == 0 || pos.x == size-1 || pos.y == 0 || pos.y == size-1 )
    {
        return 0;
    }
    enum tiletype n, s, e, w;
    enum direction o = get_opposite ( d );
    n = get_tiletype ( tiles, size, get_adj ( pos, NORTH ) );
    s = get_tiletype ( tiles, size, get_adj ( pos, SOUTH ) );
    e = get_tiletype ( tiles, size, get_adj ( pos, EAST ) );
    w = get_tiletype ( tiles, size, get_adj ( pos, WEST ) );

    if ( ! ( n == WALL || o == NORTH && s == WALL || o == SOUTH && e == WALL || o == EAST && w == WALL || o == WEST ) )
    {
        return 0;
    }

    return 1;
}
int check_all ( struct mazetile *tiles, int size, struct position pos, enum direction d )
{
    int f, l, r;

    f = check_space ( tiles, size, get_adj ( pos, d ), d );
    l = check_space ( tiles, size, get_adj ( pos, get_side ( d, LEFT ) ), get_side ( d, LEFT ) );
    r = check_space ( tiles, size, get_adj ( pos, get_side ( d, RIGHT ) ), get_side ( d, RIGHT ) );

    if ( f || l || r )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int advance_path ( struct mazetile *tiles, int size, struct path_head *last, struct probs probs )
{
    enum orientation o;
    enum direction dir;
    struct position try_pos;
    double r;
    if ( ! check_all ( tiles, size, last->p, last->d ) )
    {
        last->still_space = 0;
        return 0;
    }

    r = rand () / ( double ) RAND_MAX;
    if ( r < last->g )
    {
        r = rand () / ( double ) RAND_MAX;
        if ( r < probs.twisty )
        {
            o = STRAIGHT;
        }
        else
        {
            r = rand () / ( double ) RAND_MAX;
            if ( r < probs.swirly )
            {
                o = RIGHT;
            }
            else
            {
                o = LEFT;
            }
        }
        dir = get_side ( last->d, o );
        try_pos = get_adj ( last->p, dir );

        if ( check_space ( tiles, size, try_pos, dir ) )
        {
            tiles[try_pos.x * size + try_pos.y].t = SPACE;
            last->p = try_pos;
            last->d = dir;
        }
    }
    return 1;
}
        
        

int iterate_paths (struct mazetile *tiles, int size, struct path_head *last, struct probs probs )
{
    int i = 0;
    int j = 0;
    if ( last->still_space )
    {
        i = advance_path ( tiles, size, last, probs );
    }
    if ( last->next != NULL )
    {
        j = iterate_paths ( tiles, size, last->next, probs );
    }

    if ( i || j )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



struct maze *generate_maze ( int size )
{
    int x;
    int y;
    double r;
    int space = 1;
    struct mazetile *tiles;
    struct probs probs;
    struct position start_position;
    struct position goal_position;
    struct path_head *root;

    probs.twisty = 0.7;
    probs.swirly = 0.5;
    probs.branchy = 0.3;


    if ( size < 10 )
    {
        printf ( "maze size must be at least 10,\n or else you wouldn't have any space!\n" );
    }
    tiles = malloc ( sizeof ( struct mazetile ) * size * size );

    /*
     * Initialize the entire maze as walls, from which we will carve paths
     */
    for ( x = 0; x < size; x++ )
    {
        for ( y = 0; y < size; y++ )
        {
            tiles[x * size + y] = new_mazetile ( WALL, x, y );
        }
    }

    x = ( int ) ( rand () / ( double ) RAND_MAX * ( ( double ) size - 6 ) ) + 3;
    y = ( int ) ( rand () / ( double ) RAND_MAX * ( ( double ) size - 6 ) ) + 3;
    start_position = new_position ( x, y );
    printf ( "Starting position at %d, %d\n", x, y );

    root = new_path ( start_position, SOUTH, 0.8 );
    printf ( "root created\n" );

    while ( space )
    {
        space = iterate_paths ( tiles, size, root, probs );
    }

    goal_position = root->p;

    destroy_paths ( root );
    free ( root );

    return new_maze_pointer ( size, tiles, start_position, goal_position );
}
