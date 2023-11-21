#include "./cube.h"

cube::cube() {
    list_var;
}

cube::cube(vector<int> list_var) {
    this->list_var = list_var;
}

const int cube::size() {
    return list_var.size();
}

const auto cube::begin() {
    return list_var.begin();
}

const auto cube::end() {
    return list_var.end();
}

auto cube::erase(auto i) {
    return list_var.erase(i);
}

void cube::push_back(int t) {
    list_var.push_back(t);
}

const bool cube::is_one() {
    return !(list_var.size());
}

bool operator==(cube c0, cube c1) {
    int s_c1 = c1.size();
    int s_c0 = c0.size();
    if (s_c1 != s_c0)
        return 0;
    else {
        for (int i = 0; i < s_c0; i++) {
            int j;
            for (j = 0; j < s_c1; j++)
                if (c0[i] == c1[j])
                    break;
            if (j == s_c1)
                return 0;
        }
        return 1;
    }
}

bool operator<=(cube c0, cube c1) {
    int s_c1 = c1.size();
    int s_c0 = c0.size();
    if (s_c1 > s_c0)
        return 0;
    else {
        int found = 0;
        for (int i = 0; i < s_c0; i++) {
            for (int j = 0; j < s_c1; j++)
                if (c0[i] == c1[j])
                    found++;
            if (found == s_c1)
                return 1;
        }
        return 0;
    }
}

bool operator<=(cube c,int v) {
    int s_c = c.size();
    for (int i = 0; i < s_c; i++) {
        if (c[i] == v)
            return 1;
    }
    return 0;
}

cube operator-(const cube &c0, const int &v0) {
    cube temp = c0;
    for (int i = 0; i < temp.size(); i++) {
        if (temp.list_var[i] == v0)
            temp.list_var.erase(temp.list_var.begin()+i);
    }
    return temp;
}

cube operator-(const cube &c0, const cube &c1)  {
    cube t0 = c0;
    cube t1 = c1;
    for (int i = 0; i < t1.size(); i++)
        t0 = t0 - t1[i];
    return t0;
}

cube operator+(const cube &c0, const int &v0) {
    cube temp = c0;
    for (int i = 0; i < temp.list_var.size(); i++) {
        if(temp[i] == v0)
            return temp;
    }
    temp.list_var.push_back(v0);
    return temp;
}

cube operator+(const cube &c0, const cube &c1) {
    cube t0 = c0;
    cube t1 = c1;
    for (int i = 0; i < t1.size(); i++) {
        t0 = t0 + t1[i];
    }
    return t0;
}

cube get_common_var(cube c0, cube c1) {
    cube common;
    for (auto i = c0.begin(); i != c0.end();) {
        for (auto j = c1.begin(); j != c1.end();) {
            if (*i == *j) {
                common.push_back(*i);
                break;
            }
            else
                j++;
        }
        i++;
    }
    return common;
}

cube get_union_var_set(cube c0, cube c1) {
    cube temp = c0;
    for (int i = 0; i < c1.size(); i++) {
        int j;
        for (j = 0; j < temp.size(); j++) {
            if (temp[j] == c1[i])
                break;
        }
        if (j == temp.size())
             temp.push_back(c1[i]);
    }   
    return temp;
}
