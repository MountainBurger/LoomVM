#include "LoomVM.hpp"
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
