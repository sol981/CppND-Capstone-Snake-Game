#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include "node.h"
#include "snake.h"
#include <limits>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "SDL.h"

// enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};
// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

struct Node
{
  int h_value = std::numeric_limits<int>::max();
  int g_value = 0;
  Node(){}
  Node(SDL_Point pos) {
    this.pos = pos; 
  }
  Node(int x, int y) {
    pos.x = x;
    pos.y = y;
  }
  
  SDL_Point pos;
};

class RoutePlanner {
  public:
    
    //init start node and end node
    RoutePlanner(Snake& snake, int end_x, int end_y, int grid_width, int grid_height);
    
    // check cell valid
    bool checkCellValid(int x, int y);

    // Add public variables or methods declarations here.
    void AStarSearch();

    // add neighbors to open_list
    void AddNeighbors(Node *current_node);
    // caluculate h value to know how far the the node to goal
    int CalculateHValue(Node const *node);
    // update position of head snake & position of food
    void update(int head_x,int head_y, int end_x, int end_y);
    // sort by f then pop Node have lowest f
    Node *NextNode();

  private:
    // Add private variables or methods declarations here.
    std::vector<Node*> open_list;
    Node *start_node;
    Node *end_node;
    int grid_width;
    int grid_height;
};

#endif