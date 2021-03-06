/*
 *  << c-maze, a simple generated maze crawler written in C >>
 *  Copyright (C) 2013 Nick Lewchenko
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "internals.h"
#include "directionals.h"
#include "mapper.h"

struct maze* read_map ( char *map_file_name )
{
    int size = 0;
    struct mazetile *tiles;
    struct DR_position start_position;
    struct DR_position goal_position;

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
                        start_position = DR_new_position ( x, y );
                        break;
                    case 'g':
                        tiles[x * size + y] = new_mazetile ( SPACE, x, y );
                        goal_position = DR_new_position ( x, y );
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
            if ( DR_equal_pos ( maze->start_position, DR_new_position ( x, y ) ) )
            {
                c = ( int ) 'p';
            }
            else if ( DR_equal_pos ( maze->goal_position, DR_new_position ( x, y ) ) )
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
