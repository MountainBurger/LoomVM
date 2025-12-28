#include "LoomAssembler.hpp"
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<int32_t> LoomAssembler::assemble(const std::string &source) {
    std::vector<int32_t> program;
    // ...
    return program;
};

bool LoomAssembler::isInstruction(const std::string &str) const {
    return opMap_.contains(str);
}

bool LoomAssembler::isNumber(const std::string &str) const {
    // Allow one unary minus
    int startIx = 0;
    if (str.starts_with('-')) {
        startIx = 1;
    }

    // Ensure not empty string of characters
    if (str.size() == startIx)
        return false;

    // Ensure all characters are digits
    for (int i = startIx; i < str.size(); i++) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (!isdigit(c))
            return false;
    }
    return true;
};

bool LoomAssembler::isLabelDefinition(const std::string &str) const {
    int startIx = 0;  // String index to check from
    // Label definitions must start with ':'
    if (!str.starts_with(':'))
        return false;
    startIx = 1;

    // Label name must not be empty
    if (str.size() == startIx)
        return false;

    // Label names must start with letter
    if (!isalpha(str[startIx]))
        return false;
    startIx++;

    // Following characters must be alphanumeric
    for (int i = startIx; i < str.size(); i++) {
        unsigned char c = static_cast<unsigned char>(str[i]);

        if (!std::isalnum(c))
            return false;
    }
    return true;
};
