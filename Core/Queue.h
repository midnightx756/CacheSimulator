#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "linked_list.h"


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
void enqueue(queue<T>* q, T data){
    if(q -> rear == NULL){
        q -> rear = new Node<T>(data);
        q -> front = q -> rear;
        q -> size++;
        return;
    }
    Node<T>* temp = new Node<T>(data);
    q -> rear -> next = temp;
    q -> rear = temp;
    q -> size++;
}

template <typename T>
void dequeue(queue<T>* q){
    if(q -> rear == NULL || q -> front == NULL){
        return;
    }
    Node<T>* temp = q -> front;
    q-> front = temp -> next;
    delete temp;

    q -> size--;
    if(q -> front == nullptr){
        q -> rear = nullptr;
    }
}

template <typename T>
int isFull(queue<T>* q){
    return q -> size == q -> cap;
}

template <typename T>
int Empty(queue<T>* q){
    return q -> size == q -> cap;
}

template <typename T>
T Front(queue<T>* q){
    return q -> rear -> data;
}

#endif
