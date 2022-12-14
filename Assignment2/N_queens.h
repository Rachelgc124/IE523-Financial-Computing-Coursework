//
//  N_queens.h
//  Programming Assignment2: N Queens Problem
//  Created by Chang Gao on 2022/9/8.
//

#ifndef N_queens_h
#define N_queens_h

#include <string>

using namespace std;

class Board
{
    
    // private data member: size of the board
    int size;
    
    // private data member: number of solutions
    int solutions = 0;
    
    // pointer-to-pointer initialization of the board
    int **chess_board;
    string **output_f;
    
    
    // private member function:  returns 'false' if
    // the (row, col) position is not safe.
    bool is_this_position_safe(int row, int col)
    {
        // check if it is a safe row
        for (int i = 0; i < size; i++)
        {
            if (i != col && chess_board[row][i] != 0)
                return false;
        }
        
        // check if it is a safe column
        for (int i = 0; i < size; i++)
        {
            if (i != row && chess_board[i][col] != 0)
                return false;
        }
        
        // check if it is safe on the positive diagonal
        for (int i = 0; i < size; i++)
        {
            if (i != row)
            {
                int j = row - i + col;
                if (j >= size)
                    continue;
                else if (j >= 0)
                {
                    if (chess_board[i][j] != 0)
                        return false;
                } else
                    break;
            }
        }
        
        // check if it is safe on the negative diagonal
        for (int i = 0; i < size; i++)
        {
            if (i != row)
            {
                int j = col - row + i;
                if (j < 0)
                    continue;
                else if (j < size)
                {
                    if (chess_board[i][j] != 0)
                        return false;
                } else
                    break;
            }
        }
        
        return true;
    }
    
    // private member function: initializes the (n x n) chessboard
    void initialize(int n)
    {
        size = n;
        
        chess_board = new int *[size];
        for (int i = 0; i < size; i++)
            chess_board[i] = new int[size];
        
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
                chess_board[i][j] = 0;
        }
        
    }
    
    // private member function: prints the board position
    void print_board()
    {
        // Convert board output format. Denote the position of a queen as "Q",
        // otherwise, "-".
        output_f = new string *[size];
        for (int i = 0; i < size; i++)
            output_f[i] = new string[size];
        
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
                if (0 == chess_board[i][j])
                    output_f[i][j] = "-";
                else
                    output_f[i][j] = "Q";
        }
        
        std::cout << size << "-Queens Problem Solution" << std::endl;
        string bar(3 * size, '-');
        cout << bar << endl;
        
        for (int i = 0; i < size; i ++)
        {
            for (int j = 0; j < size; j++)
                cout << " " << output_f[i][j] << " ";
            cout << endl;
        }
        
        cout << bar << endl;
    }
    
    // private member function: prints all boards position
    void print_all_boards()
    {
        // Convert board output format. Denote the position of a queen as "Q",
        // otherwise, "-".
        output_f = new string *[size];
        for (int i = 0; i < size; i++)
            output_f[i] = new string[size];
        
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
                if (0 == chess_board[i][j])
                    output_f[i][j] = "-";
                else
                    output_f[i][j] = "Q";
        }
        
        cout << endl;
        std::cout << size << "x" << size << " Solution #: " << solutions << std::endl;
        string bar(3 * size, '-');
        cout << bar << endl;
        
        for (int i = 0; i < size; i ++)
        {
            for (int j = 0; j < size; j++)
                cout << " " << output_f[i][j] << " ";
            cout << endl;
        }
        
        cout << bar << endl;
    }
    
    // private member function: recursive backtracking
    // Finding one solution to the N-Queens Problem
    bool solve(int col)
    {
        if (col >= size)
        {
            solutions++;
            return true;
        } else
        {
            for (int i = 0; i < size; i++)
            {
                if (is_this_position_safe(i, col))
                {
                    chess_board[i][col] = 1;
                    if (solve(col + 1))
                        return true;
                    else
                        chess_board[i][col] = 0;
                }
            }
        }
        
        return false;
    }
    
    // private member function: recursive backtracking
    // Finding all solutions to the N-Queens Problem
    bool solve_all(int col)
    {
        if (col >= size)
        {
            solutions++;
            print_all_boards();
            return true;
        } else
        {
            int temp = 0;
            for (int i = temp; i < size; i++)
            {
                if (is_this_position_safe(i, col))
                {
                    temp = i;
                    chess_board[i][col] = 1;
                    if (solve_all(col + 1))
                    {
                        chess_board[i][col] = 0;
                        if ((temp + 1) < size)
                            continue;
                        return true;
                    } else
                    {
                        chess_board[i][col] = 0;
                        continue;
                    }
                }
            }
        }
        if (solutions != 0)
            return true;
        
        return false;
    }
    
public:
    // Solves the n-Queens problem by (recursive) backtracking
    // Finding one solution to the N-Queens Problem
    void nQueens(int n)
    {
        initialize(n);
        
        if (solve(0))
            print_board();
        else
            std::cout << "There is no solution to the " << n << "-Queens Problem" << std::endl;
    }
    
    // Solves the n-Queens problem by (recursive) backtracking
    // Finding all solutions to the N-Queens Problem
    void nQueens_all(int n)
    {
        initialize(n);
        
        if (solve_all(0))
        {
            cout << "There are " << solutions << " different solutions to the ";
            cout << size << "-Queens Problem" << endl;
        }
        else
            std::cout << "There is no solution to the " << size << "-Queens Problem" << std::endl;
    }
};

#endif /* N_queens_h */
