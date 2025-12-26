#include "LoomVM.hpp"
#include <cstdint>
#include <iostream>
#include <ostream>
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
    isRunning_ = true;
    bool isHaltReached = false;

    while (isRunning_) {
        // Fetch
        int32_t cir = 0;
        cir = fetch();

        // If fetch reached end of program, isRunning_ was set to false
        if (isRunning_ == false) {
            break;
        }

        // Decode and execute
        switch (static_cast<Op>(cir)) {
            case (Op::HLT): {
                isHaltReached = true;
                isRunning_ = false;
                break;
            }
            case (Op::PSH): {
                const int32_t val = fetch();
                push(val);
                break;
            }
            case (Op::ADD): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (isRunning_) {
                    const int32_t result = x + y;
                    push(result);
                }
                break;
            }
            case (Op::SUB): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (isRunning_) {
                    const int32_t result = x - y;
                    push(result);
                }
                break;
            }
            case (Op::MUL): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (isRunning_) {
                    const int32_t result = x * y;
                    push(result);
                }
                break;
            }
            case (Op::DIV): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (isRunning_) {
                    if (y == 0) {
                        std::cerr << "Error: Division by 0." << std::endl;
                        isRunning_ = false;
                    } else {
                        const int32_t result = x / y;
                        push(result);
                    }
                }
                break;
            }
        }
    }
    // Check whether we finished properly
    if (isHaltReached) {
        return true;
    } else {
        std::cerr << "Error: Program terminated unexpectedly (no HLT)."
                  << std::endl;
        return false;
    }
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

void LoomVM::push(int32_t const val) {
    // Halt if stack full
    if (stack_.size() >= STACK_SIZE) {
        std::cerr << "Error: Stack overflow." << std::endl;
        isRunning_ = false;
        return;
    }
    stack_.push_back(val);
}

int32_t LoomVM::pop() {
    // Halt if stack is empty
    if (stack_.empty()) {
        std::cerr << "Error: Stack underflow." << std::endl;
        isRunning_ = false;
        return 0;
    }
    // Remove and return last value (top)
    int32_t const val = stack_.back();
    stack_.pop_back();
    return val;
}
