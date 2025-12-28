#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class LoomVM {
  public:
    bool loadProgram(const std::vector<int32_t> &program);
    bool run();
    void dumpStack() const;
    enum class Op : int32_t {
        HLT = 0,   // Halt the machine
        PSH = 1,   // Push a value to the top of the stack
        DUP = 2,   // Pop a value from the stack then push it twice
        ADD = 3,   // Add the top two values on the stack
        SUB = 4,   // Subtract the top two values (top is subtrahend)
        MUL = 5,   // Multiply the top two values
        DIV = 6,   // Divide the top two values (top is divisor)
        MOD = 7,   // Mod the top two values (top is the modulus)
        PRN = 8,   // Pop and output the value at the top of the stack
        EQ = 9,    // Pop y, pop x, push x == y
        GT = 10,   // Pop y, pop x, push x > y
        LT = 11,   // Pop y, pop x, push x < y
        GEQ = 12,  // Pop y, pop x, push x >= y
        LEQ = 13,  // Pop y, pop x, push x <= y
        JMP = 14,  // Unconditional jump, set PC to target
        JZ = 15,   // Pop value. Jump to target if value == 0
        JNZ = 16,  // Pop value. Jump to target if value != 0
    };
    void loadFromFile(const std::string &filename);
    static const std::unordered_map<std::string, Op> &getOpcodeMap();

  private:
    std::size_t const STACK_SIZE = 1024;
    std::vector<int32_t> stack_;
    std::vector<int32_t> program_;
    size_t pc_ = 0;
    bool isRunning_ = false;
    int32_t fetch();
    void push(const int32_t val);
    int32_t pop();
};
