#include "CLI11.hpp"
#include "cli/pathApp.hpp"
#include "cli/sortApp.hpp"
#include "path/path.hpp"
#include "sort/sort.hpp"
#include <map>
#include <ncurses.h>
#include <string>
#include <utility>
#include <vector>

int main(int argc, char **argv) {
  CLI::App cliApp{"A simple algorithm visualizer"};
  SortAlgo sortAlgo = SortAlgo::Bubble;
  PathAlgo pathAlgo = PathAlgo::BFS;
  std::pair<int, int> dimensions = {0, 0};
  int number = 10;
  int speed = 5;
  bool diagonal = false;

  // **************************************************************************
  // Map enums to strings
  // **************************************************************************
  std::map<std::string, SortAlgo> sortAlgoMap{
      {"bubble", SortAlgo::Bubble},
      {"selection", SortAlgo::Selection},
      {"insertion", SortAlgo::Insertion},
  };

  std::map<std::string, PathAlgo> pathAlgoMap{
      {"bfs", PathAlgo::BFS},
  };

  // **************************************************************************
  // Add sort subcommand
  // **************************************************************************
  CLI::App *sortCommand =
      cliApp.add_subcommand("sort", "Sort an array of numbers");

  sortCommand->add_option("-n,--number", number,
                          "Number of elements (Default: 10)");
  sortCommand
      ->add_option("-a,--algo", sortAlgo, "Sorting algorithm (Default: bubble)")
      ->transform(CLI::CheckedTransformer(sortAlgoMap, CLI::ignore_case));
  sortCommand->add_option("-s,--speed", speed, "Speed (Default: 5)")
      ->check(CLI::Range(1, 10));

  // **************************************************************************
  // Add shortest path subcommand
  // **************************************************************************
  CLI::App *pathCommand =
      cliApp.add_subcommand("path", "Find the shortest path");

  pathCommand->add_option(
      "-d,--dimensions", dimensions,
      "Dimensions of the grid (Default: terminal dimensions - 2)");
  pathCommand->add_option("-a,--algo", pathAlgo, "Algorithm (Default: bfs)")
      ->transform(CLI::CheckedTransformer(pathAlgoMap, CLI::ignore_case));

  pathCommand->add_option("-s,--speed", speed, "Speed (Default: 5)")
      ->check(CLI::Range(1, 10));
  pathCommand->add_flag("-D,--diagonal", diagonal,
                        "Allow diagonal movement (Default: false)");

  // **************************************************************************
  // Parse
  // **************************************************************************
  cliApp.require_subcommand();
  try {
    cliApp.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return cliApp.exit(e);
  }

  // **************************************************************************
  // Run sort subcommand
  // **************************************************************************
  if (cliApp.got_subcommand("sort")) {
    // Create and shuffle
    std::vector<int> arr(number);
    for (int i = 0; i < number; i++) {
      arr[i] = i + 1;
    }
    std::random_shuffle(arr.begin(), arr.end());

    SortApp sortApp(arr, sortAlgo, speed);
    sortApp.run();
  }

  // **************************************************************************
  // Run path got_subcommand
  // **************************************************************************
  if (cliApp.got_subcommand("path")) {
    PathApp pathApp(pathAlgo, dimensions.first, dimensions.second, speed, diagonal);
    pathApp.run();
  }

  return 0;
}
