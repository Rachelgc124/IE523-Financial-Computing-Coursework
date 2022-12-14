//
//  sudoku.h
//  Dutch Miracle Sudoku Solver
//
//  Created by Chang Gao on 2022/9/9.
//
//  I merged solutions of two assignments into one class (Sudoku).
//  Different output types could be achieved by inputting "0" (for one solution)
//  or "1" (for all solutions) as the third command line input.

#ifndef sudoku_h
#define sudoku_h

#include <vector>
#include <fstream>
#include <tuple>

using std::vector;
using namespace std;

class Sudoku
{
    // Private
    int puzzle[9][9];
    int solution = 0;
    
    // Private mumber function that checks if the named row is valid
    bool row_valid(int row)
    {
        vector <int> set;
        for (int i = 0; i < 9; i++)
        {
            if (0 == puzzle[row][i]) {continue;}
            if (set.end() == find(set.begin(), set.end(), puzzle[row][i]))
                set.push_back(puzzle[row][i]);
            else
                return false;
        }
        return true;
    }
    
    // Private member function that checks if the named column is valid
    bool col_valid(int col)
    {
        vector <int> set;
        for (int i = 0; i < 9; i++)
        {
            if (0 == puzzle[i][col]) {continue;}
            if (set.end() == find(set.begin(), set.end(), puzzle[i][col]))
                set.push_back(puzzle[i][col]);
            else
                return false;
        }
        return true;
    }
    
    // Private member function that checks if the named 3x3 block is valid
    bool block_valid(int row, int col)
    {
        int row_block = row / 3;
        int col_block = col / 3;
        vector <int> set;
        
        for (int i = row_block * 3; i < row_block * 3 + 3; i++)
        {
            for (int j = col_block * 3; j < col_block * 3 + 3; j++)
            {
                if (0 == puzzle[i][j])
                    continue;
                if (set.end() == find(set.begin(), set.end(), puzzle[i][j]))
                    set.push_back(puzzle[i][j]);
                else
                    return false;
            }
        }
        return true;
    }
    
    
    // Private member function that checks if the positive-diagonal
    // (SW to NE diagonals) have no repeated digits.
    std::tuple<bool, int> positive_diagonals_valid()
    {
        // If the returned bool variable is false,
        // then the second-tuple is the X that was repeated along
        // some positive diagonal.
        
        for (int i = 1; i < 9; i++)
        {
            vector <int> set;
            for (int j = 0; j <= i; j++)
            {
                if (0 == puzzle[i - j][j]) {continue;}
                if (set.end() == find(set.begin(), set.end(), puzzle[i - j][j]))
                    set.push_back(puzzle[i - j][j]);
                else
                    return make_tuple(false, puzzle[i - j][j]);
            }
        }
        
        for (int i = 1; i < 8; i++)
        {
            vector <int> set;
            for (int j = 8; j >= i; j--)
            {
                if (0 == puzzle[j][i - j + 8]) {continue;}
                if (set.end() == find(set.begin(), set.end(), puzzle[j][i - j + 8]))
                    set.push_back(puzzle[j][i - j + 8]);
                else
                    return make_tuple(false, puzzle[j][i - j + 8]);
            }
                
        }
        
        return make_tuple(true, 0);
    }
    
    // Private member funtion that checks if adjacent cells
    // (along the positive diagonal) touching at a corner
    // have a difference of atleast 4
    std::tuple<bool, int, int> adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4()
    {
        // If the returned bool variable is false,
        // then the two ints (i and j, say) identify puzzle[i][j] and
        // puzzle[i-1][j+1] that do not meet the "minimum-difference
        // of 4".
        
        for (int i = 1; i < 9; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                if (i - j > 0)
                {
                    if (0 == puzzle[i - j][j] || 0 == puzzle[i - j - 1][j + 1]) {continue;}
                    if (abs(puzzle[i - j][j] - puzzle[i - j - 1][j + 1]) < 4)
                        return make_tuple(false, i - j, j);
                }
            }
        }
        
        for (int i = 1; i < 8; i++)
        {
            for (int j = 8; j >= i; j--)
            {
                if (i - j < 0)
                {
                    if (0 == puzzle[j][i - j + 8] || 0 == puzzle[j - 1][i - j + 9]) {continue;}
                    if (abs(puzzle[j][i - j + 8] - puzzle[j - 1][i - j + 9]) < 4)
                        return make_tuple(false, j, i - j + 8);
                }
            }
        }
        
        return make_tuple(true, 0, 0);
    }
    
