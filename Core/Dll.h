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
void insertHead(dll<T>* dl, T data){
    node<T>* temp = new node<T>(data);
    temp -> next = dl -> head;
    temp -> prev = nullptr;
    if(dl -> head)
        dl -> head -> prev = temp;
    dl -> head = temp;
    dl -> size++;
    if(dl -> tail == nullptr){
        dl -> tail = dl -> head;
    }
}

template <typename T>
void insertAtHeadFromTail(dll<T>* dl){
    if(dl -> head == nullptr || dl -> tail == nullptr || dl -> head == dl -> tail)
        return;
    node<T>* old_tail = dl->tail;
    node<T>* new_tail = old_tail->prev; // This is the node *before* the current tail

    // 1. Update the list's tail pointer and break the link after it
    dl->tail = new_tail;
    dl->tail->next = nullptr;

    // 2. Link the old tail (which will be the new head) to the old head
    old_tail->next = dl->head;
    old_tail->prev = nullptr; // New head's prev is nullptr

    // 3. Link the old head's prev to the new head
    dl->head->prev = old_tail;

    // 4. Update the list's head pointer
    dl->head = old_tail;
}

template <typename T>
void deleteTail(dll<T>* dl){
    if(!dl -> tail || !dl -> head || !dl){
        return;
    }
    node<T>* temp = dl -> tail;
    if(dl -> head == dl -> tail){
        dl-> head = nullptr;
        dl -> tail = nullptr;
    }
    else{
        dl -> tail = dl -> tail -> prev;
        dl -> tail -> next = nullptr;
    }
    delete temp;
    dl -> size--;
}

template <typename T>
bool isFull(dll<T>* dl){
    return dl -> size == dl -> capacity;
}

template <typename T>
void print(dll<T>* dl){
    if(dl -> size == 0 || !dl || !dl -> head)
        return;
    node<T>* itr = dl -> head;
    cout << "null";
    while(itr){
        cout << "<------>[" <<  itr -> prev <<"|"<< itr -> data <<"|" <<  itr -> next << "]";
        itr = itr -> next;
    }
    cout << "<------>null\n";
}

#endif
