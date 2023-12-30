#pragma once

#include "../sort/bubbleSort.hpp"
#include "../sort/insertionSort.hpp"
#include "../sort/selectionSort.hpp"
#include "../sort/sort.hpp"
#include "./app.hpp"
#include <ncurses.h>
#include <stdlib.h>
#include <string>
#include <vector>

class SortApp : public App {
public:
  SortApp(std::vector<int> &arr, SortAlgo sortType, int speed)
      : arr(arr), sortType(sortType), speed(speed) {
    switch (sortType) {
    case SortAlgo::Bubble:
      sort = new BubbleSort(arr);
      sortName = "Bubble Sort";
      break;
    case SortAlgo::Selection:
      sort = new SelectionSort(arr);
      sortName = "Selection Sort";
      break;
    case SortAlgo::Insertion:
      sort = new InsertionSort(arr);
      sortName = "Insertion Sort";
      break;
    }
  }

  ~SortApp() { delete sort; }

  void init() override {
    numWidth = cols / arr.size();
    textWidth = numWidth * arr.size() - 1;
    paddingX = (cols - textWidth) / 2;

    if (numWidth < 2) {
      endwin();
      printf("Your terminal is too small\n");
      printf("Possible solutions: \n");
      printf("- Maximize your terminal\n");
      printf("- Reduce the number of elements\n");
      printf("- Use a smaller font\n");
      exit(1);
    }
  }

  void update() override {
    delay--;
    if (delay <= 0 && !sort->done()) {
      delay = MAX_DELAY - (3 * speed);
      sort->step();
    }
  }

  void handle_input(int c) override {
    /* char c = getch(); */
    if (c == 'q') {
      running = false;
    }
    if (c == 'h' && speed > 1) {
      speed--;
    }
    if (c == 'l' && speed < 10) {
      speed++;
    }
    if (c == 's') {
      std::random_shuffle(arr.begin(), arr.end());
      sort->reset();
    }
  }

  void draw() override {
    erase();
    // Draw bars
    for (int i = 0; i < arr.size(); i++) {
      short color = 0;
      char c = '#';
      if (sort->done()) {
        color = FG_GREEN;
        c = '@';
      } else if (i == sort->current()) {
        color = FG_BLUE;
        c = '*';
      } else if (sort->sorted(i)) {
        color = FG_GREEN;
        c = '@';
      }
      if (color > 0)
        attron(COLOR_PAIR(color));
      for (int j = 0; j < numWidth - 1; j++) {
        for (int k = 0; k < arr[i]; k++) {
          mvprintw(rows - k - 2, paddingX + i * (numWidth) + j, "%c", c);
        }
      }
      if (color > 0)
        attroff(COLOR_PAIR(color));
    }

    // Print values
    for (int i = 0; i < arr.size(); i++) {
      mvprintw(rows - 1, paddingX + i * (numWidth) + (numWidth / 2) - 1, "%d",
               arr[i]);
    }

    // Draw message
    attron(COLOR_PAIR(FG_CYAN));
    mvprintw(0, paddingX + textWidth - 19, "************************");
    mvprintw(1, paddingX + textWidth - 19, "* Sort Visualizer      *");
    mvprintw(2, paddingX + textWidth - 19, "* Algo: %-15s*", sortName.c_str());
    mvprintw(3, paddingX + textWidth - 19, "* Speed: %02d            *", speed);
    mvprintw(4, paddingX + textWidth - 19, "* q: Quit              *");
    mvprintw(5, paddingX + textWidth - 19, "* h: Slow down         *");
    mvprintw(6, paddingX + textWidth - 19, "* l: Speed up          *");
    mvprintw(7, paddingX + textWidth - 19, "* s: Shuffle           *");
    mvprintw(8, paddingX + textWidth - 19, "************************");
    attroff(COLOR_PAIR(FG_CYAN));

    // Draw sort name
    refresh();
  }

private:
  int textWidth, paddingX, numWidth, speed, delay = 0;
  Sort *sort;
  SortAlgo sortType;
  std::string sortName;
  std::vector<int> &arr;
};
