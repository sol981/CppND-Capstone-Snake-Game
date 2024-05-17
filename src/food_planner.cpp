#include "food_planner.h"
#include <algorithm>

 
//init start node and end node
RoutePlanner::RoutePlanner(std::shared_ptr<Snake> snake, int end_x, int end_y, int grid_width, int grid_height, std::vector<Obstacle*> obs)
{
    this->snake = snake;
    this->grid_height = grid_height;
    this->grid_width = grid_width;
    start_node = new Node(static_cast<int>(snake->head_x), static_cast<int>(snake->head_y));
    end_node = new Node(end_x, end_y);
    this->obs = obs;
}

RoutePlanner::~RoutePlanner()
{
    if(start_node != nullptr)
        delete start_node;
    if(end_node != nullptr)
        delete end_node;
}

// check snake go right direction 
bool RoutePlanner::checkDirection(int x, int y)
{
    // vector head -> end
    int x0 = end_node->pos.x - start_node->pos.x;
    int y0 = end_node->pos.y - start_node->pos.y;

    // vector head -> point
    int x1 = x - start_node->pos.x;
    int y1 = y - start_node->pos.y;

    if(x0 * x1 >= 0 && y0 * y1 >= 0)
        return true;
    else 
        return false;
}

Snake::Direction RoutePlanner::getDirection(const SDL_Point& start, const SDL_Point& goal, const SDL_Point& next) {
    // Calculate horizontal and vertical distance from start to goal
    double dx_goal = goal.x - start.x;
    double dy_goal = goal.y - start.y;

    // Calculate horizontal and vertical distance from start to point
    double dx_next = next.x - start.x;
    double dy_next = next.y - start.y;

    // Normalize direction vectors
    double norm_dx_goal = std::sqrt(dx_goal * dx_goal);
    double norm_dy_goal = std::sqrt(dy_goal * dy_goal);
    double norm_dx_next = std::sqrt(dx_next * dx_next);
    double norm_dy_next = std::sqrt(dy_next * dy_next);

    // Calculate normalized dot product for horizontal and vertical components
    double dot_product_x = (dx_next * dx_goal) / (norm_dx_next * norm_dx_goal);
    double dot_product_y = (dy_next * dy_goal) / (norm_dy_next * norm_dy_goal);

    // Determine direction based on dot products and relative positions
    if (dot_product_x > 0.7 && dot_product_y > 0.7) {
        // Move towards goal (same direction)
        return Snake::Direction::kNone;
    } else if (std::abs(dy_next) > std::abs(dx_next)) {
        // Vertical movement is more significant
        if (dy_next > 0) {
            // Move down
            return Snake::Direction::kDown;
        } else {
            // Move up
            return Snake::Direction::kUp;
        }
    } else if (std::abs(dx_next) > std::abs(dy_next)) {
        // Horizontal movement is more significant
        if (dx_next > 0) {
            // Move right
            return Snake::Direction::kRight;
        } else {
            // Move left
            return Snake::Direction::kLeft;
        }
    } else {
        // No clear direction (point is aligned with start-goal or too close to start)
        return Snake::Direction::kNone;
    }
}

// check cell valid
bool RoutePlanner::checkCellValid(int x, int y)
{
  bool on_grid_x = (x >= 0 && x < grid_width);
  bool on_grid_y = (y >= 0 && y < grid_height);
  if (on_grid_x && on_grid_y)
  {
    for(auto ob : obs)
    {
        if(ob->ObstacleCell(x,y))
            return false;
    }
    return true;
  }
    
  return false;
}

// caluculate h value to know how far the the node to goal
int RoutePlanner::CalculateHValue(Node const *node)
{
    return abs(node->pos.x - end_node->pos.x) + abs(node->pos.y - end_node->pos.y);
}

int RoutePlanner::CalculateHValue(int x, int y)
{
    return abs(x - end_node->pos.x) + abs(y - end_node->pos.y);
}

// add neighbors to open_list
void RoutePlanner::AddNeighbors(Node *current_node, Controller* controller)
{
  // Get current node's data.
  int x = current_node->pos.x;
  int y = current_node->pos.y;
  SDL_Point current_point;

  // Loop through current node's potential neighbors.
  for (int i = 0; i < 4; i++) {
    current_point.x = x + delta[i][0];
    current_point.y = y + delta[i][1];

    // Check that the potential neighbor's x and y values are on the grid and not be obstacle.
    // add newNode to open list
    if (checkCellValid(current_point.x, current_point.y) && checkDirection(current_point.x,current_point.y)) {
        int h2 = CalculateHValue(current_point.x, current_point.y);
        auto newNode = new Node(current_point.x,current_point.y,h2);
        open_list.emplace_back(newNode);
    }
  }
}

// sort by f then pop Node have lowest f
// always reseach -> g = 0, f = g + h = h
Node* RoutePlanner::NextNode()
{
    std::sort(open_list.begin(), open_list.end(),[](Node* node1, Node* node2)->bool {
        return node1->h_value > node2->h_value; 
    });

    Node* retNode = open_list.back();
    open_list.pop_back();   
      
    return retNode;
}

// Add public variables or methods declarations here.
void RoutePlanner::AStarSearch(Controller* controller, SDL_Point goal)
{
    Node *current_node = nullptr;
    start_node->pos.x = static_cast<int>(snake->head_x);
    start_node->pos.y = static_cast<int>(snake->head_y);
    end_node->pos = goal;

    open_list.emplace_back(start_node);
    current_node = start_node;

    if(open_list.size() > 0)
    {   
        if(start_node == end_node)
        {
            return;
        }
        AddNeighbors(start_node, controller);
        SDL_Point next_point;
        current_node = NextNode();
        next_point.x = current_node->pos.x;
        next_point.y = current_node->pos.y;
        Snake::Direction direction = getDirection(start_node->pos, start_node->pos, next_point);
        controller->HandleInput(snake->alive, snake, direction);
        open_list.clear();
    }
}
