#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"{
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
}

#include "player.h"
#include "map.h"
#include "renderer.h"

int getMapValue(lua_State *L, int x, int y);

int main(int argc, char **argv) {

  Player *p = new Player(0,0,0,0);

  Map *map = new Map("island.lua");
  map->generateMap();

  Renderer renderer;

  bool isRunning = true;

  while(isRunning) {
    SDL_Event event;

    while(SDL_PollEvent(&event) != 0) {
      if(event.type == SDL_QUIT) {
        isRunning = false;
      }
    }

    renderer.render(0, map, p);
  }

  delete map;

  return 0;
}
