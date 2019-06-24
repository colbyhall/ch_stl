#include "ch_memory.h"

void ch::mem_copy(void* dest, const void* src, usize size) {
    const u8* casted_src = (u8*)src;
    u8* casted_dest = (u8*)dest;

    for (usize i = 0; i < size; i++) {
        casted_dest[i] = casted_src[i];
    }
}

void* ch::mem_move(void* dest, const void* src, usize size) {
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