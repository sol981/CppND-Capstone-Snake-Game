#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  virtual void HandleInput(bool &running, Snake &snake) = 0;

 protected:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

class UserController : public Controller
{
public:
    UserController() = default;
    void HandleInput(bool &running, Snake &snake);
};

class ProcessController : public Controller
{
public:
    ProcessController() = default;
    virtual void HandleInput(bool &running, Snake &snake, Snake::Direction dir);
};

#endif