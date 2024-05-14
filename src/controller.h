#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <memory>

class Controller {
 public:
  // virtual void HandleInput(bool &running, std::shared_ptr<Snake> snake) = 0;
  virtual void HandleInput(bool &running, std::shared_ptr<Snake> snake, Snake::Direction dir) = 0;
 protected:
  void ChangeDirection(std::shared_ptr<Snake> snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

class UserController : public Controller
{
public:
    UserController() = default;
    void HandleInput(bool &running, std::shared_ptr<Snake> snake);
};

class ProcessController : public Controller
{
public:
    ProcessController() = default;
    void HandleInput(bool &running, std::shared_ptr<Snake> snake, Snake::Direction dir);
};

#endif