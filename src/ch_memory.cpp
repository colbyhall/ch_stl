/**
 * AUTHOR: Colby Hall
 * DATE: 5/22/2019
 * COMMENT:
 */

#include "ch_memory.h"

void* ch::alloc(p_size size) {
#if PLATFORM_WINDOWS
    return HeapAlloc(GetProcessHeap(), 0, size);
#endif
}

void* ch::realloc(void* ptr, p_size size) {
#if PLATFORM_WINDOWS
    return HeapReAlloc(GetProcessHeap(), 0, ptr, size);
#endif
}

void ch::free(void* ptr) {
#if PLATFORM_WINDOWS
    HeapFree(GetProcessHeap(), 0, ptr);
#endif
}

#if BUILD_DEBUG
void* ch::alloc_debug(p_size size, const char* file, u32 line) {

}

void* ch::realloc_debug(void* ptr, p_size size) {

}

void ch::free_debug(void* ptr) {

}
#endif