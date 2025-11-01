#ifndef TRACE_READER_H
#define TRACE_READER_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iostream>

// Structure to represent a memory access from trace file
struct MemoryTrace {
    char operation;          // 'R' for read, 'W' for write, 'I' for instruction
    uint32_t address;        // Full memory address
    uint32_t size;           // Access size in bytes (optional)

    MemoryTrace() : operation('R'), address(0), size(0) {}
    MemoryTrace(char op, uint32_t addr, uint32_t sz = 0)
    : operation(op), address(addr), size(sz) {}
};

class TraceReader {
private:
    std::ifstream file;
    std::string filename;
    size_t lineNumber;
    size_t totalTraces;

    // Helper function to parse address (handles hex and decimal)
    uint32_t parseAddress(const std::string& addrStr) {
        try {
            // Check if hex format (0x prefix)
            if (addrStr.length() >= 2 &&
                (addrStr.substr(0, 2) == "0x" || addrStr.substr(0, 2) == "0X")) {
                return static_cast<uint32_t>(std::stoull(addrStr, nullptr, 16));
                }
                // Decimal format
                return static_cast<uint32_t>(std::stoull(addrStr, nullptr, 10));
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid address format at line " +
            std::to_string(lineNumber) + ": " + addrStr);
        }
    }

    // Skip comments and empty lines
    bool skipToNextValidLine(std::string& line) {
        while (std::getline(file, line)) {
            lineNumber++;

            // Remove leading/trailing whitespace
            size_t start = line.find_first_not_of(" \t\r\n");
            if (start == std::string::npos) continue; // Empty line

            size_t end = line.find_last_not_of(" \t\r\n");
            line = line.substr(start, end - start + 1);

            // Skip comment lines
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }

            return true;
        }
        return false;
    }

public:
    TraceReader(const std::string& fname)
    : filename(fname), lineNumber(0), totalTraces(0) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open trace file: " + filename);
        }
    }

    ~TraceReader() {
        if (file.is_open()) {
            file.close();
        }
    }

    // Read next memory access from trace file
    // Returns true if successful, false if end of file
    bool readNext(MemoryTrace& trace) {
        std::string line;

        if (!skipToNextValidLine(line)) {
            return false; // EOF
        }

        std::istringstream iss(line);

        // Parse line: operation address [size]
        // Supported formats:
        // 1. "R 0x7fff5fbff8"
        // 2. "W 0x7fff5fbff8 4"
        // 3. "I 0x400000,8" (Valgrind-like format)
        // 4. "0 0x1000" (operation as number: 0=read, 1=write)

        std::string firstToken, secondToken, thirdToken;
        iss >> firstToken >> secondToken;

        if (firstToken.empty() || secondToken.empty()) {
            std::cerr << "Warning: Skipping invalid line " << lineNumber
            << ": " << line << std::endl;
            return readNext(trace);
        }

        // Parse operation
        if (firstToken.length() == 1 &&
            (firstToken[0] == 'R' || firstToken[0] == 'W' ||
            firstToken[0] == 'I' || firstToken[0] == 'L' || firstToken[0] == 'S')) {
            trace.operation = firstToken[0];

        // Handle Valgrind format: "I 0400d7d4,8"
        size_t commaPos = secondToken.find(',');
        if (commaPos != std::string::npos) {
            std::string addrPart = secondToken.substr(0, commaPos);
            std::string sizePart = secondToken.substr(commaPos + 1);
            trace.address = parseAddress(addrPart);
            trace.size = std::stoi(sizePart);
        } else {
            trace.address = parseAddress(secondToken);
            if (iss >> thirdToken) {
                trace.size = std::stoi(thirdToken);
            } else {
                trace.size = 4; // Default size
            }
        }
            }
            // Handle numeric operation format
            else if (firstToken == "0" || firstToken == "1" || firstToken == "2") {
                int opCode = std::stoi(firstToken);
                trace.operation = (opCode == 0) ? 'R' : (opCode == 1 ? 'W' : 'I');
                trace.address = parseAddress(secondToken);
                if (iss >> thirdToken) {
                    trace.size = std::stoi(thirdToken);
                } else {
                    trace.size = 4;
                }
            }
            else {
                std::cerr << "Warning: Unknown operation at line " << lineNumber
                << ": " << firstToken << std::endl;
                return readNext(trace);
            }

            // Normalize operation codes (L->R, S->W for Valgrind format)
            if (trace.operation == 'L') trace.operation = 'R';
            if (trace.operation == 'S') trace.operation = 'W';

            totalTraces++;
        return true;
    }

    // Read all traces into a vector
    std::vector<MemoryTrace> readAll() {
        std::vector<MemoryTrace> traces;
        MemoryTrace trace;

        // Reset to beginning
        reset();

        while (readNext(trace)) {
            traces.push_back(trace);
        }

        return traces;
    }

    // Reset file pointer to beginning
    void reset() {
        file.clear();
        file.seekg(0, std::ios::beg);
        lineNumber = 0;
        totalTraces = 0;
    }

    // Check if file is open
    bool isOpen() const {
        return file.is_open();
    }

    // Get statistics
    size_t getLineNumber() const { return lineNumber; }
    size_t getTotalTraces() const { return totalTraces; }
    std::string getFilename() const { return filename; }
};

// Helper function to generate sample trace file for testing
inline void generateSampleTrace(const std::string& filename,
                                size_t numAccesses = 1000,
                                bool includeWrites = true) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create sample trace file: " + filename);
    }

    file << "# Sample memory trace file\n";
    file << "# Format: Operation Address [Size]\n";
    file << "# Operation: R (read), W (write), I (instruction)\n";
    file << "# Generated with " << numAccesses << " accesses\n\n";

    // Generate memory accesses with some locality
    uint32_t baseAddr = 0x00400000;  // Text segment
    uint32_t stackBase = 0x7fff0000; // Stack
    uint32_t heapBase = 0x00600000;  // Heap

    for (size_t i = 0; i < numAccesses; i++) {
        char op;
        uint32_t addr;

        // 60% reads, 30% writes, 10% instruction fetches
        int rand_val = i % 10;

        if (rand_val < 6) {
            op = 'R';
            // Read from stack or heap
            if (i % 2 == 0) {
                addr = stackBase + ((i % 256) * 4);
            } else {
                addr = heapBase + ((i % 512) * 4);
            }
        } else if (rand_val < 9 && includeWrites) {
            op = 'W';
            // Write to stack or heap
            if (i % 3 == 0) {
                addr = stackBase + ((i % 256) * 4);
            } else {
                addr = heapBase + ((i % 512) * 4);
            }
        } else {
            op = 'I';
            // Instruction fetch from text segment
            addr = baseAddr + ((i % 1024) * 4);
        }

        // Add some spatial locality
        if (i > 0 && i % 5 == 0) {
            addr += 64; // Next cache line
        }

        file << op << " 0x" << std::hex << addr << std::dec << " 4\n";
    }

    file.close();
    std::cout << "Generated " << numAccesses << " traces in " << filename << std::endl;
                                }
#endif // TRACE_READER_H
