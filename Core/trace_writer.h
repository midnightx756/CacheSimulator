#ifndef TRACE_WRITER_H
#define TRACE_WRITER_H

#include <fstream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <cstdint> // For uint32_t

/**
 * @brief A class for writing memory access traces to a file in a simple format.
 * * Format: Each line is "OPERATION 0xADDRESS"
 * Example:
 * R 0x1234ABCD
 * W 0x5678EF01
 */
class TraceWriter {
public:
    /**
     * @brief Constructs a TraceWriter and opens the specified file.
     * @param filename The path to the output trace file.
     * @throws std::runtime_error if the file cannot be opened.
     */
    TraceWriter(const std::string& filename) : filename(filename) {
        output_file.open(filename,std::ios::app);
        if (!output_file.is_open()) {
            throw std::runtime_error("Failed to open trace file for writing: " + filename);
        }
    }

    /**
     * @brief Destructor, closes the trace file.
     */
    ~TraceWriter() {
        if (output_file.is_open()) {
            output_file.close();
        }
    }

    /**
     * @brief Writes a read (R) operation to the trace file.
     * @param address The memory address accessed.
     */
    void writeRead(uint32_t address) {
        writeEntry('R', address);
    }

    /**
     * @brief Writes a write (W) operation to the trace file.
     * @param address The memory address accessed.
     */
    void writeWrite(uint32_t address) {
        writeEntry('W', address);
    }

    /**
     * @brief Writes an instruction fetch (I) operation to the trace file.
     * This format is less common for data cache simulation but can be included.
     * @param address The memory address of the instruction.
     */
    void writeInstruction(uint32_t address) {
        writeEntry('I', address);
    }

    /**
     * @brief Checks if the trace file is currently open and valid.
     * @return true if the file is open, false otherwise.
     */
    bool isOpen() const {
        return output_file.is_open();
    }

private:
    std::ofstream output_file; ///< The output file stream.
    std::string filename;      ///< The name of the file being written to.

    // Private helper to write a generic entry
    void writeEntry(char operation, uint32_t address) {
        if (!output_file.is_open()) {
            throw std::runtime_error("Trace file is not open for writing: " + filename);
        }
        // Write in format: OPERATION 0xADDRESS
        output_file << operation << " 0x"
        << std::hex << std::uppercase << std::setw(8) << std::setfill('0')
        << address << std::endl;
    }

    // Disable copy constructor and assignment operator for RAII
    TraceWriter(const TraceWriter&) = delete;
    TraceWriter& operator=(const TraceWriter&) = delete;
};

#endif // TRACE_WRITER_H
