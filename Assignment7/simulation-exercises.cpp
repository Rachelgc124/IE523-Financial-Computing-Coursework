//
//  simulation-exercise.cpp
//  Programming Assignment7: Simulation Exercises
//  Created by Chang Gao on 2022/10/31.
//

#include <iostream>
#include "simulation.h"

int main(int argc, const char * argv[]) {
    int part;
    
    cout << "Please enter the part of the assignment (for the first part, enter '1'; for the second part, enter '2'): ";
    cin >> part;
    cout << endl;
    
    simulation s;
    
    if (1 == part)
        s.optimization_part1();
    else if (2 == part)
        s.verification_part2();
    else
        cout << "Please enter the correct part number!" << endl;
    
    cout << endl;
   
}

