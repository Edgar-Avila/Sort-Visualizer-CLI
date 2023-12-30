#pragma once

#include "../path/bfs.hpp"
#include "../path/path.hpp"
#include "app.hpp"
#include <ncurses.h>
#include <set>
#include <string.h>
#include <utility>

enum PathScene { Setup, Visualize };

class PathApp : public App {
public:
  PathApp(PathAlgo pathAlgo, int width = 0, int height = 0, int speed = 5)
      : width(width), height(height), speed(speed), pathAlgo(pathAlgo) {}

  ~PathApp() { delete pathSolver; }

  void init() override {
    startRow = 0;
    startCol = 0;
    if (width == 0) {
      width = cols - 2;
    }
    if (height == 0) {
      height = rows - 2;
    }
    endCol = width - 1;
    endRow = height - 1;
    cursorRow = height / 2;
    cursorCol = width / 2;

    if (width < 5 || height < 5) {
      endwin();
      printf("Your terminal is too small or you set a width/height too small "
             "(Minimum is 5)\n");
      printf("Possible solutions: \n");
      printf("- Maximize your terminal\n");
      printf("- Use a smaller font\n");
      exit(1);
    }

    if (width > cols - 2 || height > rows - 2) {
      endwin();
      printf("You set a width/height too big for your terminal\n");
      printf("The maximum size is %d x %d, you set %d x %d \n", cols - 2,
             rows - 2, width, height);
      printf("Possible solutions: \n");
      printf("- Maximize your terminal\n");
      printf("- Reduce the width/height\n");
      printf("- Use a smaller font\n");
      exit(1);
    }
  }

  void setupUpdate(int c) {
    if (c == 'q') {
      running = false;
    }
    if (c == 's') {
      startRow = cursorRow;
      startCol = cursorCol;
    }
    if (c == 'e') {
      endRow = cursorRow;
      endCol = cursorCol;
    }
    if (c == 'h') {
      if (cursorCol > 0)
        cursorCol--;
    }
    if (c == 'j') {
      if (cursorRow < height - 1)
        cursorRow++;
    }
    if (c == 'k') {
      if (cursorRow > 0)
        cursorRow--;
    }
    if (c == 'l') {
      if (cursorCol < width - 1)
        cursorCol++;
    }
    if (c == '\t') {
      showHelp = !showHelp;
    }
    if (c == '\n') {
      scene = PathScene::Visualize;
      if (pathSolver != nullptr)
        delete pathSolver;
      switch (pathAlgo) {
      case PathAlgo::BFS:
        pathSolver = new BFSSolver(startRow, startCol, endRow, endCol,
                                   height - 1, width - 1, obstacles);
        algoName = strdup("BFS");
        break;
      }
      pathSolver->reset();
    }
    if (c == ' ') {
      if (obstacles.find({cursorRow, cursorCol}) != obstacles.end()) {
        obstacles.erase({cursorRow, cursorCol});
      } else {
        bool isStart = cursorRow == startRow && cursorCol == startCol;
        bool isEnd = cursorRow == endRow && cursorCol == endCol;
        if (!isStart && !isEnd) {
          obstacles.insert({cursorRow, cursorCol});
        }
      }
    }
    MEVENT event;
    if (c == KEY_MOUSE) {
      if (getmouse(&event) == OK) {
        int worldRow, worldCol;
        std::pair<int, int> coor = windowCoordinatesToWorld(event.y, event.x);
        worldRow = coor.first;
        worldCol = coor.second;
        if (event.bstate & BUTTON1_PRESSED) {
          drawing = true;
        }
        if (event.bstate & BUTTON1_RELEASED) {
          drawing = false;
        }
        if (worldRow < 0 || worldRow >= height || worldCol < 0 ||
            worldCol >= width) {
          return;
        }
        cursorRow = worldRow;
        cursorCol = worldCol;
        if (drawing) {
          if (obstacles.find({worldRow, worldCol}) != obstacles.end()) {
            obstacles.erase({worldRow, worldCol});
          } else {
            bool isStart = worldRow == startRow && worldCol == startCol;
            bool isEnd = worldRow == endRow && worldCol == endCol;
            if (!isStart && !isEnd) {
              obstacles.insert({worldRow, worldCol});
            }
          }
        }
      }
    }
  }

  void visualizeInput(int c) {
    if (c == 'q') {
      running = false;
    }
    if (c == 'h' && speed > 1) {
      speed--;
    }
    if (c == 'l' && speed < 10) {
      speed++;
    }
    if (c == '\t') {
      showHelp = !showHelp;
    }
    if (c == '\n') {
      scene = PathScene::Setup;
    }
  }

  void visualizeUpdate() {
    delay--;
    if (delay <= 0 && !pathSolver->done()) {
      delay = MAX_DELAY - (3 * speed);
      pathSolver->step();
    }
  }

  void handle_input(int c) override {
    switch (scene) {
    case PathScene::Setup:
      setupUpdate(c);
      break;
    case PathScene::Visualize:
      visualizeInput(c);
      break;
    }
  }

  void update() override {
    switch (scene) {
    case PathScene::Setup:
      break;
    case PathScene::Visualize:
      visualizeUpdate();
      break;
    }
  }

