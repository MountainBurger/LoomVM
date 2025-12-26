#pragma once
#include <cstdint>
#include <vector>

class LoomVM {
    public:
        void loadProgram(const std::vector<int32_t>& program);
        void run();
    private:
        std::vector<int32_t> stack_;
        std::vector<int32_t> program_;
        int pc_ = 0;
        bool isRunning_ = false;
};