public:
    // Different output types could be achieved by inputting "0" (for one solution)
    // or "1" (for all solutions) as the third command line input.
    int solution_type;

    // Public member function that reads the incomplete puzzle
    void read_puzzle(int argc, char * const argv[])
    {
        // write code that reads the input puzzle using the
        // guidelines of figure 23 of the bootcamp material
        
        char file_name[20];
        int value_just_read_from_file;
        vector <int> P;
        
        // sscanf(argv[1], "%s", file_name);
        // sscanf(argv[2], "%d", &solution_type);
        cout << "Please input the file name: ";
        cin >> file_name;
        cout << "Please input the solution type (If you want to get 1 solution, please input 0. ";
        cout << "For all solution, please input 1): ";
        cin >> solution_type;
        cout << "Input File Name: " << file_name << endl;
        
        ifstream input_file(file_name);
        
        if (input_file.is_open())
        {
            while (input_file >> value_just_read_from_file)
                P.push_back(value_just_read_from_file);
        }
        else
            cout << "Input file does not exist in PWD." << endl;
        
        for (int i = 0; i < P.size(); i++)
            puzzle[i / 9][i % 9] = P[i];
        
        // check if the input puzzle has solutions
        bool is_row_valid = true, is_col_valid = true, is_block_valid = true, is_diagonals_valid, is_adjacent_valid;
        int fail_row = 9, fail_col = 9, fail_block_r = 9, fail_block_c = 9, fail_diag, fail_adj_r, fail_adj_c;
        tie(is_diagonals_valid, fail_diag) = positive_diagonals_valid();
        tie(is_adjacent_valid, fail_adj_r, fail_adj_c) = adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4();
        
        
        for (int i = 0; i < 9; i++)
        {
            if (!row_valid(i))
            {
                is_row_valid = false;
                fail_row = i;
            }
            if (!col_valid(i))
            {
                is_col_valid = false;
                fail_col = i;
            }
        }
        
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (!block_valid(i * 3, j * 3))
                {
                    is_block_valid = false;
                    fail_block_r = i * 3;
                    fail_block_c = j * 3;
                }
            }
        }
        
        if (is_row_valid && is_col_valid && is_block_valid &&
            is_diagonals_valid && is_adjacent_valid)
        {
            cout << endl;
            cout << "Initial Sudoku Puzzle meets Dutch Miracle requirements" << endl;
            print_puzzle();
        }
        else
        {
            print_puzzle();
            cout << endl;
            cout << "Partially-Filled Sudoku Puzzle does not meet Dutch Miracle requirments... ";
            cout << "Quitting!" << endl;
            
            if (!is_row_valid)
                cout << "The row " << fail_row << " has number appearing multiple times." << endl;
            if (!is_col_valid)
                cout << "The column " << fail_col << " has number appearing multiple times." << endl;
            if (!is_block_valid)
            {
                cout << "The block which begin with puzzle[" << fail_block_r << "][" << fail_block_c << "] ";
                cout << "has number appearing multiple times." << endl;
            }
            if (!is_diagonals_valid)
                cout << "The number " << fail_diag << " appears multiple times along the positice-diagnal." << endl;
            if (!is_adjacent_valid)
            {
                cout << "puzzle[" << fail_adj_r << "][" << fail_adj_c << "] = " << puzzle[fail_adj_r][fail_adj_c] << ", ";
                cout << "puzzle[" << fail_adj_r - 1 << "][" << fail_adj_c + 1 << "] = ";
                cout << puzzle[fail_adj_r - 1][fail_adj_c + 1] << ", ";
                cout << "and they do not meet the minimum-difference of 4." << endl;
            }
            
        }
        
        cout << endl;
    }
    
    // Public member function that prints the puzzle when called
    void print_puzzle()
    {
        //std::cout << number_of_calls << std::endl;
        std::cout << std::endl << "Board Position" << std::endl;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // check if we have a legitimate integer between 1 and 9
                if ((puzzle[i][j] >= 1) && (puzzle[i][j] <= 9))
                {
                    // printing initial value of the puzzle with some formatting
                    std::cout << puzzle[i][j] << " ";
                }
                else {
                    // printing initial value of the puzzle with some formatting
                    std::cout << "X ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    // Public member function that called when try to print all solutionos
    void print_all_puzzle()
    {
        cout << endl << "Solution #" << solution << endl;
        std::cout << "Board Position" << std::endl;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // check if we have a legitimate integer between 1 and 9
                if ((puzzle[i][j] >= 1) && (puzzle[i][j] <= 9))
                {
                    // printing initial value of the puzzle with some formatting
                    std::cout << puzzle[i][j] << " ";
                }
                else {
                    // printing initial value of the puzzle with some formatting
                    std::cout << "X ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    // Public member function that (recursively) implements the brute-force
    // search for one possible solution to the incomplete Sudoku puzzle
    bool Solve(int row, int col)
    {
        // check if col >= 9, if so, the Sudoku has been solved
        if (col >= 9)
        {
            solution++;
            
            cout << endl << "Enumerating one solution to the Dutch Miracle Sudoku instance shown above." << endl;
            print_puzzle();
            return true;
        }
        
        // check if there is any cell left blank
        bool is_blank = false;
        for (int i = col; i < 9; i++)
        {
            for (int j = row; j < 9; j++)
            {
                if (0 == puzzle[j][i])
                {
                    is_blank = true;
                    row = j;
                    col = i;
                    break;
                }
                if (8 == j)
                    row = 0;
            }
            if (is_blank) {break;}
        }

        if (!is_blank)
        {
            // if there are no cells left blank, the Sudoku has been solved
            solution++;
            print_puzzle();
            return true;
        }
        else
        {
            for (int k = 1; k <= 9; k++)
            {
                puzzle[row][col] = k;
                
                bool is_diagonals_valid, is_adjacent_valid;
                int fail_diag, fail_adj_r, fail_adj_c;
                tie(is_diagonals_valid, fail_diag) = positive_diagonals_valid();
                tie(is_adjacent_valid, fail_adj_r, fail_adj_c) = adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4();
                
                // check if the k meet the requirments
                if (row_valid(row) && col_valid(col) && block_valid(row, col) &&
                    is_diagonals_valid && is_adjacent_valid)
                {
                    if (row + 1 < 9)
                    {
                        if (Solve(row + 1, col))
                            return true;
                        else
                            puzzle[row][col] = 0;
                            continue;
                    }
                    else
                    {
                        if (Solve(0, col + 1))
                            return true;
                        else
                            puzzle[row][col] = 0;
                            continue;
                    }
                }
                
                puzzle[row][col] = 0;
            }
        }
        
        return false;
    }
    
    // Public member function that (recursively) implements the brute-force
    // search for all possible solutions to the incomplete Sudoku puzzle
    bool Solve_all(int row, int col)
    {
        // check if col >= 9, if so, get one possible solution for this Sudoku
        if (col >= 9)
        {
            solution++;
            if (1 == solution)
                cout << "Enumerating all solutions to the Dutch Miracle Sudoku instance shown above." << endl;
            print_all_puzzle();
            return true;
        }
        
        // check if there is any cell left blank
        bool is_blank = false;
        for (int i = col; i < 9; i++)
        {
            for (int j = row; j < 9; j++)
            {
                if (0 == puzzle[j][i])
                {
                    is_blank = true;
                    row = j;
                    col = i;
                    break;
                }
                if (8 == j)
                    row = 0;
            }
            if (is_blank) {break;}
        }
    
        if (!is_blank)
        {
            solution++;
            print_all_puzzle();
            return true;
        }
        else
        {
            for (int k = 1; k <= 9; k++)
            {
                puzzle[row][col] = k;
                
                bool is_diagonals_valid, is_adjacent_valid;
                int fail_diag, fail_adj_r, fail_adj_c;
                tie(is_diagonals_valid, fail_diag) = positive_diagonals_valid();
                tie(is_adjacent_valid, fail_adj_r, fail_adj_c) = adjacent_cells_along_positive_diagonals_have_at_least_a_difference_of_4();
                
                // check if the k meet the requirments
                // continue to find all possible solutions
                if (row_valid(row) && col_valid(col) && block_valid(row, col) &&
                    is_diagonals_valid && is_adjacent_valid)
                {
                    if (row + 1 < 9)
                    {
                        if (Solve_all(row + 1, col))
                        {
                            if (k < 9)
                                continue;
                            puzzle[row][col] = 0;
                            return true;
                        }
                        else
                            puzzle[row][col] = 0;
                            continue;
                    }
                    else
                    {
                        if (Solve_all(0, col + 1))
                        {
                            if (k < 9)
                                continue;
                            puzzle[row][col] = 0;
                            return true;
                        }
                        else
                            puzzle[row][col] = 0;
                            continue;
                    }
                }
                puzzle[row][col] = 0;
            }
        }
        if (solution != 0)
            return true;
        
        return false;
    }
};

#endif /* sudoku_h */
