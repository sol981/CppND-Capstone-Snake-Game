#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(std::shared_ptr<Snake> snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake->direction != opposite || snake->size == 1) snake->direction = input;
  return;
}

void UserController::HandleInput(bool &running, std::shared_ptr<Snake> snake, Snake::Direction dir) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}

void ProcessController::HandleInput(bool &running, std::shared_ptr<Snake> snake, Snake::Direction dir) {
  switch (dir) {
    case Snake::Direction::kUp:
      ChangeDirection(snake, Snake::Direction::kUp,
                      Snake::Direction::kDown);
      break;

    case Snake::Direction::kDown:
      ChangeDirection(snake, Snake::Direction::kDown,
                      Snake::Direction::kUp);
      break;

    case Snake::Direction::kLeft:
      ChangeDirection(snake, Snake::Direction::kLeft,
                      Snake::Direction::kRight);
      break;

    case Snake::Direction::kRight:
      ChangeDirection(snake, Snake::Direction::kRight,
                      Snake::Direction::kLeft);
      break;
    case Snake::Direction::kNone:
      break;
  }
}