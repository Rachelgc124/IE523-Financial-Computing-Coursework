//
//  Card Game.cpp
//  Programming Assignment4: How much would you pay to play this card game?
//  Created by Chang Gao on 2022/10/4.
//

#include <iostream>
#include "cardgame.h"

using namespace std;

int main(int argc, const char * argv[])
{
    CardGame cardgame;
    
    int number_of_cards;
    cout << "Please enter the total numebr of cards: ";
    cin >> number_of_cards;
    // sscanf(argv[1], "%d", &number_of_cards);
    
    // exit if the number inputted is not an even number
    if (0 == number_of_cards % 2)
    {
        double value = cardgame.no_arbitrage_price(number_of_cards);
        cout << "Total Number of Cards = " << number_of_cards << endl;
        cout << "Value of the game = " << value << endl;
    }
    else
        cout << "Please input an even number." << endl;
    
    return 0;
}
