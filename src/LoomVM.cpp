#include "LoomVM.hpp"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
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
            case (Op::DUP): {
                const int32_t top = pop();
                push(top);
                push(top);
                break;
            }
            case (Op::ADD): {
                const int32_t y = pop();
                const int32_t x = pop();
                const int32_t result = x + y;
                push(result);
                break;
            }
            case (Op::SUB): {
                const int32_t y = pop();
                const int32_t x = pop();
                const int32_t result = x - y;
                push(result);
                break;
            }
            case (Op::MUL): {
                const int32_t y = pop();
                const int32_t x = pop();
                const int32_t result = x * y;
                push(result);
                break;
            }
            case (Op::DIV): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (y == 0) {
                    std::cerr << "Error: Division by 0." << std::endl;
                    isRunning_ = false;
                } else {
                    const int32_t result = x / y;
                    push(result);
                }
                break;
            }
            case (Op::MOD): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (y == 0) {
                    std::cerr << "Error: Modulo by 0." << std::endl;
                    isRunning_ = false;
                } else {
                    const int32_t result = x % y;
                    push(result);
                }
                break;
            }
            case (Op::PRN): {
                const int32_t top = pop();
                std::cout << top << std::endl;
                break;
            }
            case (Op::EQ): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (x == y) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            }
            case (Op::GT): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (x > y) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            }
            case (Op::LT): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (x < y) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            }
            case (Op::GEQ): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (isRunning_) {
                    if (x >= y) {
                        push(1);
                    } else {
                        push(0);
                    }
                }
                break;
            }
            case (Op::LEQ): {
                const int32_t y = pop();
                const int32_t x = pop();
                if (x <= y) {
                    push(1);
                } else {
                    push(0);
                }
                break;
            }
            case (Op::JMP): {
                const int32_t target = fetch();
                if (target < 0 || target >= program_.size()) {
                    std::cerr << "Error: Invalid jump target (" << target
                              << ")." << std::endl;
                    isRunning_ = false;
                }
                pc_ = static_cast<size_t>(target);
                break;
            }
            case (Op::JZ): {
                const int32_t target = fetch();
                const int32_t value = pop();
                if (value == 0) {
                    if (target < 0 || target >= program_.size()) {
                        std::cerr << "Error: Invalid jump target (" << target
                                  << ")." << std::endl;
                        isRunning_ = false;
                        break;
                    }
                    pc_ = static_cast<size_t>(target);
                }
                break;
            }
            case (Op::JNZ): {
                const int32_t target = fetch();
                const int32_t value = pop();
                if (value != 0) {
                    if (target < 0 || target >= program_.size()) {
                        std::cerr << "Error: Invalid jump target (" << target
                                  << ")." << std::endl;
                        isRunning_ = false;
                        break;
                    }
                    pc_ = static_cast<size_t>(target);
                }
                break;
            }
        }
    }
    return isHaltReached;
}

void LoomVM::dumpStack() const {
    std::cout << "Stack Dump" << std::endl;
    for (int32_t i = 0; i < stack_.size(); i++) {
        std::cout << i << ": " << stack_[i] << std::endl;
    }
}

void LoomVM::loadFromFile(const std::string &filename) {
    std::ifstream fileStream(filename);

    // Ensure file opened successfully
    if (!fileStream.is_open()) {
        std::cerr << "Error: Could not read from file '" << filename << "'."
                  << std::endl;
        return;
    }

    // Clear any existing program
    program_.clear();
    std::string line;

    // Read lines from input file stream
    while (std::getline(fileStream, line)) {
        // Search for the first comment symbol on this line
        size_t commentPos = line.find('#');
        // Truncate string if symbol was found
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // Read symbols from line string
        std::stringstream lineStream(line);
        int32_t value;
        while (lineStream >> value) {
            program_.push_back(value);
        }
    }
}

const std::unordered_map<std::string, LoomVM::Op> &getOpcodeMap() {
    static const std::unordered_map<std::string, LoomVM::Op> opcodeMap = {

        {"HLT", LoomVM::Op::HLT}, {"PSH", LoomVM::Op::PSH},
        {"DUP", LoomVM::Op::DUP}, {"ADD", LoomVM::Op::ADD},
        {"SUB", LoomVM::Op::SUB}, {"MUL", LoomVM::Op::MUL},
        {"DIV", LoomVM::Op::DIV}, {"MOD", LoomVM::Op::MOD},
        {"PRN", LoomVM::Op::PRN}, {"EQ", LoomVM::Op::EQ},
        {"GT", LoomVM::Op::GT},   {"LT", LoomVM::Op::LT},
        {"GEQ", LoomVM::Op::GEQ}, {"LEQ", LoomVM::Op::LEQ},
        {"JMP", LoomVM::Op::JMP}, {"JZ", LoomVM::Op::JZ},
        {"JNZ", LoomVM::Op::JNZ},
    };
    return opcodeMap;
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
    if (!isRunning_)
        return;
    // Halt if stack full
    if (stack_.size() >= STACK_SIZE) {
        std::cerr << "Error: Stack overflow." << std::endl;
        isRunning_ = false;
        return;
    }
    stack_.push_back(val);
}

int32_t LoomVM::pop() {
    if (!isRunning_)
        return 0;
    if (stack_.empty()) {
        std::cerr << "Error: Stack underflow. (PC: " << pc_ << ")" << std::endl;
        isRunning_ = false;
        return 0;
    }
    // Remove and return last value (top)
    int32_t const val = stack_.back();
    stack_.pop_back();
    return val;
}
