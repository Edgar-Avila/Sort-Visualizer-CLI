# CLI Algorithm Visualizer

This project is a Linux only Algorithm Visualizer that you can use directly from your
command line. It has controls that allow you to perform different actions so you
can better understand the inner workings of the sorting and pathfinding algorithms.

## Commands
### Sorting
To visualize sorting algorithms, use the `sort` command

https://github.com/Edgar-Avila/Sort-Visualizer-CLI/assets/72365420/517e1a71-e465-4e8c-8f38-1c6d49063b94

- **s** to shuffle
- **h** to slow down
- **l** to speed up

### Shortest path
To visualize pathfinding algorithms use the `path` command

https://github.com/Edgar-Avila/Sort-Visualizer-CLI/assets/72365420/998b5dea-2314-4ea2-9011-eb7f6080582d

#### Path Setup 
Draw obstacles for the algorithm to avoid!
- **Mouse/hjkl** to move the cursor
- **Left click/space** to toggle an obstacle
- **Enter** to start the visualizer

#### Path Visualize
See the algorithm navigating and avoiding the obstacles!
- **Enter** to go back to setup mode
- **h** to slow down
- **l** to speed up

## Options
- **(-h, --help)** Will display all the options _(app)_
- **(-a, --algo)** Sorting algorithm used _(sort, path)_
- **(-s, --speed)** Speed of the visualizer _(sort, path)_
- **(-n, --number)** Number of elements _(sort)_
- **(-d, --dimensions)** Width and height of the visualizer _(path)_
- **(-D, --diagonal)** Allow diagonal movement _(path)_

## Development
To compile the project you need `g++` ,the `ncurses` library and make, then just run `make`. This will produce an executable `app` that you can use.
