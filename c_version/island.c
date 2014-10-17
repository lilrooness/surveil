#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
#include <SDL2/SDL.h>

int getMapValue(lua_State *L, int x, int y);

int main(int argc, char **argv) {
  // int mapSize = 100;

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, "island.lua");

  lua_getglobal(L, "getMapSize");
  lua_pcall(L, 0, 1, 0);
  int mapSize = (int)lua_tointeger(L, -1);
  int tileSize = 5;
  int tileSizeX = 640 / mapSize;
  int tileSizeY = 480 / mapSize;

  //do lua stuff
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
    return 1;
  }
  
  atexit(SDL_Quit);

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
    for(y=0; y<mapSize; y++) {
      for(x=0; x<mapSize; x++) {
        SDL_Rect tile = {x*tileSizeX, y*tileSizeY, tileSizeX, tileSizeY};
        int val = getMapValue(L, x, y);

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

    SDL_RenderPresent(context);
  }
    

  lua_close(L);
  SDL_Quit();

  return 0;
}

int getMapValue(lua_State *L, int x, int y) {
  lua_getglobal(L, "retreiveMapValue");
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  lua_pcall(L, 2, 1, 0);
  int value = (int)lua_tointeger(L, -1);
  lua_pop(L, 1);
  return value;
}
