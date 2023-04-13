/*
    Brooke Czerwinski

    Extra Credit for Midterm #2
    CS 350 Algorithms & Complexity
    Tabular solution for the pebble collecting robot

    The program receives the dimensions of a grid and the pebble 
    configuration in each cell. It calculates the maximum number of 
    pebbles a robot can collect by following specific rules (the robot
    can only move down or to the right).
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using std::cout;
using std::endl;
using std::stoi;
using std::left;
using std::setw;
using std::cin;
using std::string;
using std::getline;
using std::to_string;
using std::invalid_argument;
using std::out_of_range;

const int MAX_CHAR = 256;

int initializeMatrix(int **board, int rows, int col);
// Takes input for rows and col
bool getDimensions(int &rows, int &col);
// Takes input for the stones on the board
bool boardSetup(int **board, const int rows, const int col);
// Set all values for valueMatrix (tabulation occurs here)
int setValues(int **valuesMatrix, int **board, int rows, int col);
// Print out the contents of a matrix with numbered rows
void printMatrix(int **matrix, int rows, int col);


int main() {
    int rows = -1;
    int col = -1;
    int max = 0;
    
    // Get the dimensions of the board.
    // Only returns true if both rows and col are greater than 0
    if (getDimensions(rows, col)) {
        // Create a board matrix
        int **board = new int*[rows];
        for(int i = 0; i < rows; ++i) {
            board[i] = new int[col];
        }
        // Initialize board matrix
        initializeMatrix(board, rows, col);
        // take input from user for board setup
        boardSetup(board, rows, col);
        // Print out the board
        cout << endl << "Board: " << endl;
        printMatrix(board, rows, col);
        
        // Create a values matrix
        int **valuesMatrix = new int*[rows];
        for(int i = 0; i < rows; ++i) {
            valuesMatrix[i] = new int[col];
        }
        // Initialize values matrix
        initializeMatrix(valuesMatrix, rows, col);
        // Calculate the maximum number of pebbles by filling out the valuesMatrix
        max = setValues(valuesMatrix, board, rows, col);
        // Print out the valuesMatrix
        cout << endl << "Values Matrix: " << endl;
        printMatrix(valuesMatrix, rows, col);
        cout << endl;

        // Print the final values of the program
        cout << "Maximum number of pebbles the robot can collect is: " << max << endl;
        cout << endl;
 
        // Delete dynamic memory
        for(int i = 0; i < rows; ++i) {
            delete [] board[i];
        }
        delete [] board;

        for(int i = 0; i < rows; ++i) {
            delete [] valuesMatrix[i];
        }
        delete [] valuesMatrix;
    }

    return 0;
}


// Setup the game board for the robot
bool boardSetup(int **board, const int rows, const int col) {
    bool returnValue = false;
    string boardRows[rows - 1];

    char c = '0';
    int val = 0;

    cout <<  "Enter " << (col - 1) << " 1's and 0's for each row" << endl;

    for (int i = 1; i < rows; i++) {
        cout << "Row " << i << ": ";
        getline(cin, boardRows[i - 1]);

        try {
            int len = boardRows[i - 1].length();
            // First check that the correct number of values was entered
            if (len != (col - 1)) {
                string errMsg = "Error! " + to_string(len) + " values entered for row " + to_string(i) + "! Exiting...";
                throw invalid_argument(errMsg);
            }
            else {
                // Then check that only 1's or 0's were entered
                string allowedChars = "01";

                // Get the position of any character that isn't a 0 or 1
                int pos = boardRows[i - 1].find_first_not_of(allowedChars);

                if (pos >= 0) {
                    string errMsg = "Error! Only enter 0's or 1's for each row! Row " + to_string(i) + " is: \"" + boardRows[i - 1] + "\" Exiting...";
                    throw invalid_argument(errMsg);
                }
            }
        }
        catch (const invalid_argument& error) {
            cout << error.what() << endl;
            exit(1);
        }
        
    }

    for(int i = 0; i < (rows - 1); i++) {
        int currentBoardCol = 1;                // Current column number on the board
        int length = boardRows[i].length();     // Length of current string (row that was entered)
        for (int n = 0; n < length; n++) {
            // Go through each character in the string at boardRows[i]
            // If it's a 0 or 1, enter a 0 or 1 (as an int) onto the board
            if (boardRows[i][n] == '1' || boardRows[i][n] == '0') {
                board[i + 1][currentBoardCol] = ((int)boardRows[i][n] - 48);
                currentBoardCol++;
            }
        }
    }

    return returnValue;
}


// Get the board dimensions from the user
bool getDimensions(int &rows, int &col) {
    bool returnValue = false;
    string input;

    try {
        // Read the number of rows as a string
        cout << endl;
        cout << "Enter the number of rows: ";
        getline(cin, input);
        // Convert the string to an integer
        rows = stoi(input);

        // Read the number of columns as a string
        cout << "Enter the number of columns: ";
        getline(cin, input);
        // Convert the string to an integer
        col = stoi(input);
        
        if ((rows > 0) && (col > 0)) {
            returnValue = true;

            // Increment the rows and col to account for the "silent" row 0 and column 0
            rows++; col++;
        }
        else {
            cout << "Error! Dimensions must be positive integers. Exiting..." << endl;
        }
    }
    catch (invalid_argument&) {
        cout << "Error! Input must be an integer. Exiting..." << endl;
        exit(1);
    }
    catch (out_of_range&) {
        cout << "Error! Input is out of range for an integer. Exiting..." << endl;
        exit(1);
    }
    
    return returnValue;
}


// Set the values on the board
int setValues(int **valuesMatrix, int **board, int rows, int col) {
    int max = 0;
    int up = 0;
    int left = 0;

    for (int i = 1; i < rows; i++) {
        for (int n = 1; n < col; n++) {
            valuesMatrix[i][n] = board[i][n];

            up = valuesMatrix[i][n - 1];
            left = valuesMatrix[i - 1][n];

            if (up > left) {
                valuesMatrix[i][n] += up;
            }
            else {
                valuesMatrix[i][n] += left;
            }
        }
    }

    return valuesMatrix[rows - 1][col - 1];
}


// Initialize the matrix to the correct size
int initializeMatrix(int **matrix, int rows, int col) {
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int n = 0; n < col; n++) {
            matrix[i][n] = 0;
            ++ count;
        }
    }

    return count;
}


// Initialize all values to 0 in values matrix
int initializeValues(int **valuesMatrix, int rows, int col) {
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int n = 0; n < col; n++) {
            valuesMatrix[i][n] = 0;
            ++count;
        }
    }

    return count;
}


// Print a matrix of rows and columns to the screen
void printMatrix(int **matrix, int rows, int col) {
    cout << "     ";
    for (int i = 0; i < col; i++) {
        cout << left << setw(4) << i;
    }
    cout << endl << endl;
    for (int i = 0; i < rows; i++) {
        cout << left << setw(5) << i;
        for (int n = 0; n < col; n++) {
            cout << left << setw(4) << matrix[i][n];
        }
        cout << endl;
    }
}