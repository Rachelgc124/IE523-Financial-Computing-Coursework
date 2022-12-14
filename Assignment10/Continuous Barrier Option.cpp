//
//  Continuous Barrier Option.cpp
//  Programming Assignment 10: First Part-Pricing European Down-and-Out Continuous Barrier Options
//  Created by Chang Gao on 2022/11/29.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <random>
#include <chrono>

using namespace std;

double up_factor, uptick_prob, risk_free_rate, strike_price, barrier_price;
double initial_stock_price, expiration_time, volatility, R;
int no_of_trials, no_of_divisions;
double **call_cache;
double **put_cache;
double simulate_call_option_price = 0.0;
double simulate_put_option_price = 0.0;
double adj_call_option_price = 0.0;
double adj_put_option_price = 0.0;

unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator;

double max(double a, double b)
{
    return (b < a)? a:b;
}

double compare_to_barrier(double stock_price)
{
    return (stock_price > barrier_price)? stock_price:0.0;
}

double pc(double stock_price)
{
    if (initial_stock_price <= barrier_price || stock_price <= barrier_price)
        return 1.0;
    
    double pc = exp((-2.0 / (volatility * volatility * expiration_time)) *
                    log(initial_stock_price / barrier_price) *
                    log(stock_price / barrier_price));
    return pc;
}

double get_uniform()
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double number = distribution(generator);
    return number;
}

void simulate()
{
    // chop expiration time into no_of_divisions many segments
    // figure out the motion within each segment
    double delta_T = expiration_time/((double) no_of_divisions);
    double delta_R = (risk_free_rate - 0.5*pow(volatility, 2))*delta_T;
    double delta_SD = volatility*sqrt(delta_T);

    for (int i = 0; i < no_of_trials; i++) {
        // by sharing random variables we create 4 paths
        double current_stock_price1 = initial_stock_price;
        double current_stock_price2 = initial_stock_price;
        double current_stock_price3 = initial_stock_price;
        double current_stock_price4 = initial_stock_price;
        
        double adj_s1 = initial_stock_price;
        double adj_s2 = initial_stock_price;
        double adj_s3 = initial_stock_price;
        double adj_s4 = initial_stock_price;
        for (int j = 0; j < no_of_divisions; j++)
        {
            // create the unit normal variates using the Box-Muller Transform
            double x = get_uniform();
            double y = get_uniform();
            double a =  sqrt(-2.0*log(x)) * cos(6.283185307999998*y);
            double b =  sqrt(-2.0*log(x)) * sin(6.283185307999998*y);
            
            current_stock_price1 = compare_to_barrier(current_stock_price1 * exp(delta_R + delta_SD*a));
            current_stock_price2 = compare_to_barrier(current_stock_price2 * exp(delta_R - delta_SD*a));
            current_stock_price3 = compare_to_barrier(current_stock_price3 * exp(delta_R + delta_SD*b));
            current_stock_price4 = compare_to_barrier(current_stock_price4 * exp(delta_R - delta_SD*b));
            
            adj_s1 = adj_s1*exp(delta_R + delta_SD*a);
            adj_s2 = adj_s2*exp(delta_R - delta_SD*a);
            adj_s3 = adj_s3*exp(delta_R + delta_SD*b);
            adj_s4 = adj_s4*exp(delta_R - delta_SD*b);
        }
        
        simulate_call_option_price += (max(0.0, current_stock_price1 - strike_price)
                              + max(0.0, current_stock_price2 - strike_price)
                              + max(0.0, current_stock_price3 - strike_price)
                              + max(0.0, current_stock_price4 - strike_price)) / 4.;
        simulate_put_option_price += (max(0.0, strike_price - current_stock_price1)
                             + max(0.0, strike_price - current_stock_price2)
                             + max(0.0, strike_price - current_stock_price3)
                             + max(0.0, strike_price - current_stock_price4)) / 4.;
        
        adj_call_option_price += ((1 - pc(adj_s1)) * max(0.0, adj_s1 - strike_price)
                                  + (1 - pc(adj_s2)) * max(0.0, adj_s2 - strike_price)
                                  + (1 - pc(adj_s3)) * max(0.0, adj_s3 - strike_price)
                                  + (1 - pc(adj_s4)) * max(0.0, adj_s4 - strike_price)) / 4.0;
        adj_put_option_price += ((1 - pc(adj_s1)) * max(0.0, strike_price - adj_s1)
                                 + (1 - pc(adj_s2)) * max(0.0, strike_price - adj_s2)
                                 + (1 - pc(adj_s3)) * max(0.0, strike_price - adj_s3)
                                 + (1 - pc(adj_s4)) * max(0.0, strike_price - adj_s4)) / 4.0;
    }
    
    simulate_call_option_price = simulate_call_option_price / (no_of_trials * pow(R, no_of_divisions));
    simulate_put_option_price = simulate_put_option_price / (no_of_trials * pow(R, no_of_divisions));
    adj_call_option_price = adj_call_option_price / (no_of_trials * pow(R, no_of_divisions));
    adj_put_option_price = adj_put_option_price / (no_of_trials * pow(R, no_of_divisions));
}

