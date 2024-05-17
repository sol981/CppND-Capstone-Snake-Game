#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include "snake.h"
#include <limits>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "SDL.h"
#include "gameobject.h"
#include <memory>
#include "controller.h"

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

struct Node
{
  int h_value = std::numeric_limits<int>::max();
  Node(){}
  Node(SDL_Point pos) {
    this->pos = pos; 
  }
  Node(int x, int y) {
    this->pos.x = x;
    this->pos.y = y;
  }
  Node(int x, int y, int h) {
    this->h_value = h;
    this->pos.x = x;
    this->pos.y = y;
  }
  
  SDL_Point pos;
};

class RoutePlanner {
  public:
    
    //init start node and end node
    RoutePlanner(std::shared_ptr<Snake> snake, int end_x, int end_y, int grid_width, int grid_height, std::vector<Obstacle*> obs);
    ~RoutePlanner();
    // check direction
    bool checkDirection(int x, int y);
    Snake::Direction getDirection(const SDL_Point& start, const SDL_Point& goal, const SDL_Point& next);

    // check cell valid
    bool checkCellValid(int x, int y);
    
    // Add public variables or methods declarations here.
    void AStarSearch(Controller* controller, SDL_Point goal);
    
    // add neighbors to open_list
    void AddNeighbors(Node *current_node, Controller* controller);
    
    // caluculate h value to know how far the the node to goal
    int CalculateHValue(Node const *node);
    int CalculateHValue(int x, int y);
    
    // sort by f then pop Node have lowest f
    Node *NextNode();

  private:
    std::shared_ptr<Snake> snake;
    std::vector<Node*> open_list;
    Controller* controller;
    Node *start_node;
    Node *end_node;
    int grid_width;
    int grid_height;
    std::vector<Obstacle*> obs;
};

#endif