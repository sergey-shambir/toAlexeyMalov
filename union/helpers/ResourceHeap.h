#pragma once
#include <unordered_map>

/**
 * @class Resource heap creates objects, counts references on objects and
 * handles deletion
 */

namespace GL {

class ResourceHeap
{
public:
    typedef unsigned(*Constructor)();
    typedef void(*Destructor)(unsigned);

    enum Type {
        Texture2D = 0,
        VertexBuffer,
        FrameBufferExt,
        RenderBufferExt,
        Shader,
        Program,

        TypesNum
    };

    static ResourceHeap &instance();

    /**
     * @param ctor - resource construction function
     * @param dtor - resource destruction function
     * @return Creates and returns retained object
     */
    unsigned create(Constructor ctor, Destructor dtor, Type type);

    /**
     * @brief Increases refs counter if id is not 0
     * @param id - 0 or resource identifier
     */
    void retain(unsigned id, Type type);

    /**
     * @brief Increases refs counter if id is not 0
     * @param id - 0 or resource identifier
     */
    void release(unsigned id, Type type);

#if !defined(UNION_LIBRARY)
protected:
#endif
    ResourceHeap(); /* do not use accidentally */
    ~ResourceHeap(); /* do not use accidentally */

private:
    static ResourceHeap *ms_heap;

    struct Entry {
        unsigned m_refCount;
        Destructor m_dtor;
    };
    void destroy(unsigned id);

    unsigned m_activeProgramId;
    std::unordered_map<unsigned, Entry> m_entries[TypesNum];
};

} // namespace GL
