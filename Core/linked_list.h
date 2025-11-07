#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <functional>
//#include "cache.cpp"

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
bool in(ll<T>* l, T val){
    while(l -> head){
        if(val == l -> head -> data)
            return true;
        l -> head = l-> head ->next;
    }
    return false;
}

template <typename T>
void deleteNode(ll<T>*l, T  val){
    if(!( l -> head))
        return;

    Node<T>* tm = l -> head;
    if(l -> head -> data == val){
        l -> head = l-> head -> next;
        delete tm;
        return;
    }
    Node<T>* itr = l -> head;
    while(itr -> next){
        if(itr ->  next -> data == val){
            break;
        }
        itr = itr -> next;
    }

    if(!itr -> next)
        return;
    tm = itr -> next;
    itr -> next = tm -> next;
    delete tm;
}

template <typename T>
void insertHeadll(ll<T>*l, T val){
    Node<T>* tm = new Node<T>(val, l -> head);
    l -> head = tm;
}

#endif