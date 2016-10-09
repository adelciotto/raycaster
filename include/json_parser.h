#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <fstream>
#include "picojson.h"
#include "utils.h"

class JsonParser {
public:
    JsonParser(const std::string& filename) {
        std::ifstream infile;

        infile.open(filename);
        if (infile.fail()) {
            throw std::runtime_error(
                utils::stringFormat("Could not open %s\n", filename.c_str())
            );
        }

        infile >> jsonVal;
        std::string err = picojson::get_last_error();
        if (!err.empty()) {
            throw std::runtime_error(
                utils::stringFormat("Could not parse %s\n", filename.c_str())
            );
        }

        if (!jsonVal.is<picojson::object>()) {
            throw std::runtime_error(
                utils::stringFormat("Root must be an object: %s\n", filename.c_str())
            );
        }

        rootObject = jsonVal.get<picojson::object>();
    }

    template<typename T>
    const T& get(const std::string& name) const {
        auto it = rootObject.find(name);
        auto value = it->second;

        if (value.is<T>() && it != rootObject.end()) {
            return value.get<T>();
        } else {
            return T();
        }
    }

private:
    picojson::value jsonVal;
    picojson::value::object rootObject;
};

#endif // JSON_PARSER_H
