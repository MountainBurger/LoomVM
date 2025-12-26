#pragma once
#include <cstdint>
#include <vector>

class LoomVM {
  public:
    bool loadProgram(const std::vector<int32_t> &program);
    bool run();

  private:
    std::vector<int32_t> stack_;
    std::vector<int32_t> program_;
    int pc_ = 0;
    bool isRunning_ = false;
};
