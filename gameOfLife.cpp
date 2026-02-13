#include <filesystem>
#include <string>
#include "gameOfLife.h"

using namespace std;

/*
Function to clear terminal depending on OS
*/
void clearScreen()
{
    // use "cls" in windows and "clear" command in Mac and Linux
    #ifdef _WIN32
        system("clS");
    #else
        system("clear");
    #endif
}

/*
Function to initialize all cells in the board.
Takes in 2D array of pointers and size of array
Should create structs and populate the array
*/
void initCells(Cell* board[][10], int boardSize)
{
    for(int i = 0; i<boardSize; i++)//i is rows
    {
        for(int j = 0; j<boardSize; j++)//j is cols
        {
            board[i][j] = new Cell;
        }
    }
}

/*
Function to read the board from a file
Prompt for the file to read inside of the function
Structure of file should consist of 10 lines of 0 or 1 to indicate cell state
*/
void readBoard(Cell* board[][10], int boardSize) 
{
    string inputFile = " ";
    string inputLine;
    ifstream fin;

    //Get file
    cout << "Enter file to open (don't forget \".txt\"): ";
    cin >> inputFile;
    fin.open(inputFile);
    while(!fin)
    {
        cout << "Could not open file, please enter a known file: ";
        cin >> inputFile;
        fin.open(inputFile);
    }

    int i = 0;//rows
    //Read file
    while(getline(fin, inputLine))
    {
        for(int j = 0; j<boardSize; j++)
        {
            board[i][j]->x = j;
            board[i][j]->y = i;
            board[i][j]->state = inputLine[j] - '0';
        }
        i++;
    }

    fin.close();
}

/*
Function to print out all cells to cout
*/
void printCells(Cell* board[][10], int boardSize)
{
    for(int i=0; i<boardSize; i++)//i is rows
    {
        for(int j=0; j<boardSize; j++)//j is cols
        {
            // cout << board[i][j]->state << "";
            if(board[i][j]->state == 1)
            {
                cout << "0";
            }
            else{
                cout << " ";
            }
        }
        cout << endl;
    }

}

/*
Function to count the number of live neighbors for each cell.
Must use the x, y position stored with each cell to determine which neighbors they have
*/
void findNumNeighbors(Cell* board[][10], int boardSize, Cell* curCell) 
{
    int x = curCell->y;
    int y = curCell->x;
    int numNeighbors = 0;//Num of live neighbors
    vector<Cell*> neighbors;
    //Up-Left
    if(y != 0 && x != 0){neighbors.push_back(board[x-1][y-1]);}
    //Up
    if(y != 0){neighbors.push_back(board[x][y-1]);}
    //Up-Right
    if(y != 0 && x != boardSize - 1){neighbors.push_back(board[x+1][y-1]);}
    //Right
    if(x != boardSize - 1){neighbors.push_back(board[x+1][y]);}
    //Bottom-Right
    if(y != boardSize - 1 && x != boardSize - 1){neighbors.push_back(board[x+1][y+1]);}
    //Bottom
    if(y != boardSize - 1){neighbors.push_back(board[x][y+1]);}
    //Bottom-Left
    if(y != boardSize - 1 && x != 0){neighbors.push_back(board[x-1][y+1]);}
    //Left
    if(x != 0){neighbors.push_back(board[x-1][y]);}

    for(int i = 0; i < neighbors.size(); i++)
    {
        if(neighbors[i]->state == 1)
        {
            numNeighbors++;
        }
    }

    curCell->numLiveNeighbors = numNeighbors;

}

/*
Function to update each cell's state based on number of neighbors
Must use following rules:

Any live cell with fewer than two live neighbors dies, as if by underpopulation.
Any live cell with two or three live neighbors lives on to the next generation.
Any live cell with more than three live neighbors dies, as if by overpopulation.
Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

Return if you updated cells or not to break out of while loop from main.
*/
bool updateCellState(Cell* board[][10], int boardSize) 
{
    bool boardChanged = false;
    for(int i = 0; i < boardSize; i++)//i is rows
    {
        for(int j = 0; j < boardSize; j++)//j is cols
        {
            findNumNeighbors(board, boardSize, board[i][j]);
        }
    }

    for(int i = 0; i < boardSize; i++)//i is rows
    {
        for(int j = 0; j < boardSize; j++)//j is cols
        {
            //Any live cell with fewer than two live neighbors dies, as if by underpopulation.
            if(board[i][j]->state == 1 && board[i][j]->numLiveNeighbors < 2)
            {
                board[i][j]->state = 0;
                boardChanged = true;
            }
            //Any live cell with two or three live neighbors lives on to the next generation.
            if(board[i][j]->state == 1 && board[i][j]->numLiveNeighbors >= 2 && board[i][j]->numLiveNeighbors <= 3)
            {
                board[i][j]->state = 1;
            }
            //Any live cell with more than three live neighbors dies, as if by overpopulation.
            if(board[i][j]->state == 1 && board[i][j]->numLiveNeighbors > 3)
            {
                board[i][j]->state = 0;
                boardChanged = true;
            }
            //Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
            if(board[i][j]->state == 0 && board[i][j]->numLiveNeighbors == 3)
            {
                board[i][j]->state = 1;
                boardChanged = true;
            }
        }
    }

    return boardChanged;
}
