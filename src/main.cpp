#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  int kW = kScreenWidth/kGridWidth;

  std::vector<Obstacle*> obs;
  // obs.emplace_back(new Obstacle(0,0, 100,100));
  obs.emplace_back(new Obstacle(1 * kW,1 * kW, 6 * kW,6 * kW)); // 20 = kScreenWidth/kGridWidth 0-32

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight, obs);
  

  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";

  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  HighScore h;
  // if(game.GetScore() >= h.get() && game.GetScore() > 0)
  // int saveScore = h.get();
  
  // if(saveScore <= game.GetScore() && game.GetScore() > 0)
  // {
  //   std::cout << "true: \ngame now: " << game.GetScore() << "\n";
  //   std::cout << "in file: " << saveScore << "\n";
    std::cout<<"you get high score, please enter your name:\n";
    std::string name;
    // name is string without any space
    // std::getline(std::cin, name);
    // h.setScore(name, game.GetScore());

    std::cout << "get score" << h.getScore() << "\n";
    std::cout << "name: " << h.players.back().first  << "score " << h.players.back().second;

  // }
  // else
  // {
  //   std::cout << " you are not in high score\n";
  // }
  h.getScore();
  return 0;
}