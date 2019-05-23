#pragma once

/**
 * AUTHOR: Colby Hall
 * DATE: 5/22/2019
 * COMMENT:
 */

#include "ch_stl.h"

namespace ch {
    void* alloc(p_size size);
    void* realloc(void* ptr, p_size size);
    void free(void* ptr);
#if BUILD_DEBUG
    void* alloc_debug(p_size size, const char* file, u32 line);
    void* realloc_debug(void* ptr, p_size size);
    void free_debug(void* ptr);

    struct Memory_Header {
        void* data;
        p_size size;

        const char* file;
        u32 line;

        Memory_Header* parent;
        Memory_Header* child;
    };
#endif
}

#if BUILD_DEBUG
#define ch_alloc(size) ch::alloc_debug(size, __FILE__, __LINE__)
#define ch_realloc(ptr, size) ch::realloc_debug(size, __FILE__, __LINE__)
#define ch_free(ptr) ch::free_debug(size, __FILE__, __LINE__)
#else
#define ch_alloc(size) ch::alloc(size)
#define ch_realloc(ptr, size) ch::realloc(size)
#define ch_free(ptr) ch::free(size)
#endif

#define ch_new new(__FILE__, __LINE__)
#define ch_delete delete

// @TODO(Colby): Is this a MSVC only thing?
#pragma warning(disable: 4595)

inline void* operator new(ch::p_size size, const char* file, ch::u32 line) {
#if BUILD_DEBUG
    return ch::alloc_debug(size, file, line);
#else
    return ch::alloc(size);
#endif
}

inline void* operator new[](ch::p_size size, const char* file, ch::u32 line) {
#if BUILD_DEBUG
    return ch::alloc_debug(size, file, line);
#else
    return ch::alloc(size);
#endif
}

inline void operator delete(void* ptr) {
#if BUILD_DEBUG
    ch::free_debug(ptr);
#else
    ch::free(ptr);
#endif
}

inline void operator delete[](void* ptr) {
#if BUILD_DEBUG
    ch::free_debug(ptr);
#else
    ch::free(ptr);
#endif
}

// @NOTE(Colby): These are never used but they prevent a warning
inline void operator delete(void* ptr, const char* file, ch::u32 line) {
#if BUILD_DEBUG
    ch::free_debug(ptr);
#else
    ch::free(ptr);
#endif
}

inline void operator delete[](void* ptr, const char* file, ch::u32 line) {
#if BUILD_DEBUG
    ch::free_debug(ptr);
#else
    ch::free(ptr);
#endif
}


#pragma warning(default: 4595)