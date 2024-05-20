#include "game.h"
#include <iostream>
#include "SDL.h"
#include "SDL.h" 

std::mutex mt;
const std::string HIGH_SCORE_FILE = "highscore.txt";

void HighScore::saveHighScore(std::string name, int newScore) {
        std::lock_guard<std::mutex> lock(mt);
        std::ofstream file(HIGH_SCORE_FILE);
        if (file.is_open()) {
            file << name << " " <<newScore << std::endl;
            file.close();
            std::cout << "High score saved!" << std::endl;
        } else {
            std::cerr << "Error saving high score!" << std::endl;
        }
    }

int HighScore::loadHighScore() {
    std::lock_guard<std::mutex> lock(mt);
    std::ifstream file(HIGH_SCORE_FILE);
    if (file.is_open()) {
        int score{0};
        std::string name;
        file >> name >> score;
        file.close();
        return score;
    } else {
        // Handle case where file doesn't exist or can't be read
        std::cout << "No high score found. Starting fresh!" << std::endl;
        return 0;  // Or any default value
    }
}

Game::Game(std::size_t grid_width, std::size_t grid_height,  std::vector<Obstacle*> obs, LEVEL level)
    : obs(obs),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
      this->grid_height = grid_height;
      this->grid_width = grid_width;
      this->level = level;
      // user player
      controllers.emplace_back(std::make_shared<UserController>());
      snakes.emplace_back(std::make_shared<Snake>(grid_width, grid_height));

      // process player
      controllers.emplace_back(std::make_shared<ProcessController>());
      snakes.emplace_back(std::make_shared<Snake>(grid_width, grid_height));

      PlaceFood();
      router = new RoutePlanner(snakes[1],fd.fruit.x, fd.fruit.y, grid_width, grid_height, obs);
}

Game::~Game()
{
  if(router != nullptr)
    delete router;
}

void Game::Run(Renderer &renderer, std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  if(level == LEVEL::easy)
    snakes[0]->speed = 0.05;
  else if(level == LEVEL::medium) {
    snakes[0]->speed = 0.1;
  } 
  else if(level == LEVEL::hard) {
    snakes[0]->speed = 0.15;
  }
  
  while (running) {
    frame_start = SDL_GetTicks();
    // Input, Update, Render - the main game loop.
    controllers[0]->HandleInput(running, snakes[0], Snake::Direction::kNone);

    auto snake2 = std::async(&RoutePlanner::AStarSearch, router, controllers[1].get(), fd.fruit);
    snake2.wait();

    Update();
    auto ren2 = std::async(&Renderer::Render, &renderer, snakes, fd, obs);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      // renderer.UpdateWindowTitle(score, frame_count);
      auto ren_ = std::async(&Renderer::UpdateWindowTitle, &renderer, scores[0], frame_count);
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

  while (true) {
    int x = random_w(engine);
    int y = random_h(engine);
    // Check that the location is not occupied by a snake item & obstacles before placing
    // food.
    if ( (!snakes[0]->SnakeCell(x, y)) && (!snakes[1]->SnakeCell(x, y)) && (std::all_of(obs.begin(), obs.end(), [&](const auto& ob) {
        return !ob->ObstacleCell(x, y);})) ) 
    {
      fd.fruit.x = x;
      fd.fruit.y = y;
      if(scores[0] % 5 == 0 && scores[0] != 0 && fd.type == TYPE::SMALL) // use time or lenght of snake, but which snakes ? // multiple fruit ?
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
  if (!snakes[0]->alive) 
  {
    return;
  }
  
  // if snake hit the obstacle game is over
  for(auto ob : obs)
  {
    for(auto snake : snakes)
    {    
      if(ob->ObstacleCell(static_cast<int>(snake->head_x), static_cast<int>(snake->head_y)))
      {
        snake->alive = false;
        return;
      }
    }
  }

  for(auto snake: snakes)
  {
    snake->Update();
  }

  int snake0_x = static_cast<int>(snakes[0]->head_x);
  int snake0_y = static_cast<int>(snakes[0]->head_y);
  int snake1_x = static_cast<int>(snakes[1]->head_x);
  int snake1_y = static_cast<int>(snakes[1]->head_y);

  // Check if there's food over here
  if ((fd.fruit.x == snake0_x && fd.fruit.y == snake0_y)) {
    if(fd.type == TYPE::BIG)
      scores[0]+=5;
    else
      ++scores[0];
      
    PlaceFood();
    // Grow snake and increase speed.
    snakes[0]->GrowBody();
    // snake.speed += 0.02;
    // snake.speed = 0.02;
  }
  else if(fd.fruit.x == snake1_x && fd.fruit.y == snake1_y)
  {
    if(fd.type == TYPE::BIG)
      scores[1] += 5;
    else
      ++scores[1];
      
    PlaceFood();
    // Grow snake and increase speed.
    snakes[1]->GrowBody();
    // snake.speed += 0.02;
    // snake.speed = 0.02;
  }
}

int Game::GetScore() const { return scores[0]; }
int Game::GetSize() const { return snakes[0]->size; }