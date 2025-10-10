#include <iostream>
#include "cache.cpp"
using namespace std;

typedef struct node{
    CacheAddress* data;
    struct node* next;
    struct node* prev;
public: 
    node(){
        data = new  CacheAddress();
        next = NULL;
        prev = NULL;
    }
    node(CacheAddress val){
        data = &val;
        next = NULL;
        prev = NULL;
    }
    ~node(){
        delete data;
    }
}node;

typedef struct dll{
    node* head, *tail;
public:
    dll(){
        head = NULL;
        tail = NULL;
    }

    ~dll(){
      
    }
}dll;

void insertHead(dll* dl, CacheAddress data){
        node* temp = new node(data);
        temp -> next = dl ->  head;
        temp -> prev = NULL;
        dl -> head = temp;
        if(dl -> tail == NULL){
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
