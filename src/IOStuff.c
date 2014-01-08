#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"

#include "internals.h"
#include "directionals.h"
#include "image_key.h"

struct game_display
{
    SDL_Window *win;
    SDL_Renderer *ren;

    SDL_Texture *sheet;
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

    display->sheet = NULL;
    display->sheet = IMG_LoadTexture ( display->ren, "res/tile_sheet.xcf" );

    printf ( "completing init_display\n" );

    return display;
}

void apply_texture ( int x, int y, SDL_Renderer *ren, SDL_Texture *tex, enum image_key image )
{
    int ts = 16;

    SDL_Rect pos;
    pos.x = x * ts;
    pos.y = y * ts;
    pos.w = ts;
    pos.h = ts;

    SDL_Rect slice;
    slice.x = ( int ) image * ts;
    slice.y = 0;
    slice.w = ts;
    slice.h = ts;

    SDL_RenderCopy ( ren, tex, &slice, &pos );

}

void destroy_display ( struct game_display *display )
{
    SDL_DestroyTexture ( display->sheet );

    SDL_DestroyRenderer ( display->ren );
    SDL_DestroyWindow ( display->win );

    SDL_Quit ();

    free ( display );
}

void update_display ( struct game_display *display, struct mazegame *game )
{
    int x, y;
    int s = game->maze->size;
    enum image_key image;

    for ( x = 0; x < s; x++ )
    {
        for ( y = 0; y < s; y++ )
        {
            switch ( get_tiletype ( game->maze->tiles, game->maze->size, DR_new_position ( x, y ) ) )
            {
                case WALL:
                    image = WALL1;
                    break;
                case SPACE:
                    image = SPACE1;
                    break;
                default:
                    break;
            }
            
            apply_texture ( x, y, display->ren, display->sheet, image );
        }
    }

    switch ( game->player.d )
    {
        case NORTH:
            image = PN;
            break;
        case SOUTH:
            image = PS;
            break;
        case EAST:
            image = PE;
            break;
        case WEST:
            image = PW;
            break;
        default:
            break;
    }
    apply_texture ( game->maze->goal_position.x, game->maze->goal_position.y, display->ren, display->sheet, GOAL1 );

    apply_texture ( game->player.p.x, game->player.p.y, display->ren, display->sheet, image );

    for ( x = 0; x < s; x++ )
    {
        for ( y = 0; y < s; y++ )
        {
            if ( game->maze->tiles[x * s + y].light < 3 )
            {
                if ( game->maze->tiles[x * s + y].light < 1 )
                {
                    image = DARK2;
                    apply_texture ( x, y, display->ren, display->sheet, image );
                }
                else
                {
                    image = DARK1;
                    apply_texture ( x, y, display->ren, display->sheet, image );
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
