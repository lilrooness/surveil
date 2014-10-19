#ifndef RENDERER_H
#define RENDERER_H

extern "C"{
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
}

#include "player.h"
#include "map.h"

class Renderer {
public:

  Renderer();

  SDL_Renderer *context;
  SDL_Window *window;

  void render(int screen, Map *map, Player *player);

  void renderPlanningScreen();
  void renderFlightScreen();
};

#endif