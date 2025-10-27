#include <iostream>
#include <functional>
#include "ll.h"

using namespace std;
template <typename T> 
bool in(ll<T>* l, T val){
    int val1, val2;
    val2 = hash_function(val);
    while(head){
           val1 = hash_function(l -> head -> data);
            if(val1 == val2)
                return true;
            l -> head = l-> head ->next;
    }
    return false;
}

template <typename T>
void deleteNode(ll<T>*l, T  val){
    int v = hash_functiom(val);
    if(!( l -> head))
        return;

    Node* tm = l -> head;
    if(hash_function(l -> head -> data) == val){
        l -> head = l-> head -> next;
        delete tm;
        return;
    }
    Node<T>* itr = l -> head;
    while(itr -> next){
        int v1 = hash_function(itr -> next -> data);
        if(v1 == v){
            break;
        }
        itr = itr -> next;
    }

    if(!itr -> next)
        return;
    tm = itr -> next;
    itr -> next = tem -> next;
    delete tm;
}

template <typename T>
void insertHeadll(ll<T>*l, T val){
    Node<T>* tm = new node<T>(val, l -> head);
    l -> head = tm; 
}