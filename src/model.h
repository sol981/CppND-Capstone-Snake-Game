#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <vector>
#include "gameobject.h"

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

std::vector<std::vector<State>> genBoardGame(std::vector<std::vector<State>> board, int grid_width,int grid_height, std::vector<Obstacle*> obs)
{
  std::vector<std::vector<State>> board(grid_height, std::vector<State>(grid_width, State::kEmpty));

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

  // test board
  // for(auto b : board)
  // {
  //   for(auto item : b)
  //   {
  //     if(item == State::kObstacle)
  //       std::cout << " 1 ";
  //     else
  //       std::cout << " 0 ";
  //   }
  //   std::cout << "\n";
  // }
  return board;
}

#endif