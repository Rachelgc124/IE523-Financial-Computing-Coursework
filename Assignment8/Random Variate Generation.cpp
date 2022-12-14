//
//  Random Variate Generation.cpp
//  Programming Assignment8: Random Variate Generation
//  Created by Chang Gao on 2022/11/6.
//

#include <iostream>
#include <random>
#include <fstream>

using namespace std;

#define STANDARD_CAUCHY_DENSITY(x) 1/(3.141592654 * (1 + x * x))

unsigned seed = (unsigned) chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);

// generate variates from Standard Cauchy Distribution
double get_cauchy(double central_value, double spread)
{
    cauchy_distribution<double> distribution(central_value, spread);
    return distribution(generator);
}

// calculate the median
double median(vector<double> &list)
{
    sort(list.begin(), list.end());
    long length_of_list = list.size();
    if (1 == length_of_list)
        return list[(length_of_list - 1)/2];
    else
        return (list[length_of_list/2] + list[length_of_list/2 - 1]) / 2;
}

int main(int argc, const char * argv[])
{
    int no_of_trials, count[100];
    char file_name[10];
    
    // read data
    cout << "Please enter the number of trials: ";
    cin >> no_of_trials;
    cout << "Please enter the output file's name: ";
    cin >> file_name;
    cout << endl;
    
    cout << "Part1 & 2: Generate variates from a Standard Cauchy Distributiton";
    cout << " & experimental verification" << endl;
    cout << "Standard Cauchy RV Generator: a = 1, b = 0" << endl;
    cout << "#Trials (for Expirical-PDF) = " << no_of_trials << endl;
    cout << "Output File (Empirical-PDF) = " << file_name << endl;
    cout << endl;
    
    // initialize
    for (int i = 0; i < 100; i++) {
        count[i] = 0;
    }
    
    // Part1
    // generate experimental rv from standard cauchy distribution
    double mean = 0;
    for (int i = 0; i < no_of_trials; i++) {
        double x = get_cauchy(0, 1);
        mean += x;
        for (int j = 0; j < 100; j++) {
            if ((x >= (double) (j - 51) / 10) && (x < (double) (j - 50) / 10))
                count[j]++;
        }
    }
    mean = (double) mean / no_of_trials;
    
    // Part2
    // write data: experimental verification
    ofstream outf(file_name);
    for (int j = 0; j < 100; j++) {
        outf << ((double) (j - 50)/10) << "," << ((double) count[j]/no_of_trials) << ",";
        outf << 0.1 * STANDARD_CAUCHY_DENSITY((double) (j - 50)/10) << endl;
    }
    
    // Part3
    // compute the arithmetic mean
    cout << "Part3: compute the arithmetic mean" << endl;
    cout << "Experimentally observed arithmetic-mean of " << no_of_trials;
    cout << "-many i.i.d. Cauchy Rvs is = " << mean << endl;
    cout << endl;
    
    // Part4
    // present a table of sample runs
    cout << "Part4: present a table of sample runs" << endl;
    cout << "-----------------" << endl;
    cout << "#Trials" << "\t" << "Mean" << endl;
    
    ofstream outfile("trials-mean.csv", ios::trunc);
    outfile << "#Trials" << "," << "Mean" << endl;
    
    vector<double> list;
    for (int i = 10000; i <= 100000; i = i + 10000) {
        for (int j = i - 10000; j < i; j++)
            list.push_back(get_cauchy(0, 1));
        
        double total = accumulate(list.begin(), list.end(), 0.0);
        
        cout << i << "\t" << total /((double) list.size()) << endl;
        outfile << i << "," << total /((double) list.size()) << endl;
    }
    cout << "-----------------" << endl;
    cout << "According to the output, we can find that the mean of the Standard Cauchy Distribution ";
    cout << "doesn't converge to any value with the growing number of trials." << endl;
    cout << "According to CLT, the distribution of RV's mean should approximate to ";
    cout << "the normal distribution——if the mean exists, we should get it with large number of trials." << endl;
    cout << "Explanation: " << endl;
    cout << "Cauchy Distribution does not have a mean. (The mean does not exist.)" << endl;
    cout << endl;
    
    return 0;
}
