#include <iostream>
#include "queue.h"
/*
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
*/
template <typename T>
void enqueue(queue<T>* q, T data){
    if(q -> rear == NULL){
        q -> rear = new Node(data);
        q -> front = q -> rear;
        return;
    }
    Node<T>* temp = new Node(data);
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

int main(){
    queue<int>* q = new queue<int>;
    enqueue(q, 90);
    enqueue(q, 80);
    enqueue(q, 82);
    int i = 0;
    cout << isFull(q);
    while(isFull(q)){
        enqueue(q, i);
        i+=22;
    }
    dequeue(q);
    cout << Front(q) << "\n";
    dequeue(q);
    cout << Front(q) << "\n";
    while(!Empty(q)){
        enqueue(q, i);
        i+=22;
    }
    delete q;
}
