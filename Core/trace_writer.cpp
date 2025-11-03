#include "trace_writer.h"
#include <iomanip> // For std::hex, std::setw, std::setfill
#include <stdexcept> // For std::runtime_error

TraceWriter::TraceWriter(const std::string& filename) : filename(filename) {
    output_file.open(filename);
    if (!output_file.is_open()) {
        throw std::runtime_error("Failed to open trace file for writing: " + filename);
    }
}

TraceWriter::~TraceWriter() {
    if (output_file.is_open()) {
        output_file.close();
    }
}

void TraceWriter::writeRead(uint32_t address) {
    writeEntry('R', address);
}

void TraceWriter::writeWrite(uint32_t address) {
    writeEntry('W', address);
}

void TraceWriter::writeInstruction(uint32_t address) {
    writeEntry('I', address);
}

bool TraceWriter::isOpen() const {
    return output_file.is_open();
}

void TraceWriter::writeEntry(char operation, uint32_t address) {
    if (!output_file.is_open()) {
        throw std::runtime_error("Trace file is not open for writing: " + filename);
    }
    // Write in format: OPERATION 0xADDRESS
    output_file << operation << " 0x"
    << std::hex << std::uppercase << std::setw(8) << std::setfill('0')
    << address << std::endl;
}
