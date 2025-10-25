#ifndef LL_H
#define LL_H

#include <iostream>
//#include "cache.cpp"
using namespace std;

template <typename T>
struct Node{
   T data;
    struct Node* next;
public:
    Node(T val): data(val), next(nullptr){}
    Node(T val, struct Node* n): data(val) , next(n) {}
    Node(){
        next = nullptr;
    }
    ~Node(){
        
    }
};

#endif