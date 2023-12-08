#include "CLI11.hpp"
#include "bubbleSort.hpp"
#include "selectionSort.hpp"
#include "sort.hpp"
#include <chrono>
#include <map>
#include <ncurses.h>
#include <string>
#include <thread>
#include <vector>

const int FPS = 30;
const int MAX_DELTA = 1000 / FPS;
const int MAX_DELAY = 30;

enum class SortAlgo { Bubble, Selection };

class App {
public:
  App(std::vector<int> &arr, SortAlgo sortType, int speed)
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
    }
  }

  ~App() { delete sort; }

  void update() {
    delay--;
    if (delay <= 0 && !sort->done()) {
      delay = MAX_DELAY - (3 * speed);
      sort->step();
    }

    char c = getch();
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

  void draw() {
    erase();
    // Draw bars
    for (int i = 0; i < arr.size(); i++) {
      int color = 0;
      char c = '#';
      if (sort->done()) {
        color = 2;
        c = '@';
      } else if (i == sort->current()) {
        color = 1;
        c = '*';
      } else if (sort->sorted(i)) {
        color = 2;
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
      mvprintw(rows - 1, paddingX + i * (numWidth) + (numWidth / 2) - 1, "%d", arr[i]);
    }

    // Draw message
    mvprintw(0, paddingX, "****************");
    mvprintw(1, paddingX, "* q: Quit      *");
    mvprintw(2, paddingX, "* h: Slow down *");
    mvprintw(3, paddingX, "* l: Speed up  *");
    mvprintw(4, paddingX, "* s: Shuffle   *");
    mvprintw(5, paddingX, "****************");

    // Draw sort name
    mvprintw(0, paddingX + textWidth - sortName.length(), "%s", sortName.c_str());
    std::string speedMessage = "Speed: " + std::to_string(speed);
    mvprintw(1, paddingX + textWidth - speedMessage.size(), "%s", speedMessage.c_str());
    refresh();
  }

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

  void run() {
    initscr();
    if (has_colors() == FALSE) {
      endwin();
      printf("Your terminal doesn't support colors\n");
      exit(1);
    }
    start_color();
    noecho();
    nodelay(stdscr, TRUE);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    getmaxyx(stdscr, rows, cols);
    now = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
    lastFrame = now;
    numWidth = cols / arr.size();
    textWidth = numWidth * arr.size() - 1;
    paddingX = (cols - textWidth) / 2;
    running = true;

    if(numWidth < 2) {
      endwin();
      printf("Your terminal is too small\n");
      printf("Possible solutions: \n");
      printf("- Maximize your terminal\n");
      printf("- Reduce the number of elements\n");
      printf("- Use a smaller font\n");
      exit(1);
    }

    while (running) {
      fps();
      update();
      draw();
    }
    endwin();
  }

private:
  int rows, cols, textWidth, paddingX, numWidth, now, lastFrame, delay = 0, speed;
  bool running = false;
  Sort *sort;
  SortAlgo sortType;
  std::string sortName;
  std::vector<int> &arr;
};

int main(int argc, char **argv) {
  CLI::App cliApp{"App description"};
  SortAlgo sortAlgo = SortAlgo::Bubble;
  int number = 10;
  int speed = 5;

  std::map<std::string, SortAlgo> sortAlgoMap{
      {"bubble", SortAlgo::Bubble}, {"selection", SortAlgo::Selection}};

  cliApp.add_option("-n,--number", number, "Number of elements (Default: 10)");
  cliApp
      .add_option("-a,--algo", sortAlgo, "Sorting algorithm (Default: bubble)")
      ->transform(CLI::CheckedTransformer(sortAlgoMap, CLI::ignore_case));
  cliApp.add_option("-s,--speed", speed, "Speed (Default: 5)")
      ->check(CLI::Range(1, 10));

  try {
    cliApp.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return cliApp.exit(e);
  }

  // Create and shuffle
  std::vector<int> arr(number);
  for (int i = 0; i < number; i++) {
    arr[i] = i + 1;
  }
  std::random_shuffle(arr.begin(), arr.end());

  App app(arr, sortAlgo, speed);
  app.run();
  return 0;
}
