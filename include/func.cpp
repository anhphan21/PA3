#include "func.h"

void print_set_cube(vector<cube> c0) {
    for (int i = 0; i < c0.size(); i++) {
        for (int j = 0; j < c0[i].size(); j++) {
            cout << c0[i][j] << " ";
        }
        if (i != c0.size()-1)
            cout << "+ ";
    }
    cout << endl;
}

//Declaration function
func::func() {
    no_input = 0;
    input_list;
    output_name;
    cube_list;
}

func::func(int no_input, string output_name, vector<cube> cube_list) {
    this->no_input = no_input;
    this->output_name = output_name;
    this->cube_list = cube_list;
    // update_co_ker();
}

func::func(int no_input, vector<int> input_list, vector<cube> cube_list) {
    this->no_input = no_input;
    this->input_list = input_list;
    this->cube_list = cube_list;
} 

void func::set_no_input(int no_input) {
    this->no_input = no_input;
}

void func::set_input(int input_idx) {
    input_list.push_back(input_idx);
    no_input++;
}

const bool func::check_input_exist_in_list(int input_idx) {
    for (int i = 0; i < input_list.size(); i++) {
      if (input_idx == input_list[i])
        return 1;  
    }
    return 0;
}

void func::set_output_name(string output_name) {
    this->output_name = output_name;
} 

void func::set_cube_list(vector<cube> update) {
    cube_list = update;
    
    vector<int> var;
    for (int i = 0; i < update.size(); i++) {
        for (int j = 0; j < update[i].size(); j++) {
            int check;
            if (update[i][j]%2)
                check = update[i][j]-1;
            else
                check = update[i][j];
            int k = 0;
            for (; k < var.size(); k++) {
                if (var[k] == check)
                    break;
            }
            if (k == var.size())
                var.push_back(check);
        }
    }
    
    input_list = var;
    no_input = var.size();
}

const int func::get_no_cube() {
    return cube_list.size();
}

void func::add_cube(cube _cube) {
    cube_list.push_back(_cube);
}

const int func::get_input_idx(int i) {
    return input_list[i];
}

const int func::get_no_input() {
    return no_input;
}

const string func::get_output_name() {
    return output_name;
}

const vector<cube> func::get_cube_list() {
    return cube_list;
}

void func::update_input(vector<int> elim, vector<int> add ) {
    for (auto i = input_list.begin(); i != input_list.end();) {
        for (auto j = elim.begin(); j != elim.end();) {
            if (*i == *j) {
                i = elim.erase(i);
                j = input_list.erase(j);
            }
            else
                j++;
        }
        if(elim.size() == 0)
            break;
    }
    for (int i = 0; i < add.size(); i++)
        input_list.push_back(add[i]);
    no_input = no_input - elim.size() + add.size();
}

//Operator function
bool operator==(const vector<cube> &f0, const vector<cube> &f1) {
    int s_f1 = f1.size();
    int s_f0 = f0.size();
    if (s_f1 != s_f0)
        return 0;
    else {
        for (int i = 0; i < s_f0; i++) {
            int j;
            for (j = 0; j < s_f1; j++)
                if (f0[i] == f1[j])
                    break;
            if (j == s_f1)
                return 0;
        }
        return 1;
    }
}

bool operator<=(const vector<cube> &f0, const vector<cube> &f1) {
    int s_f1 = f1.size();
    int s_f0 = f0.size();
    if (s_f1 > s_f0) {
        return 0;
    }
    else {
        int found = 0;
        for (int i = 0; i < s_f0; i++) {
            for (int j = 0; j < s_f1; j++)
                if (f0[i] <= f1[j])
                    found++;
            if (found == s_f1)
                return 1;
        }
        return 0;
    }
}

bool operator<=(const vector<cube> &f, const cube &c) {
    int s_f = f.size();
    for (int i = 0; i < s_f; i++) {
        if (f[i] == c)
            return 1;
    }
    return 0;
}

