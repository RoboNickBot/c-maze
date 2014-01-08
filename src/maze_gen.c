#include <stdlib.h>
#include <stdio.h>

#include "maze_gen.h"
#include "internals.h"
#include "directionals.h"

struct probs
{
    double twisty; /* Probability of changing direction */
    double swirly; /* Probability of going right when changing direction (so non-swirly would be 0.5) */
    double branchy; /* Probability of branching */
};

struct path_head
{
    struct DR_position p;
    enum DR_direction d;
    double g; /* growth probability */
    int still_space;

    struct path_head *next;
};

struct path_head *new_path ( struct DR_position p, enum DR_direction d, double g )
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
    }

    free ( last );
}

int check_space ( struct mazetile *tiles, int size, struct DR_position pos, enum DR_direction d )
{
    int result = 1;
    int x;
    enum DR_direction check_dir;

    if ( pos.x == 0 || pos.x == size-1 || pos.y == 0 || pos.y == size-1 )
    {
        result = 0;
    }
    else
    {
        for ( x = 0; x < 5; x++ )
        {
            check_dir = DR_get_rel ( DR_get_rel ( d, LEFT ), x );
            if ( get_tiletype ( tiles, size, DR_get_adj ( pos, check_dir ) ) != WALL )
            {
                result = 0;
            }
        }
    }

    return result;
}
int check_all ( struct mazetile *tiles, int size, struct DR_position pos, enum DR_direction d )
{
    int result = 0;
    int f, l, r;

    f = check_space ( tiles, size, DR_get_adj ( pos, d ), d );
    l = check_space ( tiles, size, DR_get_adj ( pos, DR_get_rel ( d, LEFT ) ), DR_get_rel ( d, LEFT ) );
    r = check_space ( tiles, size, DR_get_adj ( pos, DR_get_rel ( d, RIGHT ) ), DR_get_rel ( d, RIGHT ) );

    if ( f || l || r )
    {
        result = 1;
    }

    return result;
}

int advance_path ( struct mazetile *tiles, int size, struct path_head *last, struct probs probs )
{
    enum DR_orientation o;
    enum DR_direction dir;
    struct DR_position try_pos;
    double r;
    int result = 0;

    if ( check_all ( tiles, size, last->p, last->d ) )
    {
        result = 1;

        r = rand () / ( double ) RAND_MAX;
        if ( r < last->g )
        {
            r = rand () / ( double ) RAND_MAX;
            if ( r < probs.twisty )
            {
                o = FRONT;
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
            dir = DR_get_rel ( last->d, o );
            try_pos = DR_get_adj ( last->p, dir );
    
            if ( check_space ( tiles, size, try_pos, dir ) )
            {
                tiles[try_pos.x * size + try_pos.y].t = SPACE;
                last->p = try_pos;
                last->d = dir;
            }
        }
    }
    else
    {
        last->still_space = 0;
    }

    return result;
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

    return ( i || j );
}



struct maze *generate_maze ( int size )
{
    int x;
    int y;
    double r;
    int space = 1;
    struct mazetile *tiles;
    struct probs probs;
    struct DR_position start_position;
    struct DR_position goal_position;
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
    start_position = DR_new_position ( x, y );
    printf ( "Starting position at %d, %d\n", x, y );
    tiles[x * size + y].t = SPACE;

    root = new_path ( start_position, SOUTH, 0.8 );
    printf ( "root created\n" );

    while ( space )
    {
        space = iterate_paths ( tiles, size, root, probs );
    }

    goal_position = root->p;

    destroy_paths ( root );

    return new_maze_pointer ( size, tiles, start_position, goal_position );
}
