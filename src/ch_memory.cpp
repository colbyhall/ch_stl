#include "ch_memory.h"

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