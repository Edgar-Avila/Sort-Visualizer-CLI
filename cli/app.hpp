#pragma once

#include <chrono>
#include <ncurses.h>
#include <stdlib.h>
#include <thread>

class App {
public:
  virtual void init() = 0;
  virtual void handle_input(int c) = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

  void fps() {
    now = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
    int delta = now - lastFrame;
    lastFrame = now;

    if (delta < MAX_DELTA) {
      std::this_thread::sleep_for(std::chrono::milliseconds(MAX_DELTA - delta));
    }
  }

  void initNcurses() {
    initscr();
    if (has_colors() == FALSE) {
      endwin();
      printf("Your terminal doesn't support colors\n");
      exit(1);
    }
    start_color();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    getmaxyx(stdscr, rows, cols);
    init_pair(FG_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(FG_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(FG_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(FG_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(FG_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    printf("\033[?1003h\n");
  }

  void initFps() {
    now = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
    lastFrame = now;
  }

  virtual void run() {
    initNcurses();
    initFps();
    init();
    running = true;
    while (running) {
      /* fps(); */
      handle_input(getch());

      now = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
      int delta = now - lastFrame;

      if (delta > MAX_DELTA) {
        update();
        draw();
        lastFrame = now;
      }
    }
    endwin();
  }

protected:
  int rows, cols, now, lastFrame;
  bool running = false;
  static const int FPS = 30;
  static const int MAX_DELTA = 1000 / FPS;
  static const int MAX_DELAY = 30;
  static const short FG_BLUE = 1;
  static const short FG_GREEN = 2;
  static const short FG_YELLOW = 3;
  static const short FG_CYAN = 4;
  static const short FG_MAGENTA = 5;
};
