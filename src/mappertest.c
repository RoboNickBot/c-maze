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
