#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <iostream>
#include <functional>

using namespace std;
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

        int hash_function(const K&key) const{
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

        //template <typename K, typename V>
        void insertKey_Val(const K& key, const V& value) {
            int index = hash_function(key);

            Mapnode<K, V> *current = hashTable[index];
            while (current != nullptr) {
                if (current->key == key) {
                    current->value = value;
                    return;
                }
                current = current->next;
            }

            Mapnode<K, V> *new_node = new Mapnode<K, V>(key, value);

            new_node->next = hashTable[index];
            hashTable[index] = new_node;
        }


        V get_val(const K& key) const {
            int index = hash_function(key);

            Mapnode<K, V> *current = hashTable[index];
            while (current != nullptr) {
                if (current->key == key) {
                    return current->value;
                }
                current = current->next;
            }

            return key_not_found_sentinel;
        }


        void deleteKey(const K& key) {
            int index = hash_function(key);
            Mapnode<K, V> *current = hashTable[index];
            Mapnode<K, V> *prev = nullptr;

            while (current != nullptr) {
                if (current->key == key) {
                    if (prev == nullptr) {
                        hashTable[index] = current->next;
                    } else {
                        prev->next = current->next;
                    }
                    delete current;
                    std::cout << "Key " << key << " deleted from index " << index << ".\n";
                    return;
                }
                prev = current;
                current = current->next;
            }
            std::cout << "Key " << key << " not found for deletion.\n";
        }


        void display() const {
            std::cout << "\n--- Hash Map Structure (Manual Separate Chaining) ---\n";
            for (int i = 0; i < table_size; i++) {
                std::cout << "Bucket " << (i < 10 ? " " : "") << i << ": ";
                Mapnode<K, V> *current = hashTable[i];

                if (current == nullptr) {
                    std::cout << "NULL\n";
                } else {
                    while (current != nullptr) {
                        std::cout << "-> [K: " << current->key << ", V: " << current->value << "] ";
                        current = current->next;
                    }
                    std::cout << "\n";
                }
            }
            std::cout << "---------------------------------------------------\n";
        }


        bool contains_key(const K& key){
            // 1. Calculate the bucket index
            int index = hash_function(key);

            // 2. Start traversal at the head of the chain
            Mapnode<K, V> *current = hashTable[index];

            // 3. Traverse the entire linked list (the chain)
            while (current != nullptr) {
                // Check for a key match
                if (current->key == key) {
                    // Key found! Exit immediately and return true.
                    return true;
                }
                current = current->next;
            }

            // 4. Key not found after checking the entire chain
            return false;
        }
};

#endif
