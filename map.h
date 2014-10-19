#ifndef MAP_H
#define MAP_H

extern "C"{
  #include <lua5.2/lua.h>
  #include <lua5.2/lauxlib.h>
  #include <lua5.2/lualib.h>
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
}

#include <math.h>

class Map {
public:
	Map(const char *infilename);
	~Map();

	void generateMap();

	int getMapPoint(int x, int y);

	lua_State *L;

	const char *filename;
	float tileSizeX;
	float tileSizeY;
	int mapSize;
};

#endif