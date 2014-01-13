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

#include "SDL.h"
#include "SDL_image.h"

int main ()
{
    if ( SDL_Init ( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf ( SDL_GetError () );
        return 1;
    }

    SDL_Window *win = NULL;
    win = SDL_CreateWindow ( "Hello World!", 100, 100, 400, 400, SDL_WINDOW_SHOWN );
    if ( win == NULL )
    {
        printf ( SDL_GetError () );
        return 1;
    }

    SDL_Renderer *ren = NULL;
    ren = SDL_CreateRenderer ( win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( ren == NULL )
    {
        printf ( SDL_GetError () );
        return 1;
    }

    /*
    SDL_Surface *srf = NULL;
    srf = SDL_LoadBMP ( "wall.bmp" );
    if ( srf == NULL )
    {
        printf ( SDL_GetError () );
        return 1;
    }
    */

    SDL_Texture *tex = NULL;
    tex = IMG_LoadTexture ( ren, "../java/maze2/playerN.png" );

    SDL_RenderClear ( ren );
    SDL_RenderCopy ( ren, tex, NULL, NULL );
    SDL_RenderPresent ( ren );

    SDL_Delay ( 8000 );

    SDL_DestroyTexture ( tex );
    SDL_DestroyRenderer ( ren );
    SDL_DestroyWindow ( win );


    SDL_Quit ();

    printf ( "Did this work\n" );

    return 0;
}
