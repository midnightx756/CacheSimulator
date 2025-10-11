#include <iostream>
#include "cache.cpp"
using namespace std;

typedef struct Node{
    CacheAddress* data;
    struct Node* next;
public:
    Node(CacheAddress val): data(&val), next(NULL){}
    Node(){
        data = nullptr;
        next = nullptr;
    }
    ~Node(){
        
    }
}Node;