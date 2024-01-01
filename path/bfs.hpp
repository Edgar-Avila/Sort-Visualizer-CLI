#pragma once

#include "path.hpp"
#include <map>
#include <queue>
#include <utility>
#include <vector>

class BFSSolver : public Path {
public:
  BFSSolver(int startRow, int startCol, int endRow, int endcol, int maxRow,
            int maxCol, std::set<std::pair<int, int>> &obstacles, bool diagonal = false)
      : Path(startRow, startCol, endRow, endcol, maxRow, maxCol, obstacles), diagonal(diagonal) {}

  void step() override {
    int row, col;

    while (true) {
      if (done())
        return;

      std::pair<int, int> next = queue.front();
      row = next.first;
      col = next.second;
      queue.pop();
      if (visited.find({row, col}) == visited.end())
        break;
    }
    visited.insert({row, col});

    for (auto &direction : directions) {
      int nr = row + direction.first;
      int nc = col + direction.second;

      // If in boundaries and not visited and is not an obstacle
      if (nr >= 0 && nr <= maxRow && nc >= 0 && nc <= maxCol &&
          visited.find({nr, nc}) == visited.end() &&
          obstacles.find({nr, nc}) == obstacles.end()) {
        // Push to queue and add trace
        queue.push({nr, nc});
        trace.insert({{nr, nc}, {row, col}});

        // Stop and make solution if end is reached
        if (nr == endRow && nc == endCol) {
          while (!queue.empty())
            queue.pop();
          makeSolution();
          return;
        }
      }
    }
  }

  bool done() override { return queue.empty(); }

  void reset() override {
    visited.clear();
    directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    if (diagonal) {
      directions.push_back({-1, -1});
      directions.push_back({-1, 1});
      directions.push_back({1, -1});
      directions.push_back({1, 1});
    }
    queue = std::queue<std::pair<int, int>>();
    for (auto &obstacle : obstacles) {
      visited.insert(obstacle);
    }
    queue.push({startRow, startCol});
  }

  void makeSolution() {
    std::pair<int, int> current = {endRow, endCol};
    while (current != std::pair<int, int>{startRow, startCol}) {
      solution.push_back(current);
      current = trace[current];
    }
    solution.push_back({startRow, startCol});
  }

  std::vector<std::pair<int, int>> getPath() override { return solution; }

private:
  std::queue<std::pair<int, int>> queue;
  std::map<std::pair<int, int>, std::pair<int, int>> trace;
  std::vector<std::pair<int, int>> solution;
  std::vector<std::pair<int, int>> directions = {
      {-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  bool diagonal = false;
};
