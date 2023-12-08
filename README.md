# CLI Sort Visualizer

This project is a Linux only Sort Visualizer that you can use directly from your
command line. It has controls that allow you to perform different actions so you
can better understand the inner workings of the sorting algorithms.

## Options
- **(-h, --help)** Will display all the options
- **(-n, --number)** Number of elements
- **(-a, --algo)** Sorting algorithm used
- **(-s, --speed)** Speed of the visualizer

## Development
To compile the project you need g++ and the ncurses library:
```properties
g++ ./main.cpp -o sort-visualizer -lncurses
```
