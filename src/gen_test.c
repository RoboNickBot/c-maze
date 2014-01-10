#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "maze_gen.h"
#include "internals.h"
#include "mapper.h"
#include "image_key.h"
#include "IOStuff.h"

int main ()
{

    struct maze *maze;
    struct game_display *display;
    int c = 1;
    enum command com;

    srand ( time ( NULL ) );
    maze = generate_maze ( 370 );

    write_map ( maze, "mazes/gentest3.txt" );

    display = init_display ( maze->size, MAZEDEBUG );

    display_maze ( display, maze );

    while ( c )
    {
        get_command ( &com );
        if ( com == QUIT )
        {
            c = 0;
        }
    }

    destroy_display ( display );


    destroy_maze ( maze );
    free ( maze );
    
    return 0;
}
