#include "LoomVM.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

bool LoomVM::loadProgram(const std::vector<int32_t> &program) {
    // Do not load if currently running
    if (isRunning_)
        return false;

    // Load new program
    program_ = program;
    // Reset program counter
    pc_ = 0;

    return true;
}

bool LoomVM::run() {
    // Fetch
    int32_t cir = 0;
    cir = fetch();

    // Decode
    // ...

    // Execute
    // ...
}

void LoomVM::dumpStack() const {
    std::cout << "Stack Dump" << std::endl;
    for (int32_t i = 0; i < stack_.size(); i++) {
        std::cout << i << ": " << stack_[i] << std::endl;
    }
}

int32_t LoomVM::fetch() {
    // Halt if nothing left to fetch
    if (pc_ >= program_.size()) {
        std::cerr << "Error: Program ended unexpectedly." << std::endl;
        isRunning_ = false;
        return 0;
    }
    // Return current then increment
    return program_[pc_++];
}
