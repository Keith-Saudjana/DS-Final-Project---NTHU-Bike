#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Pseudo-Vector Implementation
template<typename T>
class vector{
    public:
    T *a;
    int cur_size;
    int max_size;
    
    vector(){
        this->cur_size = 0;
        this->max_size = 8;
        a = new T[8];
    }

    ~vector(){delete a;}

    T& operator[](int idx) {return a[idx];}

    const T& operator[](int idx) const {return a[idx];}

    void push_back(T val){
        a[cur_size] =  val;
        cur_size ++;
        if(cur_size == max_size){
            T *new_a = new T[max_size*2];
            for(int i = 0; i < cur_size; i ++) new_a[i] = a[i];
            max_size *= 2;
            delete a;
            a = new_a;
        }
    }

    void pop_back(){
        cur_size--;
    }
};

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& rhs) {
    for(int i = 0; i < rhs.cur_size; i++) cout << rhs.a[i] << endl;
    return os;
}

#endif