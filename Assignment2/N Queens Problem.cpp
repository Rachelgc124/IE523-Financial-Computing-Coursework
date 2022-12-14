//
//  N Queens Problem.cpp
//  Programming Assignment2: N Queens Problem
//  Created by Chang Gao on 2022/9/8.
//
//  I merged solutions of two assignments into one class (Board).
//  Different output types could be achieved by inputting "0" (for one solution)
//  or "1" (for all solutions) as the third command line input.

#include <iostream>
#include "N_queens.h"

int main (int argc, char * const argv[])
{
    Board x;
    
    int board_size;
    
    // If you want to print out one solution, input "0"
    // as the third command line input.
    // If you want to print out all solutions, input "1"
    // as the third command line input.
    int solution_type;
    
    sscanf(argv[1], "%d", &board_size);
    sscanf(argv[2], "%d", &solution_type);
    
    if (0 == solution_type)
        x.nQueens(board_size);
    else if (1 == solution_type)
        x.nQueens_all(board_size);
    else
    {
        cout << "Invalid solution type." <<endl;
        cout << "Please input '0' if you want to get one solution. ";
        cout << "Input '1' if you want to get all solutions." << endl;
    }
    
    return 0;
}

