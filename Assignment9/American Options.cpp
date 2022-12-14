//
//  American Options.cpp
//  Programming Assignment 9: Pricing American Options
//  Created by Rachel Gao on 2022/11/18.
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

double up_factor, uptick_prob, downtick_prob, notick_prob, risk_free_rate, strike_price;
double initial_stock_price, expiration_time, volatility, R;
int no_of_divisions;
double **call_cache;
double **put_cache;

void initial_caches()
{
    call_cache = new double *[no_of_divisions + 1];
    put_cache = new double *[no_of_divisions + 1];
    
    for (int i = 0; i <= no_of_divisions; i++)
    {
        call_cache[i] = new double[2 * i + 1];
        put_cache[i] = new double[2 * i + 1];
    }
    
    for (int i = 0; i <= no_of_divisions; i++)
    {
        for (int j = 0; j <= 2*i; j++)
        {
            call_cache[i][j] = -1;
            put_cache[i][j] = -1;
        }
    }
}

double max(float a, float b)
{
    return (b < a) ? a:b;
}

double american_call_option(int k, int i, float current_stock_price)
{
    if (-1 == call_cache[k][i+k]) {
        if (k == no_of_divisions) {
            call_cache[k][i+k] = max(0.0, (current_stock_price - strike_price));
            return call_cache[k][i+k];
        }
        
        call_cache[k][i+k] = max((current_stock_price - strike_price),
                                 (uptick_prob * american_call_option(k+1, i+1, current_stock_price * up_factor)
                                  + notick_prob * american_call_option(k+1, i, current_stock_price)
                                  + downtick_prob * american_call_option(k+1, i-1, current_stock_price/up_factor)) / R);
    }
    
    return call_cache[k][i+k];
}

double american_put_option(int k, int i, float current_stock_price)
{
    if (-1 == put_cache[k][i+k]) {
        if (k == no_of_divisions) {
            put_cache[k][i+k] = max(0.0, (strike_price - current_stock_price));
            return put_cache[k][i+k];
        }
        
        put_cache[k][i+k] = max((strike_price - current_stock_price),
                                (uptick_prob * american_put_option(k+1, i+1, current_stock_price * up_factor)
                                 + notick_prob * american_put_option(k+1, i, current_stock_price)
                                 + downtick_prob * american_put_option(k+1, i-1, current_stock_price/up_factor)) / R);
    }
    
    return put_cache[k][i+k];
}

int main(int argc, const char * argv[]) {
    
    sscanf(argv[1], "%lf", &expiration_time);
    sscanf(argv[2], "%d", &no_of_divisions);
    sscanf(argv[3], "%lf", &risk_free_rate);
    sscanf(argv[4], "%lf", &volatility);
    sscanf(argv[5], "%lf", &initial_stock_price);
    sscanf(argv[6], "%lf", &strike_price);
    
    R = exp(risk_free_rate * expiration_time / (float) no_of_divisions);
    up_factor = exp(volatility  * sqrt((2 * expiration_time) / (float) no_of_divisions));
    uptick_prob = pow((sqrt(R) - 1 / sqrt(up_factor)) / (sqrt(up_factor) - 1 / sqrt(up_factor)), 2);
    downtick_prob = pow((sqrt(up_factor) - sqrt(R)) / (sqrt(up_factor) - 1 / sqrt(up_factor)), 2);
    notick_prob = 1 - uptick_prob - downtick_prob;
    
    initial_caches();
    
    double call_price = american_call_option(0, 0, initial_stock_price);
    double put_price = american_put_option(0, 0, initial_stock_price);
    
    cout << "(Memoized) Recursive Trinomial American Option Pricing" << endl;
    cout << "Expiration Time (Year) = " << expiration_time << endl;
    cout << "Number of Divisions = " << no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Up Factor = " << up_factor << endl;
    cout << "Uptick Probability = " << uptick_prob << endl;
    cout << "Notick Probability = " << notick_prob << endl;
    cout << "Downtick Probability = " << downtick_prob << endl;
    cout << "--------------------------------------" << endl;
    cout << "Trinomial Price of an American Call Option = " << call_price << endl;
    cout << "Trinomial Price of an American Put Option = " << put_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Let us verify Put-Call Parity: S+P-C = Kexp(-r*T) for American Options" << endl;
    cout << initial_stock_price << " + " << put_price << " - " << call_price;
    cout << " = " << strike_price << "exp(-" << risk_free_rate << " * " << expiration_time << ")" << endl;
    cout << initial_stock_price + put_price - call_price << " ?=? " << strike_price * exp(-risk_free_rate * expiration_time) << endl;
    cout << "Looks like Put-Call Parity does NOT hold" << endl;
    cout << "--------------------------------------" << endl;
    
}
