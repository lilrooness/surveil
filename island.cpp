#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

extern "C"{
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
}

#include "player.h"
#include "map.h"
#include "renderer.h"

int getMapValue(lua_State *L, int x, int y);

int main(int argc, char **argv) {

  std::vector<waypoint> wpts;

  bool usingMap = true;
  bool pressedEnter = false;

  Player *p = new Player(100,100,0,0);

  Map *map = new Map("island.lua");
  map->generateMap();

  Renderer renderer;

  bool isRunning = true;

  unsigned int lastTime = SDL_GetTicks();

  while(isRunning) {
    SDL_Event event;

    while(SDL_PollEvent(&event) != 0) {
      if(event.type == SDL_QUIT) {
        isRunning = false;
      } else if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(event.button.button == SDL_BUTTON_LEFT) {
          std::cout<<"clicked"<<std::endl;
          p->lastYClick = event.button.y;
          p->lastXClick = event.button.x;
          p->clicked = true;
          p->wayPoints.push_back({ceil((float)event.button.x / map->tileSizeX), ceil((float)event.button.y / map->tileSizeY)});
        }
      } else if(event.type == SDL_KEYDOWN) {
        std::cout<<"keydown event"<<std::endl;
        switch(event.key.keysym.sym) {
          case SDLK_SPACE: {
            std::cout<<"space pressed"<<std::endl;
            pressedEnter = true;
            break;
          }
        }
      }
    }

    if(!pressedEnter) {
      renderer.render(0, map, p);
    } else {
      if(SDL_GetTicks() - lastTime > (1000 / 60)) {
        lastTime = SDL_GetTicks();
        p->update(map);
      }
      renderer.renderFlightScreen(map, p, 20, 20);
    }
  }

  delete map;

  return 0;
}
