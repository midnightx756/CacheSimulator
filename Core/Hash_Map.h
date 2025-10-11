#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <iostream>

const int table_size = 10;

template <typename K, typename V>

struct Mapnode{
    K key;
    V value;

    Mapnode <K, V> *next;
    Mapnode(const K&k, const V&v): key(k), value(v), next(nullptr){}
};

template <typename K, typename V>

class HashMap{
        Mapnode <K, V> *hashTable[table_size];

        V key_not_found_sentinel = V();

        int hash_function(const K&key){
            return std::hash<K>{}(key) %table_size;
        }

        void cleanup(){
            for(int i = 0; i< table_size; ++i){
                Mapnode<K,V> *current = hashTable[i];
                while(current != nullptr){
                    Mapnode<K,V> *todel = current;
                    current = current -> next;
                    delete todel;
                }
                hashTable[i] = nullptr;
            }
        }

public:

        HashMap(){
            for(int i=0; i < table_size; ++i){
                hashTable[i] = nullptr;
            }
        }

        ~HashMap(){
            cleanup();
        }

        void insertKey_Val(const K& key, const V& value);
        V get_val(const K& key) const;
        void deleteKey(const K& key);
        void display() const;
};

#endif
