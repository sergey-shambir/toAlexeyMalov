#include "ResourceHeap.h"
#include <vector>
#include <stdio.h> // debug messages

namespace GL {

ResourceHeap* ResourceHeap::ms_heap = nullptr;

ResourceHeap &ResourceHeap::instance()
{
    return *ms_heap;
}

unsigned ResourceHeap::create(Constructor ctor,
                              Destructor dtor,
                              Type type)
{
    unsigned id = ctor();
    Entry entry{1, dtor};
    m_entries[type][id] = entry;
//    printf("created id %d\n", id);
//    fflush(stdout);
    return id;
}

void ResourceHeap::retain(unsigned id, Type type)
{
    if (id != 0) {
        auto it = m_entries[type].find(id);
        ++(it->second.m_refCount);
    }
}

void ResourceHeap::release(unsigned id, Type type)
{
    if (id != 0) {
        auto it = m_entries[type].find(id);
        --(it->second.m_refCount);
        if (0 == it->second.m_refCount) {
//            printf("released id %d\n", id);
//            fflush(stdout);
            it->second.m_dtor(id);
            m_entries[type].erase(it);
        }
    }
}

ResourceHeap::ResourceHeap()
{
    ms_heap = this;
}

ResourceHeap::~ResourceHeap()
{
    for (auto& map : m_entries) {
        for (const auto& pair : map) {
            pair.second.m_dtor(pair.first);
        }
        map.clear();
    }
}

} // namespace GL
