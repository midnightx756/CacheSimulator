#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "ll.h"

using namespace std;

template <typename T>
struct queue{
    Node <T>* front;
    Node <T>* rear;
    int size, cap;
public:
    queue(int &s){
        front = nullptr;
        rear = nullptr;
        size = 0;
        cap = s;
    }
    queue(){
        front = nullptr;
        rear = nullptr;
        size = 0;
        cap = 20;
    }
    ~queue(){
        Node<T>* itr = front, *tm;
        while(itr){
            tm = itr;
            itr = itr -> next;
            delete tm;
        }
        front = nullptr;
        rear = nullptr;
    }
};

template <typename T>
void enqueue(queue<T>* q, T data);

template <typename T>
void dequeue(queue<T>* q);

template <typename T>
int isFull(queue<T>* q);

template <typename T>
int Empty(queue<T>* q);

template <typename T>
T  Front(queue<T>* q);
#endif