vector<cube> operator+(const vector<cube> &f0, const vector<cube> &f1) {
    vector<cube> temp = f0;
    for (int i = 0; i < f1.size(); i++) {
        int j;
        for (j = 0; j < temp.size(); j++) {
            if (temp[j] == f1[i])
                break;
        }
        if (j == temp.size())
            temp.push_back(f1[i]);
    }
    return temp;
}

vector<cube> operator-(const vector<cube> &f0, const vector<cube> &f1) {
    int s_f1 = f1.size();
    int s_f0 = f0.size();
    vector<cube> temp;
    if (s_f1 < s_f0) {
        if (f0<=f1) {
            for (int i = 0; i < s_f0; i++) {
                int j;
                for (j = 0; j < s_f1; j++) {
                    if (f0[i] == f1[j])
                        break;
                }
                if (j == s_f1)
                temp.push_back(f0[i]);
            }
        }
    }
    return temp;
}

vector<cube> operator*(const vector<cube> &f0, cube c0) {
    int s_f0 = f0.size();
    int s_c0 = c0.size();
    vector<cube> temp;
    for (int i = 0; i < s_f0; i++) {
        temp.push_back(f0[i]);
        for (int j = 0; j < s_c0; j++)
            temp[i] = temp[i] + c0[j];
    }
    return temp;
}

vector<cube> operator*(const vector<cube> &f0, const vector<cube> &f1) {
    int s_f0 = f0.size();
    int s_f1 = f1.size();
    vector<cube> temp; 
    for (int i = 0; i < s_f1; i++)
        temp = temp + f0 * f1[i];
    return temp;
}

func::div operator/(const vector<cube> &F, const vector<cube> &D) {
    func::div temp;
    for (int i = 0; i < D.size(); i++) {
        vector<cube> C;
        for (int j = 0; j < F.size(); j++)  {
            if (F[j] <= D[i])
                C.push_back(F[j]-D[i]);
        }
        if (!C.size()) {
            temp.R = F;
            return temp;
        }
        if (i==0)
            temp.Q = C;
        else
            temp.Q = get_intersect_cube_set(temp.Q, C);
    }
    temp.R = F - temp.Q * D;
    for (int i = 0; i < temp.Q.size(); i++) {
        if (temp.Q[i].is_one()) {
            temp.Q.clear();
            cube one;
            temp.Q.push_back(one);
        }
    }
    return temp;
}

vector<cube> operator/(const vector<cube> &f0, const cube &c) {
    vector<cube> temp;
    for (int i = 0; i < f0.size(); i++) {
        if (f0[i] <= c)
            temp.push_back(f0[i]);
    }
    
    for (int i = 0; i < temp.size(); i++) {
        temp[i] = temp[i] - c;
    }

    for (int i = 0; i < temp.size(); i++) {      //Exist case that div and get 1 in result
        if (temp[i].is_one()) {
            temp.clear();               
            cube one;
            temp.push_back(one);
        }
    }
    return temp;
}

//Kernel, co_kernel operation
vector<vector<cube>> func::get_kernel(vector<cube> f, vector<cube> &already_check, cube &pre_co) {
    vector<vector<cube>> temp;
    
    if (f.size() == 1)
        return {f};
    else {
        //Step 1: Get the common part of the cube to check cube free
        cube common = f[0];
        for (int i = 1; i < f.size(); i++)          
            common = get_common_var(common, f[i]);
        
        //Get cube free_function
        vector<cube> func_remove_common_var = f/common;                  //Obtain cube-free function from original function

        //Step 2: Get variables in the function
        cube var_in_func = func_remove_common_var[0];
        for (int i = 1; i < func_remove_common_var.size(); i++)          //Get var in func including the bar variable
            var_in_func = get_union_var_set(var_in_func, func_remove_common_var[i]);

        //Step 3: Check for each variable in function, get the kernel
        for (int i = 0; i < var_in_func.size(); i++) {
            vector<cube> check;
            
            for (int j = 0; j < func_remove_common_var.size(); j++) {   //Obtain the cube contain that variable
                if (func_remove_common_var[j]<=var_in_func[i])
                    check.push_back(func_remove_common_var[j]);
            }
            
            if (check.size() >= 2) {                        //If there are at least 2 cubes have the common variable

                cube co = check[0];
                for (int j = 0; j < check.size(); j++)
                    co = get_common_var(co, check[j]);
                
                //Check if var is already checked if true then skip
                if (already_check<=(co+pre_co))
                    continue;
                else
                    already_check.push_back(co);

                vector<cube> check_div = func_remove_common_var/co;

                temp = get_union_func_set(temp, get_kernel(func_remove_common_var/co, already_check, co));
            }
        }
        temp = get_union_func_set(temp, {func_remove_common_var});
        return temp;
    }
}

