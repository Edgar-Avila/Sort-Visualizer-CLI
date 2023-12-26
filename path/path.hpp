#pragma once

enum PathAlgo { BFS };

class Path {
public:
  Path(int startX, int startY, int endX, int endY)
      : startX(startX), startY(startY), endX(endX), endY(endY){};
  void run();

private:
  int startX;
  int startY;
  int endX;
  int endY;
};
