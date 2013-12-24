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