void initial_caches()
{
    call_cache = new double *[no_of_divisions + 1];
    put_cache = new double *[no_of_divisions + 1];
    
    for (int i = 0; i <= no_of_divisions; i++)
    {
        call_cache[i] = new double[i + 1];
        put_cache[i] = new double[i + 1];
    }
    
    for (int i = 0; i <= no_of_divisions; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            call_cache[i][j] = -1;
            put_cache[i][j] = -1;
        }
    }
}

double european_down_and_out_call_option(int k, int i)
{
    if (-1 == call_cache[k][(i+k)/2]) {
        if (k == no_of_divisions) {
            if (initial_stock_price * pow(up_factor, ((double) i)) > barrier_price) {
                double pb = exp((-2.0 / (volatility * volatility * expiration_time)) *
                                log(initial_stock_price / barrier_price) *
                                log(initial_stock_price * pow(up_factor, (double) i) / barrier_price));
                call_cache[k][(i+k)/2] = (1 - pb) * max(0.0, initial_stock_price * pow(up_factor, (double) i) - strike_price);
            }
            else
                call_cache[k][(i+k)/2] = 0.0;
            
            return call_cache[k][(i+k)/2];
        }
        
        call_cache[k][(i+k)/2] = (uptick_prob * european_down_and_out_call_option(k+1, i+1)
                                  + (1 - uptick_prob) * european_down_and_out_call_option(k+1, i-1)) / R;
    }
    
    return call_cache[k][(i+k)/2];
}

