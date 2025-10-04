#include <cstdint>
typedef struct CacheRequest{
    uint32_t address;
    bool isWrite;
}CacheRequest;

void copy(CacheRequest* l1, CacheRequest* l2){
    l1 -> address = l2 -> address;
    l1 -> isWrite = l2 -> isWrite;
}

void assign(CacheRequest* l1, uint32_t i, bool t){
    l1 -> address = i;
    l1 -> isWrite = t;
}
