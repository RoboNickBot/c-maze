#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"

#include "internals.h"

struct game_display
{
    SDL_Window *win;
    SDL_Renderer *ren;

    SDL_Texture *ground;
    SDL_Texture *wall;
    SDL_Texture *pN, *pS, *pE, *pW;
    SDL_Texture *goal;
};

struct game_display *init_display ( int mapsize )
{
    printf ( "starting init_display\n" );

    if ( SDL_Init ( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf ( SDL_GetError () );
    }

    struct game_display *display = malloc ( sizeof ( struct game_display ) );

    display->win = NULL;
    display->win = SDL_CreateWindow ( "Eye of the Pharaohs", 100, 100, mapsize * 32, mapsize * 32, SDL_WINDOW_SHOWN );
    if ( display->win == NULL )
    {
        printf ( SDL_GetError () );
    }

    display->ren = NULL;
    display->ren = SDL_CreateRenderer ( display->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( display->ren == NULL )
    {
        printf ( SDL_GetError () );
    }

    display->ground = NULL;
    display->ground = IMG_LoadTexture ( display->ren, "ground.png" );
    display->wall = NULL;
    display->wall = IMG_LoadTexture ( display->ren, "wall.png" );
    display->pN = NULL;
    display->pN = IMG_LoadTexture ( display->ren, "playerN.png" );
    display->pS = NULL;
    display->pS = IMG_LoadTexture ( display->ren, "playerS.png" );
    display->pE = NULL;
    display->pE = IMG_LoadTexture ( display->ren, "playerE.png" );
    display->pW = NULL;
    display->pW = IMG_LoadTexture ( display->ren, "playerW.png" );
    display->goal = NULL;
    display->goal = IMG_LoadTexture ( display->ren, "goal.png" );

    printf ( "completing init_display\n" );

    return display;
}

void apply_texture ( int x, int y, SDL_Renderer *ren, SDL_Texture *tex )
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = 32;
    pos.h = 32;

    printf ( "applying a texture.. " );

    SDL_RenderCopy ( ren, tex, NULL, &pos );

    printf ( "texture applied!\n" );
}

void destroy_display ( struct game_display *display )
{
    SDL_DestroyTexture ( display->wall );
    SDL_DestroyTexture ( display->ground );
    SDL_DestroyTexture ( display->pN );
    SDL_DestroyTexture ( display->pS );
    SDL_DestroyTexture ( display->pE );
    SDL_DestroyTexture ( display->pW );

    SDL_DestroyRenderer ( display->ren );
    SDL_DestroyWindow ( display->win );

    SDL_Quit ();

    free ( display );
}

void update_display ( struct game_display *display, struct mazegame *game )
{
    int x, y;
    SDL_Texture *tex = NULL;

    printf ( "starting display update.. " );

    printf ( " .. with mapsize %d.. ", game->mapsize );
    for ( x = 0; x < game->mapsize; x++ )
    {
        printf ( "we are in the for block x " );
        for ( y = 0; y < game->mapsize; y++ )
        {
            switch ( game->tiles[x][y].t )
            {
                case WALL:
                    tex = display->wall;
                    printf ( "wall" );
                    break;
                case SPACE:
                    tex = display->ground;
                    printf ( "ground" );
                    break;
                default:
                    printf ( "problem" );
                    break;
            }
            
            apply_texture ( x * 32, y * 32, display->ren, tex );
        }
    }

    tex = NULL;
    switch ( game->player.d )
    {
        case NORTH:
            tex = display->pN;
            break;
        case SOUTH:
            tex = display->pS;
            break;
        case EAST:
            tex = display->pE;
            break;
        case WEST:
            tex = display->pW;
            break;
        default:
            break;
    }
    apply_texture ( game->player.p.x * 32, game->player.p.y * 32, display->ren, tex );

    apply_texture ( game->goal_position.x * 32, game->goal_position.y * 32, display->ren, display->goal );

    SDL_RenderPresent ( display->ren );

    SDL_Delay ( 4000 );

    printf ( "done!\n");
}
