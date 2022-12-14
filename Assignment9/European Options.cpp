//
//  European Options.cpp
//  Programming Assignment 9: Pricing European Options
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

double european_call_option(int k, int i)
{
    if (-1 == call_cache[k][i+k]) {
        if (k == no_of_divisions) {
            call_cache[k][i+k] = max(0.0, (initial_stock_price * pow(up_factor, ((float) i))) - strike_price);
            return call_cache[k][i+k];
        }
        
        call_cache[k][i+k] = (uptick_prob * european_call_option(k + 1, i + 1)
                              + notick_prob * european_call_option(k + 1, i)
                              + downtick_prob * european_call_option(k + 1, i - 1)) / R;
    }
    
    return call_cache[k][i+k];
}

double european_put_option(int k, int i)
{
    if (-1 == put_cache[k][i+k]) {
        if (k == no_of_divisions) {
            put_cache[k][i+k] = max(0.0, strike_price - (initial_stock_price*pow(up_factor, ((float) i))));
            return put_cache[k][i+k];
        }
        
        put_cache[k][i+k] = (uptick_prob * european_put_option(k + 1, i + 1)
                             + notick_prob * european_put_option(k + 1, i)
                             + downtick_prob * european_put_option(k + 1, i - 1)) / R;
    }
    
    return put_cache[k][i+k];
}

double N(const double& z) {
    if (z > 6.0) { return 1.0; }; // this guards against overflow
    if (z < -6.0) { return 0.0; };
    double b1 = 0.31938153;
    double b2 = -0.356563782;
    double b3 = 1.781477937;
    double b4 = -1.821255978;
    double b5 = 1.330274429;
    double p = 0.2316419;
    double c2 = 0.3989423;
    double a=fabs(z);
    double t = 1.0/(1.0+a*p);
    double b = c2*exp((-z)*(z/2.0));
    double n = ((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t;
    n = 1.0-b*n;
    if ( z < 0.0 ) n = 1.0 - n;
    return n;
}

double option_price_call_black_scholes(const double& S,       // spot (underlying) price
                                       const double& K,       // strike (exercise) price,
                                       const double& r,       // interest rate
                                       const double& sigma,   // volatility
                                       const double& time) {  // time to maturity
    double time_sqrt = sqrt(time);
    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt)+0.5*sigma*time_sqrt;
    double d2 = d1-(sigma*time_sqrt);
    return S*N(d1) - K*exp(-r*time)*N(d2);
}

double option_price_put_black_scholes(const double& S,      // spot price
                                      const double& K,      // Strike (exercise) price,
                                      const double& r,      // interest rate
                                      const double& sigma,  // volatility
                                      const double& time){
    double time_sqrt = sqrt(time);
    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1-(sigma*time_sqrt);
    return K*exp(-r*time)*N(-d2) - S*N(-d1);
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
    
    double call_price = european_call_option(0, 0);
    double put_price = european_put_option(0, 0);
    double bs_call_price = option_price_call_black_scholes(initial_stock_price, strike_price, risk_free_rate,
                                                           volatility, expiration_time);
    double bs_put_price = option_price_put_black_scholes(initial_stock_price, strike_price, risk_free_rate,
                                                         volatility, expiration_time);
    
    cout << "(Memoized) Recursive Trinomial Europeean Option Pricing" << endl;
    cout << "Expiration Time (Year) = " << expiration_time << endl;
    cout << "Number of Divisions = " << no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Up Factor = " << up_factor << endl;
    cout << "Uptick Probability = " << uptick_prob << endl;
    cout << "--------------------------------------" << endl;
    cout << "Trinomial Price of an European Call Option = " << call_price << endl;
    cout << "Call Price according to Black-Scholes = " << bs_call_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Trinomial Price of an European Put Option = " << put_price << endl;
    cout << "Put Price according to Black-Scholes = " << bs_put_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Verifying Put-Call Parity: S+P-C = Kexp(-r*T)" << endl;
    cout << initial_stock_price << " + " << put_price << " - " << call_price;
    cout << " = " << strike_price << "exp(-" << risk_free_rate << " * " << expiration_time << ")" << endl;
    cout << initial_stock_price + put_price - call_price << " = " << strike_price * exp(-risk_free_rate * expiration_time) << endl;
    cout << "--------------------------------------" << endl;
    
}
