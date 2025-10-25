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
    queue(){
        front = nullptr;
        rear = nullptr;
        size = 0;
        cap = 20;
    }
    queue(int s){
        front = nullptr;
        rear = nullptr;
        size = 0;
        cap = s;
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
T Front(queue<T>* q);
#endif
