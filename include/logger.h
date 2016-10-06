#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <SDL2/SDL_log.h>
#include "utils.h"

namespace logger {
    template<typename... Args>
    inline void info(const std::string& format, Args... args) {
        SDL_Log(format.c_str(), args...);
    }

    template<typename... Args>
    inline void debug(const std::string& format, Args... args) {
        SDL_LogDebug(format.c_str(), args...);
    }

    template<typename... Args>
    inline void warn(const std::string& format, Args... args) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, format.c_str(), args...);
    }

    template<typename... Args>
    inline void error(const std::string& format, Args... args) {
        SDL_LogError(format.c_str(), args...);
    }
}

#endif // LOGGER_H
