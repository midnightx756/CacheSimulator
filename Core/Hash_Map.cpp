#include "Hash_Map.h"
#include <iostream>
#include <functional>

template <typename K, typename V>
void HashMap<K, V>::insertKey_Val(const K& key, const V& value) {
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


template <typename K, typename V>
V HashMap<K, V>::get_val(const K& key) const {
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


template <typename K, typename V>
void HashMap<K, V>::deleteKey(const K& key) {
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


template <typename K, typename V>
void HashMap<K, V>::display() const {
    std::cout << "\n--- Hash Map Structure (Manual Separate Chaining) ---\n";
    for (int i = 0; i < TABLE_SIZE; i++) {
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

template class HashMap<long long, int>;

// =======================================================
// Main testing function
// =======================================================
int main() {
    // Define the types for this instance
    using KEY_TYPE = long long;
    using VALUE_TYPE = int;

    // Create an instance of the custom HashMap
    HashMap<KEY_TYPE, VALUE_TYPE> myMap;

    // Insertion
    myMap.insertKey_Val(5, 500);  // Hash: 5
    myMap.insertKey_Val(15, 150); // Hash: 5 (Collision)
    myMap.insertKey_Val(25, 250); // Hash: 5 (Collision)
    myMap.insertKey_Val(2, 200);  // Hash: 2
    myMap.insertKey_Val(12, 120); // Hash: 2 (Collision)

    std::cout << "\n--- After Initial Insertion ---\n";
    myMap.display();

    // Lookup
    std::cout << "\n--- Lookup Tests ---\n";
    std::cout << "Get key 15: Value is " << myMap.get_val(15) << "\n";
    std::cout << "Get key 2: Value is " << myMap.get_val(2) << "\n";
    std::cout << "Get key 99 (non-existent): Value is " << myMap.get_val(99) << " (Expected: 0)\n";

    // Update
    std::cout << "\n--- Update Test ---\n";
    myMap.insertKey_Val(15, 999); // Update key 15's value
    std::cout << "Get key 15 after update: Value is " << myMap.get_val(15) << "\n";

    // Deletion
    std::cout << "\n--- Deletion Test ---\n";
    myMap.deleteKey(5);
    myMap.deleteKey(12);
    myMap.deleteKey(100); // Try deleting a non-existent key

    myMap.display();

    // The destructor (~HashMap) automatically cleans up remaining memory.

    return 0;
}
