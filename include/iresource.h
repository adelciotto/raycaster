#ifndef IRESOURCE_H
#define IRESOURCE_H

#include <string>
#include "utils.h"

class ResourceException: public std::exception {
public:
    explicit ResourceException(const std::string& message, const std::string& filename) 
        : msg(utils::stringFormat("%s: %s", filename.c_str(), message.c_str())),
          filename(filename) { }
    virtual ~ResourceException() throw() { }
    
    virtual const char *what() const throw() {
        return msg.c_str();
    }

private:
    std::string msg;
    std::string filename;
};

class IResource {
public:
    IResource() { }
    IResource(const std::string& filename) : filename(filename) { }
    virtual ~IResource() { }

    const std::string& getFilename() const { return filename; }
    virtual void load() = 0;

protected:
    std::string filename;
};

#endif // IRESOURCE_H
