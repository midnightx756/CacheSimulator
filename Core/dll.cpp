#include <iostream>
#include "cache.cpp"
using namespace std;

typedef struct node{
    CacheAddress* data;
    struct node* next;
    struct node* prev;
public: 
    node(){
        data = nullptr;
        next = nullptr;
        prev = nullptr;
    }
    node(CacheAddress val){
        data = &val;
        next = nullptr;
        prev = nullptr;
    }
    ~node(){
        //delete data;
    }
}node;

typedef struct dll{
    node* head, *tail;
public:
    dll(){
        head = nullptr;
        tail = nullptr;
    }

    ~dll(){
      
    }
}dll;

void insertHead(dll* dl, CacheAddress data){
        node* temp = new node(data);
        temp -> next = dl ->  head;
        temp -> prev = nullptr;
        dl -> head = temp;
        if(dl -> tail == nullptr){
            dl -> tail = dl -> head;
        }
}

void deleteTail(dll* dl){
    if(!dl -> tail){
        return;
    }
    node* temp = dl -> tail;
    dl -> tail = dl -> tail -> prev;
    delete temp;
}
