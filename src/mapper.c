#include <stdlib.h>
#include <stdio.h>

#include "internals.h"
#include "mapper.h"

enum tiletype* read_map ( int mapsize, char *map_file_name, struct position *p_pos, struct position *g_pos )
{
    FILE *file;
    int x = 0;
    int y = 0;
    int c = ( int ) 'a';
    int breaker = 1;
    enum tiletype *types;
    
    file = fopen ( map_file_name, "r" );
    if ( file == NULL )
    {
        printf ( "File failed to open!!\n" );
    }
    
    types = malloc ( sizeof ( enum tiletype ) * mapsize * mapsize );

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
                        types[x*mapsize + y] = WALL;
                        break;
                    case '.':
                        types[x*mapsize + y] = SPACE;
                        break;
                    case 'p':
                        types[x*mapsize + y] = SPACE;
                        *p_pos = new_position ( x, y );
                        break;
                    case 'g':
                        types[x*mapsize + y] = SPACE;
                        *g_pos = new_position ( x, y );
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

    return types;
}
