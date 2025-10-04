#include <iostream>

using namespace std;

typedef struct node{
    int data;
    node* next;
}node;


class HashMap{
    int size;
    node** table;
public:
    HashMap(int size){
        this -> size = size;
        table = new node*[size];
        for(int i=0; i<size; i++){
            table[i] = nullptr;
        }
    }

    int hashFunction(const string& key){
        int hash = 0;
        for(char c : key){
            hash += c;
        }

        return hash % size;
    }
}
