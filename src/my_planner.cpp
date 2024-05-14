#include "my_planner.h"
#include <algorithm>

 
//init start node and end node
RoutePlanner::RoutePlanner(Snake& snake, int end_x, int end_y, int grid_width, int grid_height)
{
    this->grid_height = grid_height;
    this->grid_width = grid_width;
    start_node = new Node(snake.head_x, snake.head_y);
    end_node = new Node(end_x, end_y);
}

// check cell valid
bool RoutePlanner::checkCellValid(int x, int y)
{
  bool on_grid_x = (x >= 0 && x < grid_width);
  bool on_grid_y = (y >= 0 && y < grid_height);
  if (on_grid_x && on_grid_y)
    return true;
  return false;
}
// caluculate h value to know how far the the node to goal
int RoutePlanner::CalculateHValue(Node const *node)
{
    return abs(node->pos.x - end_node->x) + abs(node->pos.y - end_node->y);
}

// add neighbors to open_list
void RoutePlanner::AddNeighbors(Node *current_node);

// update position of head snake & position of food
void RoutePlanner::update(int head_x,int head_y, int end_x, int end_y);
// sort by f then pop Node have lowest f
Node* RoutePlanner::NextNode();

// Add public variables or methods declarations here.
void RoutePlanner::AStarSearch();
