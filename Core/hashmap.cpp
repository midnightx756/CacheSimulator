#include <iostream>
#include <vector>
#include <optional>
using namespace std;

template <typename K, typename V>

class Hashing{
    struct Entry{
        K key;
        V value;
        bool isOccupied;
        bool isDeleted;

        Entry(): isOccupied(false), isDeleted(false) {}
    };

    vector<Entry> table;
    int capacity;
    int size;

    int hash(const K& key) const{
        return hash<K>{kay} % capacity;
    }
};
