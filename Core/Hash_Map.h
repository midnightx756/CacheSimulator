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

emplate <typename K, typename V>

class HashMap{
        Mapnode <K, V> *hashTable[Table_size];

        V key_not_found_sentinel = V();


};
