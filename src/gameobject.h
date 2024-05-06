#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__
#include "SDL.h"
#include <iostream>

enum TYPE
{
  SMALL,
  BIG,
};

struct food
{
  SDL_Point fruit;
  TYPE type;
};

struct Obstacle
{
  Obstacle(int x, int y, int w, int h)
  {
    ob.x = x;
    ob.y = y;
    ob.w = w;
    ob.h = h;
  }

  bool ObstacleCell(int x, int y) {
    if ((ob.x <= x && x <= (ob.x + ob.w))  && (ob.y <= y && y <= (ob.y + ob.h))) {
      return true;
    }

    return false;
  }
  SDL_Rect ob;
};

#endif // __GAMEOBJECT__