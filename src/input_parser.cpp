#include "input_parser.h"

InputParser::InputParser(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        tokens.push_back(std::string(argv[i]));
    }
}

const std::string& InputParser::getOption(const std::string& option, const std::string& defVal) const {
    auto val = std::find(std::begin(tokens), std::end(tokens), option);

    if (val != std::end(tokens) && ++val != std::end(tokens)) {
        return *val;
    }

    return defVal;
}

bool InputParser::optionExists(const std::string& option) const {
    auto val = std::find(std::begin(tokens), std::end(tokens), option);

    return val != std::end(tokens);
}
