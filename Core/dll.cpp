#include <iostream>
#include "cache.cpp"
using namespace std;

typedef struct node{
    CacheRequest data;
    struct node* next;
    struct node* prev;
}node;

typedef struct dll{
    node* head, node* tail;
}dll;

dll* init(CacheRequest val){
    dll *temp = new dll;
    temp -> next = NULL;
    temp -> prev = NULL:
    copy(temp, &val);
    return temp;
}
