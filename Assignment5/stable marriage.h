//
//  stable marrage.h
//  Programming Assignment5: The Stable Marriage Problem
//  Created by Chang Gao on 2022/10/13.
//

#ifndef stable_marriage_h
#define stable_marriage_h

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class stable_marriage_instance
{
    // Private
    char file_name[10];
    int no_of_couples;
    vector <vector <int>> Preference_of_men;
    vector <vector <int>> Preference_of_women;
    vector <int> match_for_men;
    vector <int> match_for_women;
    
    // private member function: checks if anybody is free in boolean "my_array"
    // returns the index of the first-person who is free in "my_array"
    // if no one is free it returns a -1.
    int anybody_free(vector <bool> my_array)
    {
        // the "true" in my_array means the person is free
        // the "false" in my_array means the person is not free
        for (int i = 0; i < my_array.size(); i++)
        {
            if (my_array[i])
                return i;
        }
        return -1;
    }
    
    // private member function: if index1 is ranked higher than index2
    // in a preference array called "my_array" it returns "true"; otherwise
    // it returns "false"
    bool rank_check(vector <int> my_array, int index1, int index2)
    {
        vector<int>::iterator it1 = find(my_array.begin(), my_array.end(), index1);
        vector<int>::iterator it2 = find(my_array.begin(), my_array.end(), index2);
        if (distance(it1, it2) > 0)
            return true;
        return false;
    }
    
    // private member function: implements the Gale-Shapley algorithm
    void Gale_Shapley()
    {
        vector <bool> is_man_free;
        vector <bool> is_woman_free;
        vector <vector <bool>> has_this_man_proposed_to_this_woman;
        
        int man_index, woman_index = 0;
        
        // initializing everything
        for (int i = 0; i < no_of_couples; i++)
        {
            is_man_free.push_back(true);
            is_woman_free.push_back(true);
            match_for_men.push_back(-1);
            match_for_women.push_back(-1);
            vector <bool> temp;
            has_this_man_proposed_to_this_woman.push_back(temp);
            for (int j = 0; j < no_of_couples; j++)
                has_this_man_proposed_to_this_woman[i].push_back(false);
        }
        
        // Gale-Shapley Algorithm
        while ((man_index = anybody_free(is_man_free)) >= 0)
        {
            for (int i = 0; i < no_of_couples; i++)
            {
                woman_index = Preference_of_men[man_index][i];
                if (!has_this_man_proposed_to_this_woman[man_index][woman_index])
                    break;
            }
            
            if (is_woman_free.at(woman_index))
            {
                match_for_men.at(man_index) = woman_index;
                match_for_women.at(woman_index) = man_index;
                is_man_free.at(man_index) = false;
                is_woman_free.at(woman_index) = false;
            }
            else
            {
                if (rank_check(Preference_of_women[woman_index], man_index, match_for_women.at(woman_index)))
                {
                    match_for_men.at(man_index) = woman_index;
                    is_man_free.at(man_index) = false;
                    match_for_men.at(match_for_women.at(woman_index)) = -1;
                    is_man_free.at(match_for_women.at(woman_index)) = true;
                    match_for_women.at(woman_index) = man_index;
                    is_woman_free.at(woman_index) = false;
                }
            }
            
            has_this_man_proposed_to_this_woman[man_index][woman_index] = true;
        }
    }
    
    // private member function: reads data
    void read_data(int argc, const char * argv[])
    {
        // The first entry in the input file is the #couples, 
        // followed by the preferences of the men
        // and preferences of the women.
        int value_just_read_from_file;
        vector <int> P;
        
        sscanf(argv[1], "%s", file_name);
        
        ifstream input_file(file_name);
        
        if (input_file.is_open())
        {
            while (input_file >> value_just_read_from_file)
                P.push_back(value_just_read_from_file);
        }
        else
            cout << "Input file does not exist in PWD." << endl;
        
        no_of_couples = P[0];
        
        for (int i = 0; i < no_of_couples; i++)
        {
            vector <int> temp1;
            vector <int> temp2;
            Preference_of_men.push_back(temp1);
            Preference_of_women.push_back(temp2);
        }
        
        int n = no_of_couples * no_of_couples;
        for (int i = 1; i <= n; i++)
            Preference_of_men[(i - 1) / no_of_couples].push_back(P[i]);
        for (int i = n + 1; i <= n + n; i++)
            Preference_of_women[(i - n - 1) / no_of_couples].push_back(P[i]);
    }
    
    // private member function: print solution
    void print_soln()
    {
        // write the appropriate code here
        cout << "Gale-Shapley Algorithm" << endl;
        cout << "Input File Name: " << file_name << endl;
        cout << "Number of couples = " << no_of_couples << endl;
        cout << endl;
        
        string bar(18, '-');
        cout << "Preferences of Men" << endl;
        cout << bar << endl;
        for (int i = 0; i < no_of_couples; i++)
        {
            cout << "(" << i << "): ";
            for (int j = 0; j < no_of_couples; j++)
                cout << Preference_of_men[i][j] << " ";
            cout << endl;
        }
        cout << endl;
        
        cout << "Preferences of Women" << endl;
        cout << bar << endl;
        for (int i = 0; i < no_of_couples; i++)
        {
            cout << "(" << i << "): ";
            for (int j = 0; j < no_of_couples; j++)
                cout << Preference_of_women[i][j] << " ";
            cout << endl;
        }
        cout << endl;
        
        cout << "Matching for Men" << endl;
        for (int i = 0; i < no_of_couples; i++)
        {
            cout << "Man: " << i << " -> Woman: " << match_for_men.at(i) << endl;
        }
        cout << endl;
        
        cout << "Matching for Women" << endl;
        for (int i = 0; i < no_of_couples; i++)
        {
            cout << "Woman: " << i << " -> Man: " << match_for_women.at(i) << endl;
        }
        cout << endl;
    }
    
public:
    void solve_it(int argc, const char * argv[])
    {
        read_data(argc, argv);

        Gale_Shapley();

        print_soln();
    }
};

#endif /* stable_marriage_h */
