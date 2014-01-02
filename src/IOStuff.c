#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"

#include "internals.h"

enum tile_image { WALL1, SPACE1, ERROR, GOAL1, DARK1, DARK2, PN, PS, PE, PW };

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

void apply_texture ( int x, int y, SDL_Renderer *ren, SDL_Texture *tex, enum tile_image image )
{
    int ts = 16;

    SDL_Rect pos;
    pos.x = x * ts;
    pos.y = y * ts;
    pos.w = ts;
    pos.h = ts;

    SDL_Rect slice;
    switch ( image )
    {
        case SPACE1:
            slice.x = 1 * ts;
            slice.y = 0 * ts;
            break;
        case WALL1:
            slice.x = 2 * ts;
            slice.y = 0 * ts;
            break;
        case DARK1:
            slice.x = 3 * ts;
            slice.y = 0 * ts;
            break;
        case DARK2:
            slice.x = 4 * ts;
            slice.y = 0 * ts;
            break;
        case GOAL1:
            slice.x = 5 * ts;
            slice.y = 0 * ts;
            break;
        case PN:
            slice.x = 6 * ts;
            slice.y = 0 * ts;
            break;
        case PS:
            slice.x = 7 * ts;
            slice.y = 0 * ts;
            break;
        case PE:
            slice.x = 8 * ts;
            slice.y = 0 * ts;
            break;
        case PW:
            slice.x = 9 * ts;
            slice.y = 0 * ts;
            break;
        default:
            slice.x = 0 * ts;
            slice.y = 0 * ts;
            break;
    }
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
    enum tile_image image;

    for ( x = 0; x < game->mapsize; x++ )
    {
        for ( y = 0; y < game->mapsize; y++ )
        {
            switch ( game->tiles[x][y].t )
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
    apply_texture ( game->goal_position.x, game->goal_position.y, display->ren, display->sheet, GOAL1 );

    apply_texture ( game->player.p.x, game->player.p.y, display->ren, display->sheet, image );

    for ( x = 0; x < game->mapsize; x++ )
    {
        for ( y = 0; y < game->mapsize; y++ )
        {
            if ( game->tiles[x][y].light < 2 )
            {
                if ( game->tiles[x][y].light < 1 )
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
