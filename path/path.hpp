#pragma once

#include <set>
#include <utility>

enum PathAlgo { BFS };

class Path {
public:
  Path(int startRow, int startCol, int endRow, int endcol, int maxRow,
       int maxCol, std::set<std::pair<int, int>> &obstacles)
      : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endcol),
        maxRow(maxRow), maxCol(maxCol), obstacles(obstacles) {};

  virtual ~Path(){};

  virtual void step() = 0;
  virtual bool done() = 0;
  virtual void reset() = 0;

  std::set<std::pair<int, int>> visited;
  std::set<std::pair<int, int>> obstacles;

protected:
  int startRow;
  int startCol;
  int endRow;
  int endCol;
  int maxRow;
  int maxCol;
};
