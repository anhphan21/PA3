#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class cube {
public:
    vector<int> list_var;

    cube();
    cube(vector<int>);
    int size() const;
    auto begin() const;
    auto end() const;
    auto erase(auto);
    void push_back(int);

    const void print_cube() {
        for (int i = 0; i < list_var.size(); i++) {
            cout << list_var[i] << " ";
        }
    }

    const bool is_one();

    int operator[](int idx) const {
        if ((idx < 0) || (idx >= list_var.size())) {
            cout << "index out of bound !!!" << endl;
            return list_var[0];
        }
        else
            return list_var[idx];
    }

    friend bool operator==(const cube&, const cube&);
    friend bool operator<=(const cube&, const cube&);   //Check cube contains cube
    friend bool operator<=(const cube&, int);    //Check var is in cube ?
    friend cube operator-(const cube &, const int &);
    friend cube operator-(const cube &, const cube &);
    friend cube operator+(const cube &, const int &);
    friend cube operator+(const cube &, const cube &);
};

cube get_common_var(const cube&, const cube&);
cube get_union_var_set(const cube&, const cube&);
#endif