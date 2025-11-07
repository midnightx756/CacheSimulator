#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <cmath>
#include <functional>
#include <iostream>


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

    bool operator ==(const CacheAddress& other) const{
        return fullAddress == other.fullAddress;
    }
    CacheAddress() : fullAddress(0), tag(0), index(0), offset(0) {}
}CacheAddress;



std::ostream& operator<<(std::ostream& os, const CacheAddress& addr) {
    // Output format: 0x<fullAddress> (in hex)
    // We print the full address in hex for easy debugging
    os << "0x" << std::hex << addr.fullAddress << std::dec;
    return os;
}

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

namespace std{
    template <>
    struct hash<CacheAddress> {
        size_t operator()(const CacheAddress& addr) const noexcept {
            // Hashing on the fullAddress is sufficient for uniqueness
            // Since fullAddress is a uint32_t, we can simply cast it to size_t
            // For better distribution, use std::hash on the fullAddress
            return hash<uint32_t>{}(addr.fullAddress); 
        }
    };
};

#endif
