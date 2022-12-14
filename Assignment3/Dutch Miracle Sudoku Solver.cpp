//
//  Dutch Miracle Sudoku Solver.cpp
//  Programming Assignment 3: Dutch Miracle Sudoku Solver
//
//  Created by Chang Gao on 2022/9/9.
//
//  I merged solutions of two assignments into one class (Sudoku).
//  Different output types could be achieved by inputting "0" (for one solution)
//  or "1" (for all solutions) as the third command line input.

#include <iostream>
#include "sudoku.h"

int main (int argc, char * const argv[])
{
    Sudoku x;
    x.read_puzzle(argc, argv);
    
    // If you want to print out one solution, input "0"
    // as the third command line input.
    // If you want to print out all solutions, input "1"
    // as the third command line input.
    if (0 == x.solution_type)
        x.Solve(0, 0);
    else
        x.Solve_all(0, 0);
    
    return 0;
}
