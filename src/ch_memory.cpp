#include "ch_memory.h"

#if CH_PLATFORM_WINDOWS

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

#endif

void ch::memcpy(void* dest, const void* src, usize size) {
    const u8* casted_src = (u8*)src;
    u8* casted_dest = (u8*)dest;

    for (usize i = 0; i < size; i++) {
        casted_dest[i] = casted_src[i];
    }
}

void ch::memset(void* ptr, usize size, u8 c) {
    u8* casted_ptr = (u8*)ptr;

    for (usize i = 0; i < size; i++) {
        casted_ptr[i] = c;
    }
}

void ch::memzero(void* ptr, usize size) {
    ch::memset(ptr, size, 0);
}

void* ch::memmove(void* dest, const void* src, usize size) {
    if (dest == src) return dest;

    const u8* casted_src = (const u8*)src;
    u8* casted_dest = (u8*)dest;

    if (dest > src) {
        for (usize i = size - 1; i != 0; i--) {
            casted_dest[i] = casted_src[i];
        }
    }
    else {
        for (usize i = 0; i < size; i++) {
            casted_dest[i] = casted_src[i];
        }
    }

    return dest;
}