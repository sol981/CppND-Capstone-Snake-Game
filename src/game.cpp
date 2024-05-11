#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height,  std::vector<Obstacle*> obs)
    :
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)), obs(obs) {

  board.resize(grid_height);
  for (int i = 0; i < grid_height; ++i) {
    board[i].resize(grid_width, State::kEmpty);
  }
  snake.emplace_back(new Snake(grid_width, grid_height));
  genBoardGame(grid_width, grid_height, obs);
  PlaceFood();
}

void Game::genBoardGame(int grid_width,int grid_height, std::vector<Obstacle*> obs)
{
  for(auto obstacle : obs)
  {
    // std::cout << "jdjdjdj\n";
    for(int i = 0;i <= grid_height;i++)
    {
      for(int j = 0;j <= grid_width;j++)
      {
        if ((obstacle->ob.x * 0.05 <= i) && (i <= (obstacle->ob.x + obstacle->ob.w) * 0.05) &&
            (obstacle->ob.y * 0.05 <= j) && (j <= (obstacle->ob.y + obstacle->ob.h) * 0.05)) 
        {
          board[j][i] = State::kObstacle;
        }
      }
    }
  }
}

void Game::Run(Controller* controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller->HandleInput(running, *snake[0]);
    // controller->HandleInput(running, *snake[1]);
    Update();
    // renderer.Render(snake, fd, obs);
    // auto ren = std::async(&Renderer::Render, &renderer, snake, fd, obs);
    auto ren2 = std::async(&Renderer::Render, &renderer, snake, fd, obs);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      // renderer.UpdateWindowTitle(score, frame_count);
      auto ren_ = std::async(&Renderer::UpdateWindowTitle, &renderer, score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  

  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item & obstacles before placing
    // food.
    if ( (!snake[0]->SnakeCell(x, y)) && (std::all_of(obs.begin(), obs.end(), [&](const auto& ob) {
        return !ob->ObstacleCell(x, y);})) ) 
    {
      board[y][x] = State::kFinish;
      fd.fruit.x = x;
      fd.fruit.y = y;
      if(score % 5 == 0 && score != 0 && fd.type == TYPE::SMALL) // use time or lenght of snake, but which snakes ? // multiple fruit ?
      {
        fd.type = TYPE::BIG;
      }
      else 
      {
        fd.type = TYPE::SMALL;
      }
      
      return;
    }
  }
}

void Game::Update() {
  if (!snake[0]->alive) 
  {
    return;
  }
  
  // if snake hit the obstacle game is over
  for(auto ob : obs)
  {
    if(ob->ObstacleCell(static_cast<int>(snake[0]->head_x), static_cast<int>(snake[0]->head_y)))
    {
      snake[0]->alive = false;
      return;
    }
  }

  snake[0]->Update();
  // snake[1]->Update();

  int new_x = static_cast<int>(snake[0]->head_x);
  int new_y = static_cast<int>(snake[0]->head_y);

  // Check if there's food over here
  if (fd.fruit.x == new_x && fd.fruit.y == new_y) {
    if(fd.type == TYPE::BIG)
      score+=5;
    else
      ++score;
      
    PlaceFood();
    // Grow snake and increase speed.
    snake[0]->GrowBody();
    // snake.speed += 0.02;
    // snake.speed = 0.02;
  }
}

int Game::GetScore() const { return score; }
// int Game::GetSize() const { return snake.size; }