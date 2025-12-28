#include "LoomAssembler.hpp"
#include <cctype>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<int32_t> LoomAssembler::assemble(const std::string &source) {
    labels_.clear();
    std::vector<int32_t> program;

    // Strip comments from source
    std::istringstream stringStream = std::istringstream(source);
    std::string line;
    std::string cleanSource = "";

    while (std::getline(stringStream, line)) {
        // Search for the first comment symbol on this line
        size_t commentPos = line.find('#');
        // Truncate string if symbol was found
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        // Ensure there is still separation between tokens on different lines
        cleanSource += line + " ";
    }

    // First pass
    stringStream = std::istringstream(cleanSource);
    std::string token;
    int pc = 0;

    while (stringStream >> token) {
        if (isLabelDefinition(token)) {
            // Save label identifier (exclude first char ':')
            labels_.insert({token.substr(1, std::string::npos), pc});
        } else  // Increment PC only if not label
            pc++;
    }

    // Second pass
    stringStream = std::istringstream(cleanSource);

    while (stringStream >> token) {
        if (isLabelDefinition(token)) {
            ;  // Ignore label definitions
        } else if (isInstruction(token)) {
            int32_t opCode = static_cast<int32_t>(opMap_.at(token));
            program.push_back(opCode);
        } else if (isNumber(token)) {
            int32_t num = static_cast<int32_t>(std::stoi(token));
            program.push_back(num);
        } else if (isLabelReference(token)) {
            const int32_t target = labels_.at(token);
            program.push_back(target);
        } else {
            std::cerr << "Error: Invalid token '" << token << "'." << std::endl;
            return std::vector<int32_t>();
        }
    }

    return program;
};

bool LoomAssembler::isInstruction(const std::string &str) const {
    return opMap_.find(str) != opMap_.end();
}

bool LoomAssembler::isNumber(const std::string &str) const {
    // Allow one unary minus
    int startIx = 0;
    if (str[startIx] == '-') {
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
    if (str[startIx] != ':')
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

bool LoomAssembler::isLabelReference(const std::string &str) const {
    return labels_.find(str) != labels_.end();
}
