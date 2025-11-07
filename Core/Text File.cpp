#include "trace_reader.h"
#include "cache.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm> // For std::find, std::min

// --- Define Policy Node/Data Structures Here ---
// Since your policies work on nodes holding addresses, you would define
// that structure here. Example:
/*
struct PolicyNode {
    uint32_t tag;
    // Policy-specific data (e.g., LFU count, timestamp)
    uint32_t counter;
    // Pointers for linked list or heap structure
    // ...
};
*/
// We'll skip complex policy structures for simplicity in this integration script.

// --- Global Cache Configuration (Match your GUI input) ---
const uint32_t CACHE_SIZE_BYTES = 256 * 1024;  // 256 KB
const uint32_t BLOCK_SIZE_BYTES = 64;          // 64 Bytes
const uint32_t ASSOCIATIVITY = 8;              // 8-way

const uint32_t NUM_SETS = CACHE_SIZE_BYTES / (BLOCK_SIZE_BYTES * ASSOCIATIVITY);

const uint32_t BLOCK_SIZE_LOG2 = calculateLog2(BLOCK_SIZE_BYTES);
const uint32_t NUM_SETS_LOG2 = calculateLog2(NUM_SETS);

// ----------------------------------------------------------------------

// Placeholder for your actual policy function
// This function must decide hit/miss and update the 'CacheLine' metadata.
// It returns 1 for hit, 0 for miss.
int policy_access(std::vector<CacheLine>& current_set,
                  const CacheAddress& c_addr,
                  char operation) {

    // --- 1. CHECK FOR HIT ---

    // Search the set for a matching Tag that is valid
    for (CacheLine& line : current_set) {
        if (line.isValid && line.tag == c_addr.tag) {
            // HIT FOUND!

            // 2. Policy-Specific Node Update (Where your policy script comes in)
            // If hit, your policy script would update its internal node structure
            // (e.g., increment LFU count, move node to MRU position, etc.)

            // 3. Update CacheLine Metadata
            if (operation == 'W' || operation == 'S') {
                line.isDirty = true;
            }
            return 1; // Return Hit
        }
    }

    // --- 4. MISS LOGIC (Eviction/Insertion) ---

    // Policy decides which line (index in the vector) to replace.
    // For simplicity, we'll use a crude FIFO replacement here: the first invalid or the first valid line.

    auto victim_it = std::find_if(current_set.begin(), current_set.end(),
                                  [](const CacheLine& l) { return !l.isValid; });

    if (victim_it == current_set.end()) {
        // No invalid line found, must evict a valid one (Policy decides which one!)
        // Since we don't have your policy logic, we'll replace the first one for now
        victim_it = current_set.begin();
    }

    CacheLine& victim = *victim_it;

    // 5. Check if victim is dirty (Write-back mechanism)
    if (victim.isDirty) {
        // In a real simulator, this triggers a write-back to main memory
        // For now, we just reset the dirty bit.
        victim.isDirty = false;
    }

    // 6. Insert New Block / Update CacheLine Metadata
    // The Tag from the request is stored in the victim line
    victim.tag = c_addr.tag;
    victim.isValid = true;
    victim.isDirty = (operation == 'W' || operation == 'S');

    // Policy-Specific Node Creation/Insertion happens here in your other script.

    return 0; // Return Miss
}

// ----------------------------------------------------------------------

// The main cache structure wrapper
class CacheSimulator {
private:
    std::vector<std::vector<CacheLine>> cache_sets;
    size_t total_accesses = 0;
    size_t total_hits = 0;

public:
    CacheSimulator() {
        if (NUM_SETS == 0) {
            throw std::runtime_error("Cache configuration is invalid (NUM_SETS=0). Check sizes.");
        }
        // Initialize cache memory array
        cache_sets.resize(NUM_SETS);
        for (uint32_t i = 0; i < NUM_SETS; ++i) {
            cache_sets[i].resize(ASSOCIATIVITY);
        }
        std::cout << "Cache Initialized: " << NUM_SETS << " Sets, "
                  << ASSOCIATIVITY << " Ways, " << BLOCK_SIZE_BYTES << " Byte Blocks." << std::endl;
    }

    void process_trace_access(const MemoryTrace& trace) {
        if (trace.operation == 'I') {
            // Ignore instruction fetches for data cache simulation
            return;
        }

        total_accesses++;

        // 1. Decompose the raw address into the CacheAddress structure
        // The CacheAddress constructor automatically calculates tag, index, and offset.
        CacheAddress c_addr(trace.address, BLOCK_SIZE_LOG2, NUM_SETS_LOG2);

        // 2. Get the specific set based on the calculated index
        if (c_addr.index >= NUM_SETS) {
            std::cerr << "Error: Calculated index " << c_addr.index
                      << " is out of bounds (" << NUM_SETS << ")" << std::endl;
            return;
        }

        std::vector<CacheLine>& current_set = cache_sets[c_addr.index];

        // 3. Evaluate hit/miss using the policy function
        int result = policy_access(current_set, c_addr, trace.operation);

        if (result == 1) {
            total_hits++;
        }
    }

    void print_stats() const {
        double hit_rate = (total_accesses == 0) ? 0.0 :
                          static_cast<double>(total_hits) / total_accesses;

        std::cout << "\n--- Simulation Summary ---\n";
        std::cout << "Total Accesses: " << total_accesses << std::endl;
        std::cout << "Total Hits:     " << total_hits << std::endl;
        std::cout << "Overall Hit Rate: " << hit_rate * 100.0 << "%" << std::endl;
    }
};

// ----------------------------------------------------------------------

int main() {
    std::string trace_filename = "sample_trace.txt";

    // First, generate a sample trace file for testing
    try {
        generateSampleTrace(trace_filename);
    } catch (const std::runtime_error& e) {
        std::cerr << "Trace generation error: " << e.what() << std::endl;
        return 1;
    }

    // Now, run the simulation
    try {
        CacheSimulator simulator;
        TraceReader reader(trace_filename);
        MemoryTrace trace;

        std::cout << "\nStarting trace processing..." << std::endl;

        while (reader.readNext(trace)) {
            simulator.process_trace_access(trace);
        }

        simulator.print_stats();

    } catch (const std::exception& e) {
        std::cerr << "Simulation Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
