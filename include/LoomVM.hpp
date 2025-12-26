#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

class LoomVM {
  public:
    bool loadProgram(const std::vector<int32_t> &program);
    bool run();
    void dumpStack() const;
    enum class Op : int32_t {
        HLT = 0,  // Halt the machine
        PSH = 1,  // Push a value to the top of the stack
        ADD = 2,  // Add the top two values on the stack
        SUB = 3,  // Subtract the top two values (top is subtrahend)
        MUL = 4,  // Multiply the top two values
        DIV = 5,  // Divide the top two values (top is divisor)
        PRN = 6,  // Pop and output the value at the top of the stack
    };

  private:
    std::size_t const STACK_SIZE = 1024;
    std::vector<int32_t> stack_;
    std::vector<int32_t> program_;
    size_t pc_ = 0;
    bool isRunning_ = false;
    int32_t fetch();
    void push(int32_t const val);
    int32_t pop();
};
