#include <stdlib.h>
#include <stdio.h>

#include "internals.h"
#include "mapper.h"

struct maze* read_map ( char *map_file_name )
{
    int size = 0;
    struct mazetile *tiles;
    struct position start_position;
    struct position goal_position;

    FILE *file;
    int x = 0;
    int y = 0;
    int c = ( int ) 'a';
    int breaker = 1;
    
    file = fopen ( map_file_name, "r" );
    if ( file == NULL )
    {
        printf ( "File failed to open!!\n" );
    }

    c = fgetc ( file );
    while ( ( char ) c != '\n' )
    {
        c = fgetc ( file );
        size++;
    }

    rewind ( file );

    tiles = malloc ( sizeof ( struct mazetile ) * size * size );

    printf ( "file opened, entering while loop .. \n" );

    while ( c != EOF )
    {
        x = 0;
        breaker = 1;
        while ( breaker )
        {
            c = fgetc ( file );

            if ( ( char ) c != '\n' && c != EOF )
            {
                switch ( ( char ) c )
                {
                    case 'x':
                        tiles[x * size + y] = new_mazetile ( WALL, x, y );
                        break;
                    case '.':
                        tiles[x * size + y] = new_mazetile ( SPACE, x, y );
                        break;
                    case 'p':
                        tiles[x * size + y] = new_mazetile ( SPACE, x, y );
                        start_position = new_position ( x, y );
                        break;
                    case 'g':
                        tiles[x * size + y] = new_mazetile ( SPACE, x, y );
                        goal_position = new_position ( x, y );
                        break;
                    default:
                        printf ( "Invalid map character\n" );
                        break;
                }
            }
            else
            {
                breaker = 0;
            }
            x++;
        }
        y++;
    }

    if ( fclose ( file ) != 0 )
    {
        printf ( "error in closing file\n" );
    }
    printf ( "file IO completed\n" );

    return new_maze_pointer ( size, tiles, start_position, goal_position );
}

void write_map ( struct maze *maze, char *map_file_name )
{
    int x;
    int y;
    int s = maze->size;
    int c;

    FILE *file;

    file = fopen ( map_file_name, "w" );
    if ( file == NULL )
    {
        printf ( "File failed to open!!\n" );
    }

    for ( y = 0; y < s; y++ )
    {
        for ( x = 0; x < s; x++ )
        {
            switch ( maze->tiles[x * s + y].t )
            {
                case SPACE:
                    c = ( int ) '.';
                    break;
                case WALL:
                    c = ( int ) 'x';
                    break;
                default:
                    c = ( int ) 'E';
                    break;
            }
            if ( equal_pos ( maze->start_position, new_position ( x, y ) ) )
            {
                c = ( int ) 'p';
            }
            else if ( equal_pos ( maze->goal_position, new_position ( x, y ) ) )
            {
                c = ( int ) 'g';
            }
            
            fputc ( c, file );
        }
        if ( y < s-1 )
        {
            c = ( int ) '\n';
            fputc ( c, file );
        }
    }

    if ( fclose ( file ) != 0 )
    {
        printf ( "error in closing file\n" );
    }
    printf ( "file IO completed\n" );
}
