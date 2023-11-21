#ifndef FUNC_H
#define FUNC_H

#include <algorithm>
#include "./cube.h"
using namespace std;

class func:protected cube {
public:
    //Main variable in class
    int                     no_input;
    vector<int>             input_list;     //Store idx of input in input list of the main program
    
    string                  output_name;
    
    vector<cube>            cube_list;

    //Update when complete reading all upper variables
    vector<cube>            co_kernel_list;
    vector<vector<cube>>    kernel_list;

    vector<vector<cube>>    get_kernel(vector<cube>, vector<cube>&, cube &);
    vector<cube>            get_co_kernel(vector<cube>, vector<vector<cube>>);

    //Necessary struc in class
    struct div {
        vector<cube> Q;
        vector<cube> R;
    };
    
    //Function in class
    func();
    func(int, string, vector<cube>);
    func(int, vector<int>, vector<cube>);

    void set_no_input(int);
    const int get_no_input();
    void set_input(int);
    const int get_input_idx(int);
    const bool check_input_exist_in_list(int);

    
    const string get_output_name();
    
    
    const vector<cube> get_cube_list();
    const int get_no_cube();
    void add_cube(cube);

    const int get_kernel_size();
    const vector<vector<cube>> get_kernel_list();
    const vector<cube> get_kernel_at(int);
    const vector<cube> get_co_kernel_list();
    const cube get_co_kernel_at(int);
    void update_co_ker();
    
    
    cube &operator[](int idx) {
        if ((idx < 0) || (idx >= cube_list.size())) {
            cout << "index out of bound !!!" << endl;
            return cube_list[0];
        }
        else
            return cube_list[idx];
    }

    friend bool operator==(const vector<cube> &, const vector<cube> &);
    friend bool operator<=(const vector<cube> &, const vector<cube> &);
    friend bool operator<=(const vector<cube> &, const vector<cube> &);
    friend vector<cube> operator+(const vector<cube> &, const vector<cube> &);
    friend vector<cube> operator-(const vector<cube> &, const vector<cube> &);
    friend vector<cube> operator*(const vector<cube> &, cube);
    friend vector<cube> operator*(const vector<cube> &, const vector<cube> &);
    friend div operator/(const vector<cube> &, const vector<cube> &);   //1st quotient, 2nd: remainder
    friend vector<cube> operator/(const vector<cube> &, const cube &);

    void update_input(vector<int>,vector<int>); //Update No_input, input_list
    void set_cube_list(vector<cube>);   //Set cube_list
    void set_output_name(string);           //Set output_idx
};

func::div get_div(const vector<cube> &, const vector<cube> &);

bool func_is_one(vector<cube>);
vector<cube> get_intersect_cube_set(vector<cube> , vector<cube>);
vector<cube> get_union_cube_set(vector<cube> , vector<cube>);
vector<vector<cube>> get_union_func_set(vector<vector<cube>>, vector<vector<cube>>);
#endif