#include "player.h"

void Player::update(Map *map) {
  float wpx = wayPoints.at(nextWayPoint).x * map->tileSizeX;
  float wpy = wayPoints.at(nextWayPoint).y * map->tileSizeY;

  float distToWp = (float) sqrt(pow(xPos - wpx, 2) + pow(yPos - wpy, 2));

  if(distToWp < 1) {
    nextWayPoint ++;
    wpx = wayPoints.at(nextWayPoint).x * map->tileSizeX;
    wpy = wayPoints.at(nextWayPoint).y * map->tileSizeY;
  }

  double dy = yPos - wpy;
  double dx = xPos - wpx; 
  angle = (float) atan(yPos / xPos);

  float mag = (float) sqrt(pow(dx, 2) + pow(dy, 2));

  xVel = dx / mag * speed;
  yVel = dy / mag * speed;

  xPos -= xVel;
  yPos -= yVel;
}