  void printCharInWorld(int row, int col, char c) {
    int leftPadding = (cols - width - 2) / 2;
    int topPadding = (rows - height - 2) / 2;
    mvprintw(topPadding + row + 1, leftPadding + col + 1, "%c", c);
  }

  std::pair<int, int> windowCoordinatesToWorld(int row, int col) {
    int leftPadding = (cols - width - 2) / 2;
    int topPadding = (rows - height - 2) / 2;
    return {row - topPadding - 1, col - leftPadding - 1};
  }

  void drawVisited() {
    attron(COLOR_PAIR(FG_YELLOW));
    for (auto [row, col] : pathSolver->visited) {
      printCharInWorld(row, col, '@');
    }
    attroff(COLOR_PAIR(FG_YELLOW));
  }

  void drawBoundaries() {
    attron(COLOR_PAIR(FG_BLUE));
    for (int i = 0; i <= width + 1; i++) {
      printCharInWorld(-1, i - 1, '#');
      printCharInWorld(height, i - 1, '#');
    }

    for (int i = 0; i <= height + 1; i++) {
      printCharInWorld(i - 1, -1, '#');
      printCharInWorld(i - 1, width, '#');
    }
    attroff(COLOR_PAIR(FG_BLUE));
  }

  void drawObstacles() {
    attron(COLOR_PAIR(FG_MAGENTA));
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (obstacles.find({i, j}) != obstacles.end()) {
          printCharInWorld(i, j, '#');
        }
      }
    }
    attroff(COLOR_PAIR(FG_MAGENTA));
  }

  void drawStartEnd() {
    attron(COLOR_PAIR(FG_GREEN));
    printCharInWorld(startRow, startCol, 'S');
    printCharInWorld(endRow, endCol, 'E');
    attroff(COLOR_PAIR(FG_GREEN));
  }

  void setupDrawHelp() {
    if (!showHelp)
      return;
    attron(COLOR_PAIR(FG_CYAN));
    mvprintw(0, cols - 29, "*****************************");
    mvprintw(1, cols - 29, "* Shortest Path (SETUP)     *");
    mvprintw(2, cols - 29, "* - hjkl: Move cursor       *");
    mvprintw(3, cols - 29, "* - mouse: Move cursor      *");
    mvprintw(4, cols - 29, "* - s: set Start            *");
    mvprintw(5, cols - 29, "* - e: set End              *");
    mvprintw(6, cols - 29, "* - space: Toggle obstacle  *");
    mvprintw(7, cols - 29, "* - lclick: Toggle obstacle *");
    mvprintw(8, cols - 29, "* - enter: Visualize        *");
    mvprintw(9, cols - 29, "* - q: Quit                 *");
    mvprintw(10, cols - 29, "* - tab: Toggle help        *");
    mvprintw(11, cols - 29, "*****************************");
    attroff(COLOR_PAIR(FG_CYAN));
  }

  void visualizeDrawHelp() {
    if (!showHelp)
      return;
    attron(COLOR_PAIR(FG_CYAN));
    mvprintw(0, cols - 29, "*****************************");
    mvprintw(1, cols - 29, "* Shortest Path (VISUALIZE) *");
    mvprintw(2, cols - 29, "* Speed: %02d                 *", speed);
    mvprintw(3, cols - 29, "* Algorithm: %-15s*", algoName);
    mvprintw(4, cols - 29, "* - q: Quit                 *");
    mvprintw(5, cols - 29, "* - enter: Setup            *");
    mvprintw(6, cols - 29, "* - h: Slow Down            *");
    mvprintw(7, cols - 29, "* - l: Speed up             *");
    mvprintw(8, cols - 29, "* - tab: Toggle help        *");
    mvprintw(9, cols - 29, "*****************************");
    attroff(COLOR_PAIR(FG_CYAN));
  }

  void setupDraw() {
    drawBoundaries();
    drawObstacles();
    drawStartEnd();
    printCharInWorld(cursorRow, cursorCol, '@');
    setupDrawHelp();
  }

  void drawPath() {
    if (!pathSolver->done())
      return;
    attron(COLOR_PAIR(FG_GREEN));
    for (auto [row, col] : pathSolver->getPath()) {
      printCharInWorld(row, col, '@');
    }
    attroff(COLOR_PAIR(FG_GREEN));
  }

  void visualizeDraw() {
    drawBoundaries();
    drawVisited();
    drawObstacles();
    drawPath();
    drawStartEnd();
    visualizeDrawHelp();
  }

  void draw() override {
    erase();
    switch (scene) {
    case PathScene::Setup:
      setupDraw();
      break;
    case PathScene::Visualize:
      visualizeDraw();
      break;
    }
    refresh();
  }

private:
  std::set<std::pair<int, int>> obstacles;
  int startRow, startCol, endRow, endCol, cursorRow, cursorCol, width, height,
      speed = 5, delay = 0;
  PathScene scene = PathScene::Setup;
  PathAlgo pathAlgo;
  Path *pathSolver = nullptr;
  bool showHelp = true;
  bool drawing = false;
  char *algoName;
};
