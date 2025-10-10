#include <iostream>
#include "ll.cpp"

using namespace std;

typedef struct queue{
    Node* front;
    Node* rear;
public: 
        queue(){
            front = NULL;
            rear = NULL;
        }
}queue;

void enqueue(queue* q, CacheAddress data){
    if(q -> rear == NULL){
        q -> rear = new Node(data);
        q -> front = q -> rear;
        return;
    }
    Node* temp = new Node(data);
    q -> rear -> next = temp;
    q -> rear = temp;
}

void dequeue(queue* q){
    if(q -> rear == NULL || q -> front == NULL){
        return;
    }
    Node* temp = q -> front;
    q-> front = temp -> next;
    delete temp;
}
