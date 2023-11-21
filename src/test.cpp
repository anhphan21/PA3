#include "../include/func.h"
#include <iostream>
using namespace std;

int main() {
    cube empt;

    cube c0 = {{2, 1}};
    cube c1 = {{1, 2}};
    cube c2 = {{0, 1, 2, 1}};
    cube c3 = {{1, 2, 3}};
    cube c4 = {{4, 1}};

    vector<cube> f0 = {c0, c2, c4 , c2};


    cube c6 = get_union_var_set(c0, c4);
    cout << "Check 0: ";
    c6.print_cube();

    cout << endl;
    cout << "1 check: " << empt.is_one() << endl;
    cout << "1 check: " << c0.is_one() << endl;
    
    cube c5 = c2 - 0;
    cout << "Check -: ";
    c5.print_cube();
    cout << endl;

    cube c7 = c2 - c0;
    cout << "Check -: ";
    c7.print_cube();
    cout << endl;

    cout << "Check 2: " << (c2<=c4) << endl;
    cout << "Check 3: " << (c2<=c0) << endl;
    cout << "Check 4: " << (c1==c0) << endl;

    
    // vector<cube> f1 = f0/c0;
    cout << "Check /: ";

    return 0;
}