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
  
  int level{0};
  std::cout << "Choose level 0 - easy, 1 - medium, 2 - hard:\n";
  std::cin >> level;
  
  std::vector<Obstacle*> obs;
  obs.emplace_back(new Obstacle(0 * kW,1 * kW, 6 * kW,6 * kW)); // 20 = kScreenWidth/kGridWidth 0-32
  obs.emplace_back(new Obstacle(0 * kW,12 * kW, 8 * kW,6 * kW)); // 20 = kScreenWidth/kGridWidth 0-32

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);

  Game game(kGridWidth, kGridHeight, obs, static_cast<LEVEL>(level));
  game.Run(renderer, kMsPerFrame);
  
  std::cout << "Game has terminated successfully!\n";

  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  int highestScore{0};
  { 
    HighScore h;
    highestScore = h.getScore();
  }
  
  HighScore h;
  if(highestScore <= game.GetScore() && game.GetScore() > 0)
  {
    std::string name;
    std::cout <<"the highest score: " << highestScore << "\n";
    std::cout<<"however you are better, you are the best now  !!!! please enter your name:\n";
    
    // name is string without any space
    std::cin >> name;
    h.setScore(name, game.GetScore());
  }

  for(auto ob: obs)
  {
    if(ob != nullptr)
      delete ob;
  }
  return 0;
}