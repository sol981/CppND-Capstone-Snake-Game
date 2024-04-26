#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <fstream>
#include <sstream>
#include <iostream>

class HighScore {
public:
  HighScore()
  {
    file.open("highscore.txt", std::ios::in | std::ios::out | std::ios::app);
    if (!file.is_open()) {
      std::cout << "can not open file\n";
    }
  }
  void openFile() {
    file.open("highscore.txt", std::ios::in | std::ios::out);
    if (!file.is_open()) {
      std::cout << "can not open file\n";
    }
  }

  void closeFile() {
    if (file.is_open()) {
      file.close();
    }
  }

  void set(std::string name, int score) {
    openFile(); // Open the file before writing
    if (file.is_open()) {
      file << name << " " << score << std::endl;
    }
    closeFile(); // Close the file after writing
  }

  int get() {
    openFile(); // Open the file before reading
    int highScore = 0;
    std::string line, token;
    if (file.is_open()) {
      while (std::getline(file, line)) {
        std::stringstream ll(line);
        int currentScore;
        if (ll >> token >> currentScore) {
          highScore = std::max(highScore, currentScore); // Update high score
        }
      }
    }
    closeFile(); // Close the file after reading
    return highScore;
  }

private:
  std::fstream file; // File stream
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
  
};

#endif