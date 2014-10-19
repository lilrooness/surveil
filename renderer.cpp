#include "renderer.h"

Renderer::Renderer() {
  atexit(SDL_Quit);

    //do lua stuff
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
    exit(1);
  }

  if(IMG_Init(IMG_INIT_PNG) != 0) {
    fprintf(stderr, "\nUnable to initialize SDL_image:  %s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow("Surveil", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  context = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if(context == NULL) {
    fprintf(stderr, "Could not creat rendering context: %s\n", SDL_GetError());
    printf("Falling back on software renderer . . .\n");
    context = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(context == NULL) {
      fprintf(stderr, "Could not initialize software renderer. Well this is embarrasing . . .%s\n", SDL_GetError());
      exit(1);
    }
  }
}

void Renderer::render(int screen, Map *map, Player *player) {
    SDL_SetRenderDrawColor(context, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(context);

    int x, y;
    for(y=0; y<map->mapSize; y++) {
      for(x=0; x<map->mapSize; x++) {
        SDL_Rect tile = {ceil(x*map->tileSizeX), ceil(y*map->tileSizeY), ceil(map->tileSizeX), ceil(map->tileSizeY)};
        int val = map->getMapPoint(x, y);
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
          default: {
            SDL_SetRenderDrawColor(context, 0x00, 0x00, 0x00, 0x00);
          }
        }
        SDL_RenderFillRect(context, &tile);
      }
    }
    // SDL_RenderCopy(context, cross, NULL, NULL);
    SDL_RenderPresent(context);
}
