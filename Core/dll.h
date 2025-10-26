#ifndef DLL_H
#define DLL_H

#include <iostream>

template <typename T>
struct node{
    T data;
    node<T>* next;
    node<T>* prev;
public: 
    node(){ 
        next = nullptr;
        prev = nullptr;
    }
    node(T val){
        data = val;
        next = nullptr;
        prev = nullptr;
    }
    ~node(){
        //delete data;
    }
};

template <typename T>
struct dll{
    node<T>* head, *tail;
    int size, capacity;
public:
    dll(){
        head = nullptr;
        tail = nullptr;
        size = 0;
        capacity = 20;
    }
    dll(int c){
        head = nullptr;
        tail = nullptr;
        size = 0;
        capacity = c;
    }

    ~dll(){
            node<T>* itr = head;
            while(itr){
                   node<T>* t = itr;
                    itr = itr -> next;
                    delete t;
            }
            head = nullptr;
            tail = nullptr;
    }
};

template <typename T>
void insertHead(dll<T>* dl, T val);

template <typename T>
void insertAtHeadFromTail(dll<T>* dl);

template <typename T>
void deleteTail(dll<T>* dl);

template <typename T>
bool isFull(dll<T>* dl);

template <typename T>
void print(dll<T>* dl);
#endif
