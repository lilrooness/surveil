#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>

typedef struct {
  int x;
  int y;
}waypoint;

class Player {
  public:

    Player(float xPos, float yPos, float xVel, float yVel) : xPos(xPos), yPos(yPos), xVel(xVel), yVel(yVel) {}
    
    int lastYClick;
    int lastXClick;
    bool clicked;

    bool usingMap;
    float xPos, yPos;
    float xVel, yVel;

    std::vector<waypoint> wayPoints;

    private:
};

#endif