#include <iostream>
#include "clock.cpp"
#include "Policy.cpp"
#include "trace_reader.h"
#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include<string>
#include <map>
using namespace std;

const uint32_t CACHE_SIZE_BYTES = 64 * 1024;
const uint32_t CACHE_BLOCK_SIZE = 64;
const uint32_t ASSOCIATIVITY = 8;

const uint32_t NUM_SETS = CACHE_SIZE_BYTES / (CACHE_BLOCK_SIZE * ASSOCIATIVITY);

const uint32_t BLOCK_SIZELOG2 = calculateLog2(CACHE_BLOCK_SIZE);
const uint32_t NUM_SETSLOG2 = calculateLog2(NUM_SETS);
enum PolicyType {LFU_POLICY, LRU_POLICY, FIFO_POLICY, UNKNOWN_POLICY};

int policy_access_wrapper(Policies* policy, vector<CacheLine>& current_set, const CacheAddress& c_addr, char operation, PolicyType chosen_policy) {

    auto hit_it = find_if(current_set.begin(), current_set.end(), [&c_addr](const CacheLine& l) {
                                   return l.isValid && (l.tag == c_addr.tag);
                               });
    bool metadata_hit = (hit_it != current_set.end());

    int result = 0;
    CacheLine dummy_line; // Dummy line as required by Policy.cpp signature

    // 2. Delegate Hit/Miss determination and policy updates
    switch (chosen_policy) {
        case LRU_POLICY:
            result = policy->LRU(c_addr, dummy_line);
            break;
        case LFU_POLICY:
            result = policy->LFU_NonHybrid(c_addr, dummy_line);
            break;
        case FIFO_POLICY:
            result = policy->FIFO(c_addr, dummy_line);
            break;
        default:
            return 0; // Unknown policy
    }

    // --- SYNCHRONIZATION AND METADATA UPDATE ---

    if (result == 1) { // POLICY HIT
        // Policy hit means the block is in the set. Update the metadata.
        if (metadata_hit) {
            if (operation == 'W' || operation == 'S') {
                hit_it->isDirty = true;
            }
        }
        // If the policy said HIT but metadata was not found (a bug or a race in the original logic),
        // we trust the policy's internal state. This is highly unlikely with a correctly synched cache.
        return 1;

    } else { // POLICY MISS
        // A MISS occurred, so a replacement happened inside the policy structures.
        // We must find a CacheLine slot to overwrite and synchronize it.

        // a. Find the victim CacheLine slot (first invalid, or oldest valid slot)
        auto victim_it = std::find_if(current_set.begin(), current_set.end(),
                                      [](const CacheLine& l) { return !l.isValid; });

        if (victim_it == current_set.end()) {
            victim_it = current_set.begin();
        }

        CacheLine& victim = *victim_it;


        if (victim.isDirty) {
            victim.isDirty = false;
        }

        victim.tag = c_addr.tag;
        victim.isValid = true;
        victim.isDirty = (operation == 'W' || operation == 'S');

        return 0;
    }
}


class CacheSimulator {
private:
        vector<vector<CacheLine>> cache_sets;
        vector<Policies*> policy_objects; // One Policies object per set
        PolicyType active_policy;
        size_t total_accesses = 0;
        size_t total_hits = 0;

public:
                              CacheSimulator(PolicyType policy_type) : active_policy(policy_type) {
                                  if (NUM_SETS == 0) {
                                      throw std::runtime_error("Cache configuration is invalid (NUM_SETS=0). Check sizes.");
                                  }

                                  // 1. Initialize CacheLine metadata storage (physical slots)
                                  cache_sets.resize(NUM_SETS);
                                  for (uint32_t i = 0; i < NUM_SETS; ++i) {
                                      cache_sets[i].resize(ASSOCIATIVITY);
                                  }

                                  // 2. Initialize Policy objects (one per set, using ASSOCIATIVITY as capacity)
                                  policy_objects.reserve(NUM_SETS);
                                  for (uint32_t i = 0; i < NUM_SETS; ++i) {
                                      policy_objects.push_back(new Policies(ASSOCIATIVITY));
                                  }

                                  std::cout << "Cache Initialized: " << NUM_SETS << " Sets, "
                                  << ASSOCIATIVITY << " Ways, " << CACHE_BLOCK_SIZE << " Byte Blocks." << std::endl;
                              }

