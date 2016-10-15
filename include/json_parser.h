#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <fstream>
#include "picojson.h"
#include "utils.h"


class JsonParser {
class JsonException: public std::exception {
public:
    explicit JsonException(const std::string& message, const std::string& filename) 
        : msg(utils::stringFormat("%s: %s", filename.c_str(), message.c_str())),
          filename(filename) { }
    virtual ~JsonException() throw() { }
    
    virtual const char *what() const throw() {
        return msg.c_str();
    }

private:
    std::string msg;
    std::string filename;
};

public:
    JsonParser(const std::string& filename) {
        std::ifstream infile;

        infile.open(filename);
        if (infile.fail()) {
            throw JsonException("Could not open file", filename);
        }

        infile >> jsonVal;
        std::string err = picojson::get_last_error();
        if (!err.empty()) {
            throw JsonException("Could not parse file", filename);
        }

        if (!jsonVal.is<picojson::object>()) {
            throw JsonException("Root must be an object", filename);
        }

        rootObject = jsonVal.get<picojson::object>();
    }

    template<typename T>
    const T get(const std::string& name, const T& defaultValue) const {
        auto it = rootObject.find(name);
        auto value = it->second;

        if (value.is<T>() && it != rootObject.end()) {
            return value.get<T>();
        } else {
            return defaultValue;
        }
    }

private:
    picojson::value jsonVal;
    picojson::value::object rootObject;
};

#endif // JSON_PARSER_H
