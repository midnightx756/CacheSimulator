#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <cmath>

inline uint32_t calculateLog2(uint32_t n){
    if(n==0) return 0;
    return static_cast<uint32_t>(std::log2(n));
}

typedef struct CacheAddress{
    uint32_t fullAddress;
    uint32_t tag;
    uint32_t index;
    uint32_t offset;

    CacheAddress(uint32_t addr, uint32_t block_size_log2, uint32_t num_sets_log2): fullAddress(addr) {

        offset = addr & ((1 << block_size_log2) - 1);

        uint32_t index_mask = ((1 << num_sets_log2) - 1);
        index = (addr >> block_size_log2) & index_mask;

        tag = addr >> (block_size_log2 + num_sets_log2);
    }

    CacheAddress() : fullAddress(0), tag(0), index(0), offset(0) {}
}CacheAddress;

typedef struct CacheLine {
    bool        isValid;
    bool        isDirty;
    uint32_t    tag;

/*
    uint32_t    lruCounter;

    uint32_t    fifoCounter;

    uint32_t    lfuCounter;
*/

    CacheLine() : isValid(false), isDirty(false){}

} CacheLine;

#endif
