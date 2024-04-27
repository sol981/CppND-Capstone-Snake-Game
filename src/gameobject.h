#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__
#include "SDL.h"

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

struct object
{
    SDL_Rect ob;
};

#endif // __GAMEOBJECT__