#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>

typedef struct {
  int x, y;
}point;

class Player {
  public:

    Player(float xPos, float yPos, float xVel, float yVel) : xPos(xPos), yPos(yPos), xVel(xVel), yVel(yVel) {}
    
    bool usingMap;
    float xPos, yPos;
    float xVel, yVel;

    std::vector<point> wayPoints;

    private:
};

#endif