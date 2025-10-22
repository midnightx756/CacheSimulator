#include <iostream>
#include "ll.cpp"

using namespace std;

template <typename T>
typedef struct queue{
    Node <T>* front;
    Node <T>* rear;
    int size, cap;
public: 
        queue(){
            front = NULL;
            rear = NULL;
            size = 0;
            cap = 20;
        }
}queue;

void enqueue(queue* q, T data){
    if(q -> rear == NULL){
        q -> rear = new Node(data);
        q -> front = q -> rear;
        return;
    }
    Node* temp = new Node(data);
    q -> rear -> next = temp;
    q -> rear = temp;
    q -> size++;
}

void dequeue(queue* q){
    if(q -> rear == NULL || q -> front == NULL){
        return;
    }
    Node* temp = q -> front;
    q-> front = temp -> next;
    delete temp;

    q -> size++;
}

int isFull(queue* q){
    return q -> size == q -> cap;
}

int main(){
    queue* q = new queue;
    delete q;
}
