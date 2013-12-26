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
    SDL_Texture *dark1;
    SDL_Texture *dark2;
};

struct game_display *init_display ( int mapsize )
{
    printf ( "starting init_display\n" );

    if ( SDL_Init ( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf ( SDL_GetError () );
    }
    printf ( "sdl start successful\n" );

    struct game_display *display = malloc ( sizeof ( struct game_display ) );

    display->win = NULL;
    display->win = SDL_CreateWindow ( "Eye of the Pharaohs", 0, 0, mapsize * 16, mapsize * 16, SDL_WINDOW_SHOWN );
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
    display->dark1 = NULL;
    display->dark1 = IMG_LoadTexture ( display->ren, "dark1.png");
    display->dark2 = NULL;
    display->dark2 = IMG_LoadTexture ( display->ren, "dark2.png");

    printf ( "completing init_display\n" );

    return display;
}

void apply_texture ( int x, int y, SDL_Renderer *ren, SDL_Texture *tex )
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    pos.w = 16;
    pos.h = 16;

    SDL_RenderCopy ( ren, tex, NULL, &pos );

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

    for ( x = 0; x < game->mapsize; x++ )
    {
        for ( y = 0; y < game->mapsize; y++ )
        {
            switch ( game->tiles[x][y].t )
            {
                case WALL:
                    tex = display->wall;
                    break;
                case SPACE:
                    tex = display->ground;
                    break;
                default:
                    break;
            }
            
            apply_texture ( x * 16, y * 16, display->ren, tex );
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
    apply_texture ( game->goal_position.x * 16, game->goal_position.y * 16, display->ren, display->goal );

    apply_texture ( game->player.p.x * 16, game->player.p.y * 16, display->ren, tex );

    for ( x = 0; x < game->mapsize; x++ )
    {
        for ( y = 0; y < game->mapsize; y++ )
        {
            if ( game->tiles[x][y].light < 2 )
            {
                if ( game->tiles[x][y].light < 1 )
                {
                    apply_texture ( x * 16, y * 16, display->ren, display->dark2 );
                }
                else
                {
                    apply_texture ( x * 16, y * 16, display->ren, display->dark1 );
                }
            }
        }
    }

    SDL_RenderPresent ( display->ren );

    SDL_Delay ( 20 );

}

void get_command ( enum command *command )
{
    *command = NONE;
    /* The compiler complained when this was initialized to NULL,
     * so I guess I'll leave it as is for now..
     */
    SDL_Event e;

    while ( SDL_PollEvent ( &e ) )
    {
        if ( e.type == SDL_QUIT )
        {
            *command = QUIT;
        }
        else if ( e.type == SDL_KEYDOWN )
        {
            switch ( e.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                    *command = QUIT;
                    break;
                case SDLK_w:
                    *command = MOVE_N;
                    break;
                case SDLK_s:
                    *command = MOVE_S;
                    break;
                case SDLK_a:
                    *command = MOVE_W;
                    break;
                case SDLK_d:
                    *command = MOVE_E;
                    break;
                default:
                    break;
            }
        }
    }
}
