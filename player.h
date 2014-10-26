#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>
#include <cmath>

#include "map.h"

typedef struct {
  int x;
  int y;
}waypoint;

class Player {
  public:

    Player(float xPos, float yPos, float xVel, float yVel) : xPos(xPos), yPos(yPos), xVel(xVel), yVel(yVel) {
      speed = 1;
    }
    
    int lastYClick;
    int lastXClick;
    bool clicked;

    float speed;
    float angle;

    bool usingMap;
    float xPos, yPos;
    float xVel, yVel;

    int nextWayPoint;

    std::vector<waypoint> wayPoints;

    void update(Map *map);

    private:
};

#endif