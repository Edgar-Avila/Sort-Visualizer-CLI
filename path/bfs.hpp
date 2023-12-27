#pragma once

#include "path.hpp"
#include <queue>
#include <utility>

class BFSSolver : public Path {
public:
  BFSSolver(int startRow, int startCol, int endRow, int endcol, int maxRow,
      int maxCol, std::set<std::pair<int, int>> &obstacles)
      : Path(startRow, startCol, endRow, endcol, maxRow, maxCol, obstacles) {}

  void step() override {
    int row, col;

    while(true) {
      if (queue.empty()) return;
      std::pair<int, int> next = queue.front();
      row = next.first;
      col = next.second;
      if(row == endRow && col == endCol) return;
      queue.pop();
      if(visited.find({row, col}) == visited.end()) break;
    }


    visited.insert({row, col});

    if (row > 0) {
      queue.push({row - 1, col});
    }
    if (row < maxRow) {
      queue.push({row + 1, col});
    }
    if (col > 0) {
      queue.push({row, col - 1});
    }
    if (col < maxCol) {
      queue.push({row, col + 1});
    }
  }

  bool done() override { return queue.empty(); }

  void reset() override {
    visited.clear();
    queue = std::queue<std::pair<int, int>>();
    for (auto &obstacle : obstacles) {
      visited.insert(obstacle);
    }
    queue.push({startRow, startCol});
  }

private:
  std::queue<std::pair<int, int>> queue;
};
