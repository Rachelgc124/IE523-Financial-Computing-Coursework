//
//  simulation.h
//  Programming Assignment7: Simulation Exercises
//  Created by Chang Gao on 2022/10/31.
//

#ifndef simulation_h
#define simulation_h

#include <vector>
#include <math.h>
#include <random>
#include <fstream>

using namespace std;

class simulation
{
    // private
    // the probability of heads for Alice's coin is q,
    // and that for Bob's coin is p; q < p
    char file_name[10];
    double q;
    double p;
    int no_of_trials;
    
    // combination
    int C(int n, int m)
    {
        if (n == m || 0 == m)
            return 1;
        
        int com = 1;
        m = min(m, n - m);
        for (int i = 1; i <= m; i++)
            com = com * (n - i + 1) / i;
        
        return com;
    }
    
    // Alice's chances of winning
    double f(int n)
    {
        double probA_win = 0;
        for (int r = 0; r <= n; r++)
        {
            double probA = 0;
            for (int s = r + 1; s <= n; s++)
                probA = probA + C(n, s) * pow(q, s) * pow(1 - q, n - s);
            
            probA_win = probA_win + C(n, r) * pow(p, r) * pow(1 - p, n - r) * probA;
        }
        
        return probA_win;
    }
    
    // Part1: How to loose as little as possible
    // pick the optimal value of n for values of p and q
    // (Theorem 2.2) for the f(n) is unimodality, find the peak of the f(n)'s
    int loose_alap(int n)
    {
        if (f(n) >= f(n + 1))
            return n;
        return loose_alap(n + 1);
    }
    
    // Part2: trials, get the probability of Alice's winning
    double trials_Awin(int n)
    {
        // unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator1(0);
        default_random_engine generator2(123456);
        binomial_distribution<int> distrA(n, q);
        binomial_distribution<int> distrB(n, p);
        
        int Awin = 0;
        for (int i = 0; i < no_of_trials; i++)
        {
            int headA = distrA(generator1);
            int headB = distrB(generator2);
            if (headA > headB)
                Awin = Awin + 1;
        }
        double trials_Awin = Awin * 1. / no_of_trials;
        
        return trials_Awin;
    }
    
    // Part2: The experimental Verification of Reference
    // write the experiment and theory probability into a file
    void write_file_part2(int n)
    {
        ofstream outfile(file_name);
        
        if (outfile.is_open())
        {
            for (int i = 0; i < n; i++)
                outfile << i + 1 << ", " << trials_Awin(i + 1) << ", " << f(i + 1) << endl;
        }
        outfile.close();
        
    }
    
    // Part1: read data from command line
    void read_data_part1()
    {
        cout << "Please enter the probability of head up for Alice: ";
        cin >> q;
        cout << "Please enter the probability of head up for Bob: ";
        cin >> p;
        cout << endl;
    }
    
    // Part2: read data from command line
    void read_data_part2()
    {
        cout << "Please enter the number of trials: ";
        cin >> no_of_trials;
        cout << "Please enter the probability of head up for Alice: ";
        cin >> q;
        cout << "Please enter the probability of head up for Bob: ";
        cin >> p;
        cout << "Please enter the file name (to store data): ";
        cin >> file_name;
        cout << endl;
    }
    
    // Part1: print data
    void print_data_part1()
    {
        cout << "Probability of head up for Alice = " << q << endl;
        cout << "Probability of head up for Bob = " << p << endl;
        cout << "The optimal number of coin tosses is " << loose_alap(0) << endl;
    }
    
    // Part2: print data
    void print_data_part2()
    {
        cout << "Number of Trials = " << no_of_trials << endl;
        cout << "Probability of head up for Alice = " << q << endl;
        cout << "Probability of head up for Bob = " << p << endl;
    }
    
public:
    void optimization_part1()
    {
        read_data_part1();
        print_data_part1();
    }
    
    void verification_part2()
    {
        read_data_part2();
        print_data_part2();
        write_file_part2(30);
    }
    
};

#endif /* simulation_h */
