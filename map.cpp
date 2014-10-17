#include "map.h"

Map::Map(const char *infilename) {
  filename = infilename;
}

void Map::generateMap() {
  L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, filename);

  lua_getglobal(L, "getMapSize");
  lua_pcall(L, 0, 1, 0);
  mapSize = (int)lua_tointeger(L, -1);

  // int tileSize = 5;
  tileSizeX = 640 / mapSize;
  tileSizeY = 480 / mapSize;
}

int Map::getMapPoint(int x, int y) {
  lua_getglobal(L, "retreiveMapValue");
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  lua_pcall(L, 2, 1, 0);
  int value = (int)lua_tointeger(L, -1);
  lua_pop(L, 1);
  return value;
}