#pragma once
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
        POP = 2,  // Pop the top value from the stack
        ADD = 3,  // Add the top two values on the stack
        SUB = 4,  // Subtract the top two values (top is subtrahend)
        MUL = 5,  // Multiply the top two values
        DIV = 6,  // Divide the top two values (top is divisor)
    };

  private:
    std::vector<int32_t> stack_;
    std::vector<int32_t> program_;
    int32_t pc_ = 0;
    bool isRunning_ = false;
    int32_t fetch();
};
