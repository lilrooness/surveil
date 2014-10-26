#include "renderer.h"

Renderer::Renderer() {
  atexit(SDL_Quit);

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

  SDL_Surface *crossSurface = SDL_LoadBMP("cross.bmp");
  if(crossSurface == NULL) {
    std::cout<<"unable to load cross"<<std::endl;
  }

  SDL_SetColorKey(crossSurface, SDL_TRUE, SDL_MapRGB(crossSurface->format, 255, 0, 255));
  cross = SDL_CreateTextureFromSurface(context, crossSurface);

  if(cross == NULL) {
    std::cout<<"No cross"<<std::endl;
    printf( "Unable to create texture! SDL Error: %s\n", SDL_GetError());
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

    SDL_SetRenderDrawColor(context, 0xFF, 0x00, 0x00, 0xFF);
    for(int i=0; i<player->wayPoints.size(); i++) {
      waypoint wp = player->wayPoints.at(i);
      SDL_Rect wpTile = {floor(wp.x * map->tileSizeX), floor(wp.y * map->tileSizeY), map->tileSizeX*2, map->tileSizeY*2};
      SDL_RenderCopy(context, cross, NULL, &wpTile);
    }

    SDL_SetRenderDrawColor(context, 0x00, 0x00, 0x00, 0xFF);
    SDL_Rect playerMarker = {player->xPos, player->yPos, map->tileSizeX*3, map->tileSizeY*3};
    SDL_RenderFillRect(context, &playerMarker);

    SDL_RenderPresent(context);
}

void Renderer::renderFlightScreen(Map *map, Player *player, int viewWidth, int viewHeight) {
  
  float tilex, tiley;
  tiley = 480.0f / (float)viewHeight;
  tilex = 640.0f / (float)viewWidth;

  for(int y=player->yPos - viewHeight / 2; y<player->yPos + viewHeight / 2; y++) {
    for(int x=player->yPos - 1; x<player->xPos + viewWidth+1; x++) {

      SDL_Rect tile = {ceil(x*tilex), ceil(y*tiley), ceil(tilex), ceil(tiley)};
      
      if(x < 0 || x > map->mapSize-1 || y < 0 || y > map->mapSize-1) {
        SDL_SetRenderDrawColor(context, 0x00, 0x00, 0x00, 0x00);
      
      } else {
      
        switch(map->getMapPoint(x, y)) {
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
            SDL_SetRenderDrawColor(context, 0x00, 0x00, 0x00, 0xFF);
          }
        }
      }
    }
  }

  SDL_RenderPresent(context);
}