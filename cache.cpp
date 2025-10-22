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

        // 1. Calculate Offset (least significant bits)
        // Offset mask: (1 << block_size_log2) - 1. E.g., for 64B block_size_log2=6, mask = 0b111111 (63)
        offset = addr & ((1 << block_size_log2) - 1);

        // 2. Calculate Index (middle bits)
        // Shift right by offset_bits to bring index to LSB position, then mask for index_bits.
        uint32_t index_mask = ((1 << num_sets_log2) - 1);
        index = (addr >> block_size_log2) & index_mask;

        // 3. Calculate Tag (most significant bits)
        // Shift right by (offset_bits + index_bits) to isolate the tag.
        tag = addr >> (block_size_log2 + num_sets_log2);
    }

    // Default constructor for cases where you might declare it without immediate parsing
    CacheAddress() : fullAddress(0), tag(0), index(0), offset(0) {}
}CacheAddress;

typedef struct CacheLine {
    bool        isValid;     // Indicates if this cache line currently holds valid data
    bool        isDirty;     // Indicates if the data in this line has been modified (needs write-back to main memory)
    uint32_t    tag;         // The 'tag' portion of the memory address that this cache line is currently storing

    // --- Fields for Replacement Policies ---
    // You will use one or more of these depending on the policy being simulated.
    // Your custom data structures (Doubly Linked List, Queue, Min-Heap) will manage these values.

    // For LRU (Least Recently Used) - requires a Doubly Linked List for optimal O(1) updates
    // This counter would typically be a global "time" counter, updated on each access.
    uint32_t    lruCounter;  // A timestamp or counter reflecting the last time this line was accessed.
                               // Higher value = more recently used.

    // For FIFO (First-In, First-Out) - requires a Queue for optimal O(1) management
    // This counter would be set upon insertion and not changed.
    uint32_t    fifoCounter; // A counter indicating the order of insertion into the cache.
                               // Lower value = inserted earlier.

    // For LFU (Least Frequently Used) - requires a Min-Heap for optimal O(1) retrieval of min-freq item
    uint32_t    lfuCounter;  // A counter for how many times this specific cache line (data block) has been accessed
                               // while it has resided in the cache. Lower value = less frequently used.

    // Constructor to initialize a cache line to a default empty/invalid state
    CacheLine() : isValid(false), isDirty(false), tag(0), lruCounter(0), lfuCounter(0), fifoCounter(0) {}

} CacheLine;