#include <stdlib.h>
#include <stdio.h>

#include "maze_gen.h"
#include "internals.h"

struct maze generate_maze ( int size )
{
    double twistyness = 10;
    double branchyness = 10;
    double roomcount = 10;
    double roomsize = 10;
