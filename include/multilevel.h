#ifndef MULTILEVEL_H
#define MULTILEVEL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include "./func.h"
using namespace std;

class multilevel:func{
private:
    bool debug = 1;                     //1 for debugging mode

    string          module_name;
    vector<string>  input_list;
    vector<int>     var_list;           //even idx: normal var, odd_idx: bar var
    vector<string>  output_list;

    vector<func>    func_list;

    vector<func>    synth_func_list;
    vector<string>  synth_var_list;     //idx = input_lis.size()*2+idx
public:
    multilevel();
    ~multilevel();
 
    void	setModuleName(string);
    string 	getModuleName();
    void 	setMode(string);

    void            add_input_var(string);
    const int       get_input_idx(string);

    void            add_output_var(string);
    const int       get_output_idx(string);
    const string    get_output_name(int);

    const string    get_var_name(int);

    void read_blif_file(string);
    void write_blif_file();

    void test_co_ker();
    void print_cube_set_with_name(vector<cube>);
    void print_cube(cube);

    int add_new_gate(vector<cube>);
    
    // Upper done
    void single_extraction();
    void decompose(func &);

    void test_decompose();

    int cal_literal();
};

#endif