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
#include "gameobject.h"
#include <mutex>
#include<bits/stdc++.h>
#include <filesystem>
#include <algorithm>
#include <future>
#include <memory>
#include "food_planner.h"

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

class HighScore
{
public:
    HighScore();
    HighScore(std::string fileName);
    ~HighScore();
    
    void setScore(std::string name, int score);
    int getScore();
    
    std::vector<std::pair<std::string, int>> players;
private:
    std::fstream file;
    std::mutex mt;
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::vector<Obstacle*> obs, LEVEL lev);
  ~Game();
  void Run(Renderer &renderer, std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
 private:
  std::vector<std::shared_ptr<Snake>> snakes; // use unique pointer
  std::vector<std::shared_ptr<Controller>> controllers;
  RoutePlanner* router = nullptr;
  food fd;
  std::vector<Obstacle*> obs;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  int grid_width;
  int grid_height;
  LEVEL level;

  std::vector<int> scores{0,0};

  void PlaceFood();
  void Update();
};

#endif