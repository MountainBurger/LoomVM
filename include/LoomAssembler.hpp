#include "LoomVM.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
class LoomAssembler {
  public:
    std::vector<int32_t> assemble(const std::string &source);

  private:
    std::unordered_map<std::string, LoomVM::Op> opMap_ = LoomVM::getOpcodeMap();
    std::unordered_map<std::string, int32_t> labels_;
    bool isInstruction(const std::string &str) const;
    bool isNumber(const std::string &str) const;
    bool isLabelDefinition(const std::string &str) const;
    bool isLabelReference(const std::string &str) const;
};
