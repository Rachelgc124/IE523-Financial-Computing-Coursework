//
//  cardgame.h
//  Programming Assignment4: How much would you pay to play this card game?
//  Created by Chang Gao on 2022/10/4.
//

#ifndef cardgame_h
#define cardgame_h

#include <algorithm>

using namespace std;

class CardGame
{
    // private
    int number;
    double **cache;
    
    // initialize the cache
    void initialize(int number)
    {
        this->number = number;
        int cardnum = this->number / 2;
        cache = new double *[cardnum + 1];
        for (int i = 0; i <= cardnum; i++)
            cache[i] = new double[cardnum + 1];
        for (int i = 0; i <= cardnum; i++)
        {
            for (int j = 0; j <= cardnum; j++)
                cache[i][j] = 0;
        }
    }
    
    // compute the value of the game to you at any point
    double value(int r, int b)
    {
        if (0 == r)
        {
            cache[r][b] = (double) b;
            return cache[r][b];
        }
        if (0 == b)
        {
            cache[r][b] = 0;
            return cache[r][b];
        }
        
        // memoization
        if (!cache[r][b])
        {
            double term1 = ((double) r / (r + b)) * value(r - 1, b);
            double term2 = ((double) b / (r + b)) * value(r, b - 1);
            cache[r][b] = max((term1 + term2), (double) (b - r));
        }
        
        return cache[r][b];
    }
    
public:
    double no_arbitrage_price(int number)
    {
        initialize(number);
        int cardnum = number / 2;
        
        return value(cardnum, cardnum);
    }
};

#endif /* cardgame_h */
