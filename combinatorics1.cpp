//
//  combinatorics1.cpp
//  Programming Assignment1: Combinatorics
//
//  Created by Chang Gao on 2022/9/6.
//

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    
    int size_of_array, number_of_objects_to_be_picked;
    int *array;
    
    cout << "Array Size? ";
    cin >> size_of_array;
    cout << "#Objects to be picked from array? ";
    cin >> number_of_objects_to_be_picked;
    
    // Just fill the array with integers
    array = new int[size_of_array];
    for (int i = 0; i < size_of_array; i++)
        array[i] = i + 1;
    
    // Create a boolean array where the last n-r items are "true"
    // the first r items are "false"
    vector <bool> did_not_pick_this(size_of_array);
    fill(did_not_pick_this.begin() + number_of_objects_to_be_picked, did_not_pick_this.end(), true);
    
    // Use next_permutation to generate the required combinatorial outputs
    do {
        cout << "{ ";
        for (int i = 0; i < size_of_array; i++) {
            if (!did_not_pick_this[i])
                cout << array[i] << " ";
        }
        cout << "}" << endl;
    } while (next_permutation(did_not_pick_this.begin(), did_not_pick_this.end()));
    
    return 0;
}
