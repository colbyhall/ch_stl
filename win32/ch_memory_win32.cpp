#include "../ch_memory.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

void* ch::malloc(usize size) {
    return HeapAlloc(GetProcessHeap(), 0, size);
}

void* ch::realloc(void* ptr, usize size) {
    return HeapReAlloc(GetProcessHeap(), 0, ptr, size);
}

void ch::free(void* ptr) {
    HeapFree(GetProcessHeap(), 0, ptr);
}

