#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

void init_prompt(string & inputfilename, bool & wrappedAround);
Grid<string> init_grid_from(string inputfilename);
bool alive(Grid<string> & grid);
Grid<string> next_generation(Grid<string> & curt_grid, bool wrappedAround);
void advance(Grid<string> & curt_grid, Grid<string> & next_grid, bool wrappedAround);
int countNeighboursRegular(Grid<string> & curt_grid, int r, int c);
int countNeighboursWrapped(Grid<string> & curt_grid, int x, int y);
void print2DStrGrid(Grid<string> & grid);

int main() {

    string inputfilename;
    bool wrappedAround;
    init_prompt(inputfilename, wrappedAround);

    Grid<string> curt_grid = init_grid_from(inputfilename);
    Grid<string> next_grid;

    print2DStrGrid(curt_grid);
    while (alive(curt_grid)) {
        string action = getLine("a)nimate, t)ick, q)uit?");
        if (action == "a" || action == "A") {
            string frames = getLine("How many frames?");
            while (!stringIsInteger(frames)) {
                cout << "Wrong format. Try again." << endl;
                frames = getLine("How many frames?");
            }
            int f = stringToInteger(frames);
            while (f > 0) {
                next_grid = next_generation(curt_grid, wrappedAround);
                curt_grid = next_grid;
                print2DStrGrid(curt_grid);
                f--;
                clearConsole();
            }
        }
        else if (action == "t" || action == "T") {
            next_grid = next_generation(curt_grid, wrappedAround);
            curt_grid = next_grid;
            print2DStrGrid(curt_grid);
        }
        else if (action == "q" || action == "Q") {
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

void init_prompt(string & inputfilename, bool & wrappedAround) {
    cout << "Welcome to the CS 106B Game of Life,               " << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:     " << endl;
    cout << "- A cell with 1 or fewer neighbors dies.           " << endl;
    cout << "- Locations with 2 neighbors remain stable.        " << endl;
    cout << "- Locations with 3 neighbors will create life.     " << endl;
    cout << "- A cell with 4 or more neighbors dies.            " << endl;

    inputfilename = getLine("Grid input file name?");
    while (!fileExists(inputfilename)) {
        cout << "The file doesn't exist. Try again." << endl;
        inputfilename = getLine("Grid input file name?");
    }

    string wrapAround;
    wrapAround = getLine("Should the stimulation wrap around the grid? (y/n)");
    if (wrapAround == "y" || wrapAround == "Y") {
        wrappedAround = true;
    }
    else if (wrapAround == "n" || wrapAround == "N") {
        wrappedAround = false;
    }
    else {
        throw 1;
    }
}

Grid<string> init_grid_from(string inputfilename) {
    ifstream inputfilestream;
    openFile(inputfilestream, inputfilename);

    int rowNum;
    int colNum;
    string line;
    if (getline(inputfilestream, line)) {
        rowNum = stringToInteger(line);
    }
    else {
        throw 1;
    }
    if (getline(inputfilestream, line)) {
        colNum = stringToInteger(line);
    }
    else {
        throw 1;
    }

    Grid<string> grid;
    grid.resize(rowNum, colNum);

    int counter = 0;
    while (counter < rowNum) {
        getLine(inputfilestream, line);
        for (int i = 0; i < (int) line.length(); i++) {
            grid[counter][i] = line[i];
        }
        counter++;
    }

    inputfilestream.close();

    return grid;
}

bool alive(Grid<string> & grid) {
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            if (grid[r][c] == "-") {
                continue;
            }
            else {
                return true;
            }
        }
    }
    return false;
}

Grid<string> next_generation(Grid<string> & curt_grid, bool wrappedAround) {
    Grid<string> next_grid = Grid<string>(curt_grid.numRows(), curt_grid.numCols(), "-"); // prepare temp grid
    advance(curt_grid, next_grid, wrappedAround);
    return next_grid;
}

void advance(Grid<string> & curt_grid, Grid<string> & next_grid, bool wrappedAround) {
    for (int r = 0; r < next_grid.numRows(); r++) {
        for (int c = 0; c < next_grid.numCols(); c++) {
            int aliveCells;
            if (wrappedAround) {
                aliveCells = countNeighboursWrapped(curt_grid, r, c);
            }
            else {
                aliveCells = countNeighboursRegular(curt_grid, r, c);
            }

            if (aliveCells < 2 || aliveCells > 3) {
                next_grid[r][c] = "-";
            }
            else if (aliveCells == 3) {
                next_grid[r][c] = "X";
            }
            else {
                next_grid[r][c] = curt_grid[r][c];
            }
        }
    }
}

int countNeighboursRegular(Grid<string> & curt_grid, int x, int y) {
    int counter = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if (x + i < 0 || x + i >= curt_grid.numRows() || y + j < 0 || y + j >= curt_grid.numCols()) {
                continue;
            }
            if (curt_grid[x + i][y + j] == "-") {
                continue;
            }
            counter ++;
        }
    }
    return counter;
}

int countNeighboursWrapped(Grid<string> & curt_grid, int x, int y) {
    int counter = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if (x + i > -1 && x + i < curt_grid.numRows() && y + j > -1 && y + j < curt_grid.numCols()) {
                if (curt_grid[x + i][y + j] == "-") {
                    continue;
                }
                else {
                    counter ++;
                }
            }
            else {
                int newX;
                int newY;
                if (x + i < 0) {
                    newX = x + i + curt_grid.numRows();
                }
                else {
                    newX = (x + i) % curt_grid.numRows();
                }
                if (y + j < 0) {
                    newY = y + j + curt_grid.numCols();
                }
                else {
                    newY = (y + j) % curt_grid.numCols();
                }
                if (curt_grid[newX][newY] == "-") {
                    continue;
                }
                else {
                    counter++;
                }
            }
        }
    }
    return counter;
}

void print2DStrGrid(Grid<string> & grid) {
    string line;
    for (int r = 0; r < grid.numRows(); r++) {
        line = "";
        for (int c = 0; c < grid.numCols(); c++) {
            line += grid[r][c];
        }
        cout << line << endl;
    }
}
