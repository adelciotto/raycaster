#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <string>
#include <vector>

class InputParser {
    std::vector<std::string> tokens;

public:
    InputParser(int argc, char **argv);

    const std::string& getOption(const std::string& option, const std::string& defVal) const;
    bool optionExists(const std::string& option) const;
};

#endif // INPUT_PARSER_H
