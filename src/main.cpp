#include "LoomAssembler.hpp"
#include "LoomVM.hpp"
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

int main() {
    LoomVM vm;
    LoomAssembler assembler;
    std::vector<int32_t> myFirstProgram = {
        static_cast<int32_t>(LoomVM::Op::PSH), 5,  // Push 5
        static_cast<int32_t>(LoomVM::Op::PSH), 5,  // Push 5
        static_cast<int32_t>(LoomVM::Op::ADD),     // Add them (5+5=10)
        static_cast<int32_t>(LoomVM::Op::PSH), 3,  // Push 3
        static_cast<int32_t>(LoomVM::Op::MUL),     // Multiply (10*3=30)
        static_cast<int32_t>(LoomVM::Op::PRN),     // Print (30)
        static_cast<int32_t>(LoomVM::Op::HLT)      // Exit
    };

    // vm.loadProgram(myFirstProgram);
    // vm.loadFromFile("mySecondProgram.loom");
    // vm.loadFromFile("myThirdProgram.loom");

    std::ifstream file("myFourthProgram.loom");
    std::string contents((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    std::vector<int32_t> program = assembler.assemble(contents);
    if (program.empty()) {
        std::cerr << "Failed to assemble program." << std::endl;
        return 1;
    }

    vm.loadProgram(program);
    const bool isSuccess = vm.run();

    if (!isSuccess)
        vm.dumpStack();

    return 0;
}