double european_down_and_out_put_option(int k, int i)
{
    if (-1 == put_cache[k][(i+k)/2]) {
        if (k == no_of_divisions) {
            if (initial_stock_price * pow(up_factor, ((double) i)) > barrier_price) {
                double pb = exp((-2.0 / (volatility * volatility * expiration_time)) *
                                log(initial_stock_price / barrier_price) *
                                log(initial_stock_price * pow(up_factor, (double) i) / barrier_price));
                put_cache[k][(i+k)/2] = (1 - pb) * max(0.0, strike_price - initial_stock_price * pow(up_factor, (double) i));
            }
            else
                put_cache[k][(i+k)/2] = 0.0;
            
            return put_cache[k][(i+k)/2];
        }
        
        put_cache[k][(i+k)/2] = (uptick_prob * european_down_and_out_put_option(k+1, i+1)
                                 + (1 - uptick_prob) * european_down_and_out_put_option(k+1, i-1)) / R;
    }
    
    return put_cache[k][(i+k)/2];
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
    double a = fabs(z);
    double t = 1.0/(1.0+a*p);
    double b = c2*exp((-z)*(z/2.0));
    double n = ((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t;
    n = 1.0-b*n;
    if ( z < 0.0 ) n = 1.0 - n;
    return n;
};

double option_price_put_black_scholes(const double& S,      // spot price
                                      const double& K,      // Strike (exercise) price,
                                      const double& r,      // interest rate
                                      const double& sigma,  // volatility
                                      const double& time)
{
    double time_sqrt = sqrt(time);
    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1-(sigma*time_sqrt);
    return K*exp(-r*time)*N(-d2) - S*N(-d1);
};

double option_price_call_black_scholes(const double& S,       // spot (underlying) price
                                       const double& K,       // strike (exercise) price,
                                       const double& r,       // interest rate
                                       const double& sigma,   // volatility
                                       const double& time)      // time to maturity
{
    double time_sqrt = sqrt(time);
    double d1 = (log(S/K)+r*time)/(sigma*time_sqrt)+0.5*sigma*time_sqrt;
    double d2 = d1-(sigma*time_sqrt);
    return S*N(d1) - K*exp(-r*time)*N(d2);
};

double closed_form_down_and_out_european_call_option()
{
    float K = (2*risk_free_rate)/(volatility*volatility);
    float A = option_price_call_black_scholes(initial_stock_price, strike_price,
                                              risk_free_rate, volatility, expiration_time);
    float B = (barrier_price*barrier_price)/initial_stock_price;
    float C = pow(initial_stock_price/barrier_price, -(K-1));
    float D = option_price_call_black_scholes(B, strike_price, risk_free_rate, volatility, expiration_time);
    return (A - D*C);
}

float closed_form_down_and_in_european_put_option()
{
    // just making it easier by renaming the global variables locally
    float S = initial_stock_price;
    float r = risk_free_rate;
    float T = expiration_time;
    float sigma = volatility;
    float H = barrier_price;
    float X = strike_price;
    
    // Took these formulae from some online reference
    float lambda = (r+((sigma*sigma)/2))/(sigma*sigma);
    float temp = 2*lambda - 2.0;
    float x1 = (log(S/H)/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
    float y = (log(H*H/(S*X))/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
    float y1 = (log(H/S)/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
    return (-S*N(-x1) + X*exp(-r*T)*N(-x1 + sigma*sqrt(T)) +
            S*pow(H/S, 2*lambda)*(N(y)-N(y1)) -
            X*exp(-r*T)*pow(H/S, temp)*(N(y-sigma*sqrt(T))-N(y1-sigma*sqrt(T))));
}

float closed_form_down_and_out_european_put_option()
{
    float vanilla_put = option_price_put_black_scholes(initial_stock_price, strike_price,
                                                       risk_free_rate, volatility, expiration_time);
    float put_down_in = closed_form_down_and_in_european_put_option();
    return (vanilla_put - put_down_in);
}

int main(int argc, const char * argv[]) {
    
    sscanf(argv[1], "%lf", &expiration_time);
    sscanf(argv[2], "%lf", &risk_free_rate);
    sscanf(argv[3], "%lf", &volatility);
    sscanf(argv[4], "%lf", &initial_stock_price);
    sscanf(argv[5], "%lf", &strike_price);
    sscanf(argv[6], "%d", &no_of_trials);
    sscanf(argv[7], "%d", &no_of_divisions);
    sscanf(argv[8], "%lf", &barrier_price);
    
    up_factor = exp(volatility * sqrt(expiration_time/(float) no_of_divisions));
    R = exp(risk_free_rate * expiration_time / (float) no_of_divisions);
    uptick_prob = (R - (1/up_factor)) / (up_factor - (1/up_factor));
    
    simulate();
    
    cout << "--------------------------------------" << endl;
    cout << "European Down-and-Out Continuous Barrier Options Pricing via Monte Carlo Simulation" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "Barrier Price = " << barrier_price << endl;
    cout << "Number of Tials = " << no_of_trials << endl;
    cout << "Number of Divisions = " << no_of_divisions << endl;
    cout << "--------------------------------------" << endl;
    cout << "--------------------------------------" << endl;
    cout << "The average Call Price by explicit simulation  = " << simulate_call_option_price << endl;
    cout << "The call price using the (1-p)-adjustment term = " << adj_call_option_price << endl;
    cout << "Theoretical Call Price                         = " << closed_form_down_and_out_european_call_option() << endl;
    cout << "--------------------------------------" << endl;
    cout << "The average Put Price by explicit simulation  = " << simulate_put_option_price << endl;
    cout << "The put price using the (1-p)-adjustment term = " << adj_put_option_price << endl;
    cout << "Theoretical Put Price                         = " << closed_form_down_and_out_european_put_option() << endl;
    cout << "--------------------------------------" << endl;
    
}