vector<cube> func::get_co_kernel(vector<cube> f,vector<vector<cube>> kernel_list) {
    vector<cube> temp;
    for (int i = 0; i < kernel_list.size(); i++) {
        div check = (f/kernel_list[i]);
        temp.push_back(check.Q[0]);
    }
    return temp;
}

void func::update_co_ker() {
    vector<cube> no_var;
    cube pre;
    kernel_list = get_kernel(cube_list, no_var, pre);
    co_kernel_list = get_co_kernel(cube_list, kernel_list);
}

const int func::get_kernel_size() {
    return kernel_list.size();
}

const vector<vector<cube>> func::get_kernel_list() {
    return kernel_list;
}

const vector<cube> func::get_kernel_at(int idx) {
    return kernel_list[idx];
}

const vector<cube> func::get_co_kernel_list() {
    return co_kernel_list;
}

const cube func::get_co_kernel_at(int idx) {
    return co_kernel_list[idx];
}

//Misc function
func::div get_div(const vector<cube> &F, const vector<cube> &D) {
    func::div temp;
    for (int i = 0; i < D.size(); i++) {
        vector<cube> C;
        for (int j = 0; j < F.size(); j++)  {
            if (F[j] <= D[i])
                C.push_back(F[j]-D[i]);
        }
        if (!C.size()) {
            temp.R = F;
            return temp;
        }
        if (i==0)
            temp.Q = C;
        else
            temp.Q = get_intersect_cube_set(temp.Q, C);
    }
    temp.R = F - temp.Q * D;
    for (int i = 0; i < temp.Q.size(); i++) {
        if (temp.Q[i].is_one()) {
            temp.Q.clear();
            cube one;
            temp.Q.push_back(one);
        }
    }
    return temp;
}

bool func_is_one(vector<cube> check) {
    for (int i = 0; i < check.size(); i++) {
        if (check[i].is_one())
            return 1;
    }
    return 0;
}

vector<cube> get_intersect_cube_set(vector<cube> f0, vector<cube> f1) {
    int s_f0 = f0.size();
    int s_f1 = f1.size();
    vector<cube> temp;
    for (int i = 0; i < s_f0; i++) {
        int j;
        for (int j = 0; j < s_f1; j++) {
            if (f0[i] == f1[j])
                temp.push_back(f0[i]);
        }
    }
    return temp;
}

vector<cube> get_union_cube_set(vector<cube> f0, vector<cube> f1) {
    vector<cube> temp = f0;
    for (int i = 0; i < f1.size(); i++) {
        int j;
        for (j = 0; j < temp.size(); j++) {
            if (temp[j] == f1[i])
                break;
        }
        if (j == temp.size())
             temp.push_back(f1[i]);
    }   
    return temp;
}

vector<vector<cube>> get_union_func_set(vector<vector<cube>> s0, vector<vector<cube>> s1){
    vector<vector<cube>> temp = s0;
    for (int i = 0; i < s1.size(); i++) {
        int j;
        for (j = 0; j < temp.size(); j++) {
            if (temp[j] == s1[i])
                break;
        }
        if (j == temp.size())
             temp.push_back(s1[i]);
    }   
    return temp;
}