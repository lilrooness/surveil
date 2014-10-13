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

  int tileSize = 50;
  int mapSize = 11;

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, "island.lua");

  int x, y;
  for(y = 0; y<11; y++) {
    for(x = 0; x<11; x++) {
      lua_getglobal(L, "retreiveMapValue");
      lua_pushnumber(L, x);
      lua_pushnumber(L, y);
      lua_pcall(L, 2, 1, 0);
      printf("%d", (int)lua_tointeger(L, -1));
    }
    printf("\n");
  }

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

    for(y=0; y<mapSize; y++) {
      for(x=0; x<mapSize; x++) {
        SDL_Rect tile = {x*tileSize, y*tileSize, tileSize, tileSize};
        int val = getMapValue(L, x, y);

        switch(val) {
          case 0: {
            SDL_SetRenderDrawColor(context, 0x00, 0x00, 0xFF, 0xFF);
            break;
          }
          case 1: {
            SDL_SetRenderDrawColor(context, 0x00, 0xFF, 0x00, 0xFF);
            break;
          }
          case 2: {
            SDL_SetRenderDrawColor(context, 0x88, 0x88, 0x88, 0xFF);
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
  return (int)lua_tointeger(L, -1);
}