#include <iostream>
using namespace std;

template <typename T>
typedef struct node{
    T data;
    struct node* next;
    struct node* prev;
public: 
    node(){
        next = nullptr;
        prev = nullptr;
    }
    node(T val){
        data = val
        next = nullptr;
        prev = nullptr;
    }
    ~node(){
        //delete data;
    }
}node;

typedef struct dll{
    node* head, *tail;
    int size, capacity;
public:
    dll(){
        head = nullptr;
        tail = nullptr;
        size = 0;
        capacity = 20;
    }

    ~dll(){
      
    }
}dll;

void insertHead(dll* dl, Tdata){
        node* temp = new node(data);
        temp -> next = dl ->  head;
        temp -> prev = nullptr;
        dl -> head = temp;
        if(dl -> tail == nullptr){
            dl -> tail = dl -> head;
        }
}

void insertAtHeadFromTail(dll* dl){
    dl -> tail -> next = dl -> head;
    dl -> head -> prev = dl -> tail;
    dl -> head = dl -> head -> prev;
    dl -> tail = dl -> tail -> prev;
    dl -> head -> prev = nullptr;
    dl -> tail -> next = nullptr;
}

void deleteTail(dll* dl){
    if(!dl -> tail){
        return;
    }
    node* temp = dl -> tail;
    dl -> tail = dl -> tail -> prev;
    delete temp;
}

bool isFull(dll* dl){
    return dl -> size == dl -> capacity;
}

int main(){
//do tests on dll for LRU
}
