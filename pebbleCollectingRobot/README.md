# Pebble Collecting Robot

This code was written by the author below in its entirety. However, ChatGPT (March 23, 2023 version) was asked to generate the README.md.

Author: Brooke Czerwinski
Course: CS350 Algorithms and Complexity

A tabular solution for the pebble collecting robot problem.

## Overview

The program receives the dimensions of a grid and the pebble configuration in each cell. It calculates the maximum number of pebbles a robot can collect by following specific rules.

## Functions

- `initializeMatrix(int **board, int rows, int col)`: Initializes a matrix with zeros.
- `getDimensions(int &rows, int &col)`: Gets the dimensions of the grid from the user.
- `boardSetup(int **board, const int rows, const int col)`: Sets up the board configuration based on the user input.
- `setValues(int **valuesMatrix, int **board, int rows, int col)`: Calculates the maximum number of pebbles the robot can collect by filling out the values matrix.
- `printMatrix(int **matrix, int rows, int col)`: Prints the contents of a matrix with numbered rows.

## Usage

1. Compile the program: `g++ pebbleCollectingRobot.cpp -o pebbleCollectingRobot`
2. Run the program: `./pebbleCollectingRobot`
3. Enter the number of rows and columns for the grid.
4. Enter the pebble configuration for each row (using 1's and 0's).
5. The program will display the input board, the values matrix, and the maximum number of pebbles the robot can collect.