                              ~CacheSimulator() {
                                  for (Policies* p : policy_objects) {
                                      delete p; // Clean up policy objects
                                  }
                              }

                              void process_trace_access(const MemoryTrace& trace) {
                                  if (trace.operation == 'I') return;
                                  total_accesses++;

                                  // 1. Decompose the raw address
                                  CacheAddress c_addr(trace.address, BLOCK_SIZELOG2, NUM_SETSLOG2);

                                  if (c_addr.index >= NUM_SETS) {
                                      std::cerr << "Error: Calculated index out of bounds." << std::endl;
                                      return;
                                  }

                                  // Get the specific set's metadata and policy object
                                  std::vector<CacheLine>& current_set = cache_sets[c_addr.index];
                                  Policies* current_policy = policy_objects[c_addr.index];

                                  // 2. Evaluate hit/miss using the wrapper function
                                  int result = policy_access_wrapper(current_policy, current_set, c_addr, trace.operation, active_policy);

                                  if (result == 1) {
                                      total_hits++;
                                  }
                              }

                              void print_stats(const std::string& policy_name) const {
                                  double hit_rate = (total_accesses == 0) ? 0.0 :
                                  static_cast<double>(total_hits) / total_accesses;

                                  std::cout << "\n--- Simulation Summary for " << policy_name << " ---\n";
                                  std::cout << "Total Accesses: " << total_accesses << std::endl;
                                  std::cout << "Total Hits:     " << total_hits << std::endl;
                                  std::cout << "Overall Hit Rate: " << hit_rate * 100.0 << "%" << std::endl;
                              }
};

                          // ----------------------------------------------------------------------

                          PolicyType get_policy_type(const std::string& policy_str) {
                              std::string s = policy_str;
                              std::transform(s.begin(), s.end(), s.begin(), ::toupper);
                              if (s == "LRU") return LRU_POLICY;
                              if (s == "LFU") return LFU_POLICY;
                              if (s == "FIFO") return FIFO_POLICY;
                              return UNKNOWN_POLICY;
                          }

                          std::string get_policy_name(PolicyType type) {
                              switch(type) {
                                  case LRU_POLICY: return "LRU";
                                  case LFU_POLICY: return "LFU";
                                  case FIFO_POLICY: return "FIFO";
                                  default: return "UNKNOWN";
                              }
                          }

                          int main(int argc, char* argv[]) {
                              // 1. Handle command-line arguments for policy selection
                              if (argc < 3) {
                                  std::cerr << "Usage: " << argv[0] << " <trace_file_name> <policy_name (LRU|LFU|FIFO)>" << std::endl;
                                  std::cerr << "Example: " << argv[0] << " sample_trace.txt LRU" << std::endl;
                                  return 1;
                              }

                              std::string trace_filename = argv[1];
                              std::string policy_str = argv[2];
                              PolicyType policy_type = get_policy_type(policy_str);

                              if (policy_type == UNKNOWN_POLICY) {
                                  std::cerr << "Error: Invalid policy name. Choose from LRU, LFU, or FIFO." << std::endl;
                                  return 1;
                              }

                              // First, generate a sample trace file for testing if it doesn't exist
                              // Note: The trace_reader.h provided has generateSampleTrace
                              try {
                                  // Only generate if the file is the default sample file, or if logic needs it.
                                  // Assuming the file provided in argv[1] exists, we skip generation.
                                  // If the user wants to generate it, uncomment the next block:
                                  /*
                                   i f (trace_filename == "sample_trace.*txt") {
                                   generateSampleTrace(trace_filename);
                              }
                              */
                              } catch (const std::runtime_error& e) {
                                  std::cerr << "Trace generation error: " << e.what() << std::endl;
                                  return 1;
                              }

                              // Now, run the simulation
                              try {
                                  CacheSimulator simulator(policy_type);
                                  TraceReader reader(trace_filename);
                                  MemoryTrace trace;

                                  std::cout << "\nStarting trace processing with " << get_policy_name(policy_type) << " policy..." << std::endl;

                                  while (reader.readNext(trace)) {
                                      simulator.process_trace_access(trace);
                                  }

                                  simulator.print_stats(get_policy_name(policy_type));

                              } catch (const std::exception& e) {
                                  std::cerr << "Simulation Error: " << e.what() << std::endl;
                                  return 1;
                              }

                              return 0;
                          }
