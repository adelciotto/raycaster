#include <cmath>
#include <algorithm>
#include "resource_loader.h"
#include "logger.h"

namespace {
    const int concurrentThreadsSupported = std::thread::hardware_concurrency();
}

void ResourceLoader::loadAll() {
    // Create vector of resources to load from the resources store.
    for (const auto& r : resources) {
        resourceQueue.push(r.second.get());
    }

    // Divide the number of resources to load evenly between threads.
    int numResourcesPerThread = std::ceil(double(resources.size()) / double(concurrentThreadsSupported));
    for (int i = 0; i < concurrentThreadsSupported; i++) {
        auto resourcesToLoad = std::vector<IResource *>();
        int len = std::min(numResourcesPerThread, int(resourceQueue.size()));

        for (int j = 0; j < len; j++) {
            auto res = resourceQueue.front();

            resourceQueue.pop();
            resourcesToLoad.push_back(res);
        }

        threads.push_back(std::thread(loadResources, resourcesToLoad));
    }

    // For now wait for all the threads to finish running.
    for (auto& t : threads) {
        t.join();
    }
    logger::info("All resources loaded");
}

void ResourceLoader::loadResources(std::vector<IResource *> resources) {
    for (IResource *r : resources) {
        r->load();
        logger::info("%s loaded", r->getFilename().c_str());
    }
}
