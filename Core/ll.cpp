#include <iostream>
#include "cache.cpp"
using namespace std;

typedef struct Node{
    CacheRequest data;
    struct Node* next;
}Node;

Node* init(CacheRequest val){
    Node* temp = new node;
    temp -> next = NULL;
    copy(temp, val);
    return temp;
}
