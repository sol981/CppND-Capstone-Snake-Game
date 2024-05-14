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

// #include "route_planner.h"

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

class HighScore
{
public:
    HighScore() {
        if (!std::filesystem::exists("highscore.txt")) {
            std::ofstream out("highscore.txt");
            out.close();
        }
        file.open("highscore.txt");
    }

    HighScore(std::string fileName) {
        if (!std::filesystem::exists(fileName)) {
            std::ofstream out(fileName);
            out.close();
        }
        file.open(fileName);
    }
    ~HighScore() {
        file.close();
    }
    
    void setScore(std::string name, int score)
    {
        std::lock_guard<std::mutex> lk(mt);
        if(file.is_open())
        {
            std::string str = name + " " + std::to_string(score) + "\n";
            file << str;
        }
        else
        {
            std::cout << "can not open file to set score\n";
        }
    }

    int getScore()
    {
        std::lock_guard<std::mutex> lk(mt);
        file.seekg(0, std::ios::beg);
        std::string line;
        if(file.is_open())
        {
            while(std::getline(file, line))
            {
                std::string name;
                int score{0};
                std::stringstream stream(line);
                if(stream >> name >> score)
                    players.push_back(std::make_pair(name,score));
            }
        }
        else
        {
            std::cout << "fail to get score, can not open file\n";
        }
        // TODO: improve for multiple top players
        return players.back().second; 
    }

    std::vector<std::pair<std::string, int>> players;
private:
    std::fstream file;
    std::mutex mt;
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::vector<Obstacle*> obs);
  void Run(Controller* controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  // int GetSize() const;
//   void genBoardGame(int grid_width,int grid_height, std::vector<Obstacle*> obs);
 private:
//   std::vector<std::vector<State>> board;//(grid_height, std::vector<State>(grid_width, State::kEmpty));
  std::vector<std::shared_ptr<Snake>> snake; // use unique pointer
  // Snake snake;
  // Snake snake2;
  RoutePlanner* router = nullptr;
  food fd;
  std::vector<Obstacle*> obs;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif