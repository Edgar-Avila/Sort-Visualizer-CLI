#include "CLI11.hpp"
#include "app.hpp"
#include <map>
#include <ncurses.h>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  CLI::App cliApp{"App description"};
  SortAlgo sortAlgo = SortAlgo::Bubble;
  int number = 10;
  int speed = 5;

  std::map<std::string, SortAlgo> sortAlgoMap{
      {"bubble", SortAlgo::Bubble},
      {"selection", SortAlgo::Selection},
      {"insertion", SortAlgo::Insertion},
  };

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
