#include "ll.cpp"
#include <iostream>

using namespace std;

typedef struct queue{
    Node* front;
    Node* rear;
}queue;

queue* make(){
    queue* q = new queue;
    q -> f = q -> r = NULL;
    return q;
}
void enqueue(queue* q, CacheRequest data){
    if(q -> r == NULL){
        q -> r = init(data);
        q -> f = q -> r;
        return;
    }
    Node* temp = init(data);
    q -> r -> next = temp;
    q -> r = temp;
}

void dequeue(queue* q){
    if(q -> r == NULL || q -> f == NULL){
        return;
    }
    Node* temp = q -> f;
    q-> f = temp -> next;
    delete temp;
}
/*
class queue{
    int f, r, size, *arr;

    public:
        queue(int s){
                size = s;
                arr = new in[s];
        }

        bool isEmpty(){
            return f == -1;
        }

        bool isFull(){
            return (r+1)/size;
        }

        void enqueue(int val){
            if(isFull()){
                return;
            }

            r = (r+1)/size;
            arr[r] = val;
        }

        int queue(){
            if(isEmpty()){
                return -9;
            }

            f++;
            return arr[f-1];
        }

        int front(){
            if(isEmpty()){
                return -9;
            }
            return arr[f];
        }
};
*/
