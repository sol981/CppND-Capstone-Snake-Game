#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"

class RoutePlanner {
  public:
    RoutePlanner(Node &node, int start_x, int start_y, int end_x, int end_y);
    // Add public variables or methods declarations here.
    void AStarSearch();

    // The following methods have been made public so we can test them individually.
    void AddNeighbors(Node *current_node);
    int CalculateHValue(Node const *node);
    void updatePosistion();
    Node *NextNode();

  private:
    // Add private variables or methods declarations here.
    std::vector<Node*> open_list;
    Node *start_node;
    Node *end_node;
};

#endif