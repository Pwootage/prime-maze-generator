#include <iostream>
#include "CScriptMazeNode.hpp"
#include "seeds.h"

int main() {

  for (auto seed : seeds) {
    auto maze = std::make_unique<CMazeState>(skEnterCol, skEnterRow, skTargetCol, skTargetRow);
//  maze->Reset(sMazeSeeds[mgr.GetActiveRandom()->Next() % sMazeSeeds.size()]);
    maze->Reset(seed);
    maze->Initialize();
    maze->GenerateObstacles();
//  mgr.SetCurrentMaze(std::move(maze));


    std::string str = maze->to_str();

    std::cout << str << std::endl << std::endl;
  }

  return 0;
}
