#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <cstdio>

namespace utils {
    template<typename... Args>
    inline std::string stringFormat(const std::string& format, Args... args) {
        // We have to +1 here for the extra '\0' character.
        size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
        char buf[size];

        snprintf(buf, size, format.c_str(), args ...);
        return std::string(buf, buf + size - 1);
    }

    template<typename... Args>
    inline void printfflush(const std::string& format, Args... args) {
        const auto fmt = stringFormat("\r%s", format.c_str());

        printf(fmt.c_str(), args ...);
        fflush(stdout);
    }

    inline std::vector<std::string> stringSplit(const std::string& s, char delim) {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> tokens;

        while(std::getline(ss, item, delim)) {
            tokens.push_back(item);
        }

        return tokens;
    }
};

#endif // UTIL_H
