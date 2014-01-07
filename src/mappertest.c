#include <stdlib.h>

#include "internals.h"
#include "mapper.h"

int main ()
{
    char *input_file = "mazes/mazeFile1.txt";

    char *output_file = "mazes/mazeFile2.txt";

    char *output_file2 = "mazes/mazeFile3.txt";

    struct maze *maze;


    maze = read_map ( input_file );

    write_map ( maze, output_file );


    destroy_maze ( maze );
    free ( maze );

    maze = read_map ( output_file );

    write_map ( maze, output_file2 );

    destroy_maze ( maze );
    free ( maze );

    return 0;
}
