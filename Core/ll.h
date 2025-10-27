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

template <typename T>
struct ll{
    Node<T>* head;
public:
    ll(): head(nullptr){};
    ~ll(){
        Node<T>* itr, *temp;
        while(itr){
            temp = itr;
            itr = itr -> next;
            delete  temp;
        }
        head = nullptr;
    }
};

template <typename T> 
bool in(ll<T>* l, T val);

template <typename T>
void deleteNode(ll<T>*l, T val);

template <typename T>
void insertHeadll(ll<T>*l, T val);
#endif