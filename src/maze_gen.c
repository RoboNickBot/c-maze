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

int count_paths ( struct path_head *last )
{
    int c;

    if ( last->next == NULL )
    {
        c = 0;
    }
    else
    {
        c = count_paths ( last->next );
    }

    return c + 1;
}

struct path_head *get_path ( struct path_head *last, int i )
{
    struct path_head *result;

    if ( i <= 0 )
    {
        result = last;
    }
    else
    {
        result = get_path ( last->next, i-1 );
    }

    return result;
}

struct path_head *new_path ( struct DR_position p, enum DR_direction d, double g )
{
    struct path_head *path;
    path = malloc ( sizeof ( struct path_head ) );
    path->p = p;
    path->d = d;
    path->g = g;
    path->still_space = 1;
    path->next = NULL;

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
int try_move ( struct mazetile *tiles, int size, struct path_head *last, struct DR_position try_pos, enum DR_direction dir )
{
    int result = 0;

    if ( check_space ( tiles, size, try_pos, dir ) )
    {
        tiles[try_pos.x * size + try_pos.y].t = SPACE;
        last->p = try_pos;
        last->d = dir;
        result = 1;
    }

    return result;
}

void try_branch ( struct mazetile *tiles, int size, struct path_head *last, enum DR_orientation o, struct probs probs )
{
    double r;
    int i;
    struct path_head *new_branch;

    for ( i = 0; i < 2; i++ )
    {
    new_branch = NULL;
    r = rand () / ( double ) RAND_MAX;
    if ( r < probs.branchy )
    {
        r = rand () / ( double ) RAND_MAX;
        switch ( i )
        {
            case 0:
        switch ( o )
        {
            case FRONT:
                new_branch = new_path ( last->p, DR_get_rel ( last->d, RIGHT ), r );
                break;
            case LEFT:
                new_branch = new_path ( last->p, DR_get_rel ( last->d, FRONT ), r );
                break;
            case RIGHT:
                new_branch = new_path ( last->p, DR_get_rel ( last->d, LEFT ), r );
                break;
            default:
                break;
        }
                break;
            case 1:
        switch ( o )
        {
            case FRONT:
                new_branch = new_path ( last->p, DR_get_rel ( last->d, LEFT ), r );
                break;
            case LEFT:
                new_branch = new_path ( last->p, DR_get_rel ( last->d, RIGHT ), r );
                break;
            case RIGHT:
                new_branch = new_path ( last->p, DR_get_rel ( last->d, FRONT ), r );
                break;
            default:
                break;
        }
                break;
            default:
                break;
        }
        if ( try_move ( tiles, size, new_branch, DR_get_adj ( new_branch->p, new_branch->d ), new_branch->d ) )
        {
            add_path ( last, new_branch );
            printf ( "added path at %d, %d with direction %d\n", new_branch->p.x, new_branch->p.y, new_branch->d );
        }
    }
    }
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
    
            try_branch ( tiles, size, last, o, probs );
            try_move ( tiles, size, last, try_pos, dir );

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

    x=0;
    while ( space && x < 5000 )
    {
        space = iterate_paths ( tiles, size, root, probs );
        x++;
    }

    x = ( int ) ( rand () / ( double ) RAND_MAX * ( ( double ) count_paths ( root ) ) );

    goal_position = get_path ( root, x-1 )->p;

    printf ( "finished, destroying paths\n" );

    destroy_paths ( root );

    return new_maze_pointer ( size, tiles, start_position, goal_position );
}
