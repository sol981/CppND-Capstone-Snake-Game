#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "SDL.h"

class Node
{
public:
  Node* parent = nullptr;
  int h_value = std::numeric_limits<int>::max();
  int g_value = 0;
  // bool visited = int; research every step so do not save it
  std::vector<Node *> neighbors;
  Node(){}
  Node(SDL_Point lPos) { pos = lPos; }

  void FindNeighbor();
  
  int distance(Node other) const {
    return std::abs(pos.x - other.getPosision().x) + std::abs(pos.y - other.getPosision().y);
  }
  SDL_point getPosision()
  {
    return pos;
  }

private:
  SDL_Point pos;
};
#endif
