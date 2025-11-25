#include <pybind11/pybind11.h>
#include "bridger.h"
#include "trace_writer.h" // You need this header for the implementation
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

namespace py = pybind11;

// Bind the function
PYBIND11_MODULE(cache_sim, m) {
    m.doc() = "PyBind11 interface for cache simulator core";

    m.def("write_trace_file", [](const std::string& trace_content, const std::string& file_path) {
          // NOTE: You must include necessary headers (like trace_writer.h, sstream)
          // in bindings.cpp BEFORE the PYBIND11_MODULE block.
          TraceWriter writer(file_path);

          if (!writer.isOpen()) {
                throw std::runtime_error("C++ Error: Failed to open trace file for writing: " + file_path);
          }

          std::stringstream ss(trace_content);
          std::string line;

          while (std::getline(ss, line, '\n')) {
                if (line.empty()) continue;

                char op;
                std::string address_str;
                uint32_t address;

                // Trace entry parsing logic
                if (line.length() < 3 || line[1] != ' ') {
                      std::cerr << "Warning: Skipping malformed trace line: " << line << std::endl;
                      continue;
                }

                op = std::toupper(line[0]);
                address_str = line.substr(2);

                try {
                      address = std::stoul(address_str, nullptr, 16);
                } catch (const std::exception& e) {
                      std::cerr << "Warning: Skipping line due to invalid hex address in: " << line << std::endl;
                      continue;
                }

                if (op == 'R') {
                      writer.writeRead(address);
                } else if (op == 'W') {
                      writer.writeWrite(address);
                } else if (op == 'I') {
                      writer.writeInstruction(address);
                }
          }
    }, "Writes the trace entries to the specified file using C++ TraceWriter.",
    py::arg("trace_content"), py::arg("file_path"));

    // 2. run_cache_simulator signature (retains the file path as the FIRST argument)
    m.def("run_cache_simulator", &run_cache_simulator, "Run cache simulation",
          py::arg("input_path"), py::arg("policy_name"), py::arg("associativity"), py::arg("block_size"), py::arg("seconds"));
}
