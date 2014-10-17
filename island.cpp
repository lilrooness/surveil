#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"{
  #include <lua5.2/lua.h>
  #include <lua5.2/lauxlib.h>
  #include <lua5.2/lualib.h>
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
}

#include "player.h"
#include "map.h"

int getMapValue(lua_State *L, int x, int y);

int main(int argc, char **argv) {

  Player p(0,0,0,0);

  Map map("island.lua");
  map.generateMap();

  atexit(SDL_Quit);

    //do lua stuff
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
    return 1;
  }

  if(IMG_Init(IMG_INIT_PNG) != 0) {
    fprintf(stderr, "\nUnable to initialize SDL_image:  %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Surveil", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  SDL_Renderer *context = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


  if(context == NULL) {
    fprintf(stderr, "Could not creat rendering context: %s\n", SDL_GetError());
    printf("Falling back on software renderer . . .\n");
    context = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(context == NULL) {
      fprintf(stderr, "Could not initialize software renderer. Well this is embarrasing . . .%s\n", SDL_GetError());
    }
  }

  SDL_Surface *crossSurface = IMG_Load("cross.bmp");
  SDL_Texture *cross;
  SDL_SetColorKey(crossSurface, SDL_TRUE, SDL_MapRGB(crossSurface->format, 0xFF, 0, 0xFF));
  cross = SDL_CreateTextureFromSurface(context, crossSurface);

  if(cross == NULL) {
    printf("Failed to load image!!");
  }

  SDL_FreeSurface(crossSurface);

  if(window == NULL) {
    fprintf(stderr, "\nCould not create SDL Window %s\n", SDL_GetError());
  }

  bool isRunning = true;

  while(isRunning) {
    SDL_Event event;

    while(SDL_PollEvent(&event) != 0) {
      if(event.type == SDL_QUIT) {
        isRunning = false;
      }
    }

    SDL_SetRenderDrawColor(context, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(context);

    int x, y;
    for(y=0; y<map.mapSize; y++) {
      for(x=0; x<map.mapSize; x++) {
        SDL_Rect tile = {x*map.tileSizeX, y*map.tileSizeY, map.tileSizeX, map.tileSizeY};
        // int val = getMapValue(L, x, y);
        int val = map.getMapPoint(x, y);
        switch(val) {
          case 0: {//Water
            SDL_SetRenderDrawColor(context, 0x00, 0x00, 0xFF, 0xFF);
            break;
          }
          case 1: {//Grass
            SDL_SetRenderDrawColor(context, 0x00, 0xFF, 0x00, 0xFF);
            break;
          }
          case 3: {//Snow
            SDL_SetRenderDrawColor(context, 0xFF, 0xFF, 0xFF, 0xFF);
            break;
          }
          case 2: {//Rock
            SDL_SetRenderDrawColor(context, 0x88, 0x88, 0x88, 0xFF);
            break;
          }
          case 4: {//Forest
            SDL_SetRenderDrawColor(context, 0x00, 0x88, 0x00, 0xFF);
            break;
          }
          case 5: {//sand
            SDL_SetRenderDrawColor(context, 0xED, 0xC9, 0xAF, 0xFF);
            break;
          }
          case 6: {//Foam
            SDL_SetRenderDrawColor(context, 0x35, 0x35, 0xFF, 0xFF);
            break;
          }
        }
        SDL_RenderFillRect(context, &tile);
      }
    }
    // SDL_RenderCopy(context, cross, NULL, NULL);
    SDL_RenderPresent(context);
  }

  SDL_DestroyTexture(cross);
  // lua_close(L);

  return 0;
}
