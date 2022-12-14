//
//  main.cpp
//  Programming Assignment1: Combinatorics2
//
//  Created by Chang Gao on 2022/9/6.
//

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    
    int size_of_array;
    int *array;
    
    cout << "Array Size? ";
    cin >> size_of_array;
    
    // Just fill the array with integers
    array = new int[size_of_array];
    for (int i = 0; i < size_of_array; i++)
        array[i] = i + 1;
    
    // Create a boolean array which has the same number of elements of A
    vector <bool> did_i_pick_this(size_of_array);
    
    // Traverse the valid number of elements and pick elements up from A
    // Use prev_permutation to generate the required output
    for (int i = 0; i <= size_of_array; i++) {
        fill(did_i_pick_this.begin(), did_i_pick_this.begin() + i, true);
        do {
            cout << "{ ";
            for (int j = 0; j < size_of_array; j++) {
                if (did_i_pick_this[j])
                    cout << array[j] << " ";
            }
            cout << "}" << endl;
            
        } while (prev_permutation(did_i_pick_this.begin(), did_i_pick_this.end()));
        
    }
    return 0;
}
