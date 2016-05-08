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
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "internals.h"
#include "directionals.h"
#include "image_key.h"
#include "IOStuff.h"

const char *game_image_set = "res/tile_sheet.xcf";
const char *debug_image_set = "res/mazetest.xcf";

const char *image_locations[] = {
  "./", /* local image-sets take priority */

  USRPREFIX "/share/c-maze/", /* the usual system place is second choice */

  NULL,
};

char *str_init( const int len ) {
  char *str;

  str = malloc( sizeof( char ) * ( len + 1 ) );
  str[0] = '\0';

  return str;
}

int file_exists( const char *filename ) {
  FILE *file;
  int exists = 0;
  
  if( file = fopen( filename, "r" ) ) {
    fclose( file );
    exists = 1;
  }
  return exists;
}

char *select_path( const char *candidates[], const char *filename ) {
  char *path;
  int sc;
  int sf;

  int done = 0;

  sf = strlen( filename );

  for( int i = 0; NULL != candidates[i] && !done; i++ ) {
    sc = strlen( candidates[i] );

    path = str_init( sc + sf );

    strncat( path, candidates[i], sc );
    strncat( path, filename, sf );

    printf( "Checking if %s exists... ", path );
    if( file_exists( path ) ) {
      printf( "Success!\n" );
      done = 1;
    } else {
      printf( "It does not :(\n" );
      free( path );
    }
  }
  if( !done ) path = NULL;
  return path;
}   

struct game_display
{
    SDL_Window *win;
    SDL_Renderer *ren;

    SDL_Texture *sheet;

    int tileSize;
};

struct game_display *init_display ( int mapsize, enum displaytype type )
{
    int tileSize;
    char *images;

    printf ( "starting init_display\n" );

    switch ( type )
    {
        case MAINGAME:
            tileSize = 16;
            images = select_path( image_locations, "res/tile_sheet.xcf" );
            break;
        case MAZEDEBUG:
            tileSize = 2;
            images = select_path( image_locations, "res/mazetest.xcf" );
            break;
        default:
            tileSize = 1;
            break;
    }

    if ( SDL_Init ( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf ( SDL_GetError () );
    }
    printf ( "sdl start successful\n" );

    struct game_display *display = malloc ( sizeof ( struct game_display ) );

    display->win = NULL;
    display->win = SDL_CreateWindow ( "Eye of the Pharaohs", 0, 0, mapsize * tileSize, mapsize * tileSize, SDL_WINDOW_SHOWN );
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
    display->sheet = IMG_LoadTexture ( display->ren, images );

    if ( NULL != images ) {
      printf( "Freeing images path...\n" );
      free( images );
    }

    display->tileSize = tileSize;

    printf ( "completing init_display\n" );

    return display;
}

void apply_texture ( int x, int y, struct game_display *display, int image )
{
    int ts = display->tileSize;

    SDL_Rect pos;
    pos.x = x * ts;
    pos.y = y * ts;
    pos.w = ts;
    pos.h = ts;

    SDL_Rect slice;
    slice.x = image * ts;
    slice.y = 0;
    slice.w = ts;
    slice.h = ts;

    SDL_RenderCopy ( display->ren, display->sheet, &slice, &pos );

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
            
            apply_texture ( x, y, display, image );
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
    apply_texture ( game->maze->goal_position.x, game->maze->goal_position.y, display, GOAL1 );

    apply_texture ( game->player.p.x, game->player.p.y, display, image );

    for ( x = 0; x < s; x++ )
    {
        for ( y = 0; y < s; y++ )
        {
            if ( game->maze->tiles[x * s + y].light < 3 )
            {
                if ( game->maze->tiles[x * s + y].light < 1 )
                {
                    image = DARK2;
                    apply_texture ( x, y, display, image );
                }
                else
                {
                    image = DARK1;
                    apply_texture ( x, y, display, image );
                }
            }
        }
    }

    SDL_RenderPresent ( display->ren );


}

void display_maze ( struct game_display *display, struct maze *maze )
{
    int x, y;
    int s = maze->size;
    enum maze_debug_image_key image;

    for ( x = 0; x < s; x++ )
    {
        for ( y = 0; y < s; y++ )
        {
            switch ( get_tiletype ( maze->tiles, s, DR_new_position ( x, y ) ) )
            {
                case WALL:
                    image = DB_WALL;
                    break;
                case SPACE:
                    image = DB_SPACE;
                    break;
                default:
                    image = DB_WALL;
                    break;
            }
            
            apply_texture ( x, y, display, image );
        }
    }

    apply_texture ( maze->goal_position.x, maze->goal_position.y, display, DB_GOAL );

    apply_texture ( maze->start_position.x, maze->start_position.y, display, DB_START );

    SDL_RenderPresent ( display->ren );
}


void get_command ( enum command *command ) {

  *command = NONE;
  /* The compiler complained when this was initialized to NULL,
   * so I guess I'll leave it as is for now..
   */
  SDL_Event e;

  while ( SDL_PollEvent ( &e ) ) {
    if ( e.type == SDL_QUIT ) {
      *command = QUIT;
    } else if ( e.type == SDL_KEYDOWN ) {

      switch ( e.key.keysym.sym ) {

        /* ESC and Q seem the most natural for quiting */
      case SDLK_ESCAPE:
        *command = QUIT;
        break;
      case SDLK_q:
        *command = QUIT;
        break;

        /* SPACE to run */
      case SDLK_SPACE:
        *command = RUN;
        break;

        /* WASD moves character */
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

        /* arrow keys work too */
      case SDLK_UP:
        *command = MOVE_N;
        break;
      case SDLK_DOWN:
        *command = MOVE_S;
        break;
      case SDLK_LEFT:
        *command = MOVE_W;
        break;
      case SDLK_RIGHT:
        *command = MOVE_E;
        break;

      default:
        break;
      }
    }
  }
  SDL_Delay ( 20 );
}
