# CLI Algorithm Visualizer

This project is a Linux only Algorithm Visualizer that you can use directly from your
command line. It has controls that allow you to perform different actions so you
can better understand the inner workings of the sorting and pathfinding algorithms.

## Commands
- **sort** Will show sorting algorithms
- **path** Will show pathfinding algorithms

## Options
- **(-h, --help)** Will display all the options _(app)_
- **(-a, --algo)** Sorting algorithm used _(sort, path)_
- **(-s, --speed)** Speed of the visualizer _(sort, path)_
- **(-n, --number)** Number of elements _(sort)_
- **(-d, --dimensions)** Width and height of the visualizer _(path)_

## Development
To compile the project you need `g++` ,the `ncurses` library and make, then just run `make`. This will produce an executable `app` that you can use.
