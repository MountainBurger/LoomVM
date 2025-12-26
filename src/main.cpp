#include "LoomVM.hpp"

int main() {
    LoomVM vm;
    std::vector<int32_t> myFirstProgram = {
        static_cast<int32_t>(LoomVM::Op::PSH), 5,  // Push 5
        static_cast<int32_t>(LoomVM::Op::PSH), 5,  // Push 5
        static_cast<int32_t>(LoomVM::Op::ADD),     // Add them (5+5=10)
        static_cast<int32_t>(LoomVM::Op::PSH), 3,  // Push 3
        static_cast<int32_t>(LoomVM::Op::MUL),     // Multiply (10*3=30)
        static_cast<int32_t>(LoomVM::Op::PRN),     // Print (30)
        static_cast<int32_t>(LoomVM::Op::HLT)      // Exit
    };

    vm.loadProgram(myFirstProgram);
    bool const isSuccess = vm.run();

    return 0;
}
