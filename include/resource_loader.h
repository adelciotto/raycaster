#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <vector>
#include <queue>
#include <thread>
#include <map>
#include <type_traits>
#include "iresource.h"

class ResourceLoader {
public:
    static ResourceLoader& instance() {
        return instance_;
    }

    template<typename T>
    void queue(const std::string& path, const std::string& key) {
        resources.insert(std::make_pair(key, std::unique_ptr<T>(new T(path))));
    }

    template<typename T>
    T* get(const std::string& key) const {
        return dynamic_cast<T *>(resources.at(key).get());
    }

    void loadAll();

private:
    ResourceLoader() {};

    static void loadResources(std::vector<IResource *> resources);
    static ResourceLoader instance_;

    std::vector<std::thread> threads;
    std::queue<IResource *> resourceQueue;
    std::map<std::string, std::unique_ptr<IResource>> resources;
};

#endif // RESOURCE_LOADER_H
